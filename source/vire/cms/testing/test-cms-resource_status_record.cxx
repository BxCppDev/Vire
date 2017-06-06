// test-resource-resource_status_record.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

// Third party:
#include <bayeux/datatools/io_factory.h>

// This project:
#include <vire/vire.h>
#include <vire/cms/resource_status_record.h>

void test_resource_status_record_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::resource::resource' class."
              << std::endl;

    test_resource_status_record_1();

    std::clog << "The end." << std::endl;
  }  catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return (error_code);
}

void test_resource_status_record_1()
{
  std::clog << "\ntest_resource_status_record_1: basics" << std::endl;

  vire::cms::resource_status_record rsr;
  rsr.set_path("SuperNEMO:/Demonstrator/CMS/Coil/Control/Current/__dp_read__");
  rsr.set_timestamp(vire::time::now());
  rsr.set_pending();
  std::clog << "Resource '" << rsr.get_path() << "' : " << std::endl;
  std::clog << " -> status record : " << rsr.get_status_string() << std::endl;
  std::clog << std::endl;

  rsr.set_missing();

  std::clog << "Resource '" << rsr.get_path() << "' : " << std::endl;
  std::clog << " -> status record : " << rsr.get_status_string() << std::endl;
  std::clog << std::endl;

  rsr.unset_all();

  std::clog << "Resource '" << rsr.get_path() << "' : " << std::endl;
  std::clog << " -> status record : " << rsr.get_status_string() << std::endl;
  std::clog << std::endl;

  rsr.set_disabled();

  rsr.tree_dump(std::clog, "Resource status record: ");
  std::clog << std::endl;

  {
    datatools::data_writer dw("test-resource-resource_status_record.xml");
    dw.store(rsr);
  }

  return;
}
