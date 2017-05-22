//! \file device/testing/test-device-base_port_model.cxx
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
#include <vire/device/port_types.h>
#include <vire/device/base_port_model.h>
#include <vire/device/device_utils.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    // Device port model:
    vire::device::base_port_model port_model;
    port_model.set_name("MyPortModel");
    port_model.set_display_name("My port model");
    port_model.set_terse_description("A mock port model");
    port_model.set_type(vire::device::PORT_GENERIC);
    port_model.set_io_type(vire::device::PORT_IO_IN);
    port_model.set_format("ACME-666");
    port_model.grab_auxiliaries().store("mode", "input");
    port_model.grab_auxiliaries().store("type", "analog");
    port_model.grab_auxiliaries().store("manufacturer", "lemo");
    port_model.grab_auxiliaries().store("model", "coaxial-00");
    port_model.grab_auxiliaries().store("interface", "socket");
    port_model.initialize_simple();
    port_model.tree_dump(std::clog, "Virtual port: ");
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
