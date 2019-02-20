#ifndef VIRE_COM_TESTING_COM_MANAGER_TOOLS_H
#define VIRE_COM_TESTING_COM_MANAGER_TOOLS_H

// Standard library:
#include <vire/com/manager.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/version_id.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/utils.h>

// This project:
#include <vire/vire.h>
#include <vire/cms/application.h>
#include <vire/com/domain.h>
#include <vire/com/domain_builder.h>

namespace vire {
  
  namespace com {
    
    namespace testing {

      std::string default_system_access_name_prefix()
      {
        return  std::string("_snemod_");
      }

      std::string default_domain_name_prefix()
      {
        return  std::string("/_snemod_");
      }
      
      void init_com_manager(vire::com::manager & com_,
                            const vire::cms::application::category_type appcat_ = vire::cms::application::CATEGORY_SERVER)
      {
        DT_THROW_IF(com_.is_initialized(), std::logic_error,
                    "Com manager service is already initialized!");
        
        std::string system_access_name_prefix = default_system_access_name_prefix();
        std::string domain_name_prefix = default_domain_name_prefix();
        
        vire::utility::model_identifier default_transport_driver_type_id("vire::rabbitmq::transport_driver",
                                                                         datatools::version_id(1));
        vire::utility::model_identifier default_encoding_driver_type_id("vire::com::protobuf_encoding_driver",
                                                                        datatools::version_id(3));

        com_.set_name("Com");
        com_.set_terse_description("SuperNEMO Vire Server Communication Service");
        com_.set_display_name("Communication");
        com_.set_app_category(appcat_);
        com_.set_system_access_login_prefix(system_access_name_prefix);
        com_.set_domain_name_prefix(domain_name_prefix);
        com_.set_default_encoding_driver_type_id(default_encoding_driver_type_id);
        com_.set_default_transport_driver_type_id(default_transport_driver_type_id);

        std::string com_config_path("${VIRE_TESTING_ONE_DIR}/config/com.conf");
        datatools::fetch_path_with_env(com_config_path);
        datatools::properties com_config;
        com_config.read_configuration(com_config_path);
  
        com_.initialize_standalone(com_config);
                           
        return;
      }
        
      void add_com_manager(vire::com::manager & com_)
      {
        DT_THROW_IF(!com_.is_initialized(), std::logic_error,
                    "Com manager service is not initialized!");
        std::string sc_name = "orleans";
        std::string sc_domain_name =
          vire::com::domain_builder::build_cms_subcontractor_system_name(com_.get_domain_name_prefix(),
                                                                         sc_name);
        vire::com::domain & sc_sys_domain =
          com_.create_domain(sc_domain_name,
                             vire::com::DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM,
                             com_.get_default_transport_driver_type_id(),
                             com_.get_default_encoding_driver_type_id());
        std::clog << "S/C Vire system domain:" << std::endl;
        sc_sys_domain.print_tree(std::clog);
        return;
      }
      
    }
    
  }

}

#endif // VIRE_COM_TESTING_COM_MANAGER_TOOLS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
