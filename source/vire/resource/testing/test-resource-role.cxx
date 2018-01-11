// test-resource-role.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <datatools/io_factory.h>

// This project:
#include <vire/vire.h>
#include <vire/resource/role.h>
#include <vire/resource/enumerated_resource_selector.h>

void test_role_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::resource::resource' class."
              << std::endl;

    test_role_1();

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

void test_role_1()
{
  std::clog << "\ntest_role_1: basics" << std::endl;

  vire::resource::role r;
  r.set_name("calohvexpert");
  r.set_display_name("CalorimeterHVExpert");
  r.set_terse_description("The calorimeter HV expert role");
  r.grab_auxiliaries().store("emergency", "(+33)567 890 123");
  r.set_id(12);
  r.set_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/expert");
  r.add_allowed_user(1243);
  r.add_allowed_user(1244);
  r.add_allowed_user(1267);
  r.add_allowed_group(1042);

  // Create an enumerated resource selector:
  cuts::cut_handle_type h_ers(new vire::resource::enumerated_resource_selector);
  {
    vire::resource::enumerated_resource_selector & ers =
      dynamic_cast<vire::resource::enumerated_resource_selector &>(h_ers.grab());
    ers.set_name("hv_calo_current_selector");
    ers.add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_0/current");
    ers.add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_1/current");
    ers.add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_2/current");
    ers.add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_3/current");
    ers.initialize_simple();
  }
  r.set_functional_resource_selector_handle(h_ers);
  r.initialize_simple();
  r.tree_dump(std::clog, r.get_display_name());

  std::clog << std::endl;
  return;
}
