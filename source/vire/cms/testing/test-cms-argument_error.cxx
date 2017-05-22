//! \file cms/testing/test-cms-argument_error.cxx
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cms/argument_error.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    {
      // Error:
      vire::cms::argument_error err;
      err.make_invalid_number_of_arguments(2, 1);
      err.print(std::clog);
      std::clog << std::endl;
      std::clog << std::endl;
      err.tree_dump(std::clog, "Invalid number of arguments error: ");
      std::clog << std::endl;
    }

    {
      // Error:
      vire::cms::argument_error err;
      err.make_invalid_argument("foo");
      err.print(std::clog);
      std::clog << std::endl;
      std::clog << std::endl;
      err.tree_dump(std::clog, "Invalid argument error: ");
      std::clog << std::endl;
    }

    {
      // Error:
      vire::cms::argument_error err;
      err.make_invalid_argument_value("Pi", "3.15");
      err.print(std::clog);
      std::clog << std::endl;
      std::clog << std::endl;
      err.tree_dump(std::clog, "Invalid argument value error: ");
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
