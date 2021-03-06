//! \file  vire/user/testing/test-user-simple_password_generator.cxx
//! \brief Vire user simple password generator
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

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

// This project:
#include <vire/vire.h>
#include <vire/user/simple_password_generator.h>

void test_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::user::simple_password_generator' class."
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
  std::clog << "\ntest_1: basics" << std::endl;

  unsigned int seed = 314159;
  unsigned int length = 12;
  vire::user::simple_password_generator gen(vire::user::simple_password_generator::CHARSET_ALL,
                                            seed);
  for (std::size_t i = 0; i < 5; i++) {
    std::string password;
    gen.generate_password(password, length);
    std::clog << "Password = '" << password << "'" << std::endl;
  }

  return;
}
