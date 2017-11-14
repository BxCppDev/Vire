//! \file vire/cmsclient/testing/test-vireclient-server_infos.cxx
//! \brief Test Vire server infos
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
#include <vire/cmsclient/server_infos.h>

// Standard library:
#include <cstdlib>
#include <string>
#include <iostream>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/vire.h>

void test_cmsclient_si_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsclient::server_infos' class." << std::endl;

    bool interactive = false;
    interactive = true;

    test_cmsclient_si_1(interactive);

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

void test_cmsclient_si_1(bool interactive_)
{
  std::clog << "\ntest_cmsclient_si_1: basics" << std::endl;

  // Configuration:
  datatools::properties serverInfosParams;
  serverInfosParams.store("setup_id", "SuperNEMO");
  serverInfosParams.store("host", "localhost");
  serverInfosParams.store("port", 5671);
  serverInfosParams.store("domain_name_prefix", "/supernemo/demonstrator");
  serverInfosParams.store("transport_protocol_id", "vire::com::rabbitmq_transport_driver");
  serverInfosParams.store("encoding_protocol_id", "vire::com::protobuf_encoding_driver");
  serverInfosParams.store("gate_login", "snclient");
  serverInfosParams.store("gate_password", "IeM8rohghu");
  serverInfosParams.tree_dump(std::clog, "Vire CMS server infos: ");

  // Server infos:
  vire::cmsclient::server_infos serverInfos;
  serverInfos.initialize(serverInfosParams);
  serverInfos.tree_dump(std::clog, "Server infos: ");
  serverInfos.reset();
  std::clog << std::endl;

  return;
}
