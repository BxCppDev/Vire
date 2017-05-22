//! \file  vire/protobuf/testing/test-protobuf-ModelIdentifier.cxx
//! \brief Protobuf support Vire library
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
#include <iostream>
#include <string>
#include <exception>
#include <fstream>

// Third party:
#include <google/protobuf/stubs/common.h>

// This project:
#include <vire/vire.h>
#include <vire/version.h>

void test_1();

void on_shutdown();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the Protocol Buffer library."
              << std::endl;

    test_1();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return (error_code);
}

void test_1()
{
  std::clog << "\ntest_1: " << std::endl;
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  google::protobuf::internal::OnShutdown(&on_shutdown);

  std::clog << "GOOGLE_PROTOBUF_VERSION = " << GOOGLE_PROTOBUF_VERSION << std::endl;
  std::clog << "Version = "
            << google::protobuf::internal::VersionString(GOOGLE_PROTOBUF_VERSION)
            << std::endl;

  google::protobuf::ShutdownProtobufLibrary();

  return;
}

void on_shutdown()
{
  std::clog << "Google Protobuf library shutdown." << std::endl;
  return;
}
