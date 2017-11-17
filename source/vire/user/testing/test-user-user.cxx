//! \file  vire/user/testing/test-user-user.cxx
//! \brief Vire user
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vector>

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>
// - Camp:
#include <camp/enum.hpp>
#include <camp/class.hpp>
#include <camp/args.hpp>
#include <camp/value.hpp>
#include <camp/userobject.hpp>
// - Bayeux/datatools:
// #include <datatools/io_factory.h>

// This project:
#include <vire/vire.h>
#include <vire/user/user.h>

void test_user_1();
void test_user_2();
void test_user_3();
// void test_user_4();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::user::user' class."
              << std::endl;

    test_user_1();
    test_user_2();
    test_user_3();
    //  test_user_4();

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

void test_user_1()
{
  std::clog << "\ntest_user_1: basics" << std::endl;

  vire::user::user admin;
  admin.set_uid(vire::user::user::ADMIN_UID);
  admin.set_name("admin");
  admin.set_display_name("Administrator");
  admin.set_terse_description("This is a mock Vire user");
  admin.grab_auxiliaries().store("type", "system");
  admin.grab_auxiliaries().store("email", "admin@snemo.org");
  admin.tree_dump(std::clog, admin.get_display_name());
  admin.initialize_simple();

  // Save as a configuration:
  datatools::properties admin_config;
  admin.export_to_config(admin_config);
  admin_config.tree_dump(std::clog, "Configuration parameters for 'admin' : ");

  // Reset:
  admin.reset();

  // Restore from a configuration:
  admin.initialize(admin_config);
  admin.tree_dump(std::clog, admin.get_display_name());

  vire::user::user johndoe;
  johndoe.set_uid(1020);
  johndoe.set_name("johndoe");
  johndoe.set_display_name("John Doe");
  johndoe.set_terse_description("This is a mock Vire user");
  johndoe.grab_auxiliaries().store(vire::user::user::type_key(),
                                   vire::user::user::type_human_label());
  johndoe.grab_auxiliaries().store(vire::user::user::phone_key(),
                                   "(+00) 1 23 45 67 89");
  johndoe.grab_auxiliaries().store(vire::user::user::email_key(),
                                   "johndoe@snemo.org");
  johndoe.initialize_simple();
  johndoe.tree_dump(std::clog, johndoe.get_display_name());

  vire::user::user hvmonitor;
  hvmonitor.set_uid(1021);
  hvmonitor.set_name("hvmonitor");
  hvmonitor.set_display_name("HV monitor");
  hvmonitor.set_terse_description("This is a mock Vire user");
  hvmonitor.grab_auxiliaries().store(vire::user::user::type_key(),
                                     vire::user::user::type_robot_label());
  hvmonitor.initialize_simple();
  hvmonitor.tree_dump(std::clog, hvmonitor.get_display_name());

  datatools::properties dummy_config;
  dummy_config.store("name", "dummy");
  dummy_config.store("display_name", "Dummy");
  dummy_config.store("terse_description", "This is a dummy Vire user");
  dummy_config.store("aux.type", "unknown");
  dummy_config.store("aux.email", "dummy@snemo.org");
  dummy_config.store("uid", 1234);

  vire::user::user dummy;
  dummy.initialize(dummy_config);
  dummy.tree_dump(std::clog, dummy.get_display_name());

  return;
}

void test_user_2()
{
  std::clog << "\ntest_user_2: initialization from properties" << std::endl;

  datatools::properties admin_config;
  admin_config.store_integer("uid", vire::user::user::ADMIN_UID);
  // admin_config.store_string("name", "admin");
  // admin_config.store_string("display_name", "Administrator");
  admin_config.store_string("terse_description", "Administration account");
  admin_config.store_string("aux.type", "system");
  admin_config.store_string("aux.email", "admin@snemo.org");

  vire::user::user admin;
  // Explicitely set the name and display name:
  admin.set_name("admin");
  admin.set_display_name("Administrator");
  // Initialize other attributes from a property set:
  admin.initialize(admin_config);
  admin.tree_dump(std::clog, admin.get_display_name());

  return;
}

void test_user_3()
{
  std::clog << "\ntest_user_3: reflection" << std::endl;

  // Access meta class for class 'vire::user::user' :
  const camp::Class & userMetaClass = camp::classByName("vire::user::user");
  camp::UserObject user_obj = userMetaClass.construct();
  user_obj.call("tree_print", camp::Args(1, "Empty Vire user : "));
  user_obj.set("uid", camp::Value(1020));
  user_obj.set("name", camp::Value("johndoe"));
  user_obj.set("display_name", camp::Value("John Doe"));
  user_obj.set("terse_description", camp::Value("This is a mock Vire user"));
  camp::Value  user_aux_val = user_obj.call("grab_auxiliaries");
  camp::UserObject user_aux_obj = user_aux_val.to<camp::UserObject>();
  user_aux_obj.call("store_string",
                    camp::Args("type", "human", "The type of the user", false));
  user_aux_obj.call("store_string",
                    camp::Args("phone", "(+00) 1 23 45 67 89", "The phone number of the user", false));
  user_aux_obj.call("store_string",
                    camp::Args("email", "johndoe@snemo.org", "The email address of the user", false));
  user_aux_obj.call("tree_print",
                    camp::Args(1, "Vire user auxiliaries: "));
  user_obj.call("initialize_simple");
  user_obj.call("tree_print",
                camp::Args(1, "CMS user : "));

  return;
}

// void test_user_4()
// {
//   std::clog << "\ntest_user_4: serialization" << std::endl;

//   vire::user::user johndoe;
//   johndoe.set_uid(1020);
//   johndoe.set_name("johndoe");
//   johndoe.set_display_name("John Doe");
//   johndoe.set_terse_description("This is a mock Vire user");
//   johndoe.grab_auxiliaries().store("type", "human");
//   johndoe.grab_auxiliaries().store("phone", "(+00) 1 23 45 67 89");
//   johndoe.grab_auxiliaries().store("email", "johndoe@snemo.org");
//   johndoe.initialize_simple();
//   johndoe.tree_dump(std::clog, johndoe.get_display_name());

//   {
//     datatools::data_writer writer("test_user.xml");
//     writer.store(johndoe);
//   }

//   johndoe.reset();

//   {
//     datatools::data_reader reader("test_user.xml");
//     reader.load(johndoe);
//   }

//   johndoe.tree_dump(std::clog, johndoe.get_display_name());

//   return;
// }
