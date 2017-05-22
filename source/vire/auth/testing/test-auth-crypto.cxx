//! \file  vire/auth/testing/test-auth-crypto.cxx
//! \brief Vire encryption
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

// Ourselves:
#include <vire/auth/crypto.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>

// This project:
#include <vire/vire.h>

void test_auth_crypto_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::auth::credentials' class."
              << std::endl;
    bool interactive = false;
    test_auth_crypto_1(interactive);

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

void test_auth_crypto_1(bool interactive_)
{
  std::clog << "\ntest_auth_crypto_1: basics" << std::endl;

  bool debug  = false;
  debug = true;
  std::string key("abcdefgh");
  std::clog << "Key = '" << key << "'\n";
  std::string encrypted1;
  uint32_t encrypt_flags = 0;
  encrypt_flags |= vire::auth::sha512::encrypt_clear_key;
  if (debug) encrypt_flags |= vire::auth::sha512::encrypt_debug;
  if (vire::auth::sha512::encrypt(key, encrypted1, encrypt_flags)) {
    std::clog << "buffer = " << encrypted1.length() << " (should be 20+86 = 106)\n";
    std::clog << "encrypted1 = '" << encrypted1 << "'\n";
    std::clog << "Key = '" << key << "'\n\n";
  }

  std::string encrypted2;
  std::clog << "Key = '" << key << "'\n";
  if (vire::auth::sha512::encrypt(key, encrypted2, encrypt_flags)) {
    std::clog << "encrypted2 = '" << encrypted2 << "'\n";
    std::clog << "Key = '" << key << "'\n\n";
  }

  std::string key2("azerty");
  std::clog << "Key2 = '" << key2 << "'\n";
  std::string encrypted3;
  if (vire::auth::sha512::encrypt(key2, encrypted3, encrypt_flags)) {
    std::clog << "encrypted3 = '" << encrypted3 << "'\n";
    std::clog << "Key = '" << key << "'\n\n";
  }

  std::string salt1;
  if (vire::auth::sha512::extract_salt(encrypted1, salt1)) {
    std::clog << "Salt1 = '" << salt1 << "'\n";
  }
  std::string salt2;
  if (vire::auth::sha512::extract_salt(encrypted2, salt2)) {
    std::clog << "Salt2 = '" << salt2 << "'\n";
  }
  std::string salt3;
  if (vire::auth::sha512::extract_salt(encrypted3, salt3)) {
    std::clog << "Salt3 = '" << salt3 << "'\n";
  }
  std::clog << "\n";

  std::string sesame;
  if (interactive_) {
    std::clog << "Enter your password: ";
    std::cin >> sesame;
  } else {
    std::string input = "abcdefgh";
    std::istringstream inputss(input);
    inputss >> sesame;
  }

  if (vire::auth::sha512::match(sesame, encrypted1)) {
    std::clog << "Password matches!\n";
  } else {
    std::cerr << "error: Invalid password!\n";
  }

  if (vire::auth::sha512::match(sesame, encrypted2)) {
    std::clog << "Password matches!\n";
  } else {
    std::cerr << "error: Invalid password!\n";
  }

  if (vire::auth::sha512::match(sesame, encrypted3)) {
    std::clog << "Password matches!\n";
  } else {
    std::cerr << "error: Invalid password!\n";
  }

  return;
}
