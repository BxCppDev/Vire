//! \file cms/testing/test-cms-resource_exec_success.cxx
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

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>

// This project:
#include <vire/cms/resource_exec_success.h>
#include <vire/time/utils.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    {
      vire::cms::resource_status_record rsr;
      rsr.set_path("SuperNEMO:/Demonstrator/CMS/Coil/Control/Current/__dp_write__");
      rsr.set_timestamp(vire::time::now());
      rsr.set_pending();

      vire::cms::resource_exec_success resr;
      resr.set_status(rsr);
      resr.add_output_argument("voltage", "234.5", "unit=V");
      resr.add_output_argument("test", "bar", "dummy");
      resr.tree_dump(std::clog, "Resource execution success: ");
      std::clog << std::endl;
    }

  } catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}
