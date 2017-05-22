//! \file  device/testing/test-device-base_electronics_module_model.cxx
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>

// This project:
#include <vire/device/base_electronics_module_model.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    // Mezzanine board model:
    vire::device::base_electronics_module_model mezz_board_model;
    mezz_board_model.set_name("MyMezzBoardModel");
    mezz_board_model.set_display_name("My mezzanine board model");
    mezz_board_model.set_terse_description("A mock mezzanine board model");
    mezz_board_model.set_type(vire::device::TYPE_MODULE_MEZZANINE_BOARD);
    mezz_board_model.set_model_key(2000);
    mezz_board_model.set_slot_width(1);
    mezz_board_model.set_format("IEEE 1386 PMC");
    mezz_board_model.set_max_number_of_mezzanine_modules(0);
    mezz_board_model.grab_auxiliaries().store_string("model.label", "Dummy");
    mezz_board_model.grab_auxiliaries().store_string("model.version", "1.0");
    mezz_board_model.grab_auxiliaries().store_string("model.manufacturer", "ACME");
    mezz_board_model.initialize_simple();
    mezz_board_model.tree_dump(std::cout, "Virtual mezzanine board model: ");
    std::cout << std::endl;

    // Board model:
    vire::device::base_electronics_module_model board_model;
    board_model.set_name("MyBoardModel");
    board_model.set_display_name("My board model");
    board_model.set_terse_description("A mock board model");
    board_model.set_type(vire::device::TYPE_MODULE_BOARD);
    board_model.set_model_key(1500);
    board_model.set_slot_width(2);
    board_model.set_format("VME-186");
    board_model.set_mezzanine_format("IEEE 1386 PMC");
    board_model.set_max_number_of_mezzanine_modules(2);
    board_model.grab_auxiliaries().store_string("model.label", "Dummy");
    board_model.grab_auxiliaries().store_string("model.version", "1.0");
    board_model.grab_auxiliaries().store_string("model.manufacturer", "ACME");

    // Add devices in the setup:
    board_model.add_mezzanine_module(0, mezz_board_model);
    board_model.add_mezzanine_module(1, mezz_board_model);
    board_model.initialize_simple();
    board_model.tree_dump(std::clog, "Virtual board model: ");
    std::clog << std::endl;

  }
  catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 error.what());
    error_code = EXIT_FAILURE;
  }
  catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
