//! \file vire/cmsserver/uc_factory.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Ourselves:
#include <vire/cmsserver/uc_factory.h>
  
namespace vire {

  namespace cmsserver {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(uc_factory, "vire::cmsserver::uc_factory")
    
    uc_factory::uc_factory(uint32_t /* flags_ */)
      : datatools::base_service("use_cases", "Use case factory service", "")
    {
      _set_defaults_();
      return;
    }

    uc_factory::~uc_factory()
    {
      return;
    }

    bool uc_factory::has_setup_name() const
    {
      return !_setup_name_.empty();
    }
    
    void uc_factory::set_setup_name(const std::string & setup_name_)
    {
      _setup_name_ = setup_name_;
      return ;
    }
                        
    const std::string & uc_factory::get_setup_name() const
    {
      return _setup_name_;
    }

    bool uc_factory::has_uc_base_dir() const
    {
      return !_uc_base_dir_.empty();
    }
    
    void uc_factory::set_uc_base_dir(const std::string & uc_base_dir_)
    {
      _uc_base_dir_ = uc_base_dir_;
      return;
    }
                        
    const std::string & uc_factory::get_uc_base_dir() const
    {
      return _uc_base_dir_;
    }

    bool uc_factory::has_uc_registry_config_path() const
    {
      return !_uc_registry_config_path_.empty();
    }
    
    void uc_factory::set_uc_registry_config_path(const std::string & path_)
    {
      _uc_registry_config_path_ = path_;
      return ;
    }
                        
    const std::string & uc_factory::get_uc_registry_config_path() const
    {
      return _uc_registry_config_path_;
    }

    bool uc_factory::has_device_service_name() const
    {
      return !_device_service_name_.empty();
    }
    
    void uc_factory::set_device_service_name(const std::string & device_service_name_)
    {
      _device_service_name_ = device_service_name_;
      return;
    }
                        
    const std::string & uc_factory::get_device_service_name() const
    {
      return _device_service_name_;
    }

    bool uc_factory::has_resource_service_name() const
    {
      return !_resource_service_name_.empty();
    }
    
    void uc_factory::set_resource_service_name(const std::string & resource_service_name_)
    {
      _resource_service_name_ = resource_service_name_;
      return;
    }
                        
    const std::string & uc_factory::get_resource_service_name() const
    {
      return _resource_service_name_;
    }

    bool uc_factory::has_devices() const
    {
      return _devices_ !=  nullptr;
    }

    void uc_factory::set_devices(const vire::device::manager & mgr_)
    {
      _devices_ = &mgr_;
      return;
    }

    const vire::device::manager & uc_factory::get_devices() const
    {
      DT_THROW_IF(!has_devices(), std::logic_error, "No device manager is set!");
      return *_devices_;
    }

    bool uc_factory::has_resources() const
    {
      return _resources_ !=  nullptr;
    }

    void uc_factory::set_resources(const vire::resource::manager & mgr_)
    {
      _resources_ = &mgr_;
      return;
    }

    const vire::resource::manager & uc_factory::get_resources() const
    {
      DT_THROW_IF(!has_resources(), std::logic_error, "No resource manager is set!");
      return *_resources_;
    }

    void uc_factory::_set_defaults_()
    {
      return;
    }

    // virtual
    bool uc_factory::is_initialized() const
    {
      return _initialized_;
    }

