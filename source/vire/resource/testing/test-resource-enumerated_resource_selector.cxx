// test-resource-enumerated_resource_selector.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>
// - Bayeux/datatools:
#include <datatools/io_factory.h>

// This project:
#include <vire/vire.h>
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>
#include <vire/resource/enumerated_resource_selector.h>
#include <vire/resource/testing/populate_manager.h>

void test_ers_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::resource::enumerated_resource_selector' class."
              << std::endl;

    boost::filesystem::remove("roles.def");
    test_ers_1();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  boost::filesystem::remove("roles.def");
  vire::terminate();
  return (error_code);
}

void test_ers_1()
{
  std::clog << "\ntest_ers_1: " << std::endl;

  std::clog << "\nCreating the resource manager..." << std::endl;
  vire::resource::manager mgr(vire::resource::manager::DONT_STORE_TABLES);
  mgr.set_name("ResourceManager");
  mgr.set_display_name("The resource manager");
  mgr.set_terse_description("This is a mock resource manager");
  mgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  std::clog << "\nPopulating the resource manager with mock resources..." << std::endl;
  vire::resource::testing::populate_manager(mgr);
  mgr.initialize_simple();
  mgr.tree_dump(std::clog, mgr.get_display_name());

  std::clog << "\nCreating an enumerated resource selector..." << std::endl;
  vire::resource::enumerated_resource_selector ers;
  ers.set_resource_mgr(mgr);
  ers.set_name("hv_current_selector");
  std::clog << "\nAdding resource paths to it..." << std::endl;
  ers.add_path("SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_0/board_0/ch_0/current");
  ers.add_path("SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_0/board_0/ch_1/current");
  ers.add_path("SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_0/board_3/ch_3/current");
  ers.add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_1/ch_3/current");
  ers.add_path("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_3/ch_2/current");
  std::clog << "\n Done." << std::endl;
  ers.initialize_simple();
  ers.tree_dump(std::clog, "Resource selector '" + ers.get_name() + "' : ");

  for (int rid = 1200; rid < 2400; rid++) {
    if (mgr.has_resource_by_id(rid)) {
      const vire::resource::resource & r = mgr.get_resource_by_id(rid);
      ers.set_user_data(r.get_path());
      int res = ers.process();
      if (res == cuts::SELECTION_ACCEPTED) {
        std::clog << "RID=[" << rid << "] with path '"
                  << r.get_path() << "' is accepted!" << std::endl;
      }
    }
  }
  std::clog << std::endl;
  ers.tree_dump(std::clog, "Resource selector '" + ers.get_name() + "' : ");
  boost::filesystem::remove("roles.def");
  return;
}
