// test-cmsserver-gate.cxx

// Ourselves:
#include <vire/cmsserver/gate.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// This project:
#include <vire/vire.h>
#include <vire/com/manager.h>
#include <vire/com/access_hub.h>
#include <vire/com/domain.h>


void test_gate_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::gate' class."
              << std::endl;

    test_gate_1();

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

void test_gate_1()
{
  std::clog << "\ntest_gate_1: basics" << std::endl;

  std::string system_access_login_prefix = "__snemod__";
  std::string domain_name_prefix        = "/__snemod__";
  vire::utility::model_identifier default_transport_driver_id("vire::rabbitmq::transport_driver");
  vire::utility::model_identifier default_encoding_driver_id("vire::com::protobuf_encoding_driver",
                                                             datatools::version_id(3));

  vire::com::manager theCom;
  theCom.set_name("Com");
  theCom.set_display_name("Communication");
  theCom.set_terse_description("Vire Server Communication Service");
  theCom.set_app_category(vire::cms::application::CATEGORY_SERVER);
  theCom.set_default_encoding_driver_type_id(default_encoding_driver_id);
  theCom.set_default_transport_driver_type_id(default_transport_driver_id);
  theCom.set_system_access_login_prefix(system_access_login_prefix);
  theCom.set_domain_name_prefix(domain_name_prefix);
  theCom.initialize_simple();
  std::clog << theCom.get_display_name() << std::endl;
  theCom.print_tree(std::clog);
  std::clog << std::endl;

  vire::cmsserver::gate theGate;
  theGate.set_name("Gate");
  theGate.set_display_name("Vire Server Gate");
  theGate.set_terse_description("Vire Server Gate Service");
  theGate.set_com(theCom);
  theGate.initialize_simple();

  vire::com::access_hub & theGateAccessHub = theGate.grab_gate_hub();
  std::clog << "Gate access hub:" << std::endl;
  theGateAccessHub.print_tree(std::clog);
  std::clog << std::endl;

  std::clog << theGate.get_display_name() << std::endl;
  theGate.print_tree(std::clog);
  std::clog << std::endl;

  
  
                  
  std::clog << std::endl;
  return;
}
