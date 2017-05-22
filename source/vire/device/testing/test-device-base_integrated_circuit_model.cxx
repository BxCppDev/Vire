//! \file  device/testing/test-device-base_integrated_circuit_model.cxx
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
#include <vire/device/base_integrated_circuit_model.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    // Calorimeter frontend board Control FPGA chip model:
    vire::device::base_integrated_circuit_model calo_ctrl_fpga_model;
    calo_ctrl_fpga_model.set_name("CaloFEBCtrlFPGA");
    calo_ctrl_fpga_model.set_display_name("Calorimeter frontend board Control FPGA");
    calo_ctrl_fpga_model.set_terse_description("The control FPGA ship embedded in the calorimeter frontend board");
    calo_ctrl_fpga_model.set_type(vire::device::TYPE_CHIP_PLD);
    calo_ctrl_fpga_model.set_model_key(12000);
    calo_ctrl_fpga_model.grab_auxiliaries().store_string("model.label", "Cyclone III");
    calo_ctrl_fpga_model.grab_auxiliaries().store_string("model.version", "EP3C16");
    calo_ctrl_fpga_model.grab_auxiliaries().store_string("model.manufacturer", "Altera");
    calo_ctrl_fpga_model.initialize_simple();
    calo_ctrl_fpga_model.tree_dump(std::cout, "Virtual FPGA model: ");
    std::cout << std::endl;

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
