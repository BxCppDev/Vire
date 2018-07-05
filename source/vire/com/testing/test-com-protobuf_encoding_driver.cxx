//! \file vire/com/testing/test-com-protobuf_encoding_driver.cxx
//! \brief Test Vire communication protobuf encoding driver
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
#include <memory>

// This project:
#include <vire/vire.h>
#include <vire/com/protobuf_encoding_driver.h>
#include <vire/message/message.h>
#include <vire/utility/base_alarm.h>
#include <vire/message/body_layout.h>

void test_com_pbed_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::com::protobuf_encoding_driver' class."
              << std::endl;

    bool interactive = false;
    interactive = true;

    test_com_pbed_1(interactive);

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

void test_com_pbed_1(bool interactive_)
{
  std::clog << "\ntest_com_pbed_1: basics" << std::endl;

  vire::com::protobuf_encoding_driver pbEncodingDriver;
  // pbEncodingDriver.set_logging(datatools::logger::PRIO_DEBUG);
  pbEncodingDriver.initialize_simple();

  vire::com::raw_message_type raw_msg;
  {
    // Create an error event payload:
    auto my_alarm = std::make_shared<vire::utility::base_alarm>("warning", "A simple warning");
    my_alarm->tree_dump(std::clog, "Alarm event: ");
    std::clog << std::endl;

    // Create a message:
    vire::message::message msg;

    vire::message::message_header & h = msg.grab_header();
    vire::message::message_identifier msg_id("vire.server", 42);
    vire::utility::model_identifier body_layout_id;
    body_layout_id.set_name(vire::message::body_layout::name());
    body_layout_id.set_version(vire::message::body_layout::current_version());
    h.set_message_id(msg_id);
    h.set_timestamp(vire::time::now_utc());
    h.set_category(vire::message::MESSAGE_EVENT);
    h.set_in_reply_to(vire::message::message_identifier("vire.client.0", 23));
    // h.set_asynchronous(true);
    h.set_async_address("snemo.vire.cms.alarm");
    h.set_body_layout_id(body_layout_id);

    vire::message::message_body & b = msg.grab_body();
    b.set_payload(my_alarm);
    msg.tree_dump(std::clog, "Stored message: ");

    // pbEncodingDriver.set_logging(datatools::logger::PRIO_DEBUG);
    pbEncodingDriver.encode(msg, raw_msg);

    std::clog << "===========================================" << std::endl;
    std::clog << "Buffer: " << "size=" << raw_msg.buffer.size() << std::endl;
    std::clog << "===========================================" << std::endl;
    std::size_t count = 0;
    for (auto c : raw_msg.buffer) {
      std::clog << (std::isprint((int) c) ? c : '?');
      if (++count % 32 == 0) std::clog << '\n';
    }
    std::clog << "\n===========================================" << std::endl;

  }
  {
    vire::message::message msg;
    pbEncodingDriver.decode(raw_msg, msg);
    // pbEncodingDriver.set_logging(datatools::logger::PRIO_DEBUG);
    msg.tree_dump(std::clog, "Decoded message: ");
  }
  return;
}
