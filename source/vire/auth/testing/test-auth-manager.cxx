//! \file  vire/auth/testing/test-auth-manager.cxx
//! \brief Test Vire authentication manager
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
#include <list>

// Third party:
// - Boost:
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

// This project:
#include <vire/vire.h>
#include <vire/auth/manager.h>
#include <vire/auth/utils.h>
#include <vire/user/manager.h>

void test_auth_manager_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::auth::manager' class."
              << std::endl;

    bool interactive = false;
    interactive = true;

    test_auth_manager_1(interactive);

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  // boost::filesystem::remove("shadow");
  vire::terminate();
  return (error_code);
}

void test_auth_manager_1(bool interactive_)
{
  std::clog << "\ntest_auth_manager_1: basics" << std::endl;

  vire::user::manager userMgr;
  userMgr.set_name("Users");
  userMgr.set_display_name("Users&groups");
  userMgr.set_terse_description("Users/groups service");
  userMgr.set_users_table_path("users");
  userMgr.set_groups_table_path("groups");
  userMgr.set_dont_load_tables(true);
  userMgr.set_dont_store_tables(true);
  userMgr.set_dont_backup_tables(true);
  userMgr.initialize_simple();
  userMgr.tree_dump(std::clog, "Users/groups service: ");

  vire::auth::manager authMgr;
  authMgr.set_name("Auth");
  authMgr.set_display_name("Authentication");
  authMgr.set_terse_description("User authentication service");
  authMgr.set_credentials_table_path("shadow");
  authMgr.set_dont_load_tables(true);
  authMgr.set_dont_store_tables(false);
  authMgr.set_dont_backup_tables(false);
  authMgr.set_users(userMgr);
  authMgr.set_logging_priority(datatools::logger::PRIO_DEBUG);
  authMgr.initialize_simple();

  authMgr.create_new_credentials("johndoe",
                                 vire::auth::manager::create_with_password
                                 | vire::auth::manager::create_defaults,
                                 "sesame");
  authMgr.create_new_credentials("kingarthur",
                                 vire::auth::manager::create_with_password
                                 | vire::auth::manager::create_defaults,
                                 "Blue!No red!");
  authMgr.create_new_credentials("bevedere",
                                 vire::auth::manager::create_defaults
                                 | vire::auth::manager::create_with_password
                                 | vire::auth::manager::create_lock,
                                 "Arrrrhhhhhhh...");
  vire::auth::credentials & barCred =
    authMgr.create_new_credentials("bar",
                                   vire::auth::manager::create_defaults
                                   // | vire::auth::manager::create_check_login
                                   | vire::auth::manager::create_with_password,
                                   "BAR");
  barCred.set_password_expire(vire::auth::today_from_epoch() - 1);

  authMgr.tree_dump(std::clog, "Authentication service: ");

  typedef std::pair<std::string, std::string> candidate_type;
  std::list<candidate_type> candidates;
  candidates.push_back({"foo", "azerty"});
  candidates.push_back({"johndoe", "qwerty"});
  candidates.push_back({"johndoe", "sesame"});
  candidates.push_back({"kingarthur", "green"});
  candidates.push_back({"kingarthur", "Blue!No red!"});
  candidates.push_back({"bevedere", "Arrrrhhhhhhh...!"});
  candidates.push_back({"bar", "BAR"});

  for (auto c : candidates) {
    int authCode = authMgr.authenticate(c.first, c.second);
    switch (authCode) {
    case vire::auth::manager::AUTH_SUCCESS:
      std::clog << "notice: login='" << c.first<< "': Authentication success!" << std::endl;
      break;
    case vire::auth::manager::AUTH_INVALID_LOGIN:
      std::cerr << "error: login='" << c.first<< "': Invalid login!" << std::endl;
      break;
    case vire::auth::manager::AUTH_LOCKED:
      std::cerr << "error: login='" << c.first<< "': Locked account!" << std::endl;
      break;
    case vire::auth::manager::AUTH_EXPIRED:
      std::cerr << "error: login='" << c.first<< "': Expired account!" << std::endl;
      break;
     case vire::auth::manager::AUTH_INVALID_PASSWORD:
      std::cerr << "error: login='" << c.first<< "': Invalid password!" << std::endl;
      break;
    }
  }

  return;
}
