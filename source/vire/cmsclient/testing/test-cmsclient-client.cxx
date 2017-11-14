//! \file vire/cmsclient/testing/test-vireclient-client.cxx
//! \brief Test Vire CMS client
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

void test_cmsclient_client_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsclient::client' class." << std::endl;

    bool interactive = false;
    interactive = true;

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

void test_cmsclient_client_1(bool interactive_)
{
  std::clog << "\ntest_cmsclient_client_1: basics" << std::endl;

  // Client configuration:
  datatools::multi_properties clientParams0;
  std::string clientParams0Config = \
    "@snemo:config/snemo/demonstrator/cms/clients/client-devel1.conf";
  datatools::fetch_path_with_env(clientParams0Config);
  clientParams0.read(clientParams0Config);
  clientParams0.tree_dump (std::clog, "Vire CMS client configuration: ");

  // Client #0:
  vire::cmsclient::client client0;
  client0.set_logging(datatools::logger::PRIO_TRACE);
  client0.initialize(clientParams0);
  client0.tree_dump(std::clog, "Client 0: ");
  client0.reset();
  std::clog << std::endl;

  return;
}
