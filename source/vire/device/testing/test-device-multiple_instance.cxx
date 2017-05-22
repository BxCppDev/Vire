//! \file device/testing/test-device-multiple_instance.cxx
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
#include <vire/device/multiple_instance.h>

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>

// This project:
#include <vire/device/single_instance.h>
#include <vire/device/slot.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    {
      // Multiple instance:
      vire::device::multiple_instance mi;
      mi.append_slot(vire::device::slot(vire::device::space::DIM_1,3));
      mi.append_slot(vire::device::slot(vire::device::space::DIM_1,5));
      mi.append_slot(vire::device::slot(vire::device::space::DIM_1,7));
      mi.append_slot(vire::device::slot(vire::device::space::DIM_1,8));
      mi.append_slot(vire::device::slot(vire::device::space::DIM_1,12));
      mi.append_slot(vire::device::slot(vire::device::space::DIM_1,42));
      mi.tree_dump(std::clog, "Multiple instance: ");
      std::clog << "Multiple instance = " << mi << std::endl;
      std::clog << std::endl;
    }

    {
      // Multiple instance:
      vire::device::multiple_instance mi;
      mi.tree_dump(std::clog, "Multiple instance: ");
      std::clog << "Multiple instance = " << mi << std::endl;
      std::clog << std::endl;
    }

    {
      // Multiple instance:
      vire::device::multiple_instance mi;
      std::string token = "[slots = { (1:23); (1:24); (1:78); (1:1); (1:0) } ] ";
      //token = "   [ slots = { a; b; c; d } ] ";
      //token = "   [slots={1;2;3;4}]";
      //token = "   [ slots = { } ] ";
      if (vire::device::multiple_instance::from_string(token, mi)) {
        mi.tree_dump(std::clog, "Multiple instance: ");
        std::clog << "Multiple instance = " << mi << std::endl;
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR,
                     "As expected: invalid format '" << token << "' for multiple instance!");
        std::clog << std::endl;
      }
    }

    {
      // Multiple instance:
      vire::device::multiple_instance mi;
      std::string token = "   [ slots = {(2:2,23); (2:2,24); (2:3,23); (2:3,25); (2:3,26)} ] ";
      if (vire::device::multiple_instance::from_string(token, mi)) {
        mi.tree_dump(std::clog, "Multiple instance: ");
        std::clog << "Multiple instance = " << mi << std::endl;
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for multiple instance!");
        std::clog << std::endl;
      }
    }

    {
      // Multiple instance:
      vire::device::multiple_instance mi;
      std::string token = "   [ slots = {} ] ";
      if (vire::device::multiple_instance::from_string(token, mi)) {
        mi.tree_dump(std::clog, "Multiple instance: ");
        std::clog << "Multiple instance = " << mi << std::endl;
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for multiple instance!");
        std::clog << std::endl;
      }
    }

    {
      // Multiple instance:
      vire::device::multiple_instance mi;
      std::string token = "   [ slots = {aaa}  ] ";
      if (vire::device::multiple_instance::from_string(token, mi)) {
        mi.tree_dump(std::clog, "Multiple instance: ");
        std::clog << "Multiple instance = " << mi << std::endl;
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for multiple instance!");
        std::clog << std::endl;
      }
    }

    {
      // Multiple instance:
      vire::device::multiple_instance mi;
      std::string token = "   [ slots = { (1:23); (1:24)}  ] foo";
      if (vire::device::multiple_instance::from_string(token, mi)) {
        mi.tree_dump(std::clog, "Multiple instance: ");
        std::clog << "Multiple instance = " << mi << std::endl;
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for multiple instance!");
        std::clog << std::endl;
      }
    }

    {
      // Multiple instance:
      vire::device::multiple_instance mi;
      std::string token = "   [ slots =  ]";
      if (vire::device::multiple_instance::from_string(token, mi)) {
        mi.tree_dump(std::clog, "Multiple instance: ");
        std::clog << "Multiple instance = " << mi << std::endl;
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for multiple instance!");
        std::clog << std::endl;
      }
    }

    {
      // Multiple instance:
      vire::device::multiple_instance mi;
      std::string token = "   [ slots = { (0) } ]";
      if (vire::device::multiple_instance::from_string(token, mi)) {
        mi.tree_dump(std::clog, "Multiple instance: ");
        std::clog << "Multiple instance = " << mi << std::endl;
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for multiple instance!");
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
