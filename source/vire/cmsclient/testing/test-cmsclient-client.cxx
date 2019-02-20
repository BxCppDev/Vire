//! \file vire/cmsclient/testing/test-vireclient-client.cxx
//! \brief Test Vire CMS client
//
// Copyright (c) 2017-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsclient/client.h>

// Standard library:
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include <map>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/multi_properties.h>

// This project:
#include <vire/vire.h>

void test_cmsclient_client_1(const bool interactive_ = false);

int main(int argc_, char ** argv_)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsclient::client' class." << std::endl;

    bool interactive = false;

    {
      // Command-line parsing:
      int iarg = 1;
      while (iarg < argc_) {
        std::string cl_token = argv_[iarg++];
        if (cl_token == "--interactive") {
          interactive = true;
        }
      }
    }

    test_cmsclient_client_1(interactive);

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return error_code;
}

void test_cmsclient_client_1(const bool /* interactive_ */)
{
  std::clog << "\ntest_cmsclient_client_1: basics" << std::endl;

  // Client configuration:
  std::string clientParams0Config = \
    "@snemock:config/cms/test1/client/virecmsclient.conf";
  datatools::fetch_path_with_env(clientParams0Config);
  datatools::multi_properties clientParams0;
  clientParams0.read(clientParams0Config);
  std::clog << "Configuration for Vire CMS client #0:" << std::endl;
  clientParams0.print_tree(std::clog);
  std::clog << std::endl;

  // Client #0:
  vire::cmsclient::client client0;
  client0.set_logging_priority(datatools::logger::PRIO_DEBUG);
  client0.initialize(clientParams0);
  std::clog << "Client 0:" << std::endl;
  client0.print_tree(std::clog);
  std::clog << std::endl;
  client0.reset();

  return;
}
