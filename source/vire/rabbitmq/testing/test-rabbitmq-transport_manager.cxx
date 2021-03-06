//! \file vire/rabbitmq/testing/test-rabbitmq-transport_manager.cxx
//! \brief Test Vire RabbitMQ transport manager
//
// Copyright (c) 2018-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/vire.h>
#include <vire/rabbitmq/transport_manager.h>

void test_rtm_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::rabbitmq::transport_manager' class." << std::endl;

    bool interactive = false;
    interactive = true;

    test_rtm_1(interactive);

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

void test_rtm_1(bool interactive_)
{
  std::clog << "\ntest_com_rtm_1: basics" << std::endl;

  std::unique_ptr<vire::rabbitmq::transport_manager> transMgrPtr(new vire::rabbitmq::transport_manager);

  std::string rtm_config_path("${VIRE_TESTING_ONE_DIR}/config/rtm.conf");
  datatools::fetch_path_with_env(rtm_config_path);
  datatools::properties rtm_config;
  rtm_config.read_configuration(rtm_config_path);

  transMgrPtr->initialize(rtm_config);
  transMgrPtr->print_tree(std::clog);
  transMgrPtr->reset();
  
  std::clog << std::endl;
  return;
}
