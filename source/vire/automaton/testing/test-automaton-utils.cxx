 // test-automaton-utils.cxx

// Ourselves:
#include <vire/automaton/utils.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <thread>

// Third party:
// - Boost:
// #include <boost/filesystem.hpp>

// This project:
#include <vire/vire.h>

void test_utils_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::automaton::utils'."
              << std::endl;

    test_utils_1();

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

void test_utils_1()
{
  std::clog << "\ntest_utils_1: basics" << std::endl;

  std::clog << "System latency : " << vire::time::system_latency().count() << std::endl;
  
  std::clog  << std::endl;
  return;
}
