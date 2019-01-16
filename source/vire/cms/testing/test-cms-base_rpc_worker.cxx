//! \file cms/testing/test-cms-base_rpc_worker.cxx
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

// Standard library:
#include <iostream>
#include <memory>
// #include <future>

// Third party:
// - Bayeux/datatools
#include <datatools/logger.h>
#include <datatools/exception.h>

// This project:
#include <vire/cms/base_rpc_worker.h>
#include <vire/cms/resource_fetch_status.h>

#include "dummy_rpc_worker.h"

void test1();

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {
    test1();
 
  } catch (std::exception & error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL, "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}

void test1()
{
  vire::cms::testing::dummy_rpc_worker  worker;
  worker.set_logging(datatools::logger::PRIO_TRACE);
  vire::utility::const_payload_ptr_type request;
  std::string resPath = "SuperNEMO:/Demonstrator/CMS/Coil/PS1/uptime";
  request = std::make_shared<vire::cms::resource_fetch_status>(resPath);
  request->tree_dump(std::clog);
  vire::utility::payload_ptr_type response;
  vire::utility::exec_report x = worker.run(request, response);
  if (x.is_success()) {
    std::clog << "Success!" << std::endl;
    response->tree_dump(std::clog);
  } else {
    std::clog << "Failure: " << x.get_message() << std::endl;
  }
  std::clog << std::endl;
}

void test2()
{   
  /*
    std::future<vire::utility::exec_report> fut
    = std::async(&vire::cms::base_rpc_worker::run,
    &worker,
    request,
    response);
    // Do something while waiting for function to set future:
    std::clog << "Checking, please wait";
    std::chrono::milliseconds span(100);
    while (fut.wait_for(span) == std::future_status::timeout) {
    std::cout << '.' << std::flush;
    }
    vire::utility::exec_report x = fut.get(); // retrieve return value
  */
  return;
}
