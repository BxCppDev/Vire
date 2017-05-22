//! \file  vire/testing/test-user-manager.cxx
//! \brief Vire user and group manager
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
#include <boost/filesystem.hpp>
// - Camp:
#include <camp/enum.hpp>
#include <camp/class.hpp>
#include <camp/args.hpp>
#include <camp/value.hpp>
#include <camp/userobject.hpp>
// - Bayeux/datatools:
#include <datatools/io_factory.h>

// This project:
#include <vire/vire.h>
#include <vire/user/user.h>
#include <vire/user/group.h>
#include <vire/user/manager.h>
#include <vire/user/enumerated_user_selector.h>

void test_user_manager_0();
void test_user_manager_1();
void test_user_manager_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::user::manager' class."
              << std::endl;

    test_user_manager_0();
    test_user_manager_1();
    test_user_manager_2();

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

void test_user_manager_0()
{
  std::clog << "\ntest_user_manager_0: basics:" << std::endl;

  std::clog << "\ntest_user_manager_0: User manager..." << std::endl;
  vire::user::manager umgr;
  umgr.set_logging_priority(datatools::logger::PRIO_DEBUG);
  umgr.set_name("UM");
  umgr.set_terse_description("The user manager");
  umgr.set_standard_user_min_uid(1000);
  umgr.set_standard_group_min_gid(1000);
  umgr.set_users_table_path("users.lis");
  umgr.set_groups_table_path("groups.lis");
  umgr.initialize_simple();
  umgr.tree_dump(std::clog, umgr.get_display_name() + ": ");
  std::clog << "\ntest_user_manager_0: User manager is initialized." << std::endl;
  std::clog << std::endl;

  // Add some users:
  std::clog << "\ntest_user_manager_0: Add some users..." << std::endl;
  datatools::properties user_config;
  for (int i = 0; i < 40; i++) {
    std::ostringstream user_name_oss;
    user_name_oss << "user" << i;
    user_config.update("name", user_name_oss.str());
    std::ostringstream user_display_name_oss;
    user_display_name_oss << "User #" << i;
    user_config.update("display_name", user_display_name_oss.str());
    user_config.update("terse_description", "A user");
    std::ostringstream user_email_oss;
    user_email_oss << "user" << i << "@myexperiment.fr";
    user_config.update("aux.email", user_email_oss.str());
    if (! umgr.has_user_by_name(user_name_oss.str())) {
      vire::user::user & u = umgr.create_new_user(user_config);
      std::clog << "User with UID=[" << u.get_uid() << "] is created." << std::endl;
    }
  }
  umgr.tree_dump(std::clog, umgr.get_display_name() + " (with 40 users): ");
  std::clog << std::endl;

  // Add some user groups:
  {
    std::clog << "\ntest_user_manager_0: Add the 'users' groups..." << std::endl;
    datatools::properties group_config;
    group_config.store("name", "users");
    group_config.store("display_name", "Users");
    group_config.store("terse_description", "The group with all users");
    group_config.store("aux.mailing_list", "users-l@myexperiment.org",
                       "Dedicated mailing list address");
    group_config.store("aux.chat_room", "@users",
                       "Dedicated chat room");
    group_config.store("aux.group_leader.uid", 1023,
                       "Group leader identifier (UID)");
    group_config.store("selector_type_id", "cuts::accept_cut");

    if (! umgr.has_group_by_name(group_config.fetch_string("name"))) {
      vire::user::group & ug = umgr.create_new_group(group_config);
      ug.tree_dump(std::clog, ug.get_display_name());
    }
  }
  umgr.tree_dump(std::clog, umgr.get_display_name() + " (with 'users' group): ");
  std::clog << std::endl;

  {
    std::clog << "\ntest_user_manager_0: Add the 'some' groups..." << std::endl;
    datatools::properties some_group_config;
    some_group_config.store("name", "some");
    some_group_config.store("display_name", "Some users");
    some_group_config.store("description", "A group with some users");
    some_group_config.store("aux.mailing_list", "users-l@myexperiment.org",
                            "Dedicated mailing list address");
    some_group_config.store("aux.chat_room", "@users",
                            "Dedicated chat room");
    some_group_config.store("aux.group_leader.uid", 1010,
                            "Group leader identifier (UID)");
    some_group_config.store("selector_type_id", "vire::user::enumerated_user_selector");
    std::vector<int32_t> uids;
    uids.push_back(1023);
    uids.push_back(1010);
    uids.push_back(1011);
    uids.push_back(1012);
    uids.push_back(1013);
    some_group_config.store("selector.uids", uids, "The list of user identifiers (UIDs)");

    if (! umgr.has_group_by_name(some_group_config.fetch_string("name"))) {
      vire::user::group & ug = umgr.create_new_group(some_group_config, 2001);
      ug.tree_dump(std::clog, ug.get_display_name());
    }

  }
  umgr.tree_dump(std::clog, umgr.get_display_name() + " (with 'some' group): ");
  std::clog << std::endl;

  std::clog << std::endl;
  return;
}

