//! \file vire/com/testing/test-com-actor_hub.cxx
//! \brief Test Vire communication actor hub
//
// Copyright (c) 2019 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/com/utils.h>
#include <vire/com/access_profile.h>
#include <vire/com/access_hub.h>

#include "com_manager_tools.h"

void test_com_access_hub_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::com::access_hub' class." << std::endl;

    bool interactive = false;
    interactive = true;

    test_com_access_hub_1(interactive);

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return error_code;
}

void test_com_access_hub_1(bool interactive_)
{
  std::clog << "\ntest_com_access_hub_1: basics" << std::endl;
  
  // Com manager:  
  vire::com::manager comMgr;
  vire::cms::application::category_type appcat = vire::cms::application::CATEGORY_SERVER;
  vire::com::testing::init_com_manager(comMgr, appcat);
  comMgr.tree_dump(std::clog, "Communication service: ");
  vire::com::testing::add_com_manager(comMgr);

  // Access profile #0:
  std::clog << "\ntest_com_access_profile_1: Access #0" << std::endl;
  vire::com::access_profile n0(comMgr,
                              vire::com::ACCESS_CATEGORY_SERVER_GATE,
                              "",
                              "viresvrgate",
                              "viresvrgate");
  std::clog << "Access profile #0: " << std::endl;
  n0.print_tree(std::clog);
  std::clog << std::endl;

  vire::com::access_hub_ptr_type svr_gate_access = n0.create_access_hub("SvrGate");
  std::clog << "Access '" << svr_gate_access->get_name() << "' : " << std::endl;
  svr_gate_access->print_tree(std::clog);
  std::clog << std::endl;

  /*
  // Access profile #1:
  std::clog << "\ntest_com_access_profile_1: Access #1" << std::endl;
  vire::com::access_profile n1(comMgr,
                              vire::com::ACCESS_CATEGORY_CLIENT_CMS,
                              "DZAEDH83DZED",
                              "virecmsclient.DZAEDH83DZED",
                              "DZAEDH83DZED");
  n1.grab_metadata().store("rabbitmq.login", "foo");
  n1.grab_metadata().store("rabbitmq.passwd", "toto");
  std::clog << "Access profile #1: " << std::endl;
  n1.print_tree(std::clog);
  std::clog << std::endl;
  */

  /*
  // Access profile #2:
  std::clog << "\ntest_com_access_profile_2: Access #2" << std::endl;
  vire::com::access_profile n2(comMgr,
                              vire::com::ACCESS_CATEGORY_CLIENT_CMS,
                              "78DKZHFDZF",
                              "virecmsclient.78DKZHFDZF",
                              "78DKZHFDZF");
  n2.grab_metadata().store("rabbitmq.login", "bar");
  n2.grab_metadata().store("rabbitmq.passwd", "baz");
  std::clog << "Access profile #2: " << std::endl;
  n2.print_tree(std::clog);
  std::clog << std::endl;
  */
  
  /*
  // Access profile #3:
  std::clog << "\ntest_com_access_profile_3: Access #3" << std::endl;
  vire::com::access_profile n3(comMgr,
                              vire::com::ACCESS_CATEGORY_SUBCONTRACTOR,
                              "orleans",
                              "cmsorleans",
                              "cmsorleans");
  std::clog << "Access profile #3: " << std::endl;
  n3.print_tree(std::clog);
  std::clog << std::endl;
  */
  
  return;
}
