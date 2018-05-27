// test-cmsserver-uc_time_constraints.cxx

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
#include "tools.h"

void test_uctc_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::uc_time_constraints' class."
              << std::endl;

    test_uctc_1();

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

void test_uctc_1()
{
  std::clog << "\ntest_uctc_1: basics" << std::endl;

  vire::cmsserver::uc_time_constraints uctc;
  uctc.set_distributable_up_duration_max(boost::posix_time::seconds(2));
  uctc.set_functional_work_duration_min(boost::posix_time::seconds(10));
  uctc.set_functional_work_duration_max(boost::posix_time::seconds(20));
  uctc.set_distributable_down_duration_max(boost::posix_time::seconds(2));

  boost::property_tree::ptree options;
  {
    datatools::i_tree_dumpable::base_print_options popts;
    popts.title = "Use case time constraints: ";
    popts.indent = "[info] ";
    popts.export_to(options);
  }
  uctc.print_tree(std::clog, options);
  
  std::clog << std::endl;
  return;
}
