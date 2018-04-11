// test-resource-manager.cxx

// Standard library:
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include <set>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>
// - Bayeux/datatools:
#include <datatools/io_factory.h>

// This project:
#include <vire/vire.h>
#include <vire/resource/resource.h>
#include <vire/resource/role.h>
#include <vire/resource/manager.h>
#include <vire/resource/enumerated_resource_selector.h>
#include <vire/device/manager.h>

void test_manager_0();
void test_manager_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::resource::manager' class."
              << std::endl;

    boost::filesystem::remove("_roles.def");
    test_manager_0();
    // test_manager_1();

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

void test_manager_0()
{
  std::clog << "\ntest_manager_0: SuperNEMO..." << std::endl;

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

  uint32_t res_mgr_flags = 0;
  res_mgr_flags |= vire::resource::manager::LOG_TRACE;
  // res_mgr_flags |= vire::resource::manager::DONT_LOAD_TABLES;
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

  return;
}

void test_manager_1()
{
  std::clog << "\ntest_manager_1: basics" << std::endl;
  uint32_t mgr_flags = 0;
  mgr_flags |= vire::resource::manager::LOG_TRACE;
  mgr_flags |= vire::resource::manager::DONT_LOAD_TABLES;
  // mgr_flags |= vire::resource::manager::DONT_STORE_TABLES;
  // mgr_flags |= vire::resource::manager::DONT_BACKUP_TABLES;
  vire::resource::manager mgr(mgr_flags);
  mgr.set_name("ResourceManager");
  mgr.set_display_name("The resource manager");
  mgr.set_terse_description("This is a mock resource manager");
  mgr.set_roles_table_path("_roles.def");
  for (int id = 1; id < 20; id++) {
    vire::resource::resource r;
    r.set_id(id);
    std::ostringstream name_oss;
    std::ostringstream path_oss;
    {
      path_oss << "mysetup:/";
      name_oss << "mysetup.";
      if (id < 6) {
        path_oss << "A/B/C/D" << id;
        name_oss << "A.B.C.D" << id;
      } else {
        path_oss << "foo/bar_" << id;
        name_oss << "foo.bar_" << id;
      }
    }
    r.set_path(path_oss.str());
    r.set_access(vire::utility::RW_READABLE);
    r.set_max_number_of_tokens(3);
    r.set_name(name_oss.str());
    r.set_display_name("A dummy resource");
    r.set_terse_description("This is a mock Vire resource");
    r.grab_auxiliaries().store_flag("mock");
    r.initialize_simple();
    r.tree_dump(std::clog, r.get_display_name());
    std::clog << std::endl;
    mgr.add_resource(r);
  }
  mgr.tree_dump(std::clog, mgr.get_display_name());
  mgr.remove_resource_by_id(4);
  mgr.tree_dump(std::clog, mgr.get_display_name());

  {
    vire::resource::resource r;
    r.set_id(333);
    std::ostringstream path_oss;
    path_oss << "mysetup:/" << "foo" << r.get_id();
    r.set_path(path_oss.str());
    r.set_access(vire::utility::RW_READABLE);
    r.set_max_number_of_tokens(10);
    r.set_name("foo3");
    r.set_display_name("Foo object");
    r.set_terse_description("This is a mock Vire resource");
    r.grab_auxiliaries().store_flag("mock");
    r.initialize_simple();
    r.tree_dump(std::clog, r.get_display_name());
    std::clog << std::endl;
    mgr.add_resource(r);
  }
  // mgr.tree_dump(std::clog, mgr.get_display_name());
  mgr.initialize_simple();
  mgr.build_cached_resource_ids();
  mgr.tree_dump(std::clog, mgr.get_display_name());

  {
    vire::resource::role xfoo;
    xfoo.set_name("fooexpert");
    xfoo.set_display_name("Foo expert");
    xfoo.set_terse_description("The expert role for the foo");
    xfoo.set_id(1000);
    xfoo.set_path("mysetup:/foo/expert");
    xfoo.add_allowed_user(1243);

    // Create an enumerated resource selector:
    cuts::cut_handle_type h_ers(new vire::resource::enumerated_resource_selector);
    {
      vire::resource::enumerated_resource_selector & ers =
        dynamic_cast<vire::resource::enumerated_resource_selector &>(h_ers.grab());
      ers.set_name("xfoo_selector");
      ers.add_path("mysetup:/foo/bar_6");
      ers.add_path("mysetup:/foo/bar_7");
      ers.add_path("mysetup:/foo/bar_18");
      ers.initialize_simple();
    }
    xfoo.set_functional_resource_selector_handle(h_ers);
    xfoo.initialize_simple();
    xfoo.tree_dump(std::clog, xfoo.get_display_name());
    mgr.add_role(xfoo);
  }



  {
    std::set<int32_t> foo_set;
    mgr.build_set_of_resource_ids_from_path_regexp("^mysetup:/.*/B/.*", foo_set);
    std::clog << "Foo B set : [" << foo_set.size() << ']' << std::endl;
    for (auto id : foo_set) {
      std::clog << "  Extracted RID=[" << id << ']' << std::endl;
    }
  }

  {
    std::set<int32_t> foo_set;
    mgr.build_set_of_resource_ids_from_path_regexp("^mysetup:/.*3", foo_set);
    std::clog << "Foo 3 set : [" << foo_set.size() << ']' << std::endl;
    for (auto id : foo_set) {
      std::clog << "  Extracted RID=[" << id << ']' << std::endl;
    }
  }

  //boost::filesystem::remove("_roles.def");
  return;
}
