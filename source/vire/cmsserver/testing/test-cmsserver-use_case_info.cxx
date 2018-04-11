// test-cmsserver-use_case_info.cxx

// Ourselves:
#include <vire/cmsserver/use_case_info.h>

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
#include <vire/user/manager.h>
#include <vire/device/manager.h>
#include <vire/resource/manager.h>

void test_use_case_info_1();

void test_use_case_info_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::use_case_info' class."
              << std::endl;

    test_use_case_info_1();

    //  test_use_case_info_2();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  boost::filesystem::remove("roles.defs");
  boost::filesystem::remove("users.lis");
  boost::filesystem::remove("groups.lis");
  vire::terminate();
  return (error_code);
}

void test_use_case_info_1()
{
  std::clog << "\ntest_use_case_info_1: basics" << std::endl;

  vire::cmsserver::use_case_info ucInfo;
  ucInfo.set_name("Expert");
  ucInfo.set_description("An expert session info record");
  ucInfo.set_role_definition("__universe__");
  ucInfo.set_use_case_type_id("vire::cmsserver::basic_shell_use_case");
  ucInfo.grab_use_case_config().store("mode", "expert");
  ucInfo.grab_use_case_config().store("required_resources.single_root", "SuperNEMO:/Demonstrator/CMS/Coil");
  // for (int resource_id = 1000; resource_id < 1004; resource_id++) {
  //   ucInfo.set_special_functional_limited(resource_id, 2);
  // }
  // for (int resource_id = 1010; resource_id < 1020; resource_id++) {
  //   ucInfo.set_special_distributable_limited(resource_id, 4);
  // }
  ucInfo.initialize_simple();
  ucInfo.tree_dump(std::clog, "Session info: ");

  std::clog  << std::endl;
  return;
}

void test_use_case_info_2()
{
  std::clog << "\ntest_use_case_info_2: configuration" << std::endl;

  std::clog << "\nCreating the user manager..." << std::endl;
  vire::user::manager umgr;
  umgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  datatools::properties umgr_config;
  std::string umgr_config_filename = "@snemo:config/snemo/demonstrator/users/manager.conf";
  datatools::fetch_path_with_env(umgr_config_filename);
  umgr_config.read_configuration(umgr_config_filename);
  umgr.initialize_standalone(umgr_config);
  umgr.tree_dump(std::clog, umgr.get_display_name());

  std::clog << "\nCreating the device manager..." << std::endl;
  vire::device::manager dmgr;
  dmgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  datatools::properties dmgr_config;
  std::string dmgr_config_filename = "@snemo:config/snemo/demonstrator/devices/0.1/manager.conf";
  datatools::fetch_path_with_env(dmgr_config_filename);
  dmgr_config.read_configuration(dmgr_config_filename);
  std::vector<std::string> only_from_paths;
  only_from_paths.push_back("SuperNEMO:/Demonstrator/CMS");
  dmgr_config.store("tree.debug", true);
  dmgr_config.store("tree.only_from_paths", only_from_paths);
  dmgr_config.tree_dump(std::clog, "Virtual device manager's configuration: ");
  dmgr.initialize_standalone(dmgr_config);
  dmgr.tree_dump(std::clog, dmgr.get_display_name());

  std::clog << "\nCreating the resource manager..." << std::endl;
  uint32_t rmgr_flags = vire::resource::manager::LOG_TRACE
    | vire::resource::manager::DONT_LOAD_TABLES
    | vire::resource::manager::DONT_STORE_TABLES;
  vire::resource::manager rmgr(rmgr_flags);
  rmgr.set_logging_priority(datatools::logger::PRIO_TRACE);
  datatools::properties rmgr_config;
  std::string rmgr_config_filename = "@snemo:config/snemo/demonstrator/resources/manager.conf";
  datatools::fetch_path_with_env(rmgr_config_filename);
  rmgr_config.tree_dump(std::clog, "Virtual resource manager's configuration: ");
  rmgr.initialize_simple();
  rmgr.initialize_simple();
  rmgr.tree_dump(std::clog, rmgr.get_display_name());

  {
    std::string rpath = "SuperNEMO:/Demonstrator/CMS/Coil/Monitoring/current/__dp_read__";
    if (rmgr.has_resource_by_path(rpath)) {
      std::clog << "Resource manager has resource with path = '" << rpath << "'" << std::endl;
    }
  }

  vire::cmsserver::use_case_info ucInfo;

  datatools::properties ucCfg;
  ucCfg.store("name", "Dummy");
  ucCfg.store("description", "A dummy session info record");
  ucCfg.store("role.name", "calo_hv2");
  // ucCfg.store("duration", "120 minute");

  // std::vector<std::string> func_unset;
  // func_unset.push_back("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_0/current");
  // func_unset.push_back("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_0/board_0/ch_1/current");
  // ucCfg.store("special_functional_cardinalities.unset", func_unset);

  // std::vector<std::string> dist_limited;
  // dist_limited.push_back("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_0/setpoint_voltage=1");
  // dist_limited.push_back("SuperNEMO:/Demonstrator/CMS/calorimeter/HV/crate_1/board_0/ch_1/setpoint_voltage=1");
  // ucCfg.store("special_distributable_cardinalities.limited", dist_limited);

  ucCfg.store("use_case.type_id", "snemo::cms::foo_use_case");
  ucCfg.store("use_case.config_path", "@snemo:config/cms/server/use_cases/foo/foo_1.0.conf");

  ucInfo.initialize(ucCfg, umgr, rmgr);
  ucInfo.tree_dump(std::clog, "Session info: ");

  std::clog << std::endl;
  return;
}
