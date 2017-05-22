//! \file device/testing/test-device-regular_1d_instance.cxx
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
#include <vire/device/regular_1d_instance.h>

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>

// This project:
#include <vire/device/single_instance.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    {
      // Regular 1D instance:
      vire::device::regular_1d_instance ri;
      ri.tree_dump(std::clog, "Regular 1D instance: ");
      std::clog << "Regular 1D instance = " << ri << std::endl;
      std::clog << std::endl;
    }

    {
      // Regular 1D instance:
      vire::device::regular_1d_instance ri;
      vire::device::slot fs(vire::device::space::DIM_1, 3);
      std::clog << "First slot = " << fs << std::endl;
      ri.set(fs, 6, 2);
      ri.tree_dump(std::clog, "Regular 1D instance in a 1D logic space: ");
      std::clog << "Regular 1D instance = " << ri << std::endl;
      for (int i = 0; i < (int) ri.get_number_of_items(); i++) {
        vire::device::slot sl;
        ri.fetch_slot(i, sl);
        std::clog << " -> Slot = " << sl << std::endl;
      }
      std::clog << std::endl;
    }

    {
      // Regular 1D instance:
      vire::device::regular_1d_instance ri;
      vire::device::slot fs(vire::device::space::DIM_1, 3);
      std::clog << "First slot = " << fs << std::endl;
      ri.set(fs, 6, 2);
      ri.tree_dump(std::clog, "Regular 1D instance in a 2D logic space: ");
      std::clog << "Regular 1D instance = " << ri << std::endl;
      for (int i = 0; i < (int) ri.get_number_of_items(); i++) {
        vire::device::slot sl;
        ri.fetch_slot(i, sl);
        std::clog << " -> Slot = " << sl << std::endl;
      }
      std::clog << std::endl;
    }

    {
      // Regular 1D instance:
      vire::device::regular_1d_instance ri;
      vire::device::slot fs(vire::device::space::DIM_3, 0, 0, 2);
      std::clog << "First slot = " << fs << std::endl;
      ri.set(fs, 8, 0, +1, 0);
      ri.tree_dump(std::clog, "Regular 1D instance in a 3D logic space: ");
      std::clog << "Regular 1D instance = " << ri << std::endl;
      for (int i = 0; i < (int) ri.get_number_of_items(); i++) {
        vire::device::slot sl;
        ri.fetch_slot(i, sl);
        std::clog << " -> Slot = " << sl << std::endl;
      }
      std::clog << std::endl;
    }

    {
      // Regular 1D instance:
      vire::device::regular_1d_instance ri;
      std::string token = " [!] ";
      if (ri.from_string(token)) {
        ri.tree_dump(std::clog, "Parsed regular 1D instance: ");
        std::clog << "Regular 1D instance = " << ri << std::endl;
        for (int i = 0; i < (int) ri.get_number_of_items(); i++) {
          vire::device::slot sl;
          ri.fetch_slot(i, sl);
          std::clog << " -> Slot = " << sl << std::endl;
        }
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for array instance!");
        std::clog << std::endl;
      }
    }

    {
      // Regular 1D instance:
      vire::device::regular_1d_instance ri;
      std::string token = " [nslots=5; first_slot=(1:3); step=(1)] ";
      if (ri.from_string(token)) {
        ri.tree_dump(std::clog, "Parsed regular 1D instance: ");
        std::clog << "Regular 1D instance = " << ri << std::endl;
        for (int i = 0; i < (int) ri.get_number_of_items(); i++) {
          vire::device::slot sl;
          ri.fetch_slot(i, sl);
          std::clog << " -> Slot = " << sl << std::endl;
        }
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for array instance!");
        std::clog << std::endl;
      }
    }

    {
      // Regular 1D instance:
      vire::device::regular_1d_instance ri;
      std::string token = " [nslots=6; first_slot=(2:0,0); step=(1,1)] ";
      if (ri.from_string(token)) {
        ri.tree_dump(std::clog, "Parsed regular 1D instance: ");
        std::clog << "Regular 1D instance = " << ri << std::endl;
        for (int i = 0; i < (int) ri.get_number_of_items(); i++) {
          vire::device::slot sl;
          ri.fetch_slot(i, sl);
          std::clog << " -> Slot = " << sl << std::endl;
        }
        std::clog << std::endl;
      } else {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "As expected: invalid format '" << token << "' for array instance!");
        std::clog << std::endl;
      }
    }

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
