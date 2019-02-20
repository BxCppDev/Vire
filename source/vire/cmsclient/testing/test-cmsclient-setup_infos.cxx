//! \file vire/cmsclient/testing/test-vireclient-setup_infos.cxx
//! \brief Test Vire setup infos
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
#include <vire/cmsclient/setup_infos.h>

// Standard library:
#include <cstdlib>
#include <string>
#include <iostream>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/vire.h>

void test_cmsclient_setup_infos_1(bool interactive_ = false);
void test_cmsclient_setup_infos_2(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsclient::setup_infos' class." << std::endl;

    bool interactive = false;
    interactive = true;

    test_cmsclient_setup_infos_1(interactive);

    test_cmsclient_setup_infos_2(interactive);

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

void test_cmsclient_setup_infos_1(bool interactive_)
{
  std::clog << "\ntest_cmsclient_setup_infos_1: basics" << std::endl;

  // Configuration:
  datatools::properties setupInfosParams;
  setupInfosParams.store("setup_id", "SuperNEMO");
  setupInfosParams.store("host", "localhost");
  setupInfosParams.store("port", 5671);
  setupInfosParams.store("domain_name_prefix", "/__snemod__");
  setupInfosParams.store("transport_protocol_id", "vire::com::rabbitmq_transport_driver");
  setupInfosParams.store("encoding_protocol_id", "vire::com::protobuf_encoding_driver");
  setupInfosParams.store("gate_client_user_login", "snemod_vireclientgate");
  setupInfosParams.store("gate_client_user_password", "snemod_vireclientgate");
  setupInfosParams.tree_dump(std::clog, "Vire CMS setup infos: ");

  // Setup infos:
  vire::cmsclient::setup_infos setupInfos;
  setupInfos.initialize(setupInfosParams);
  std::clog << "Setup infos: \n";
  setupInfos.print_tree(std::clog);
  setupInfos.reset();
  std::clog << std::endl;

  return;
}

void test_cmsclient_setup_infos_2(bool interactive_)
{
  std::clog << "\ntest_cmsclient_setup_infos_2: configuration from file" << std::endl;

  // Configuration:
  datatools::properties setupInfosParams;
  std::string setupInfosConfigPath = "@snemock:config/cms/test1/client/setup_infos.conf";
  datatools::fetch_path_with_env(setupInfosConfigPath);
  std::clog << "setupInfosConfigPath = '" << setupInfosConfigPath << "'" << std::endl;
  setupInfosParams.read_configuration(setupInfosConfigPath);
  std::clog << "setupInfosParams:" << std::endl;
  setupInfosParams.print_tree(std::clog);

  // Setup infos:
  vire::cmsclient::setup_infos setupInfos;
  setupInfos.initialize(setupInfosParams);
  std::clog << "Setup infos: \n";
  setupInfos.print_tree(std::clog);
  setupInfos.reset();
  std::clog << std::endl;

  return;
}
