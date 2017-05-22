//! \file vire/com/testing/test-com-manager.cxx
//! \brief Test Vire communication manager
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

// This project:
#include <vire/vire.h>
#include <vire/com/domain.h>
#include <vire/com/manager.h>
#include <vire/com/event_emitter_plug.h>
#include <vire/com/domain_builder.h>
#include <vire/utility/base_alarm.h>

void test_com_manager_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::com::manager' class."
              << std::endl;

    bool interactive = false;
    // interactive = true;

    test_com_manager_1(interactive);

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

void test_com_manager_1(bool interactive_)
{
  std::clog << "\ntest_com_manager_1: basics" << std::endl;

  std::string setup_name = "supernemo/demonstrator";

  vire::com::manager comMgr;
  comMgr.set_name("Com");
  comMgr.set_display_name("Communication");
  comMgr.set_terse_description("SuperNEMO Vire Server Communication Service");
  vire::com::actor vireServer("supernemo.demonstrator.vire.server",
                              vire::com::actor::CATEGORY_SERVER);
  comMgr.set_actor(vireServer);
  comMgr.initialize_simple();

  std::string protocol_id = "rabbitmq-1.0";
  std::string encoding_id = "protobuf-3";
  comMgr.create_domain(vire::com::domain_builder::build_cms_subcontractor_system_name(setup_name, "cmslapp"),
                       "vire::com::domain::system",
                       protocol_id,
                       encoding_id);
  comMgr.create_domain(vire::com::domain_builder::build_cms_control_name(setup_name),
                       "vire::com::domain::control",
                       protocol_id,
                       encoding_id);
  comMgr.create_domain(vire::com::domain_builder::build_cms_monitoring_name(setup_name),
                       "vire::com::domain::monitoring",
                       protocol_id,
                       encoding_id);
  comMgr.create_domain(vire::com::domain_builder::build_cms_topic_name(setup_name, "sandbox"),
                       "vire::com::domain::general",
                       protocol_id,
                       encoding_id);
  comMgr.create_domain(vire::com::domain_builder::build_cms_topic_name(setup_name, "events"),
                       "vire::com::domain::general",
                       protocol_id,
                       encoding_id);

  comMgr.tree_dump(std::clog, "Communication service: ");

  // comMgr.create_plug("sys",     "client_rpc_plug", "/snemo/vire/cms/control");
  // comMgr.create_plug("pubsub1", "event_listener_plug", "vire.server.cms.monitoring");
  // comMgr.create_plug("pubsub2", "event_listener_plug", "vire.server.cms.monitoring");
  // comMgr.create_plug("pubsub3", "event_listener_plug", "vire.server.cms.monitoring");
  // comMgr.create_plug("events.in",  "event_listener_plug", "vire.server.cmsinterface.service");
  // comMgr.create_plug("events.out", "event_emitter_plug", "vire.server.cmsinterface.service");
  // comMgr.create_plug("events2.out", "event_emitter_plug", "vire.server.cms.bad");

  // comMgr.tree_dump(std::clog, "Communication service: ");

  // // Create a simple alarm event:
  // vire::utility::base_alarm my_alarm;
  // my_alarm.set_severity("warning");
  // my_alarm.set_message("It's raining again!");

  // // Use a dedicated plug to send it to some destination:
  // vire::com::event_emitter_plug & ee = comMgr.grab_event_emitter_plug("events.out");
  // int error_code = ee.send_event(my_alarm);
  // if (error_code != vire::com::event_emitter_plug::SEND_EVENT_OK) {
  //   std::cerr << "error: Alarm event was not sent: code=" << error_code << "!\n";
  // } else {
  //   std::clog << "info: Alarm event was sent!\n";
  // }

  // comMgr.tree_dump(std::clog, "Communication service: ");
  return;
}
