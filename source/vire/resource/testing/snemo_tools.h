// snemo_tools.h

#ifndef VIRE_RESOURCE_TESTING_SNEMO_TOOLS_H
#define VIRE_RESOURCE_TESTING_SNEMO_TOOLS_H

// Standard Library
#include <string>
#include <vector>
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/service_manager.h>

// This project:
#include <vire/resource/manager.h>
#include <vire/resource/resource.h>
#include <vire/resource/role.h>
#include <vire/resource/enumerated_resource_selector.h>

namespace vire {
  namespace resource {
    namespace testing {
      namespace snemo {

        void populate_service_manager(datatools::service_manager & services_);

        void populate_service_manager(datatools::service_manager & services_)
        {
        
          services_.set_logging_priority(datatools::logger::PRIO_DEBUG);
          services_.set_name("Services");
          services_.set_description("Mock service manager for the SuperNEMO experiment");
          services_.set_allow_dynamic_services(true);
          services_.set_force_initialization_at_load(true);
          services_.initialize();

          {
            datatools::properties devicesConfig;
            std::string devicesConfigPath = "@snemo:config/snemo/demonstrator/devices/0.1/manager.conf";
            datatools::fetch_path_with_env(devicesConfigPath);
            devicesConfig.read_configuration(devicesConfigPath);            
            services_.load("devices",
                           "vire::device::manager",
                           devicesConfig);
          }

          {
            datatools::properties resourcesConfig;
            std::string resourcesConfigPath = "@snemo:config/snemo/demonstrator/resources/manager.conf";
            datatools::fetch_path_with_env(resourcesConfigPath);
            resourcesConfig.read_configuration(resourcesConfigPath);
            services_.load("resources",
                           "vire::resource::manager",
                           resourcesConfig);
          }
        
          services_.tree_dump(std::clog, "Services:");
        
          return;
        }
      
      } // namespace snemo       
    } // namespace testing     
  } // namespace resource 
} // namespace vire 

#endif // VIRE_RESOURCE_TESTING_SNEMO_TOOLS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
