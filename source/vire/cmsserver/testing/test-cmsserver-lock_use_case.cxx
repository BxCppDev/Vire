// test-cmsserver-lock_use_case.cxx

// Ourselves:
#include <vire/cmsserver/lock_use_case.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// This project:
#include <vire/vire.h>

#include "dummy_use_case.h"
#include "tools.h"

void test_lock_use_case_1();
void test_lock_use_case_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::lock_use_case' class."
              << std::endl;

    test_lock_use_case_1();
    test_lock_use_case_2();

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

void test_lock_use_case_1()
{
  std::clog << "\ntest_lock_use_case_1: dry run..." << std::endl;

  vire::cmsserver::lock_use_case lock(vire::cmsserver::lock_use_case::INIT_DRY_RUN);
  lock.set_name("lock");
  lock.set_display_name("Lock");
  lock.set_terse_description("Test lock use case");
  lock.set_logging_priority(datatools::logger::PRIO_TRACE);
  lock.set_duration(boost::posix_time::time_duration(0, 0, 5));
  lock.set_tick(boost::posix_time::time_duration(0, 0, 1));
  std::clog << "Initialization..." << std::endl;
  lock.initialize_simple();
  
  lock.print_tree(std::clog);
  
  std::clog << "Termination." << std::endl;
  lock.finalize();
  
  std::clog << std::endl;
  return;
}

void test_lock_use_case_2()
{
  std::clog << "\ntest_lock_use_case_2: running..." << std::endl;

  vire::cmsserver::lock_use_case lock;
  lock.set_name("lock");
  lock.set_display_name("Lock");
  lock.set_terse_description("Test lock use case");
  lock.set_logging_priority(datatools::logger::PRIO_TRACE);
  lock.set_duration(boost::posix_time::time_duration(0, 0, 8));
  lock.set_tick(boost::posix_time::milliseconds(500));
  std::clog << "Initialization..." << std::endl;
  lock.initialize_simple();
  
  lock.print_tree(std::clog);

  vire::cmsserver::running::run_stage_completion rsc = lock.run_prepare();
  rsc = lock.run_distributable_up();
  rsc = lock.run_functional_up();
  rsc = lock.run_functional_work();
  rsc = lock.run_functional_down();
  rsc = lock.run_distributable_down();
  rsc = lock.run_terminate();
  
  std::clog << "Termination." << std::endl;
  lock.finalize();
  
  std::clog << std::endl;
  return;
}
