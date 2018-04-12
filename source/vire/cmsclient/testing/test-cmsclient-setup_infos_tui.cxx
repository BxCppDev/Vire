//! \file vire/cmsclient/testing/test-vireclient-setup_infos_tui.cxx
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
#include <vire/cmsclient/setup_infos_tui.h>

// Standard library:
#include <cstdlib>
#include <string>
#include <iostream>

// This project:
#include <vire/vire.h>

void test_cmsclient_situi_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsclient::setup_infos_tui' class." << std::endl;

    bool interactive = false;
    interactive = true;

    test_cmsclient_situi_1(interactive);

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

void test_cmsclient_situi_1(bool interactive_)
{
  std::clog << "\ntest_cmsclient_situi_1: basics" << std::endl;

  // Configuration:
  datatools::properties setupInfosParams;
  setupInfosParams.store("setup_id", "SuperNEMO");
  setupInfosParams.store("host", "localhost");
  setupInfosParams.store("port", 5671);
  setupInfosParams.store("domain_name_prefix", "/supernemo/demonstrator");
  setupInfosParams.store("transport_protocol_id", "vire::com::rabbitmq_transport_driver");
  setupInfosParams.store("encoding_protocol_id", "vire::com::protobuf_encoding_driver");
  setupInfosParams.store("gate_login", "snclient");
  setupInfosParams.store("gate_password", "IeM8rohghu");
  setupInfosParams.store_flag("nolock");
  setupInfosParams.tree_dump(std::clog, "Vire CMS setup infos: ");

  // Setup infos:
  vire::cmsclient::setup_infos setupInfos;
  setupInfos.initialize(setupInfosParams);
  setupInfos.tree_dump(std::clog, "Setup infos (initialized): ");

  // Text user interface:
  vire::cmsclient::setup_infos_tui setupInfosTui;
  if (setupInfosTui.run(setupInfos)) {
    setupInfos.lock();
    setupInfos.tree_dump(std::clog, "Setup infos (dialog): ");
  } else {
    std::cerr << "[error] Setup infos dialog failed!" << std::endl;
  }


  setupInfos.reset();
  std::clog << std::endl;

  return;
}
