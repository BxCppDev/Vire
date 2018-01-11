// test-cmsserver-base_use_case.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <set>

// This project:
#include <vire/vire.h>
#include <vire/resource/resource.h>
#include <vire/resource/role.h>
#include <vire/resource/manager.h>
#include <vire/cmsserver/base_use_case.h>

class dummy_use_case
  : public vire::cmsserver::base_use_case
{
public:

  virtual void _at_initialized_(const datatools::properties & config_)
  {
    return;
  }

  virtual void _at_reset_()
  {
    return;
  }

  virtual void _at_up_()
  {
    return;
  }

  virtual void _at_work_()
  {
    return;
  }

  virtual void _at_down_()
  {
    return;
  }

};


void test_use_case_0();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::resource::manager' class."
              << std::endl;

    boost::filesystem::remove("_roles.def");
    test_use_case_0();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  // boost::filesystem::remove("_roles.def");
  vire::terminate();
  return (error_code);
}

void test_use_case_0()
{
  std::clog << "\ntest_use_case_0: SuperNEMO..." << std::endl;

  // Device manager:
  vire::device::manager dev_mgr;

  datatools::properties dev_mgr_config;
  std::string dev_mgr_config_filename = "@snemo:config/snemo/demonstrator/devices/0.1/manager.conf";
  datatools::fetch_path_with_env(dev_mgr_config_filename);
  dev_mgr_config.read_configuration(dev_mgr_config_filename);
  std::vector<std::string> only_from_paths;
  only_from_paths.push_back("SuperNEMO:/Demonstrator/CMS");
  dev_mgr_config.store("tree.debug", true);
  dev_mgr_config.store("tree.only_from_paths", only_from_paths);
  dev_mgr_config.tree_dump(std::clog, "Virtual device manager's configuration: ");
  dev_mgr.initialize_standalone(dev_mgr_config);
  {
    boost::property_tree::ptree options;
    options.put("title", "Virtual device manager: ");
    options.put("tree.list_instances", true);
    dev_mgr.print_tree(std::clog, options);
    std::clog << std::endl;
  }

  // Resource manager:
  uint32_t res_mgr_flags = 0;
  res_mgr_flags |= vire::resource::manager::LOG_TRACE;
  res_mgr_flags |= vire::resource::manager::DONT_STORE_TABLES;
  res_mgr_flags |= vire::resource::manager::DONT_BACKUP_TABLES;
  vire::resource::manager res_mgr(res_mgr_flags);
  res_mgr.set_name("ResourceManager");
  res_mgr.set_display_name("The SuperNEMO resource manager");
  res_mgr.set_terse_description("This is a mock SuperNEMO resource manager");
  res_mgr.set_roles_table_path("@snemo:config/snemo/demonstrator/cms/common/roles.conf");
  res_mgr.build_resources_from_devices(dev_mgr);
  res_mgr.initialize_simple();
  {
    res_mgr.tree_dump(std::clog, res_mgr.get_display_name());
    std::clog << std::endl;
  }

  vire::cmsserver::session dummySession;


  dummy_use_case dummyUse;



  return;
}
