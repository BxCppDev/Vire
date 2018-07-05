// test-simulation-simple_device.cxx

// Ourselves:
#include <vire/simulation/simple_device.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <thread>

// This project:
#include <vire/vire.h>

void test_simple_device_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::simulation::simple_device' class."
              << std::endl;

    test_simple_device_1();

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

void test_simple_device_1()
{
  std::clog << "\ntest_simple_device_1: basics" << std::endl;

  using vire::simulation::simple_device;

  simple_device engine;
  engine.set_name("myEngine");
  engine.set_display_name("My engine");
  engine.set_terse_description("A dummy engine");
  {
    boost::property_tree::ptree popts;
    popts.put("title", "Engine: ");
    engine.print_tree(std::clog, popts);
  }
  
  std::clog  << std::endl;
  return;
}

