//! \file device/testing/test-device-single_instance.cxx
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

// Ourselves:
#include <vire/device/single_instance.h>

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    {
      // Instance:
      vire::device::single_instance si;
      vire::device::slot unique(vire::device::space::DIM_0, true);
      si.set_slot(unique);
      si.tree_dump(std::clog, "Single instance: ");
      std::clog << "Single instance = " << si << std::endl;
      std::clog << std::endl;

    }

    {
      // Instance:
      vire::device::single_instance si;
      std::string token = "   [ slot = (1:23)  ] ";
      if (vire::device::single_instance::from_string(token, si)) {
        si.tree_dump(std::clog, "Single instance: ");
        std::clog << "Single instance = " << si << std::endl;
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for single instance!");
        std::clog << std::endl;
      }
    }

    {
      // Instance:
      vire::device::single_instance si;
      std::string token = "   [ slot = (2: 5,2) ] ";
      if (vire::device::single_instance::from_string(token, si)) {
        si.tree_dump(std::clog, "Single instance: ");
        std::clog << "Instance = " << si << std::endl;
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for single instance!");
        std::clog << std::endl;
      }
    }

    {
      // Instance:
      vire::device::single_instance si;
      std::string token = "   [ slot = (1:666) ] aaa ";
      if (vire::device::single_instance::from_string(token, si)) {
        si.tree_dump(std::clog, "Single instance: ");
        std::clog << "Instance = " << si << std::endl;
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for single instance!");
        std::clog << std::endl;
      }
    }

    {
      // Instance:
      vire::device::single_instance si;
      std::string token = "  foo ";
      if (vire::device::single_instance::from_string(token, si)) {
        si.tree_dump(std::clog, "Single instance: ");
        std::clog << "Instance = " << si << std::endl;
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for single instance!");
        std::clog << std::endl;
       }
    }

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
