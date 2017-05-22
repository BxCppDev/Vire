//! \file  vire/testing/test-user-group.cxx
//! \brief Vire user group
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
#include <datatools/io_factory.h>
// - Bayeux/cuts:
#include <cuts/not_cut.h>

// This project:
#include <vire/vire.h>
#include <vire/user/group.h>
#include <vire/user/enumerated_user_selector.h>

void test_user_group_1();
void test_user_group_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::user::group' class."
              << std::endl;

    test_user_group_1();
    test_user_group_2();

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

void test_user_group_1()
{
  std::clog << "\ntest_user_group_1: basics" << std::endl;

  // Build a set of arbitrary UIDs:
  std::set<int32_t> some_uids;
  some_uids.insert(100);
  some_uids.insert(999);
  some_uids.insert(1000);
  some_uids.insert(1001);
  some_uids.insert(1001);
  some_uids.insert(1010);
  some_uids.insert(1011);
  some_uids.insert(1048);
  some_uids.insert(1049);
  some_uids.insert(1050);
  some_uids.insert(1100);

  // Create a selector of users (enumeration):
  vire::user::enumerated_user_selector * users_selector_ptr
    = new vire::user::enumerated_user_selector;
  cuts::cut_handle_type users_selector_h(users_selector_ptr);
  for (int32_t uid = 1000; uid < 1050; uid++) {
    users_selector_ptr->add_uid(uid);
  }
  users_selector_ptr->set_name("[1000:1050)");
  users_selector_ptr->set_description("vire::user::enumerated_user_selector");
  users_selector_ptr->initialize_simple();

  vire::user::group users;
  users.set_gid(1000);
  users.set_name("users");
  users.set_display_name("Users");
  users.set_terse_description("This is a mock Vire user group");
  users.grab_auxiliaries().store("type", "all");
  users.grab_auxiliaries().store("mailing_list", "users@myexperiment.org");
  users.grab_auxiliaries().store("chat_room", "users");
  users.set_user_selector_handle(users_selector_h);
  users.tree_dump(std::clog, users.get_display_name() + ":");

  // Save as a configuration:
  datatools::properties users_config;
  users.export_to_config(users_config, vire::user::group::GROUP_XC_DEFAULT);
  users_config.tree_dump(std::clog, "Configuration parameters for the 'users' group: ");

  // Reset:
  users.reset();

  // Restore from a configuration:
  users.initialize_standalone(users_config);
  users.tree_dump(std::clog, users.get_display_name() + " (restored):" );

  {
    for (auto uid : some_uids) {
      std::clog << "User [" << uid << "] ";
      if (users.has_uid(uid)) {
        std::clog << "belongs ";
      } else {
        std::clog << "does not belong ";
      }
      std::clog << "to the '" << users.get_name() << "' group."
                << std::endl;
    }
  }

  // Shifters:
  vire::user::enumerated_user_selector * shifters_selector_ptr
    = new vire::user::enumerated_user_selector;
  shifters_selector_ptr->add_uid(1021);
  shifters_selector_ptr->add_uid(1023);
  shifters_selector_ptr->add_uid(1024);
  shifters_selector_ptr->add_uid(1027);
  shifters_selector_ptr->add_uid(1045);
  shifters_selector_ptr->add_uid(1046);
  shifters_selector_ptr->add_uid(1048);
  shifters_selector_ptr->set_name("1010");
  shifters_selector_ptr->set_description("vire::user::enumerated_user_selector");
  shifters_selector_ptr->initialize_simple();
  cuts::cut_handle_type shifters_selector_h(shifters_selector_ptr);
  vire::user::group shifters;
  shifters.set_gid(1010);
  shifters.set_name("shifters");
  shifters.set_display_name("Experiment shifters");
  shifters.set_terse_description("This is a mock Vire user group");
  shifters.grab_auxiliaries().store("mailing_list", "shifters@myexperiment.org");
  shifters.grab_auxiliaries().store("chat_room", "shifters");
  shifters.set_user_selector_handle(shifters_selector_h);
  shifters.tree_dump(std::clog, shifters.get_display_name() + ":");

  // Save as a configuration:
  datatools::properties shifters_config;
  shifters.export_to_config(shifters_config);
  shifters_config.tree_dump(std::clog, "Configuration parameters for the 'shifters' group: ");

  const std::set<int32_t> & users_uids =
    (dynamic_cast<const vire::user::enumerated_user_selector &>(users.get_user_selector())).get_uids();
  {
    for (auto uid : users_uids) {
      std::clog << "User [" << uid << "] ";
      if (shifters.has_uid(uid)) {
        std::clog << "belongs ";
      } else {
        std::clog << "does not belong ";
      }
      std::clog << "to the '" << shifters.get_name() << "' group."
                << std::endl;
    }
  }

  // Non shifters:
  cuts::not_cut * non_shifters_selector_ptr = new cuts::not_cut;
  non_shifters_selector_ptr->set_name("1011");
  non_shifters_selector_ptr->set_description("cuts::not_cut");
  non_shifters_selector_ptr->set_cut(shifters_selector_h);
  non_shifters_selector_ptr->initialize_simple();
  cuts::cut_handle_type non_shifters_selector_h(non_shifters_selector_ptr);
  vire::user::group non_shifters;
  non_shifters.set_gid(1011);
  non_shifters.set_name("nonshifters");
  non_shifters.set_display_name("Non shifters");
  non_shifters.set_terse_description("This is a mock Vire user group");
  non_shifters.grab_auxiliaries().store("mailing_list", "nonshifters@myexperiment.org");
  non_shifters.grab_auxiliaries().store("chat_room", "nonshifters");
  non_shifters.set_user_selector_handle(non_shifters_selector_h);
  non_shifters.tree_dump(std::clog, non_shifters.get_display_name() + ":");

  // Save as a configuration:
  datatools::properties non_shifters_config;
  non_shifters.export_to_config(non_shifters_config);
  non_shifters_config.tree_dump(std::clog, "Configuration parameters for the 'non_shifters' group: ");

  {
    for (auto uid : users_uids) {
      std::clog << "User [" << uid << "] ";
      if (non_shifters.has_uid(uid)) {
        std::clog << "belongs ";
      } else {
        std::clog << "does not belong ";
      }
      std::clog << "to the '" << non_shifters.get_name() << "' group."
                << std::endl;
    }
  }

  // Calorimeter HV experts:
  vire::user::enumerated_user_selector * calo_hv_experts_selector_ptr
    = new vire::user::enumerated_user_selector;
  calo_hv_experts_selector_ptr->set_name("1020");
  calo_hv_experts_selector_ptr->set_description("vire::user::enumerated_user_selector");
  calo_hv_experts_selector_ptr->add_uid(1008);
  calo_hv_experts_selector_ptr->add_uid(1009);
  calo_hv_experts_selector_ptr->add_uid(1023);
  calo_hv_experts_selector_ptr->add_uid(1024);
  calo_hv_experts_selector_ptr->add_uid(1032);
  calo_hv_experts_selector_ptr->initialize_simple();
  cuts::cut_handle_type calo_hv_experts_selector_h(calo_hv_experts_selector_ptr);
  vire::user::group calo_hv_experts;
  calo_hv_experts.set_gid(1020);
  calo_hv_experts.set_name("calo_hv_experts");
  calo_hv_experts.set_display_name("Calorimeter high voltage experts");
  calo_hv_experts.set_terse_description("This is a mock Vire user group");
  calo_hv_experts.grab_auxiliaries().store("type", "human");
  calo_hv_experts.grab_auxiliaries().store("mailing_list", "calo-hv-experts@myexperiment.org");
  calo_hv_experts.grab_auxiliaries().store("chat_room", "calo-hv-experts");
  calo_hv_experts.set_user_selector_handle(calo_hv_experts_selector_h);
  calo_hv_experts.tree_dump(std::clog, calo_hv_experts.get_display_name() + ":");

  return;
}

void test_user_group_2()
{
  std::clog << "\ntest_user_group_2: reflection" << std::endl;

  // Access meta class for class 'vire::user::group' :
  const camp::Class & groupMetaClass = camp::classByName("vire::user::group");
  camp::UserObject group_obj = groupMetaClass.construct();
  group_obj.call("tree_print", camp::Args(1, "Vire user group : "));
  group_obj.set("gid", camp::Value(1020));
  group_obj.set("name", camp::Value("shifters"));
  group_obj.set("display_name", camp::Value("Experiment shifters"));
  group_obj.set("terse_description", camp::Value("This is a mock Vire user group"));
  camp::Value group_aux_val = group_obj.call("grab_auxiliaries");
  camp::UserObject group_aux_obj = group_aux_val.to<camp::UserObject>();
  group_aux_obj.call("tree_print", camp::Args(1, "Vire user group auxiliaries: "));
  group_aux_obj.call("store_string", camp::Args("chat_room", "#calo-hv-experts", "", false));
  group_aux_obj.call("store_string", camp::Args("mailing_list", "calo-hv-experts@myexperiment.org", "The mailing list of the group", false));
  group_obj.call("tree_print", camp::Args(1, "Vire user group : "));

  return;
}