void test_user_manager_1()
{
  std::clog << "\ntest_user_manager_1: basics" << std::endl;

  std::clog << "\ntest_user_manager_1: User manager..." << std::endl;
  uint32_t umgr_flags = 0;
  umgr_flags |= vire::user::manager::LOG_TRACE;
  umgr_flags |= vire::user::manager::DONT_LOAD_TABLES;
  // umgr_flags |= vire::user::manager::DONT_STORE_TABLES;
  // umgr_flags |= vire::user::manager::DONT_BACKUP_TABLES;
  vire::user::manager umgr(umgr_flags);
  umgr.set_logging_priority(datatools::logger::PRIO_DEBUG);
  umgr.set_name("UM");
  umgr.set_display_name("User manager");
  umgr.set_terse_description("The user manager");
  umgr.set_standard_user_min_uid(2000);
  umgr.set_standard_group_min_gid(2000);
  umgr.set_users_table_path("users.lis");
  umgr.set_groups_table_path("groups.lis");
  umgr.initialize_simple();
  umgr.tree_dump(std::clog, umgr.get_display_name() + ": ");
  std::clog << std::endl;

  std::clog << "Create more groups..." << std::endl;
  if (!umgr.has_group_by_name(vire::user::group::admin_group())) {
    vire::user::group & g =
      umgr.create_new_group_by_name(vire::user::group::admin_group(),
                                    vire::user::group::ADMIN_GID);
    g.set_display_name("Administrators");
    g.set_terse_description("This is a mock Vire group");
    g.grab_auxiliaries().store("email", vire::user::group::admin_group() +"@myexperiment.org");
    // cuts::cut_handle_type root_selector(new vire::user::enumerated_user_selector);
    // g.set_user_selector_handle(root_selector);
    g.tree_dump(std::clog, g.get_display_name());
  } else {
    std::clog << "User manager already has a '" << vire::user::group::admin_group() << "' group." << std::endl;
  }

  if (!umgr.has_group_by_name("users")) {
    vire::user::group & g = umgr.create_new_group_by_name("users");
    g.set_display_name("Standard users");
    g.set_terse_description("This is a mock Vire group");
    g.grab_auxiliaries().store("email", "users@myexperiment.org");
    g.tree_dump(std::clog, g.get_display_name());
  } else {
    std::clog << "User manager already has a 'users' group." << std::endl;
  }

  if (!umgr.has_group_by_name("experts")) {
    vire::user::group & g = umgr.create_new_group_by_name("experts");
    g.set_display_name("Expert users");
    g.set_terse_description("This is a mock Vire group");
    g.grab_auxiliaries().store("email", "experts@myexperiment.org");
    g.tree_dump(std::clog, g.get_display_name());
  } else {
    std::clog << "User manager already has a 'experts' group." << std::endl;
  }

  std::clog << "Create more users..." << std::endl;
  if (!umgr.has_user_by_name(vire::user::user::admin_user())) {
    vire::user::user & u = umgr.create_new_user_by_name(vire::user::user::admin_user(),
                                                        vire::user::user::ADMIN_UID);
    u.set_display_name("Administrator");
    u.set_terse_description("This is a mock Vire user");
    u.grab_auxiliaries().store("type", "human");
    u.grab_auxiliaries().store("email", vire::user::user::admin_user() + "@myexperiment.org");
    umgr.add_user_in_group(u.get_uid(), vire::user::group::ADMIN_GID);
    u.tree_dump(std::clog, u.get_display_name());
  }

  if (!umgr.has_user_by_name("johndoe")) {
    vire::user::user & u = umgr.create_new_user_by_name("johndoe");
    u.set_display_name("John Doe");
    u.set_terse_description("This is a mock Vire user");
    u.grab_auxiliaries().store("type", "human");
    u.grab_auxiliaries().store("phone", "(+00) 1 23 45 67 89");
    u.grab_auxiliaries().store("email", "johndoe@myexperiment.org");
    umgr.add_user_in_group(u.get_uid(), umgr.get_group_by_name("experts").get_gid());
    u.tree_dump(std::clog, u.get_display_name());
  }

  if (!umgr.has_user_by_name("hvmonitor")) {
    vire::user::user & u = umgr.create_new_user_by_name("hvmonitor");
    u.set_display_name("HV monitor");
    u.set_terse_description("This is a mock Vire robot user");
    u.grab_auxiliaries().store("type", "robot");
    umgr.add_user_in_group(u.get_uid(), umgr.get_group_by_name("experts").get_gid());
    u.tree_dump(std::clog, u.get_display_name());
  }

  if (!umgr.has_user_by_name("foo")) {
    vire::user::user & u = umgr.create_new_user_by_name("foo");
    u.set_display_name("Foo user");
    u.set_terse_description("This is a mock Vire user");
    u.grab_auxiliaries().store("type", "human");
    u.grab_auxiliaries().store("phone", "(+00) 9 87 65 43 21");
    u.grab_auxiliaries().store("email", "foo@myexperiment.org");
    umgr.add_user_in_group(u.get_uid(), umgr.get_group_by_name("experts").get_gid());
    u.tree_dump(std::clog, u.get_display_name());
  }

  if (!umgr.has_user_by_name("bar")) {
    vire::user::user & u = umgr.create_new_user_by_name("bar");
    u.set_display_name("Bar user");
    u.set_terse_description("This is a mock Vire user");
    u.grab_auxiliaries().store("type", "human");
    u.grab_auxiliaries().store("phone", "(+00) 6 19 82 73 54");
    u.grab_auxiliaries().store("email", "bar@myexperiment.org");
    umgr.add_user_in_group(u.get_uid(), umgr.get_group_by_name("experts").get_gid());
    u.tree_dump(std::clog, u.get_display_name());
  }

  umgr.tree_dump(std::clog, umgr.get_display_name() + ": ");

  /*
  // if (umgr.has_users_table()) {
  //   umgr.store_tables();
  // }

  // umgr.remove_all_users();
  // if (umgr.has_users_table()) {
  //   umgr.load_tables();
  // }
  // umgr.tree_dump(std::clog, umgr.get_display_name() + " (re-loaded): ");
  umgr.reset();
  */

  // boost::filesystem::remove("users.lis");
  // boost::filesystem::remove("users.lis~");
  std::clog << "\ntest_user_manager_1: end." << std::endl;
  std::clog << std::endl;
  return;
}

