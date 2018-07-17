//! \file test_disconnection_request.cxx
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
#include <vire/cms/disconnection_request.h>

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
    vire::cms::disconnection_request  req;
    vire::utility::instance_identifier sid;
    sid.from_string("SuperNEMO-1.0");
    req.set_setup_id(sid);
    req.tree_dump(std::clog, "disconnection request: ", "[info] ");

    jsontools::store("test_disconnection_request.json", req);
    uint32_t pb_flags = protobuftools::IO_DEBUG;
    protobuftools::store("test_disconnection_request.data", req, pb_flags);
  }

  {
    vire::cms::disconnection_request req;
    jsontools::load("test_disconnection_request.json", req);
    req.tree_dump(std::clog, "disconnection request (from JSON): ", "[info] ");
  }

  {
    vire::cms::disconnection_request req;
    protobuftools::load("test_disconnection_request.data", req);
    req.tree_dump(std::clog, "disconnection request (from Protobuf): ", "[info] ");
  }

  std::clog << "\ntest0: Exiting." << std::endl;
  return;
}
