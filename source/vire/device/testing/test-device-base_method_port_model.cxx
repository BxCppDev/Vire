//! \file device/testing/test-device-base_method_port_model.cxx
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
#include <datatools/introspection/method.h>
#include <datatools/introspection/argument.h>

// This project:
#include <vire/device/port_types.h>
#include <vire/device/base_method_port_model.h>
#include <vire/device/device_utils.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    // Create a method:
    std::shared_ptr<datatools::introspection::method> meth_ptr(new datatools::introspection::method);
    datatools::introspection::method & meth = *meth_ptr.get();
    meth.make_scalar_getter("pressure",
                            datatools::introspection::DATA_TYPE_FLOAT,
                            "mbar",
                            "mbar",
                            "Get the pressure (in explicit pressure unit)",
                            "get_pressure");

    // Device method port model:
    vire::device::base_method_port_model method_port_model;
    method_port_model.set_name("get_pressure.model");
    method_port_model.set_display_name("Get Pressure Port Model");
    method_port_model.set_terse_description("Method port for pressure measurement");
    method_port_model.set_method(meth_ptr);
    method_port_model.initialize_simple();
    method_port_model.tree_dump(std::clog, "Virtual method port: ");
    std::clog << std::endl;

  }
  catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, error.what());
    error_code = EXIT_FAILURE;
  }
  catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
