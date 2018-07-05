//! \file  vire/cmsserver/uc_factory.h
//! \brief Vire CMS use case factory utilities
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
//
// This file is part of Vire.
//
// Vire is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Vire is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRE_CMSSERVER_UC_FACTORY_H
#define VIRE_CMSSERVER_UC_FACTORY_H

// Standard Library:
#include <string>
#include <map>
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <datatools/base_service.h>
#include <datatools/properties.h>

// This project:
#include <vire/cmsserver/base_use_case.h>
#include <vire/device/manager.h>
#include <vire/resource/manager.h>
#include <vire/cmsserver/uc_model_db.h>
#include <vire/cmsserver/uc_utils.h>

namespace vire {

  namespace cmsserver {

    /// \brief Use case factory
    class uc_factory
      : public ::datatools::base_service
    {

    public:

      /// \brief Dictionary of use cases
      typedef std::map<std::string, std::shared_ptr<base_use_case> > uc_dict_type;

      /// Constructor
      uc_factory(uint32_t flags_ = 0);

      /// Destructor
      virtual ~uc_factory();

      bool has_device_service_name() const;

      void set_device_service_name(const std::string &);
      
      const std::string & get_device_service_name() const;

      bool has_resource_service_name() const;

      void set_resource_service_name(const std::string &);
      
      const std::string & get_resource_service_name() const;

      bool has_devices() const;

      void set_devices(const vire::device::manager &);

      const vire::device::manager & get_devices() const;

      bool has_resources() const;

      void set_resources(const vire::resource::manager &);

      const vire::resource::manager & get_resources() const;

      bool has_uc_registry_config_path() const;

      void set_uc_registry_config_path(const std::string &);
      
      const std::string & get_uc_registry_config_path() const;

      bool has_setup_name() const;

      void set_setup_name(const std::string &);
      
      const std::string & get_setup_name() const;

      bool has_uc_base_dir() const;

      void set_uc_base_dir(const std::string &);
      
      const std::string & get_uc_base_dir() const;

      const uc_dict_type & get_use_cases() const;
      
      //! Check the initialization flag
      bool is_initialized() const override;

      //! Initialize the manager from a container of properties
      int initialize(const datatools::properties & config_,
                     datatools::service_dict_type & service_dict_) override;

      //! Reset the manager
      int reset() override;
     
      /// Create a use case instance in "dry-run" mode
      std::shared_ptr<base_use_case> top_create_dry(const std::string & name_,
                                                    const std::string & model_id_,
                                                    const std::string & description_ = "");
      
      /// Create a use case in "dry-run" mode from a use case setup description record
      std::shared_ptr<base_use_case> create_dry(const uc_construction_context_type & cc_);

      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

    private:

      //! Set default attributes' values
      void _set_defaults_();

      /// Base factory method
      std::shared_ptr<base_use_case> _base_create_(const std::string & uc_type_id_);

      void _at_init_();

      void _at_reset_();
     
    private:
      
      // Management:
      bool _initialized_ = false;

      // Configuration:
      std::string _device_service_name_; 
      std::string _resource_service_name_; 
      std::string _uc_registry_config_path_; 
      std::string _setup_name_; 
      std::string _uc_base_dir_; 

      // Working data:
      const vire::device::manager *   _devices_   = nullptr; ///< Current device manager
      const vire::resource::manager * _resources_ = nullptr; ///< Current resource manager
      uc_model_db   _uc_models_;    ///< Registry of known use case models
      utility::path _uc_base_path_; ///< Base path for instantiated use cases
      uc_dict_type  _use_cases_;    ///< Instantiated use cases
       
      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(uc_factory)

    };
    
  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_FACTORY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
