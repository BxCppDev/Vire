//! \file device/testing/test-device-i_instance.cxx
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
#include <vire/device/i_instance.h>

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
      vire::device::i_instance * i1 = vire::device::i_instance::create("vire::device::single_instance", "[slot=(1:6)]");
      if (i1) {
        i1->tree_dump(std::clog, "Allocated instance i1 : ");
        delete i1;
      }
    }

    {
      vire::device::i_instance * i2 = vire::device::i_instance::create("vire::device::single_instance", "[slot=(1:!)]");
      if (i2) {
        i2->tree_dump(std::clog, "Allocated instance i2 : ");
        delete i2;
      }
    }

    {
      vire::device::i_instance * i3 = vire::device::i_instance::create("vire::device::multiple_instance",
                                                                       "[slots = { (2:0,0) ; (2:0,1) ; (2:0,4) } ]");
      if (i3) {
        i3->tree_dump(std::clog, "Allocated instance i3 : ");
        delete i3;
      }
    }

    {
      vire::device::i_instance * i4 = vire::device::i_instance::create("vire::device::multiple_instance",
                                                                       "[slots = { (1:0) ; (1:2) ; (1:42) } ]");
      if (i4) {
        i4->tree_dump(std::clog, "Allocated instance i4 : ");
        delete i4;
      }
    }

    /*
    {
      vire::device::i_instance * i1 = vire::device::i_instance::create("vire::device::array_instance",
                                                                       "[nslots = 3; first_slot = 2; slot_step = 4]");
      if (i1) {
        i1->tree_dump(std::clog, "Allocated instance i1 : ");
        delete i1;
      }
    }
    */

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
