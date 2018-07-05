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

  std::string domain_name_prefix = "/supernemo/demonstrator";

  vire::utility::model_identifier default_protocol_id("rabbitmq",
                                                      datatools::version_id(1, 0));
  vire::utility::model_identifier default_encoding_id("vire::com::protobuf_encoding_driver",
                                                      datatools::version_id(3));

  // vire::com::domain_builder domBuilder;
  // domBuilder.set_setup_name(domain_name_prefix);
  // domBuilder.set_encoding_type_id(protocol_id);
  // domBuilder.set_transport_type_id(encoding_id);

  vire::com::manager comMgr;
  comMgr.set_name("Com");
  comMgr.set_terse_description("SuperNEMO Vire Server Communication Service");
  comMgr.set_display_name("Communication");
  comMgr.set_domain_name_prefix(domain_name_prefix);
  comMgr.set_default_encoding_type_id(default_protocol_id);
  comMgr.set_default_transport_type_id(default_encoding_id);
  comMgr.initialize_simple();

  /*
  vire::com::domain & gate
    = comMgr.create_domain(vire::com::domain_builder::build_cms_clients_gate_name(setup_name),
                           "vire::com::domain::general",
                           protocol_id,
                           encoding_id);
  domBuilder.build_clients_gate_domain(gate);

  vire::com::domain & syscmslapp
    = comMgr.create_domain(vire::com::domain_builder::build_cms_subcontractor_system_name(setup_name, "cmslapp"),
                           "vire::com::domain::system",
                           protocol_id,
                           encoding_id);
  domBuilder.build_subcontractor_system_domain(syscmslapp, "cmslapp");

  vire::com::domain & control
    = comMgr.create_domain(vire::com::domain_builder::build_cms_control_name(setup_name),
                           "vire::com::domain::control",
                           protocol_id,
                           encoding_id);
  domBuilder.build_control_domain(control);

  vire::com::domain & monitoring
    = comMgr.create_domain(vire::com::domain_builder::build_cms_monitoring_name(setup_name),
                           "vire::com::domain::monitoring",
                           protocol_id,
                           encoding_id);
  domBuilder.build_monitoring_domain(monitoring);

  comMgr.create_domain(vire::com::domain_builder::build_cms_topic_name(setup_name, "sandbox"),
                       "vire::com::domain::general",
                       protocol_id,
                       encoding_id);
  */

  comMgr.tree_dump(std::clog, "Communication service: ");

  if (comMgr.has_domain(vire::com::domain_builder::build_cms_monitoring_name(domain_name_prefix))) {
    vire::com::domain & monitoring = comMgr.grab_domain(vire::com::domain_builder::build_cms_monitoring_name(domain_name_prefix));


    monitoring.tree_dump(std::clog, "Monitoring domain: ");
  }

  return;
}
