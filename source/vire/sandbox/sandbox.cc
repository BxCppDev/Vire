// vire/sandbox/sandbox.cc

// Ourselves:
#include <vire/sandbox/sandbox.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/exception.h>

namespace vire {

  namespace sandbox {

    void init_users(vire::user::manager & users_,
                    const std::string & sandbox_,
                    const std::string & tag_)
    {
      DT_THROW_IF(users_.is_initialized(), std::logic_error,
                  "Users service is already initialized!");
      users_.set_logging_priority(datatools::logger::PRIO_TRACE);

      datatools::properties users_config;
      std::string users_config_filename = "@" + sandbox_ + ":config/users/test1/manager.conf";
      datatools::fetch_path_with_env(users_config_filename);
      users_config.read_configuration(users_config_filename);
      users_config.print_tree(std::clog,
                              datatools::i_tree_dumpable::make_base_print_options("Users config:"));
     
      users_.initialize_standalone(users_config);
      return;
    }

    void init_devices(vire::device::manager & devices_,
                      const std::string & sandbox_,
                      const std::string & tag_ )
    {
      DT_THROW_IF(devices_.is_initialized(), std::logic_error,
                  "Devices service is already initialized!");
      datatools::properties devices_config;
      std::string devices_config_filename = "@" + sandbox_ + ":config/devices/" + tag_ + "/manager.conf";
      datatools::fetch_path_with_env(devices_config_filename);
      devices_config.read_configuration(devices_config_filename);
      std::vector<std::string> only_from_paths;
      only_from_paths.push_back("SuperNEMO:/Demonstrator/CMS");
      devices_config.store("tree.only_from_paths", only_from_paths);
      devices_config.print_tree(std::clog,
                                datatools::i_tree_dumpable::make_base_print_options("Devices config:"));
      
      devices_.initialize_standalone(devices_config);
      {
        boost::property_tree::ptree options;
        options.put("title", "Virtual device manager: ");
        options.put("tree.list_instances", true);
        devices_.print_tree(std::clog, options);
        std::clog << std::endl;
      }
      return;
    }

    void init_resources(vire::resource::manager & resources_,
                        const vire::device::manager & devices_,
                        const std::string & sandbox_,
                        const std::string & tag_ )
    {
      DT_THROW_IF(resources_.is_initialized(), std::logic_error,
                  "Resources service is already initialized!");
      DT_THROW_IF(!devices_.is_initialized(), std::logic_error,
                  "Devices service is not initialized!");
      datatools::properties resources_config;
      std::string resources_config_filename = "@" + sandbox_ + ":config/resources/" + tag_ + "/manager.conf";
      datatools::fetch_path_with_env(resources_config_filename);
      resources_config.read_configuration(resources_config_filename);
      std::clog << "Virtual resource manager's configuration: " << std::endl;
      resources_config.print_tree(std::clog,
                                  datatools::i_tree_dumpable::make_base_print_options("Resources config:"));

      resources_.set_devices_service(devices_);
      resources_.initialize_standalone(resources_config);
      return;
    }

    /* void init_resources2(vire::resource::manager & resources_) */
    /* { */
    /*   uint32_t resources_flags = vire::resource::manager::LOG_TRACE */
    /*     | vire::resource::manager::DONT_LOAD_TABLES */
    /*     | vire::resource::manager::DONT_STORE_TABLES; */
    /*   vire::resource::manager resources(resources_flags); */
    /*   resources.set_logging_priority(datatools::logger::PRIO_TRACE); */
    /*   datatools::properties resources_config; */
    /*   std::string resources_config_filename = "@snemock:config/resources/test1/manager.conf"; */
    /*   datatools::fetch_path_with_env(resources_config_filename); */
    /*   resources_config.tree_dump(std::clog, "Virtual resource manager's configuration: "); */
    /*   resources.initialize_simple(); */
    /*   resources.tree_dump(std::clog, resources.get_display_name()); */
    /*   std::clog << std::endl; */
    /*   return; */
    /* } */

    void init_com(vire::com::manager & com_,
                  const vire::resource::manager & resources_,
                  const vire::cms::application::category_type appcat_,
                  const std::string & sandbox_,
                  const std::string & tag_ )
    {
      DT_THROW_IF(com_.is_initialized(), std::logic_error,
                  "Com service is already initialized!");
      DT_THROW_IF(!resources_.is_initialized(), std::logic_error,
                  "Resources service is not initialized!");

      datatools::properties com_config;
      std::string com_config_filename = "@" + sandbox_ + ":config/cms/" + tag_ + "/server/conf.d/com_service.conf";
      datatools::fetch_path_with_env(com_config_filename);
      com_config.read_configuration(com_config_filename);
      com_config.print_tree(std::clog,
                            datatools::i_tree_dumpable::make_base_print_options("Com config:"));

      com_.set_resources(resources_);
      com_.initialize_standalone(com_config);
    
      // std::string domain_name_prefix = "/__snemod__";
      // vire::utility::model_identifier default_transport_driver_type_id("vire::rabbitmq::transport_driver");
      // vire::utility::model_identifier default_encoding_driver_type_id("vire::com::protobuf_encoding_driver",
      //                                                                 datatools::version_id(3));
      // com_.set_resources(resources_);
      // com_.set_application_category(appcat_);
      // com_.set_name("com");
      // com_.set_display_name("Communication");
      // com_.set_terse_description("SuperNEMO Vire Server Communication Service");
      // com_.set_domain_name_prefix(domain_name_prefix);
      // com_.set_default_encoding_driver_type_id(default_encoding_driver_type_id);
      // com_.set_default_transport_driver_type_id(default_transport_driver_type_id);
      // com_.set_resources(resources_);
      // com_.initialize_simple();
      // std::clog << std::endl;
      return;
    }
     
  } // namespace sandbox

} // namespace vire
