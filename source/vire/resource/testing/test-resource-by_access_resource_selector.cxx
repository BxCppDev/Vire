// test-resource-by_access_resource_selector.cxx

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
//#include <vire/resource/utils.h>
#include <vire/resource/by_access_resource_selector.h>
#include <vire/resource/testing/populate_manager.h>

void test_bars_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::resource::by_access_resource_selector' class."
              << std::endl;
    
    boost::filesystem::remove("roles.def");
    test_bars_1();

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

void test_bars_1()
{
  std::clog << "\ntest_bars_1: " << std::endl;

  std::clog << "\nCreating the resource manager..." << std::endl;
  vire::resource::manager mgr;
  mgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  mgr.set_name("ResourceManager");
  mgr.set_display_name("The resource manager");
  mgr.set_terse_description("This is a mock resource manager");
  mgr.set_roles_table_path("roles.def");
  // mgr.set_roles_table_path("roles.defs");
  // mgr.tree_dump(std::clog, mgr.get_display_name() + " (before init): ");
  std::clog << "\nPopulating the resource manager with mock resources..." << std::endl;
  vire::resource::testing::populate_manager(mgr);
  mgr.initialize_simple();
  mgr.tree_dump(std::clog, mgr.get_display_name());

  {
    std::clog << "\nCreating a by access resource selector..." << std::endl;
    vire::resource::by_access_resource_selector bars;
    //bars.set_logging_priority(datatools::logger::PRIO_TRACE);
    bars.set_resource_mgr(mgr);
    bars.set_name("wo_selector");
    std::clog << "\nSetting write only access selection ..." << std::endl;
    bars.set_access(vire::utility::RW_WRITABLE);
    //bars.set_access(2);
    //bars.set_access_by_label("ACCESS_WRITE_ONLY");
    std::clog << "\n Done." << std::endl;
    bars.initialize_simple();
    std::clog << "\n Init." << std::endl;
    bars.tree_dump(std::clog, "Resource selector '" + bars.get_name() + "' : ");

    for (int rid = 1200; rid < 2400; rid++) {
      if (mgr.has_resource_by_id(rid)) {
        std::clog << "\tTesting rid=" << rid << std::endl;
        const vire::resource::resource & r = mgr.get_resource_by_id(rid);
        bars.set_user_data(r.get_path());
        std::clog << "\t...processsing cut with rid=" << rid << std::endl;
        int res = bars.process();
        if (res == cuts::SELECTION_ACCEPTED) {
          std::clog << "\t\tRID=[" << rid << "] with path '"
                    << r.get_path() << "' is accepted!" << std::endl;
          /*    } else if (res == cuts::SELECTION_REJECTED) {
                std::clog << "RID=[" << rid << "] with path '"
                << r.get_path() << "' is rejected!" << std::endl;
                } else if (res == cuts::SELECTION_INAPPLICABLE) {
                std::clog << "RID=[" << rid << "] with path '"
                << r.get_path() << "' is inapplicable!" << std::endl;
                } else {
                std::clog << "RID=[" << rid << "] with path '"
                << r.get_path() << "' is IMPOSSIBLE!" << std::endl; */
        }
      }
    }
    std::clog << std::endl;
    bars.tree_dump(std::clog, "Resource selector '" + bars.get_name() + "' : ");

    std::clog << "Invoke selector destructor..." << std::endl;
  }
  std::clog << "Invoke manager destructor..." << std::endl;
  boost::filesystem::remove("roles.def");
  return;
}
