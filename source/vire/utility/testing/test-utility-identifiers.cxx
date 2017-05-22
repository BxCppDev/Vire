//! \file utility/testing/test-utility-identifiers.cxx
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
#include <vire/utility/model_identifier.h>
#include <vire/utility/instance_identifier.h>

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    // Model identifier:
    vire::utility::model_identifier classId("vire::utility::testing::foo");
    std::clog << "Class ID              : " << classId.to_string() << std::endl;

    // Model identifier with version:
    vire::utility::model_identifier classIdVersioned("vire::utility::testing::foo", datatools::version_id(1, 0, 3, "rc1"));
    std::clog << "Class ID with version : " << classIdVersioned.to_string() << std::endl;

    // Model identifier with tag version:
    vire::utility::model_identifier classIdTagged("vire::utility::testing::foo", datatools::version_id("test_0"));
    std::clog << "Class ID with tag     : " << classIdTagged.to_string() << std::endl;

    // Instance identifier:
    vire::utility::instance_identifier objectId("vire.utility.testing.bar");
    std::clog << "Object ID             : " << objectId.to_string() << std::endl;

    // Invalid instance identifiers:
    try {
      vire::utility::instance_identifier badObjectId("vire/kernel");
    } catch (std::exception & error) {
      std::cerr << "error: As expected : " << error.what() << std::endl;
    }

    try {
      vire::utility::instance_identifier badObjectId("vire::kernel");
    } catch (std::exception & error) {
      std::cerr << "error: As expected : " << error.what() << std::endl;
    }

    try {
      vire::utility::instance_identifier badObjectId("007");
    } catch (std::exception & error) {
      std::cerr << "error: As expected : " << error.what() << std::endl;
    }

    // Instance identifier with version:
    vire::utility::instance_identifier objectIdVersioned("John", datatools::version_id(23));
    std::clog << "Object ID w/version   : " << objectIdVersioned.to_string() << std::endl;

    // Instance identifier with tag:
    vire::utility::instance_identifier objectIdTagged("John", datatools::version_id("test"));
    std::clog << "Object ID w/tag       : " << objectIdTagged.to_string() << std::endl;

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
