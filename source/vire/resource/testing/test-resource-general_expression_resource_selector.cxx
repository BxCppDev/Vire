// test-resource-general_expression_resource_selector.cxx

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
#include <vire/resource/general_expression_resource_selector.h>
#include <vire/resource/testing/populate_manager.h>

void test_gers_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::resource::general_expression_resource_selector' class."
              << std::endl;

    boost::filesystem::remove("roles.def");
    test_gers_1();

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

void test_gers_1()
{
  std::clog << "\ntest_gers_1: " << std::endl;

  std::clog << "\nCreating the resource manager..." << std::endl;
  vire::resource::manager mgr;
  mgr.set_name("ResourceManager");
  mgr.set_display_name("The resource manager");
  mgr.set_terse_description("This is a mock resource manager");
  std::clog << "\nPopulating the resource manager with mock resources..." << std::endl;
  mgr.set_roles_table_path("roles.def");
  vire::resource::testing::populate_manager(mgr);
  mgr.initialize_simple();
  mgr.tree_dump(std::clog, mgr.get_display_name());

  std::clog << "\nCreating a general expression resource selector..." << std::endl;
  vire::resource::general_expression_resource_selector gers;
  gers.set_resource_mgr(mgr);
  gers.set_name("hv_current_selector");
  std::clog << "\nSetting general expression..." << std::endl;

  std::string expr =
    " ( (SuperNEMO:/Demonstrator/CMS/calorimeter/HV1 * SuperNEMO:/Demonstrator/CMS/calorimeter/HV2) "
    "   + [SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_1/board_1/ch_0/current;SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_1/board_3/ch_2/current] "
    "   - {SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_1/board_1/ch_1/current; SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_1/board_1/ch_3/current} "
    ") " // * @ro "
    ;

  gers.set_expression(expr);

  //  gers.set_expression("(SuperNEMO:/Demonstrator/CMS/calorimeter/HV1 * SuperNEMO:/Demonstrator/CMS/calorimeter/HV2) + [SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_1/board_1/ch_0/current;SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_1/board_1/ch_15/current] - {SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_1/board_1/ch_12/current; SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_1/board_1/ch_15/current}");
  // %\"SuperNEMO:/Demonstrator/CMS/tracker/HV/crate_2/board.*/ch_1[1,2]/current\"");
  std::clog << "\n Done." << std::endl;
  gers.initialize_simple();
  gers.tree_dump(std::clog, "Resource selector '" + gers.get_name() + "' : ");

  for (int rid = 1200; rid < 2400; rid++) {
    if (mgr.has_resource_by_id(rid)) {
      const vire::resource::resource & r = mgr.get_resource_by_id(rid);
      gers.set_user_data(r.get_path());
      int res = gers.process();
      if (res == cuts::SELECTION_ACCEPTED) {
        std::clog << "RID=[" << rid << "] with path '"
                  << r.get_path() << "' is accepted!" << std::endl;
      }
    }
  }
  std::clog << std::endl;
  gers.tree_dump(std::clog, "Resource selector '" + gers.get_name() + "' : ");
  boost::filesystem::remove("roles.def");
  return;
}
