//! \file  vire/auth/testing/test-auth-credentials.cxx
//! \brief Vire auth
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
#include <fstream>
#include <string>
#include <exception>

// Third party:
// - Boost:
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

// This project:
#include <vire/vire.h>
#include <vire/auth/credentials.h>

void test_auth_credentials_1();

void test_auth_credentials_2(bool interactive_);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::auth::credentials' class."
              << std::endl;

    bool interactive = false;

    test_auth_credentials_1();

    test_auth_credentials_2(interactive);

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  boost::filesystem::remove("test_auth_credentials.shadow");
  vire::terminate();
  return (error_code);
}

void test_auth_credentials_1()
{
  std::clog << "\ntest_auth_credentials_1: basics" << std::endl;

  vire::auth::credentials uc;
  uc.set_login("johndoe");
  std::string sesame;
  uc.set_password("Sésame, ouvre-toi!");
  uc.set_default_min_max_warn();
  uc.unlock();
  uc.to_table_record_line(std::clog);
  std::clog <<std::endl;

  uc.lock();
  uc.to_table_record_line(std::clog);
  std::clog <<std::endl;

  uc.tree_dump(std::clog, "Credentials: ");
  std::clog <<std::endl;

  return;
}

void test_auth_credentials_2(bool interactive_)
{
  std::clog << "\ntest_auth_credentials_2: I/O" << std::endl;

  {
    std::ofstream ofshwadow("test_auth_credentials.shadow");
    for (int i = 0; i < 5; i++) {
      vire::auth::credentials cred;
      cred.set_login("johndoe"+boost::lexical_cast<std::string>(i));
      std::string sesame;
      cred.set_password("Sésame, ouvre-toi!");
      cred.set_default_min_max_warn();
      cred.unlock();
      cred.to_table_record_line(ofshwadow);
    }
  }

  {
    std::map<std::string, vire::auth::credentials> creds;
    std::ifstream ifshwadow("test_auth_credentials.shadow");
    for (int i = 0; i < 5; i++) {
      vire::auth::credentials cred;
      std::string cred_record;
      std::getline(ifshwadow, cred_record);
      if (!cred.from_table_record_line(cred_record)) {
        std::cerr << "ERROR: Invalid credentials record!" << std::endl;
        break;
      }
      creds[cred.get_login()] = cred;
    }

    if (interactive_) {
      std::string login;
      std::clog << "Login: ";
      std::getline(std::cin, login);
      if (creds.find(login) == creds.end()) {
        std::cerr << "ERROR: Invalid user login!" << std::endl;
      } else {
        const vire::auth::credentials & cred = creds.find(login)->second;
        std::string sesame;
        std::clog << "Password: ";
        std::getline(std::cin, sesame);
        if (!cred.match_password(sesame)) {
          std::cerr << "ERROR: Invalid password!" << std::endl;
        } else {
          std::clog << "INFO: Authentication success." << std::endl;
        }
      }
    }

  }

  return;
}
