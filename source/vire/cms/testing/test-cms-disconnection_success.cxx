//! \file test_disconnection_success.cxx
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cms/disconnection_success.h>

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/utils.h>
// - BxJsontools
#include <bayeux/jsontools/iofile.h>
// BxProtobuftools:
#include <bayeux/protobuftools/iofile.h>

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
    vire::cms::disconnection_success  payload;
    vire::utility::instance_identifier sid;
    sid.from_string("SuperNEMO-1.0");
    payload.set_setup_id(sid);
    payload.tree_dump(std::clog, "disconnection success: ", "[info] ");

    jsontools::store("test_disconnection_success.json", payload);
    ::protobuftools::protobuf_factory::system_factory().print(std::cerr,
                                                              "Protobuf System Factory: ");
    uint32_t pb_flags = protobuftools::IO_DEBUG;
    protobuftools::store("test_disconnection_success.data", payload, pb_flags);
  }

  {
    vire::cms::disconnection_success payload;
    jsontools::load("test_disconnection_success.json", payload);
    payload.tree_dump(std::clog, "disconnection success (from JSON): ", "[info] ");
  }

  {
    vire::cms::disconnection_success payload;
    protobuftools::load("test_disconnection_success.data", payload);
    payload.tree_dump(std::clog, "disconnection success (from Protobuf): ", "[info] ");
  }

  std::clog << "\ntest0: Exiting." << std::endl;
  return;
}
