//! \file vire/com/testing/test-com-domain.cxx
//! \brief Test Vire communication domain
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
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include <map>

// This project:
#include <vire/vire.h>
#include <vire/com/domain.h>
#include <vire/com/base_plug.h>

void test_com_base_plug_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::com::base_plug' class." << std::endl;

    bool interactive = false;
    interactive = true;

    test_com_base_plug_1(interactive);

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

void test_com_base_plug_1(bool interactive_)
{
  std::clog << "\ntest_com_base_plug_1: basics" << std::endl;

  vire::com::base_plug plug0;
  plug0.set_name("server.rpc.monitor");
  plug0.set_domain_name("/snemo/vire/cms/monitoring");
  plug0.tree_dump(std::clog, "RPC monitoring plug: ");
  std::clog << std::endl;
  return;
}
