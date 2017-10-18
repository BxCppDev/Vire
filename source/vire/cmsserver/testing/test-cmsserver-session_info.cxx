// test-cmsserver-session_info.cxx

// Ourselves:
#include <vire/cmsserver/session_info.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// Third party:
// - Boost:
#include <boost/filesystem.hpp>

// This project:
#include <vire/vire.h>
#include <vire/user/manager.h>
#include <vire/user/testing/populate_manager.h>
#include <vire/resource/manager.h>
#include <vire/resource/testing/populate_manager.h>

void test_session_info_1();

void test_session_info_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::session_info' class."
              << std::endl;

    test_session_info_1();

    test_session_info_2();

    std::clog << "The end." << std::endl;
  }
  catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  }
  catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  boost::filesystem::remove("roles.defs");
  boost::filesystem::remove("users.lis");
  boost::filesystem::remove("groups.lis");
  vire::terminate();
  return (error_code);
}

void test_session_info_1()
{
  std::clog << "\ntest_session_info_1: basics" << std::endl;

  vire::cmsserver::session_info sinfo;
  sinfo.tree_dump(std::clog, "Session info: ");
  std::clog << std::endl;

  sinfo.set_id(1000);
  sinfo.set_key("Dummy");
  sinfo.set_description("A dummy session info record");
  sinfo.set_role_id(2014);
  boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
  boost::posix_time::time_period when(now, now + boost::posix_time::hours(2));
  sinfo.set_when(when);
  sinfo.set_usecase_id("basic_shift");
  for (int resource_id = 1000; resource_id < 1004; resource_id++) {
    sinfo.set_special_functional_limited(resource_id, 2);
  }
  for (int resource_id = 1010; resource_id < 1020; resource_id++) {
    sinfo.set_special_distributable_limited(resource_id, 4);
  }
  sinfo.initialize_simple();
  sinfo.tree_dump(std::clog, "Session info: ");

  std::clog << std::endl;
  return;
}

void test_session_info_2()
{
  std::clog << "\ntest_session_info_2: configuration" << std::endl;

  std::clog << "\nCreating the user manager..." << std::endl;
  vire::user::manager umgr;
  umgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  umgr.set_name("UserManager");
  umgr.set_display_name("The user manager");
  umgr.set_terse_description("This is a mock user manager");
  umgr.set_users_table_path("users.lis");
  umgr.set_groups_table_path("groups.lis");
  umgr.initialize_simple();
  std::clog << "\nPopulating the user manager with mock resources..." << std::endl;
  vire::user::testing::populate_manager(umgr);
  umgr.tree_dump(std::clog, umgr.get_display_name());

  std::clog << "\nCreating the resource manager..." << std::endl;
  uint32_t rmgr_flags = vire::resource::manager::LOG_TRACE
    | vire::resource::manager::DONT_LOAD_TABLES
    | vire::resource::manager::DONT_STORE_TABLES;
  vire::resource::manager rmgr(rmgr_flags);
  // rmgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  rmgr.set_name("ResourceManager");
  rmgr.set_display_name("The resource manager");
  rmgr.set_terse_description("This is a mock resource manager");
  rmgr.set_roles_table_path("roles.defs");
  rmgr.tree_dump(std::clog, rmgr.get_display_name() + " (before init)");
  std::clog << "\nPopulating the resource manager with mock resources..." << std::endl;
  vire::resource::testing::populate_manager(rmgr);
  rmgr.initialize_simple();
  rmgr.tree_dump(std::clog, rmgr.get_display_name());

  {
    std::string rpath = "SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_0/current";
    if (rmgr.has_resource_by_path(rpath)) {
      std::clog << "Resource manager has resource with path = '" << rpath << "'" << std::endl;
    }
  }

  vire::cmsserver::session_info sinfo;

  datatools::properties sicfg;
  sicfg.store("id", 23);
  sicfg.store("key", "Dummy");
  sicfg.store("description", "A dummy session info record");
  sicfg.store("role", "calo_hv2");
  sicfg.store("when", "(now ; 4 hour)");
  sicfg.store("usecase", "Foo");

  std::vector<std::string> func_unset;
  func_unset.push_back("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_0/current");
  func_unset.push_back("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_1/current");
  sicfg.store("special_functional_cardinalities.unset", func_unset);

  std::vector<std::string> dist_limited;
  dist_limited.push_back("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_0/setpoint_voltage=1");
  dist_limited.push_back("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_1/setpoint_voltage=1");
  sicfg.store("special_distributable_cardinalities.limited", dist_limited);

  sinfo.initialize(sicfg, umgr, rmgr);
  sinfo.tree_dump(std::clog, "Session info: ");

  std::clog << std::endl;
  return;
}
