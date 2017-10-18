// test-cmsserver-resource_cardinality.cxx

// Ourselves:
#include <vire/cmsserver/resource_cardinality.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// Third party:
// - Boost:
// #include <boost/filesystem.hpp>

// This project:
#include <vire/vire.h>
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>
#include <vire/resource/testing/populate_manager.h>

void test_resource_cardinality_1();
void test_resource_cardinality_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::resource_cardinality' class."
              << std::endl;

    test_resource_cardinality_1();
    test_resource_cardinality_2();

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

void test_resource_cardinality_1()
{
  std::clog << "\ntest_resource_cardinality_1: basics" << std::endl;

  vire::cmsserver::cardinalities_request_type cardreq;
  cardreq.set_limited_resource(23);
  cardreq.set_limited_resource(24);
  cardreq.set_limited_resource(25);
  cardreq.set_limited_resource(26, 3);
  cardreq.set_unlimited_resource(27);
  cardreq.set_unlimited_resource(28);
  cardreq.set_unlimited_resource(29);
  cardreq.print(std::clog);
  std::clog << std::endl;
  return;
}

void test_resource_cardinality_2()
{
  std::clog << "\ntest_resource_cardinality_2: more" << std::endl;

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


  datatools::properties crconfig;
  std::vector<std::string> unlimited;
  unlimited.push_back("1114");
  unlimited.push_back("1118");
  unlimited.push_back("1122");
  unlimited.push_back("1126");
  // unlimited.push_back("1115"); // trigger an exception because 1115 is LIMITED
  crconfig.store("unlimited", unlimited);
  std::vector<std::string> limited;
  limited.push_back("1115");
  limited.push_back("1116=2");
  limited.push_back("1117=1");
  limited.push_back("1119=1");
  limited.push_back("1121");
  limited.push_back("1124=3"); /* At this point, we cannot check is request for 4 tokens
                                * is possible. However, the maximum number of tokens is tested
                                * from the resource manager.
                                */
  // limited.push_back("1126"); // trigger an exception because 1126 is UNLIMITED
  crconfig.store("limited", limited);

  vire::cmsserver::cardinalities_request_type cardreq;
  cardreq.initialize(crconfig, mgr);
  cardreq.print(std::clog);

  std::clog << std::endl;
  return;
}
