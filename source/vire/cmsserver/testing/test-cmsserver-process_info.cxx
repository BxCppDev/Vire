// test-cmsserver-process_info.cxx

// Ourselves:
#include <vire/cmsserver/process_info.h>

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

void test_process_info_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::process_info' class."
              << std::endl;

    test_process_info_1();

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

void test_process_info_1()
{
  std::clog << "\ntest_process_info_1: basics" << std::endl;
  vire::cmsserver::process_info pinfo;
  pinfo.set_name("DAQ");
  pinfo.set_description("Data acquisition process");
  pinfo.set_model_id("snemo::demonstrator::cms::daq_process");
  pinfo.set_config_urn("snemo:demonstrator:daq:config:dbd-1.2");
  pinfo.tree_dump(std::clog, "Session info: ");
  std::clog << std::endl;
  return;
}
