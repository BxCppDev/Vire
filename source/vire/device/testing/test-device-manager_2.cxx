//! \file vire/device/testing/test-device-manager_2.cxx
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Ourselves:
#include <vire/device/manager.h>

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>
#include <datatools/properties.h>
#include <datatools/utils.h>

// This project:
#include <vire/vire.h>

void test0();

int main( int argc_, char * argv_[])
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;

  try {

    test0();

  } catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return error_code;
}

void test0()
{
  std::clog << "\nTest 0: Entering..." << std::endl;

  // Device manager:
  vire::device::manager dev_mgr;

  datatools::properties dev_mgr_config;
  std::string dev_mgr_config_filename = "@snemo:config/snemo/demonstrator/devices/0.1/manager.conf";
  datatools::fetch_path_with_env(dev_mgr_config_filename);
  dev_mgr_config.read_configuration(dev_mgr_config_filename);
  dev_mgr.initialize_standalone(dev_mgr_config);
  dev_mgr.tree_dump(std::clog, "Virtual device manager: ");
  std::clog << std::endl;

  // const vire::device::base_electronics_module_model & calo_feb =
  //   dev_mgr.get_device_model_as<vire::device::base_electronics_module_model>("calo_feb");
  // calo_feb.tree_dump(std::clog, "Calorimeter frontend board: ");
  // std::clog << std::endl;

  std::clog << "\nTest 0: Exiting." << std::endl;
  return;
}
