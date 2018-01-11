// test-cmsserver-kernel.cxx

// Ourselves:
#include <vire/cmsserver/kernel.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <unistd.h>

// Third party:
// - Boost:
#include <boost/filesystem.hpp>

// This project:
#include <vire/vire.h>
#include <vire/auth/manager.h>
#include <vire/user/manager.h>
#include <vire/user/testing/populate_manager.h>
#include <vire/device/manager.h>
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>
#include <vire/resource/testing/populate_manager.h>
#include <vire/logging/simple_logging_service.h>
#include <vire/cmsserver/root_process.h>
#include <vire/cmsserver/session_info.h>
#include <vire/cmsserver/utils.h>
#include <vire/time/utils.h>

void test_kernel_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::kernel' class."
              << std::endl;

    test_kernel_1();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "[fatal] " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "[fatal] " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  boost::filesystem::remove("roles");
  boost::filesystem::remove("users");
  boost::filesystem::remove("groups");
  boost::filesystem::remove("shadow");
  vire::terminate();
  return (error_code);
}

void test_kernel_1()
{
  std::clog << "\ntest_kernel_1: basics" << std::endl;

  // Set global logging level:
  vire::cmsserver::logging(datatools::logger::PRIO_TRACE);

  std::clog << "\nCreating the logger..." << std::endl;
  vire::logging::simple_logging_service logger;
  logger.set_logging_priority(datatools::logger::PRIO_TRACE);
  logger.set_name("Log");
  logger.set_display_name("Logging service");
  logger.set_terse_description("Application logging service");
  logger.set_use_logconsole(true);
  logger.initialize_simple();
  logger.tree_dump(std::clog, "Simple global logging: ");

  std::clog << "\nCreating the users manager..." << std::endl;
  vire::user::manager umgr;
  umgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  umgr.set_name("Users");
  umgr.set_display_name("The users manager");
  umgr.set_terse_description("This is a mock users manager");
  umgr.set_users_table_path("users");
  umgr.set_groups_table_path("groups");
  umgr.initialize_simple();
  std::clog << "\nPopulating the users manager with mock material..." << std::endl;
  vire::user::testing::populate_manager(umgr);
  umgr.tree_dump(std::clog, umgr.get_display_name());

  std::clog << "\nCreating the authentication manager..." << std::endl;
  vire::auth::manager amgr;
  amgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  amgr.set_name("Auth");
  amgr.set_display_name("The authentication manager");
  amgr.set_terse_description("This is a mock authentication manager");
  amgr.set_credentials_table_path("shadow");
  amgr.set_users(umgr);
  amgr.initialize_simple();
  amgr.tree_dump(std::clog, amgr.get_display_name());

  std::clog << "\nCreating the resource manager..." << std::endl;
  vire::resource::manager rmgr;
  rmgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  rmgr.set_name("Resources");
  rmgr.set_display_name("The resource manager");
  rmgr.set_terse_description("This is a mock resource manager");
  rmgr.set_roles_table_path("roles");
  rmgr.tree_dump(std::clog, rmgr.get_display_name() + " (before init)");
  std::clog << "\nPopulating the resource manager with mock resources..." << std::endl;
  vire::resource::testing::populate_manager(rmgr);
  rmgr.initialize_simple();
  rmgr.tree_dump(std::clog, rmgr.get_display_name());

  std::clog << "\nCreating the kernel..." << std::endl;
  vire::cmsserver::kernel cmsk;
  cmsk.set_logging_priority(datatools::logger::PRIO_TRACE);
  cmsk.set_setup_label("SuperNEMO");
  cmsk.set_setup_version("0.1");
  cmsk.set_setup_description("The SuperNEMO experiment CMS kernel");
  cmsk.set_logger(logger);
  cmsk.set_users_manager(umgr);
  cmsk.set_authentication_manager(amgr);
  cmsk.set_resources_manager(rmgr);
  cmsk.tree_dump(std::clog, cmsk.get_setup_description());
  cmsk.initialize_simple();
  cmsk.tree_dump(std::clog, cmsk.get_setup_description());
  if (cmsk.has_root()) {
    cmsk.get_root().tree_dump(std::clog, "Root process: ", "[trace] ");
  }
  std::clog << std::endl;

  {
    std::clog << "Creating root subprocess 'p1'..." << std::endl;
    vire::cmsserver::session_info si1;
    si1.init_set_id(1);
    si1.init_set_description("A session info");
    si1.init_set_uid(0);
    si1.init_set_gid(0);
    si1.init_set_role_id(1000);
    boost::posix_time::time_period si1_period = vire::time::invalid_time_interval();
    std::string si1_period_str = "(now;2 hour)";
    vire::time::parse_time_interval(si1_period_str, si1_period);
    si1.init_set_when(si1_period);
    si1.init_unset_special_functional(1112);
    si1.tree_dump(std::clog, si1.get_description());
    std::clog << std::endl;

    if (cmsk.has_root()) {
      datatools::command::returned_info cri
        = cmsk.grab_root().command_new_subprocess("p1",
                                                "vire::cmsserver::dummy_process",
                                                  si1);
      if (cri.is_failure()) {
        std::cerr << "Failure: " << cri.get_error_message() << std::endl;
      }
    }
  }

  if (cmsk.has_root()) {
    cmsk.get_root().tree_dump(std::clog, "Root process (2): ", "[trace] ");
    std::clog << std::endl;
  }

  {
    std::clog << "Creating root subprocess 'p2'..." << std::endl;
    vire::cmsserver::session_info si2;
    si2.init_set_id(2);
    si2.init_set_description("Another session info");
    si2.init_set_uid(0);
    si2.init_set_gid(0);
    si2.init_set_role_id(1001);
    boost::posix_time::time_period si2_period = vire::time::invalid_time_interval();
    std::string si2_period_str = "(now;4 hour)";
    vire::time::parse_time_interval(si2_period_str, si2_period);
    si2.init_set_when(si2_period);
    // Add special request on functional cardinality:
    si2.init_set_special_functional_limited(1004, 1);
    si2.init_set_special_functional_limited(1044, 1);
    si2.tree_dump(std::clog, si2.get_description());
    std::clog << std::endl;

    if (cmsk.has_root()) {
      datatools::command::returned_info cri
        = cmsk.grab_root().command_new_subprocess("p2",
                                                  "vire::cmsserver::dummy_process", si2);
      if (cri.is_failure()) {
        std::cerr << "Failure: " << cri.get_error_message() << std::endl;
      }
    }
  }

  {
    std::clog << "Creating p2 subprocess 'p2.a'..." << std::endl;
    vire::cmsserver::base_process & p2 = cmsk.grab_root().grab_subprocess("p2");
    vire::cmsserver::session_info si2a;
    si2a.init_set_id(vire::cmsserver::session_info::MAX_USER_ID);
    si2a.init_set_description("Yet another session info");
    si2a.init_set_uid(0);
    si2a.init_set_gid(0);
    boost::posix_time::time_period si2a_period = vire::time::invalid_time_interval();
    std::string si2a_period_str = "(now;3 hour)";
    vire::time::parse_time_interval(si2a_period_str, si2a_period);
    si2a.init_set_when(si2a_period);
    // Add special request on functional cardinality:
    si2a.init_unset_special_functional(1006);
    si2a.init_unset_special_functional(1038);
    si2a.tree_dump(std::clog, si2a.get_description());
    std::clog << std::endl;
     datatools::command::returned_info cri
       = p2.command_new_subprocess("p2.a",
                                   "vire::cmsserver::dummy_process",
                                   si2a);
    if (cri.is_failure()) {
      std::cerr << "Failure: " << cri.get_error_message() << std::endl;
    }
  }

  if (cmsk.has_root()) {
    cmsk.get_root().tree_dump(std::clog, "Root process (3): ", "[trace] ");
    std::clog << std::endl;
    cmsk.grab_root().tree(std::clog, "");
    std::clog << std::endl;
    {
      vire::cmsserver::base_process & p2 = cmsk.grab_root().grab_subprocess("p2");
      p2.command_delete_subprocess("p2.a");
    }

    {
      datatools::command::returned_info cri
        = cmsk.grab_root().command_delete_subprocess("p1");
      if (cri.is_failure()) {
        std::cerr << "Failure: " << cri.get_error_message() << std::endl;
      }
    }

    {
      datatools::command::returned_info cri
        = cmsk.grab_root().command_delete_subprocess("p2");
      if (cri.is_failure()) {
        std::cerr << "Failure: " << cri.get_error_message() << std::endl;
      }
    }

    cmsk.get_root().tree_dump(std::clog, "Root process (4): ", "[trace] ");
    std::clog << std::endl;
  }

  //  usleep(500000);

  std::clog << "\ntest_kernel_1: End." << std::endl;
  std::clog << std::endl;
  return;
}
