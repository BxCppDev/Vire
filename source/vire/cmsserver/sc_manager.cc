/// \file vire/cmsserver/sc_manager.cc
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

// Ourselves:
#include <vire/cmsserver/sc_manager.h>

// Standard library:
#include <sstream>
#include <chrono>
#include <thread>
#include <random>

namespace vire {

  namespace cmsserver {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(sc_manager, "vire::cmsserver::sc_manager")

    sc_manager::sc_manager(uint32_t flags_)
    {
      _initialized_ = false;
      // _set_defaults_();
      return;
    }

    sc_manager::~sc_manager()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }
 
    bool sc_manager::has_device_service_name() const
    {
      return !_device_service_name_.empty();
    }
    
    void sc_manager::set_device_service_name(const std::string & device_service_name_)
    {
      _device_service_name_ = device_service_name_;
      return;
    }
                        
    const std::string & sc_manager::get_device_service_name() const
    {
      return _device_service_name_;
    }

    bool sc_manager::has_resource_service_name() const
    {
      return !_resource_service_name_.empty();
    }
    
    void sc_manager::set_resource_service_name(const std::string & resource_service_name_)
    {
      _resource_service_name_ = resource_service_name_;
      return;
    }
                        
    const std::string & sc_manager::get_resource_service_name() const
    {
      return _resource_service_name_;
    }

    bool sc_manager::has_com_service_name() const
    {
      return !_com_service_name_.empty();
    }
    
    void sc_manager::set_com_service_name(const std::string & com_service_name_)
    {
      _com_service_name_ = com_service_name_;
      return;
    }
                        
    const std::string & sc_manager::get_com_service_name() const
    {
      return _com_service_name_;
    }

    bool sc_manager::has_devices() const
    {
      return _devices_ !=  nullptr;
    }

    void sc_manager::set_devices(const vire::device::manager & mgr_)
    {
      _devices_ = &mgr_;
      return;
    }

    const vire::device::manager & sc_manager::get_devices() const
    {
      DT_THROW_IF(!has_devices(), std::logic_error, "No device manager is set!");
      return *_devices_;
    }

    bool sc_manager::has_resources() const
    {
      return _resources_ !=  nullptr;
    }

    void sc_manager::set_resources(const vire::resource::manager & mgr_)
    {
      _resources_ = &mgr_;
      return;
    }

    const vire::resource::manager & sc_manager::get_resources() const
    {
      DT_THROW_IF(!has_resources(), std::logic_error, "No resource manager is set!");
      return *_resources_;
    }
 
    bool sc_manager::has_com() const
    {
      return _com_ !=  nullptr;
    }

    void sc_manager::set_com(vire::com::manager & mgr_)
    {
      _com_ = &mgr_;
      return;
    }

    const vire::com::manager & sc_manager::get_com() const
    {
      DT_THROW_IF(!has_com(), std::logic_error, "No com manager is set!");
      return *_com_;
    }
 
    vire::com::manager & sc_manager::grab_com()
    {
      DT_THROW_IF(!has_com(), std::logic_error, "No com manager is set!");
      return *_com_;
    }

    bool sc_manager::has_password_gen_seed() const
    {
      return _password_gen_seed_ > 0;
    }

    unsigned int sc_manager::get_password_gen_seed() const
    {
      return _password_gen_seed_;
    }
    
