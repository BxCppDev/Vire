// test-cmsserver-root_session.cxx

// Ourselves:
#include <vire/cmsserver/root_session.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// This project:
#include <vire/vire.h>
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>
#include <vire/resource/testing/populate_manager.h>

void test_root_session_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::root_session' class."
              << std::endl;

    test_root_session_1();

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
  vire::terminate();
  return (error_code);
}

void test_root_session_1()
{
  std::clog << "\ntest_root_session_1: basics" << std::endl;

  std::clog << "\nCreating the resource manager..." << std::endl;
  vire::resource::manager mgr;
  mgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  mgr.set_name("ResourceManager");
  mgr.set_display_name("The resource manager");
  mgr.set_terse_description("This is a mock resource manager");
  mgr.set_roles_table_path("roles.defs");
  mgr.tree_dump(std::clog, mgr.get_display_name() + " (before init)");
  std::clog << "\nPopulating the resource manager with mock resources..." << std::endl;
  vire::resource::testing::populate_manager(mgr);
  mgr.initialize_simple();
  mgr.tree_dump(std::clog, mgr.get_display_name());


  std::clog << std::endl;
  return;
}
