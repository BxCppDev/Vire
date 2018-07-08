// test-cmssubcontractor-sc_description.cxx

// Ourselves:
#include <vire/cmssubcontractor/sc_description.h>

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

void test_sc_description_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmssubcontractor::sc_description' class."
              << std::endl;

    test_sc_description_1();

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

void test_sc_description_1()
{
  std::clog << "\ntest_sc_description_1: basics" << std::endl;

  vire::cmssubcontractor::sc_description scDesc;
  scDesc.set_name("orleans");
  scDesc.set_display_name("CMS/Orleans");
  scDesc.set_terse_description("The Orleans CMS server");
  scDesc.grab_auxiliaries().store("transport_type_id", "rabbitmq");
  scDesc.grab_auxiliaries().store("encoding_type_id", "protobuf");
  scDesc.set_com_login("orleans");
  scDesc.set_com_password("orleans");
  scDesc.set_com_server_host("localhost");
  scDesc.set_com_server_port(5672);
  scDesc.tree_dump(std::clog, "Subcontractor description: ");

  std::clog  << std::endl;
  return;
}