    void sc_manager::set_password_gen_seed(const unsigned int seed_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "S/C manager is already initialized!");
      _password_gen_seed_ = seed_;
      return;
    }

    vire::user::simple_password_generator & sc_manager::grab_password_gen()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "S/C manager is not initialized!");
      return *_password_gen_.get();
    }
    
    // virtual
    bool sc_manager::is_initialized() const
    {
      return _initialized_;
    }

    // virtual
    int sc_manager::initialize(const datatools::properties & config_,
                               datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error, "S/C manager is already initialized!");
      
      this->::datatools::base_service::common_initialize(config_);
      DT_LOG_DEBUG(get_logging_priority(), "Has com = " << has_com() );
 
      if (!has_com()) {
        if (!has_com_service_name()) {
          std::string com_service_name;
          if (config_.has_key("com_service_name")) {
            com_service_name = config_.fetch_string("com_service_name");
          } else {
            com_service_name = vire::com::manager::default_service_name();
          }
          set_com_service_name(com_service_name);
        }
      }
      DT_LOG_DEBUG(get_logging_priority(), "Com service name = '" << _com_service_name_ << "'");
 
      if (!has_com() && has_com_service_name()) {
        const std::string com_service_type_id("vire::com::manager");
        DT_THROW_IF(!datatools::find_service_name_with_id(service_dict_,
                                                          com_service_type_id,
                                                          _com_service_name_),
                    std::logic_error,
                    "Cannot find a service named '" << _com_service_name_ << "' of type '"
                    << com_service_type_id << "'!");
        vire::com::manager & com
          = datatools::grab<vire::com::manager>(service_dict_, _com_service_name_);
        set_com(com);
      }
      DT_THROW_IF(!has_com(), std::logic_error, "No 'com' service is defined!");
    
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
     
      if (!has_devices() && has_device_service_name()) {
        const std::string device_service_type_id("vire::device::manager");
        DT_THROW_IF(!datatools::find_service_name_with_id(service_dict_,
                                                          device_service_type_id,
                                                          _device_service_name_),
                    std::logic_error,
                    "Cannot find a service named '" << _device_service_name_ << "' of type '"
                    << device_service_type_id << "'!");
        const vire::device::manager & devices
          = datatools::get<vire::device::manager>(service_dict_,
                                                  _device_service_name_);
        set_devices(devices);
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
        const vire::resource::manager & resources
          = datatools::get<vire::resource::manager>(service_dict_,
                                                    _resource_service_name_);
        set_resources(resources);
      }

      // Password generator:
      unsigned int password_gen_seed = 0;
      if (!has_password_gen_seed()) {
        if (config_.has_key("password_gen_seed")) {
          _password_gen_seed_ = config_.fetch_positive_integer("password_gen_seed");
        }
      }
      
      if (!has_password_gen_seed()) {
        unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count() - (unsigned) getpid();
        std::default_random_engine generator(seed1);
        std::uniform_int_distribution<int> distribution(0, 1000000);
        _password_gen_seed_ = distribution(generator);
      }
      DT_LOG_DEBUG(get_logging_priority(),
                   "Creating S/C accss profile password generator...");
      _password_gen_.reset(new vire::user::simple_password_generator(vire::user::simple_password_generator::CHARSET_ALPHANUM, _password_gen_seed_));
      
      // Load subcontractor configurations:
      if (config_.has_key("subcontractors.names")) {
        std::set<std::string> sc_names;
        config_.fetch("subcontractors.names", sc_names);
        for (const std::string & sc_name : sc_names) {
          std::ostringstream prefix_ss;
          prefix_ss << "subcontractors." << sc_name << ".";
          datatools::properties sc_config;
          config_.export_and_rename_starting_with(sc_config, prefix_ss.str(), "");
          _subcontractor_configs_[sc_name] = sc_config;
        }
      }
      _initialized_ = true;
      _post_initialization_();
      _rc_.set_status(vire::running::RUN_STATUS_READY);
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    void sc_manager::_post_initialization_()
    {
      // Initialize subcontractors from the loaded configurations:
      for (sc_config_dict_type::const_iterator i = _subcontractor_configs_.begin();
           i != _subcontractor_configs_.end();
           i++) {
        const std::string & sc_name = i->first;
        const datatools::properties & sc_config = i->second;
        add_subcontractor(sc_name, sc_config);
      }
      return;
    }

    void sc_manager::_pre_reset_()
    {
      return;
    }
 
    int sc_manager::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error, "S/C manager is not initialized!");
      _pre_reset_();
      
      _initialized_ = false;
      _password_gen_.reset();
      _drivers_.clear();
      _resources_ = nullptr;
      _devices_   = nullptr;
      _com_       = nullptr;
    
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    void sc_manager::build_subcontractor_names(std::set<std::string> & names_) const
    {
      names_.clear();
      for (const auto & p : _drivers_) {
        names_.insert(p.first);
      }
      return;
    }

    const vire::running::run_control & sc_manager::get_rc() const
    {
      return _rc_;
    }
   
    void sc_manager::stop()
    {
      _rc_.request_stop();
      return;
    }
 
    void sc_manager::run()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Cannot start uninitialized subcontractor manager!");
      DT_THROW_IF(!_rc_.is_ready(), std::logic_error,
                  "Subcontractor manager is not ready to run!");
      if (_rc_.is_stop_requested()) {
        _rc_.set_status(vire::running::RUN_STATUS_STOPPED);
        return;
      }
      _rc_.set_start_time(std::chrono::system_clock::now());
      _at_run_start_();
      _at_run_();
      _at_run_stop_();
      _rc_.set_stop_time(std::chrono::system_clock::now());
      _rc_.set_status(vire::running::RUN_STATUS_STOPPED);
      
      return;
    }

    void sc_manager::_at_run_start_()
    {
      DT_LOG_DEBUG(get_logging_priority(), "Starting run...");
      _rc_.set_status(vire::running::RUN_STATUS_STARTING);

      for (sc_dict_type::const_iterator sc_iter = _drivers_.begin();
           sc_iter != _drivers_.end();
           sc_iter++) {
        sc_driver & scInfo = *(sc_iter->second);
        if (datatools::logger::is_debug(get_logging_priority())) {
          scInfo.print_tree(std::cerr);
        }
        if (!scInfo.is_connected() && scInfo.is_auto_connect()) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "First attempt to connect subcontractor '" << sc_iter->first << "'...");
          scInfo.connect();
        } 
      }
      
      DT_LOG_DEBUG(get_logging_priority(), "Run is started.");
      return;
    }
    
    void sc_manager::_at_run_stop_()
    {
      DT_LOG_DEBUG(get_logging_priority(), "Stopping run...");
      _rc_.set_status(vire::running::RUN_STATUS_STOPPING);
      
      for (sc_dict_type::const_iterator sc_iter = _drivers_.begin();
           sc_iter != _drivers_.end();
           sc_iter++) {
        sc_driver & scInfo = *(sc_iter->second);
        if (scInfo.is_connected()) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Disconnect subcontractor '" << sc_iter->first << "'...");
          scInfo.disconnect();
        } 
      }
     
      DT_LOG_DEBUG(get_logging_priority(), "Run is stopped.");
      return;
    }

    void sc_manager::_at_run_loop_()
    {
      // DT_LOG_DEBUG(get_logging_priority(), "Subcontractor manager loop...");
     
      for (sc_dict_type::const_iterator sc_iter = _drivers_.begin();
           sc_iter != _drivers_.end();
           sc_iter++) {
        sc_driver & scInfo = *(sc_iter->second);
        if (!scInfo.is_connected()
            && scInfo.is_auto_connect()
            && scInfo.can_reconnect()) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Attempt to reconnect subcontractor '" << sc_iter->first << "'...");
          scInfo.connect();
        } 
      }
      
      return;
    }
   
    void sc_manager::_at_run_()
    {
      _rc_.set_status(vire::running::RUN_STATUS_RUNNING);

      while (!_rc_.is_stop_requested()) {
        vire::time::system_time_point tp_now = std::chrono::system_clock::now();
        vire::time::system_time_point tp_elapsed_tick = tp_now + _rc_.get_sys_tick();
        _at_run_loop_();
        _rc_.increment_loop_counter();
        tp_now = std::chrono::system_clock::now();
        if (tp_now < tp_elapsed_tick) {
          std::this_thread::sleep_until(tp_elapsed_tick);
        }
      }
      
      return;
    }

    bool sc_manager::has_subcontractor(const std::string & name_) const
    {
      return _drivers_.count(name_) == 1;
    }

    void sc_manager::add_subcontractor(const std::string & name_,
                                       const datatools::properties & config_)
    {
      DT_THROW_IF(has_subcontractor(name_),
                  std::logic_error,
                  "Subcontractor named '" << name_ << "' already exists!");
      _drivers_[name_] = std::make_shared<sc_driver>(*this);
      sc_driver & scDrv = *(_drivers_.find(name_)->second);
      scDrv.set_name(name_);
      scDrv.set_com_manager(*_com_);
      scDrv.set_device_manager(get_devices());
      scDrv.set_resource_manager(get_resources());
      {
        // Create server side S/C access profile login and password:
        std::string login_rand;
        _password_gen_->generate_password(login_rand, 10);
        std::string svr_login = get_com().get_system_access_login_prefix() + login_rand;
        scDrv.set_svr_login(svr_login);
        std::string svr_password;
        _password_gen_->generate_password(svr_password, 14);
        scDrv.set_svr_password(svr_password);
      }
      if (datatools::logger::is_debug(get_logging_priority())) {
        DT_LOG_DEBUG(get_logging_priority(), "Subcontractor '" << name_ << "' config: ");
        config_.print_tree(std::cerr);
      }
      scDrv.initialize(config_);
      return;
    }

    const sc_driver & sc_manager::get_driver(const std::string & name_) const
    {
      sc_dict_type::const_iterator found = _drivers_.find(name_);
      DT_THROW_IF(found == _drivers_.end(),
                  std::logic_error,
                  "No subcontractor driver named '" << name_ << "'!");
      return *(found->second);
    }

    sc_driver & sc_manager::grab_driver(const std::string & name_)
    {
      sc_dict_type::iterator found = _drivers_.find(name_);
      DT_THROW_IF(found == _drivers_.end(),
                  std::logic_error,
                  "No subcontractor driver named '" << name_ << "'!");
      return *(found->second);
    }
    
    void sc_manager::print_tree(std::ostream & out_,
                                const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);
      
      this->datatools::base_service::print_tree(out_,
                                                base_print_options::force_inheritance(options_));
      
      out_ << popts.indent << tag
           << "Device manager    : ";
      if (_devices_) {
        out_ << '@' << _devices_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << tag
           << "Resource manager    : ";
      if (_resources_) {
        out_ << '@' << _resources_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << tag
           << "Com manager    : ";
      if (_com_) {
        out_ << '@' << _com_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Subcontractor drivers : " << _drivers_.size() << std::endl;
      if (_drivers_.size()) {
      }
       
      return;
    }

  } // namespace cmsserver

} // namespace vire
