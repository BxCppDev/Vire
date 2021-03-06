// test-cmsserver-session.cxx

// Ourselves:
#include <vire/cmsserver/session.h>

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
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>
#include <vire/resource/testing/populate_manager.h>
#include <vire/cmsserver/session_info.h>
#include <vire/cmsserver/base_use_case.h>

#include "dummy_use_case.h"
#include "tools.h"

void test_session_0();
void test_session_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::session' class."
              << std::endl;

    test_session_0();
    // test_session_1();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  boost::filesystem::remove("roles.defs");
  boost::filesystem::remove("users.lis");
  boost::filesystem::remove("groups.lis");
  vire::terminate();
  return (error_code);
}

void test_session_1()
{
  std::clog << "\ntest_session_1: basics" << std::endl;

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
  vire::resource::manager rmgr;
  rmgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  rmgr.set_name("ResourceManager");
  rmgr.set_display_name("The resource manager");
  rmgr.set_terse_description("This is a mock resource manager");
  rmgr.set_roles_table_path("roles.defs");
  rmgr.tree_dump(std::clog, rmgr.get_display_name() + " (before init)");
  std::clog << "\nPopulating the resource manager with mock resources..." << std::endl;
  vire::resource::testing::populate_manager(rmgr);
  rmgr.initialize_simple();
  rmgr.tree_dump(std::clog, rmgr.get_display_name());
  std::clog << std::endl;

  // Top session:
  std::clog << "Creating a top session..." << std::endl;
  std::clog << std::endl;

  // vire::cmsserver::session_info top_si;
  // top_si.set_key("Top");
  // top_si.set_description("A dummy top session info record");
  // top_si.set_role_identifier("_universe_");
  // boost::posix_time::ptime now_utc = vire::time::now_utc();
  // boost::posix_time::time_period when(now, now + boost::posix_time::hours(2));
  // top_si.set_when(when);
  // top_si.set_usecase_type_id("snemo::cms::basic_shell_usecase");
  // top_si.grab_usecase_config().store("mode", "expert");
  // top_si.grab_usecase_config().store("required_resources.single_root", "SuperNEMO:/Demonstrator/CMS/Coil");
  // top_si.initialize(umgr, rmgr);

  vire::cmsserver::resource_pool functional;
  functional.add_unlimited(1000);
  functional.add_unlimited(1002);
  functional.add_unlimited(1006);
  functional.add_limited(1001, 1);
  functional.add_limited(1003, 1);
  functional.add_limited(1004, 3);
  functional.tree_dump(std::clog, "Functional:");
  std::clog << std::endl;

  vire::cmsserver::resource_pool distributable;
  distributable.add_unlimited(1010);
  distributable.add_unlimited(1014);
  distributable.tree_dump(std::clog, "Distributable:");
  std::clog << std::endl;

  // std::unique_ptr<vire::cmsserver::base_use_case> shellPtr(new vire::cmsserver::shell_use_case);
  // shellPtr->initialize_simple();

  // vire::cmsserver::session tops;
  // tops.set_id(0);
  // tops.set_functional(functional);
  // tops.set_distributable(distributable);
  // tops.set_use_case(*shellPtr);
  // tops.initialize_simple();
  // tops.tree_dump(std::clog, "Top session:");
  // tops.run();
  // tops.reset();

  std::clog << std::endl;
  return;
}

void test_session_0()
{
  std::clog << "\ntest_session_0: basics" << std::endl;

  // User manager:
  vire::user::manager user_mgr;
  vire::sandbox::init_users(user_mgr);

  // Device manager:
  vire::device::manager dev_mgr;
  vire::sandbox::init_devices(dev_mgr);

  // Resource manager:
  uint32_t res_mgr_flags = 0;
  res_mgr_flags |= vire::resource::manager::LOG_TRACE;
  res_mgr_flags |= vire::resource::manager::DONT_STORE_TABLES;
  res_mgr_flags |= vire::resource::manager::DONT_BACKUP_TABLES;
  vire::resource::manager res_mgr(res_mgr_flags);
  vire::sandbox::init_resources(res_mgr, dev_mgr);

  // Top session:
  std::clog << "Creating a top session..." << std::endl;

  /*
  vire::cmsserver::resource_pool functional;
  functional.add_unlimited(1000);
  functional.add_unlimited(1002);
  functional.add_unlimited(1006);
  functional.add_limited(1001, 1);
  functional.add_limited(1003, 1);
  functional.add_limited(1004, 3);
  functional.tree_dump(std::clog, "Functional:");
  std::clog << std::endl;

  vire::cmsserver::resource_pool distributable;
  distributable.add_unlimited(1010);
  distributable.add_unlimited(1014);
  distributable.tree_dump(std::clog, "Distributable:");
  std::clog << std::endl;

  vire::cmsserver::session tops;
  tops.set_logging_priority(datatools::logger::PRIO_DEBUG);
  tops.set_id(0);
  std::size_t session_duration_sec = 10;
  boost::posix_time::ptime session_start = vire::time::now_utc();
  boost::posix_time::time_duration session_duration = boost::posix_time::seconds(session_duration_sec);
  boost::posix_time::ptime session_stop = session_start + session_duration;
  boost::posix_time::time_period when(session_start, session_stop);
  tops.set_when(when);
  tops.set_functional(functional);
  tops.set_distributable(distributable);
  */
  
  vire::cmsserver::session_info sinfo;
  sinfo.set_key("root");
  sinfo.set_description("The root session");

  boost::posix_time::ptime when_start;
  boost::posix_time::ptime when_stop;
  boost::posix_time::time_period when(when_start, when_stop);
  sinfo.set_when(when);
  sinfo.set_use_case_type_id("vire::cmsserver::test::dummy_use_case");

  vire::cmsserver::session_ptr_type rootSessionPtr = 
    vire::cmsserver::session::create_root_session(sinfo,
                                                  vire::cmsserver::session::CHECK_ONLY);
  
  // vire::cmsserver::test::dummy_use_case dummyUse(5);
  // std::size_t func_up_max_duration   = 5;
  // std::size_t func_work_min_duration = 10;
  // std::size_t func_down_max_duration = 2;
  // dummyUse.set_functional_up_time_sec(func_up_max_duration);
  // dummyUse.set_functional_work_time_sec(func_work_min_duration);
  // dummyUse.set_functional_down_time_sec(func_down_max_duration);
  // std::clog << "Dummy use case: " << std::endl;
  // dummyUse.set_mother_session(tops);
  // dummyUse.print_tree(std::clog);

  // tops.set_use_case(dummyUse);
  // tops.initialize_simple();
  rootSessionPtr->tree_dump(std::clog, "Top session:");
  // tops.start();
  // tops.terminate();

  std::clog << std::endl;
  return;
}
