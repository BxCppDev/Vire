//! \file vire/com/testing/test-com-event_emitter_plug.cxx
//! \brief Test Vire event emitter plug
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
//
// This file is part of Vire.
//
// Vire is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Vire is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Standard library:
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include <map>

// This project:
#include <vire/vire.h>
#include <vire/com/manager.h>
#include <vire/com/event_emitter_plug.h>
#include <vire/utility/base_alarm.h>

void test_com_event_emitter_plug_1(bool interactive_ = false);

std::mutex outmtx;

struct producer
{
  producer(const std::string & name_,
           vire::com::event_emitter_plug & out_)
  {
    _name_ = name_;
    _stopped_.store(false, std::memory_order_relaxed);
    _out_event_plug_ = &out_;
    return;
  }

  void run()
  {
    while(!_stopped_.load(std::memory_order_relaxed)) {
      usleep(1000000);
      {
        std::lock_guard<std::mutex> guard(outmtx);
        std::clog << "producer::run: loop for producer '" << _name_ << "'..." << std::endl;
      }
      // Create a simple alarm event:
      vire::utility::base_alarm my_alarm;
      my_alarm.set_severity("warning");
      my_alarm.set_message("It's raining again!");
      int send_error = _out_event_plug_->send_event(my_alarm);
      if (send_error != vire::com::event_emitter_plug::SEND_EVENT_OK) {
        std::lock_guard<std::mutex> guard(outmtx);
        std::cerr << "error: producer::run: alarm event was not sent!" << std::endl;
      } else {
        std::lock_guard<std::mutex> guard(outmtx);
        std::clog << "info: producer::run: alarm event was sent." << std::endl;
      }
    }
    return;
  }

  void stop()
  {
    _stopped_.store(true, std::memory_order_relaxed);
  }

  const std::string & get_name() const
  {
    return _name_;
  }

private:
  std::string _name_;
  std::atomic<bool> _stopped_; //(false);
  vire::com::event_emitter_plug * _out_event_plug_ = nullptr;
};

struct controller
{
  controller(bool interactive_ = false)
  {
    _interactive_ = interactive_;
    _stopped_.store(false, std::memory_order_relaxed);
    return;
  }

  void run()
  {
    std::size_t loop_counter = 0;
    while (!_stopped_.load(std::memory_order_relaxed)) {
      usleep(500000);
      if (_interactive_) {
        std::clog << _prompt_;
        std::string cmdline;
        std::getline(std::cin, cmdline);
        if (cmdline == "quit") {
          this->stop();
          break;
        } else {
          if (cmdline == "test") {
            {
              std::lock_guard<std::mutex> guard(outmtx);
              std::clog << "info: controller::run: test" << std::endl;
            }
          } else {
            {
              std::lock_guard<std::mutex> guard(outmtx);
              std::cerr << "error: controller::run: invalid command!" << std::endl;
            }
          }
        }
      }
      loop_counter++;
      if (loop_counter > (_interactive_ ? 15 : 3)) {
        this->stop();
        break;
      }
    }
    std::clog << "info: controller::run: The end." << std::endl;
    return;
  }

  void stop()
  {
    _stopped_.store(true, std::memory_order_relaxed);
    for (auto prod : _producers_) {
      {
        std::lock_guard<std::mutex> guard(outmtx);
        std::clog << "info: controller::stop: Stopping producer '" << prod.first << "'..." << std::endl;
      }
      prod.second->stop();
    }
    return;
  }

  void add_producer(producer & p_, const std::string & name_ = "")
  {
    std::string name = name_;
    if (name.empty()) {
      name = p_.get_name();
    }
    {
      std::lock_guard<std::mutex> guard(outmtx);
      std::clog << "debug: controller::add_producer: name='" << name << "'..." << std::endl;
    }
    _producers_[name] = &p_;
    return;
  }

private:
  std::map<std::string, producer*> _producers_;
  std::string _prompt_ = "controller> ";
  std::atomic<bool> _stopped_;
  bool _interactive_ = false;
};


int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::com::event_emitter_plug' class."
              << std::endl;

    bool interactive = false;
    // interactive = true;

    test_com_event_emitter_plug_1(interactive);

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return (error_code);
}

void test_com_event_emitter_plug_1(bool interactive_)
{
  std::clog << "\ntest_com_event_emitter_plug_1: basics" << std::endl;

  vire::com::manager comMgr;
  comMgr.set_name("Com");
  comMgr.set_display_name("Communication");
  comMgr.set_terse_description("Communication service");
  comMgr.set_node_id("snemo.client1");
  comMgr.initialize_simple();
  comMgr.create_domain("service", "vire::com::domain::service");
  // comMgr.create_plug("alarm_publisher", "event_emitter_plug", "service");
  comMgr.tree_dump(std::clog, "Communication service: ");

  // Use a dedicated plug to send alarms to some destination:
  vire::com::event_emitter_plug & ee = comMgr.grab_event_emitter_plug("alarm_publisher");

  producer prod1("p1", ee);
  producer prod2("p2", ee);
  producer prod3("p3", ee);
  controller ctrl;
  ctrl.add_producer(prod1);
  ctrl.add_producer(prod2);
  ctrl.add_producer(prod3);
  std::thread tctrl(&controller::run, std::ref(ctrl));
  std::thread tprod1(&producer::run, std::ref(prod1));
  std::thread tprod2(&producer::run, std::ref(prod2));
  std::thread tprod3(&producer::run, std::ref(prod3));
  tprod1.join();
  tprod2.join();
  tprod3.join();
  tctrl.join();

  return;
}
