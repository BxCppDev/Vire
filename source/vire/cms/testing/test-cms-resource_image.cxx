// test-resource-resource_image.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>
// - Bayeux/datatools:
#include <datatools/io_factory.h>

// This project:
#include <vire/vire.h>
#include <vire/resource/resource.h>
#include <vire/resource/resource_image.h>

void test_resource_image_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::resource::resource_image' class."
              << std::endl;

    test_resource_image_1();

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

void test_resource_image_1()
{
  std::clog << "\ntest_resource_image_1: basics" << std::endl;

  vire::resource::resource r1;
  r1.set_id(1234);
  r1.set_path("myexperiment:/A/B/C/D0");
  r1.set_access(vire::utility::RW_READABLE);
  r1.set_max_number_of_tokens(3);
  r1.set_name("A.B.C.D0");
  r1.set_display_name("D0 action from A/B/C object");
  r1.set_terse_description("This is a mock Vire resource");
  r1.grab_auxiliaries().store_flag("mock");
  r1.initialize_simple();
  r1.tree_dump(std::clog, r1.get_display_name());
  std::clog << std::endl;

  vire::resource::resource_image r1_img;
  r1_img.set_handler(r1);
  r1_img.set_missing(false);
  r1_img.set_disabled(true);
  r1_img.set_failed(true);
  r1_img.tree_dump(std::clog, "Resource image: ");
  r1_img.indeterminate();
  r1_img.tree_dump(std::clog, "Resource image: ");
  std::clog << std::endl;

  return;
}
