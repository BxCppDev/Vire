// test-cmsserver-use_case.cxx

// Ourselves:
#include <vire/cmsserver/base_use_case.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// This project:
#include <vire/vire.h>

#include "dummy_use_case.h"
#include "foo_use_case.h"
#include "tools.h"

void test_use_case_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::base_use_case' class."
              << std::endl;

    test_use_case_1();

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

void test_use_case_1()
{
  std::clog << "\ntest_use_case_1: basics" << std::endl;

  vire::cmsserver::test::dummy_use_case dummy;
  dummy.set_name("dummy");
  dummy.set_display_name("Dummy");
  dummy.set_terse_description("Test use case");
  dummy.print_tree(std::clog);
  
  std::clog << std::endl;
  return;
}
