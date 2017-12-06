//! \file test_connection_failure.cxx
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
#include <vire/cmslapp/connection_failure.h>

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>
#include <datatools/properties.h>
#include <datatools/utils.h>
// - BxJsontools
#include <jsontools/iofile.h>

// This project:
// #include <vire/vire.h>

void test0();

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    test0();

  } catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}

void test0()
{
  std::clog << "\ntest0: Entering..." << std::endl;

  {
    vire::cmslapp::connection_failure resp;
    vire::utility::invalid_context_error err;
    err.set_code(666);
    err.set_message_format("Invalid testing context");
    resp.set_error(err);
    resp.tree_dump(std::log, "Connection failure: ", "[info] ");
    jsontools::store("test_connection_failure.json", resp);
  }

  {
    vire::cmslapp::connection_failure resp;
    jsontools::load("test_connection_failure.json", resp);
  }

  std::clog << "\ntest0: Exiting." << std::endl;
  return;
}