void test_user_manager_2()
{
  std::clog << "\ntest_user_manager_2: reflection" << std::endl;
  std::clog << std::endl;

  // Access meta class for class 'vire::user::manager' :
  const camp::Class & user_mgr_MetaClass = camp::classByName("vire::user::manager");
  camp::UserObject user_mgr_obj = user_mgr_MetaClass.construct();
  user_mgr_obj.set("name",                   camp::Value("UM"));
  user_mgr_obj.set("display_name",           camp::Value("User manager"));
  user_mgr_obj.set("terse_description",      camp::Value("This is a mock Vire user manager"));
  user_mgr_obj.set("standard_user_min_uid",  camp::Value(1000));
  user_mgr_obj.set("standard_group_min_gid", camp::Value(1000));
  user_mgr_obj.set("users_table_path",       camp::Value("users.lis"));
  user_mgr_obj.set("groups_table_path",      camp::Value("groups.lis"));
  user_mgr_obj.set("dont_load_tables",       camp::Value(true));
  // user_mgr_obj.set("dont_store_tables",      camp::Value(true));
  // user_mgr_obj.set("dont_backup_tables",     camp::Value(true));
  user_mgr_obj.call("initialize_simple");
  std::clog << "TEST4" << std::endl;
  /*
  camp::Value  user_mgr_aux_val = user_mgr_obj.call("grab_auxiliaries");
  camp::UserObject user_mgr_aux_obj = user_mgr_aux_val.to<camp::UserObject>();
  user_mgr_aux_obj.call("store_string", camp::Args("category", "system", "The category of the component", false));
  user_mgr_obj.call("smart_print", camp::Args(1, "User manager: ", ">>> "));
  */
  {
    camp::Value user_val = user_mgr_obj.call("create_new_user_by_name", camp::Args("hanny", -1));
    camp::UserObject user_obj = user_val.to<camp::UserObject>();
    user_obj.set("display_name",      camp::Value("Hannibal"));
    user_obj.set("terse_description", camp::Value("This is a mock Vire user"));
    camp::Value  user_aux_val = user_obj.call("grab_auxiliaries");
    camp::UserObject user_aux_obj = user_aux_val.to<camp::UserObject>();
    user_aux_obj.call("tree_print",   camp::Args(1, "Vire user auxiliaries: "));
    user_aux_obj.call("store_string", camp::Args("type", "human", "The type of the user", false));
    user_aux_obj.call("store_string", camp::Args("phone", "(+00) 1 23 45 67 89", "The phone number of the user", false));
    user_aux_obj.call("store_string", camp::Args("email", "johndoe@myexperiment.org", "The email address of the user", false));
  }

  {
    camp::Value user_val  = user_mgr_obj.call("create_new_user_by_name", camp::Args("coilmonitor", -1));
    camp::UserObject user_obj = user_val.to<camp::UserObject>();
    user_obj.set("display_name",      camp::Value("Coil monitor"));
    user_obj.set("terse_description", camp::Value("This is a mock Vire user"));
    camp::Value user_aux_val  = user_obj.call("grab_auxiliaries");
    camp::UserObject user_aux_obj = user_aux_val.to<camp::UserObject>();
    user_aux_obj.call("tree_print",   camp::Args(1, "Vire user auxiliaries: "));
    user_aux_obj.call("store_string", camp::Args("type", "robot", "The type of the user", false));
  }

  {
    camp::Value user_val  = user_mgr_obj.call("create_new_user_by_name", camp::Args("hvcontrol", -1));
    camp::UserObject user_obj = user_val.to<camp::UserObject>();
    user_obj.set("display_name",      camp::Value("HV control"));
    user_obj.set("terse_description", camp::Value("This is a mock Vire user"));
    camp::Value user_aux_val  = user_obj.call("grab_auxiliaries");
    camp::UserObject user_aux_obj = user_aux_val.to<camp::UserObject>();
    user_aux_obj.call("tree_print",   camp::Args(1, "Vire user auxiliaries: "));
    user_aux_obj.call("store_string", camp::Args("type", "robot", "The type of the user", false));
    user_aux_obj.call("store_string", camp::Args("level", "expert", "The expertise level of the user", false));
  }
  user_mgr_obj.call("tree_print", camp::Args(1, "Vire user manager : "));

  {
    camp::Value user_val = user_mgr_obj.call("remove_user_by_name", camp::Args("hanny"));
  }
  {
    camp::Value user_val = user_mgr_obj.call("remove_user_by_name", camp::Args("coilmonitor"));
  }
  {
    camp::Value user_val = user_mgr_obj.call("remove_user_by_name", camp::Args("hvcontrol"));
  }

  user_mgr_obj.call("reset");

  std::clog << std::endl;

  return;
}
