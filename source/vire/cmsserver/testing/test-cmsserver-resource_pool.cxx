// test-cmsserver-resource_pool.cxx

// Ourselves:
#include <vire/cmsserver/resource_pool.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// This project:
#include <vire/vire.h>
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>
#include <vire/resource/testing/populate_manager.h>

void test_resource_pool_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::resource_pool' class."
              << std::endl;

    test_resource_pool_1();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  // boost::filesystem::remove("roles.defs");
  vire::terminate();
  return (error_code);
}

void test_resource_pool_1()
{
  std::clog << "\ntest_resource_pool_1: basics" << std::endl;

  std::clog << "\nCreating the resource manager..." << std::endl;
  uint32_t mgr_flags = vire::resource::manager::LOG_TRACE
    | vire::resource::manager::DONT_LOAD_TABLES
    | vire::resource::manager::DONT_STORE_TABLES;
  vire::resource::manager mgr(mgr_flags);
  // mgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  mgr.set_name("ResourceManager");
  mgr.set_display_name("The resource manager");
  mgr.set_terse_description("This is a mock resource manager");
  mgr.set_roles_table_path("roles.defs");
  // mgr.tree_dump(std::clog, mgr.get_display_name() + " (before init)");

  std::clog << "\nPopulating the resource manager with mock resources..." << std::endl;
  vire::resource::testing::populate_manager(mgr);
  mgr.tree_dump(std::clog, mgr.get_display_name() + " (before init): " );
  mgr.initialize_simple();
  mgr.tree_dump(std::clog, mgr.get_display_name());

  vire::resource::role universe_role;
  mgr.build_universe_role(universe_role);
  universe_role.tree_dump(std::clog, "Universe role : ");
  std::clog << std::endl;

  std::clog << "Preparing root resource pool : " << std::endl;

  vire::cmsserver::resource_pool root_pool_f;
  // Empty pool:
  root_pool_f.initialize();
  std::cout << "# Root functional resource pool: " << std::endl;
  root_pool_f.print(std::cout, 1);
  std::cout << std::endl;

  vire::cmsserver::resource_pool root_pool_d;
  {
    vire::cmsserver::cardinalities_request_type distributable_token_request;
    // Specific cardinality requests on some resource token:
    distributable_token_request.set_limited_resource(1012, 2);
    distributable_token_request.set_limited_resource(1028, 1);
    distributable_token_request.set_limited_resource(1036, 1);
    // Following resources are explicitely discarded from the pool:
    distributable_token_request.unset_resource(1000);
    distributable_token_request.unset_resource(1001);
    distributable_token_request.unset_resource(1002);
    distributable_token_request.unset_resource(1006);
    distributable_token_request.unset_resource(1007);
    distributable_token_request.unset_resource(1008);
    distributable_token_request.unset_resource(1009);
    std::clog << "Distributable resource token cardinality request : " << std::endl;
    distributable_token_request.print(std::clog);
    vire::cmsserver::resource_pool::init_root(root_pool_d,
                                              mgr,
                                              distributable_token_request,
                                              vire::cmsserver::resource_pool::CARD_ALL);
    root_pool_d.tree_dump(std::clog, "Distributable root resource pool: ");
  }
  std::cout << "# Root distributable resource pool: " << std::endl;
  root_pool_d.print(std::cout, 1);
  std::cout << std::endl;

  vire::cmsserver::resource_pool root_pool_d_backup = root_pool_d;

  const vire::resource::role & tracko_hv1 = mgr.get_role_by_name("tracko_hv1");
  tracko_hv1.tree_dump(std::clog, "Role 'tracko_hv1' : ");
  std::clog << std::endl;

  std::clog << "Preparing TrackoHV1 resource pools : " << std::endl;
  vire::cmsserver::resource_pool tracko_hv1_pool_f;
  vire::cmsserver::resource_pool tracko_hv1_pool_d;
  {
    // Specific requests on resource token cardinalities:
    vire::cmsserver::cardinalities_request_type functional_token_request;
    functional_token_request.set_unlimited_resource(1010);
    functional_token_request.set_unlimited_resource(1014);
    functional_token_request.set_unlimited_resource(1016);
    functional_token_request.set_unlimited_resource(1018);
    functional_token_request.set_limited_resource(1003);
    functional_token_request.set_limited_resource(1004);
    functional_token_request.set_limited_resource(1012, 2);
    functional_token_request.set_limited_resource(1013);
    vire::cmsserver::cardinalities_request_type distributable_token_request;
    distributable_token_request.set_unlimited_resource(1114);
    distributable_token_request.set_unlimited_resource(1118);
    distributable_token_request.set_unlimited_resource(1120);
    distributable_token_request.set_unlimited_resource(1122);
    distributable_token_request.set_unlimited_resource(1126);
    distributable_token_request.set_limited_resource(1119);
    distributable_token_request.set_limited_resource(1121);
    distributable_token_request.set_limited_resource(1123);
    distributable_token_request.set_limited_resource(1124,2);
    distributable_token_request.set_limited_resource(1125);
    distributable_token_request.set_limited_resource(1127);
    std::clog << "# TrackoHV1 import functional resource tokens... " << std::endl;
    vire::cmsserver::resource_pool::load_daughter_from_parent(root_pool_d,
                                                              tracko_hv1_pool_f,
                                                              mgr,
                                                              functional_token_request);
    std::clog << "# done." << std::endl;
    std::cout << "# TrackoHV1 import distributable resource tokens... " << std::endl;
    vire::cmsserver::resource_pool::load_daughter_from_parent(root_pool_d,
                                                              tracko_hv1_pool_d,
                                                              mgr,
                                                              distributable_token_request);
    std::cout << "# done." << std::endl;
  }

  std::cout << "# TrackoHV1 functional resource pool: " << std::endl;
  tracko_hv1_pool_f.print(std::cout, 1);
  std::cout << std::endl;

  std::cout << "# TrackoHV1 distributable resource pool: " << std::endl;
  tracko_hv1_pool_d.print(std::cout, 1);
  std::cout << std::endl;

  if (root_pool_d == root_pool_d_backup) {
    std::clog << "Root pool ---> Same " << std::endl;
  } else {
    std::clog << "Root pool ---> Different " << std::endl;
  }

  std::cout << "# Root distributable resource pool (2): " << std::endl;
  root_pool_d.print(std::cout, 1);
  std::cout << std::endl;

  // Restore tokens to parent root pool:
  vire::cmsserver::resource_pool::restore_parent_from_daughter(root_pool_d, tracko_hv1_pool_f);
  vire::cmsserver::resource_pool::restore_parent_from_daughter(root_pool_d, tracko_hv1_pool_d);

  std::cout << "# Root distributable resource pool (3): " << std::endl;
  root_pool_d.print(std::cout, 1);
  std::cout << std::endl;

  if (root_pool_d == root_pool_d_backup) {
    std::clog << "Root pool ---> Restored " << std::endl;
  } else {
    std::clog << "Root pool ---> Not restored " << std::endl;
  }

  std::clog << std::endl;
  return;
}