    // virtual
    int uc_factory::initialize(const datatools::properties & config_,
                               datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Manager is already initialized!");

      this->::datatools::base_service::common_initialize(config_);

      if (!has_devices()) {
        if (!has_device_service_name()) {
          std::string device_service_name;
          if (config_.has_key("device_service_name")) {
            device_service_name = config_.fetch_string("device_service_name");
          } else {
            device_service_name = vire::device::manager::default_service_name();
          }
          set_device_service_name(device_service_name);
        }
      }
      /*     
      if (!has_devices() && has_device_service_name()) {
        const std::string device_service_type_id("vire::device::manager");
        DT_THROW_IF(!datatools::find_service_name_with_id(service_dict_,
                                                          device_service_type_id,
                                                          _device_service_name_),
                    std::logic_error,
                    "Cannot find a service named '" << _device_service_name_ << "' of type '"
                    << device_service_type_id << "'!");
        set_devices(datatools::get<vire::device::manager>(service_dict_,
                                                          _device_service_name_));
      }
 
      if (!has_resources()) {
        if (!has_resource_service_name()) {
          std::string resource_service_name;
          if (config_.has_key("resource_service_name")) {
            resource_service_name = config_.fetch_string("resource_service_name");
          } else {
            resource_service_name = vire::resource::manager::default_service_name();
          }
          set_resource_service_name(resource_service_name);
        }
      }
      
      if (!has_resources() && has_resource_service_name()) {
        const std::string resource_service_type_id("vire::resource::manager");
        DT_THROW_IF(!datatools::find_service_name_with_id(service_dict_,
                                                          resource_service_type_id,
                                                          _resource_service_name_),
                    std::logic_error,
                    "Cannot find a service named '" << _resource_service_name_ << "' of type '"
                    << resource_service_type_id << "'!");
        set_resources(datatools::get<vire::resource::manager>(service_dict_,
                                                              _resource_service_name_));
      }
       */

      if (!has_setup_name()) {
        if (config_.has_key("setup_name")) {
          set_setup_name(config_.fetch_string("setup_name"));
        }
      }
     
      if (!has_uc_base_dir()) {
        if (config_.has_key("uc_base_dir")) {
          set_uc_base_dir(config_.fetch_string("uc_base_dir"));
        }
      }
       
      if (!has_uc_registry_config_path()) {
        if (config_.has_key("use_case_registry_path")) {
          std::string path = config_.fetch_path("use_case_registry_path");
          set_uc_registry_config_path(path);
        }
      }
     
      _at_init_();
      
      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    void uc_factory::_at_reset_()
    {
      _use_cases_.clear();
      _uc_models_.reset();
      _resources_ = nullptr;
      _devices_   = nullptr;
      _uc_base_path_.reset();
      _uc_registry_config_path_.clear();
      _uc_base_dir_.clear();
      _setup_name_.clear();
      _resource_service_name_.clear();
      _device_service_name_.clear();
      return;
    }

    void uc_factory::_at_init_()
    {

      if (!has_setup_name()) {
        if (has_devices() && !_devices_->get_setup_label().empty()) {
          set_setup_name(_devices_->get_setup_label());
        }
      }
      
      if (!has_uc_base_dir()) {
        set_uc_base_dir("/Automatons");
      }

      {
        _uc_base_path_ = utility::path(_setup_name_, _uc_base_dir_);
      }
      
      {
        // Load the use case model registry:
        std::string ucModelDbCfgPath = _uc_registry_config_path_;
        datatools::fetch_path_with_env(ucModelDbCfgPath);
        datatools::properties ucModelDbCfg;
        ucModelDbCfg.read_configuration(ucModelDbCfgPath);
        _uc_models_.configure(ucModelDbCfg);
      }
     
      return;
    }

    // virtual
    int uc_factory::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Use case database service is not initialized!");
      _initialized_ = false;

      _at_reset_();
      
      _set_defaults_();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    const uc_factory::uc_dict_type & uc_factory::get_use_cases() const
    {
      return _use_cases_;
    }

    void uc_factory::print_tree(std::ostream & out_,
                                const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);
      
      this->::datatools::base_service::print_tree(out_,
                                                 base_print_options::force_inheritance(options_));

      out_ << popts.indent << tag
           << "Device service name   : ";
      if (has_device_service_name()) {
        out_ << "'" << _device_service_name_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
  
      out_ << popts.indent << tag
           << "Resource service name : ";
      if (has_resource_service_name()) {
        out_ << "'" << _resource_service_name_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
  
      out_ << popts.indent << tag
           << "Use case model registry config path : ";
      if (has_uc_registry_config_path()) {
        out_ << "'" << _uc_registry_config_path_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
  
      out_ << popts.indent << tag
           << "Setup name         : ";
      if (has_setup_name()) {
        out_ << "'" << _setup_name_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
 
      out_ << popts.indent << tag
           << "Use case base dir  : ";
      if (has_uc_base_dir()) {
        out_ << "'" << _uc_base_dir_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
 
      out_ << popts.indent << tag
           << "Devices service    : ";
      if (has_devices()) {
        out_ << _devices_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
 
      out_ << popts.indent << tag
           << "Resource service   : ";
      if (has_resources()) {
        out_ << _resources_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
 
      out_ << popts.indent << tag
           << "Registry of use case models : ";
      out_ << _uc_models_.get_models().size() << " model(s)";
      out_ << std::endl;
 
      out_ << popts.indent << tag
           << "Use case base path : '" << _uc_base_path_.to_string() << "'";
      out_ << std::endl;
 
      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Use case instances : ";
      out_ << _use_cases_.size() << " use case(s)";
      out_ << std::endl;
      if (_use_cases_.size()) {
        std::size_t count = 0;
        for (uc_dict_type::const_iterator i = _use_cases_.begin();
             i != _use_cases_.end();
             i++) {
          // const base_use_case & uc = *->second.get();
          out_ << popts.indent << datatools::i_tree_dumpable::inherit_skip_tag(popts.inherit);
          if (++count == _use_cases_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Instantiated use case : '" << i->first << "'";
          out_ << std::endl;
        }
        
      }
   
      return;
    }
    
    std::shared_ptr<base_use_case>
    uc_factory::_base_create_(const std::string & uc_type_id_)
    {
      const base_use_case::factory_register_type & the_factory_register
        = DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(vire::cmsserver::base_use_case);

      if (datatools::logger::is_debug(get_logging_priority())) {
        vire::cmsserver::base_use_case::get_system_factory_register().tree_dump(std::cerr,
                                                                                "System factory register:", "[debug] ");
      }
      DT_THROW_IF(!the_factory_register.has(uc_type_id_),
                  std::logic_error,
                  "Use case factory has no type with identifier '"
                  << uc_type_id_ << "'!");
      const base_use_case::factory_register_type::factory_type & the_factory
        = the_factory_register.get(uc_type_id_);
      return std::shared_ptr<base_use_case>(the_factory());
    }

    std::shared_ptr<base_use_case> uc_factory::top_create_dry(const std::string & name_,
                                                              const std::string & model_id_,
                                                              const std::string & description_)
    {
      uc_construction_context_type context;
      context.path = _uc_base_path_.make_child(name_);
      context.model_id = model_id_;
      context.description = description_;
      context.factory = this;
      context.devices = this->_devices_;
      context.resources = this->_resources_;
      return create_dry(context);
    }

    std::shared_ptr<base_use_case>
    uc_factory::create_dry(const uc_construction_context_type & cc_)
    {
      DT_THROW_IF(!cc_.is_complete(),
                  std::logic_error,
                  "Incomplete construction context!");
      std::string path_repr = cc_.path.to_string();
      std::string addr_repr = vire::utility::path::to_address(path_repr);
      std::string model_id = cc_.model_id;
      DT_THROW_IF(!_uc_models_.has(model_id),
                  std::logic_error,
                  "No use case model with identifier '" << model_id << "'!");
      const uc_model_description & model_desc = _uc_models_.get(model_id);
      std::string uc_type_id = model_desc.get_use_case_type_id();     
      std::shared_ptr<base_use_case> uc_ptr = _base_create_(uc_type_id);

      // Registration in the flat dictionary:
      _use_cases_[path_repr] = uc_ptr;

      // Basic configuration (name...)
      uc_ptr->set_name(addr_repr);
      uc_ptr->set_terse_description(cc_.description);
      uc_ptr->grab_auxiliaries().store("uc_model_id", model_id);
      uc_ptr->grab_auxiliaries().store("uc_path", path_repr);
      uc_ptr->set_cc(cc_);

      // Construction stage 1: run mode
      uc_ptr->set_run_mode(base_use_case::RUN_MODE_DRYRUN);
      
      // Construction stage 2: configure general setup
      datatools::properties uc_config;
      const datatools::properties * uc_config_p = &uc_config;
      if (model_desc.has_use_case_config()) {
        uc_config_p = &model_desc.get_use_case_config();
      } else {
        if (model_desc.has_use_case_config_path()) {
          std::string path = model_desc.get_use_case_config_path();
          datatools::fetch_path_with_env(path);
          datatools::properties::read_config(path, uc_config);
          uc_config_p = &uc_config;
        }
      }
      uc_ptr->configure_setup(*uc_config_p);

      // Construction stage 3: configure composition
      uc_ptr->configure_composition(model_desc.get_composition_description());

      // Construction stage 4: build composition
      uc_ptr->build_composition();       

      // Construction stage 5: build scope requirements
      uc_ptr->build_scope_requirements();    

      // Construction stage 6: build distributable mounting table
      uc_ptr->build_distributable_mounting_table();
 
      // Construction stage 7: build functional requirements
      uc_ptr->build_functional_requirements();
  
      // Resource constraints
      // uc_ptr->build_resource_constraints();
      
      // Time constraints
      // uc_ptr->build_time_constraints();
   
      return uc_ptr;
    }

    // std::shared_ptr<base_use_case>
    // uc_factory::create_run(const session_info & sinfo_,
    //                        const session * run_session_)
    // {
    //   std::shared_ptr<base_use_case> uc_ptr;
    //   return uc_ptr;
    // }
    
  } // namespace cmsserver

} // namespace vire
