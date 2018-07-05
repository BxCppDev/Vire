// -*- mode: c++; -*-
// tools.h

#ifndef VIRE_CMSSERVER_TEST_TOOLS_H
#define VIRE_CMSSERVER_TEST_TOOLS_H

// This project:
#include <vire/user/manager.h>
#include <vire/device/manager.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace sandbox {

    void init_users(vire::user::manager & users_)
    {
      users_.set_logging_priority(datatools::logger::PRIO_TRACE);
      datatools::properties users_config;
      std::string users_config_filename = "@snemo:config/snemo/demonstrator/users/manager.conf";
      datatools::fetch_path_with_env(users_config_filename);
      users_config.read_configuration(users_config_filename);
      users_.initialize_standalone(users_config);
      users_.tree_dump(std::clog, users_.get_display_name());
      return;
    }

    void init_devices(vire::device::manager & devices_)
    {
      datatools::properties dev_mgr_config;
      std::string dev_mgr_config_filename = "@snemo:config/snemo/demonstrator/devices/0.1/manager.conf";
      datatools::fetch_path_with_env(dev_mgr_config_filename);
      dev_mgr_config.read_configuration(dev_mgr_config_filename);
      std::vector<std::string> only_from_paths;
      only_from_paths.push_back("SuperNEMO:/Demonstrator/CMS");
      // dev_mgr_config.store("tree.debug", true);
      dev_mgr_config.store("tree.only_from_paths", only_from_paths);
      dev_mgr_config.tree_dump(std::clog, "Virtual device manager's configuration: ");
      devices_.initialize_standalone(dev_mgr_config);
      {
        boost::property_tree::ptree options;
        options.put("title", "Virtual device manager: ");
        options.put("tree.list_instances", true);
        devices_.print_tree(std::clog, options);
        std::clog << std::endl;
      }
      return;
    }

    void init_resources(vire::resource::manager & resources_, const vire::device::manager & devices_)
    {
      resources_.set_name("resources");
      resources_.set_display_name("The SuperNEMO resource manager");
      resources_.set_terse_description("This is a mock SuperNEMO resource manager");
      resources_.set_roles_table_path("@snemo:config/snemo/demonstrator/cms/common/roles.conf");
      resources_.build_resources_from_devices(devices_);
      resources_.initialize_simple();
      {
        resources_.tree_dump(std::clog, resources_.get_display_name());
        std::clog << std::endl;
      }
      return;
    }

    void init_resources2(vire::resource::manager & resources_)
    {
      uint32_t resources_flags = vire::resource::manager::LOG_TRACE
        | vire::resource::manager::DONT_LOAD_TABLES
        | vire::resource::manager::DONT_STORE_TABLES;
      vire::resource::manager resources(resources_flags);
      resources.set_logging_priority(datatools::logger::PRIO_TRACE);
      datatools::properties resources_config;
      std::string resources_config_filename = "@snemo:config/snemo/demonstrator/resources/manager.conf";
      datatools::fetch_path_with_env(resources_config_filename);
      resources_config.tree_dump(std::clog, "Virtual resource manager's configuration: ");
      resources.initialize_simple();
      resources.tree_dump(std::clog, resources.get_display_name());
      std::clog << std::endl;
      return;
    }

  } // namespace sandbox

} // namespace vire

#endif // VIRE_CMSSERVER_TEST_TOOLS_H

// end
