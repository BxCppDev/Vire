//! \file test-cmslapp-version.cxx
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

// Ourselves:
#include <vire/cmslapp/version.h>

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>
#include <datatools/properties.h>
#include <datatools/utils.h>

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

  std::clog << "\ntest0: Vire/CMSLAPP version: " << VIRE_CMSLAPPInterface_LIB_VERSION << std::endl;

  std::clog << "\ntest0: Exiting." << std::endl;
  return;
}
