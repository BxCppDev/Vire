//! \file vire/com/testing/test-com-manager.cxx
//! \brief Test Vire communication manager
//
// Copyright (c) 2016-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#include "com_manager_tools.h"

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

  /*
  std::string domain_name_prefix = "/_snemod_";

  vire::utility::model_identifier default_transport_id("rabbitmq",
                                                      datatools::version_id(1));
  vire::utility::model_identifier default_encoding_id("vire::com::protobuf_encoding_driver",
                                                      datatools::version_id(3));

  vire::com::manager comMgr;
  comMgr.set_name("Com");
  comMgr.set_terse_description("SuperNEMO Vire Server Communication Service");
  comMgr.set_display_name("Communication");
  comMgr.set_app_category(vire::cms::application::CATEGORY_SERVER);
  comMgr.set_domain_name_prefix(domain_name_prefix);
  comMgr.set_default_encoding_type_id(default_encoding_id);
  comMgr.set_default_transport_type_id(default_transport_id);

  std::string com_config_path("${VIRE_TESTING_ONE_DIR}/config/com.conf");
  datatools::fetch_path_with_env(com_config_path);
  datatools::properties com_config;
  com_config.read_configuration(com_config_path);
  
  comMgr.initialize_standalone(com_config);
  */
  
  vire::com::manager comMgr;
  vire::com::testing::init_com_manager(comMgr);
  comMgr.tree_dump(std::clog, "Communication service: ");

  if (comMgr.has_domain(vire::com::domain_builder::build_cms_monitoring_name(vire::com::testing::default_domain_name_prefix()))) {
    vire::com::domain & monitoring
      = comMgr.grab_domain(vire::com::domain_builder::build_cms_monitoring_name(vire::com::testing::default_domain_name_prefix()));
    monitoring.tree_dump(std::clog, "Monitoring domain: ");
  }

  return;
}
