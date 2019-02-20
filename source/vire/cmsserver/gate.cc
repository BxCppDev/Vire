//! \file vire/cmsserver/gate.cc
//
// Copyright (c) 2015-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/gate.h>

// Standard Library:
#include <memory>

// This project:
#include <vire/time/utils.h>
#include <vire/com/access_profile.h>

namespace vire {

  namespace cmsserver {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(gate, "vire::cmsserver::gate")

    gate::connection_info::connection_info(const uint32_t /*flags_*/)
      : _id_(-1)
      , _key_("")
      , _user_login_("")
      , _time_interval_(vire::time::invalid_time_interval())
    {
      _id_ = -1;
      return;
    }

    gate::connection_info::~connection_info()
    {
      return;
    }

    bool gate::connection_info::has_id() const
    {
      return _id_ >= 0;
    }

    void gate::connection_info::set_id(const int32_t id_)
    {
      DT_THROW_IF(id_ < 0, std::range_error, "Invalid connection ID=[" << id_ << "]!");
      _id_ = id_;
      return;
    }

    void gate::connection_info::clear_id()
    {
      _id_ = -1;
      return;
    }

    int32_t gate::connection_info::get_id() const
    {
      return _id_;
    }

    bool gate::connection_info::has_key() const
    {
      return !_key_.empty();
    }

    void gate::connection_info::set_key(const std::string & key_)
    {
      DT_THROW_IF(key_.length() < 1, std::logic_error,
                  "Invalid connection key!");
      _key_ = key_;
      return;
    }

    void gate::connection_info::clear_key()
    {
      _key_.clear();
      return;
    }

    const std::string & gate::connection_info::get_key() const
    {
      return _key_;
    }

    bool gate::connection_info::has_user_login() const
    {
      return !_user_login_.empty();
    }

    void gate::connection_info::set_user_login(const std::string & user_login_)
    {
      _user_login_ = user_login_;
      return;
    }

    void gate::connection_info::clear_user_login()
    {
      _user_login_.clear();
      return;
    }

    const std::string & gate::connection_info::get_user_login() const
    {
      return _user_login_;
    }

    bool gate::connection_info::has_time_interval() const
    {
      return vire::time::is_valid(_time_interval_);
    }

    void gate::connection_info::set_time_interval(const boost::posix_time::time_period & ti_)
    {
      DT_THROW_IF(!vire::time::is_valid(ti_), std::logic_error,
                  "Invalid connection time interval!");
      _time_interval_ = ti_;
      return;
    }

    void gate::connection_info::clear_time_interval()
    {
      vire::time::invalidate_time_interval(_time_interval_);
      return;
    }

    const boost::posix_time::time_period &
    gate::connection_info::get_time_interval() const
    {
      return _time_interval_;
    }

    // static
    const std::size_t gate::DEFAULT_MAX_NUMBER_OF_CONNECTIONS;

    void gate::_set_defaults_()
    {
      _login_ = "";
      _password_ = "";
      _client_login_ = "";
      _client_password_ = "";
      _max_number_of_connections_ = 0;
      return;
    }

    gate::gate(uint32_t /* flags_ */)
      : datatools::base_service("Gate", "Remote access gate service", "")
    {
      _initialized_ = false;
      _set_defaults_();
      return;
    }

    gate::~gate()
    {
      if (this->is_initialized()) this->gate::reset();
      return;
    }

    bool gate::has_login() const
    {
      return !_login_.empty();
    }

    void gate::set_login(const std::string & login_)
    {
      _login_ = login_;
      return;
    }

    const std::string & gate::get_login() const
    {
      return _login_;
    }

    bool gate::has_password() const
    {
      return !_password_.empty();
    }

    void gate::set_password(const std::string & password_)
    {
      _password_ = password_;
      return;
    }

    const std::string & gate::get_password() const
    {
      return _password_;
    }

    bool gate::has_client_login() const
    {
      return !_client_login_.empty();
    }

    void gate::set_client_login(const std::string & client_login_)
    {
      _client_login_ = client_login_;
      return;
    }

    const std::string & gate::get_client_login() const
    {
      return _client_login_;
    }

    bool gate::has_client_password() const
    {
      return !_client_password_.empty();
    }

    void gate::set_client_password(const std::string & client_password_)
    {
      _client_password_ = client_password_;
      return;
    }

    const std::string & gate::get_client_password() const
    {
      return _client_password_;
    }

    bool gate::authenticate(const std::string & login_, const std::string & password_) const
    {
      if (login_ != _login_) return false;
      if (password_ != _password_) return false;
      return true;
    }

    void gate::set_max_number_of_connections(const std::size_t mnoc_)
    {
      _max_number_of_connections_ = mnoc_;
      return;
    }

    std::size_t gate::get_max_number_of_connections() const
    {
      return _max_number_of_connections_;
    }

    // virtual
    bool gate::is_initialized() const
    {
      return _initialized_;
    }

    void gate::set_com_name(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Gate service is already initialized! Com cannot be set now!");
      DT_THROW_IF(has_com(), std::logic_error,
                  "Gate service already has a com service! Com name cannot be constrained now!");
      _com_name_ = name_;
      return;
    }

    const std::string & gate::get_com_name() const
    {
      return _com_name_;
    }

    bool gate::has_com() const
    {
      return _com_ != nullptr;
    }

    void gate::set_com(vire::com::manager & com_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Gate service is already initialized! Com cannot be set now!");
      if (!_com_name_.empty() and com_.get_name() != _com_name_) {
        DT_THROW(std::logic_error,
                 "Gate service name '" << com_.get_name() << "' do not match required name '"
                 << _com_name_ << "'!");        
      }
      _com_ = &com_;
      return;
    }
    
    const vire::com::manager & gate::get_com() const
    {
      return *_com_;
    }
    
    vire::com::manager & gate::grab_com()
    {
      return *_com_;
    }

    bool gate::has_gate_hub() const
    {
      return _gate_hub_ != nullptr;
    }

    const vire::com::access_hub & gate::get_gate_hub() const
    {
      return *_gate_hub_;
    }

    vire::com::access_hub & gate::grab_gate_hub()
    {
      return *_gate_hub_;
    }

    void gate::_at_init_()
    {      
      // Create actor profiles:
      vire::com::access_profile & server_gate_access_profile =
        _com_->create_access_profile(_login_,
                                    _password_,
                                    vire::com::ACCESS_CATEGORY_SERVER_GATE);
      
      _com_->create_access_profile(_client_login_,
                                  _client_password_,
                                  vire::com::ACCESS_CATEGORY_CLIENT_GATE,
                                  get_name());

      _gate_hub_ = 
        server_gate_access_profile.create_access_hub("connection");
      return;
    }
    
    void gate::_at_reset_()
    {
      return;
    }
 
    // virtual
    int gate::initialize(const datatools::properties & config_,
                         datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Manager is already initialized!");

      this->::datatools::base_service::common_initialize(config_);

      if (_login_.empty()) {
        if (config_.has_key("login")) {
          set_login(config_.fetch_string("login"));
        }
      }

      if (_password_.empty()) {
        if (config_.has_key("password")) {
          set_password(config_.fetch_string("password"));
        }
      }

      if (_client_login_.empty()) {
        if (config_.has_key("client_login")) {
          set_client_login(config_.fetch_string("client_login"));
        }
      }

      if (_client_password_.empty()) {
        if (config_.has_key("client_password")) {
          set_client_password(config_.fetch_string("client_password"));
        }
      }

      if (_max_number_of_connections_ == 0) {
        if (config_.has_key("max_number_of_connections")) {
          int mxoc = config_.fetch_positive_integer("max_number_of_connections");
          set_max_number_of_connections(mxoc);
        }
      }

      if (_max_number_of_connections_ == 0) {
        _max_number_of_connections_ = DEFAULT_MAX_NUMBER_OF_CONNECTIONS;
      }

      if (!has_com()) {
        if (_com_name_.empty()) {
          if (config_.has_key("com_name")) {
            set_com_name(config_.fetch_string("com_name"));
          }
        }

        if (_com_name_.empty()) {
          set_com_name(vire::com::manager::default_service_name());
        }
        
        if (datatools::service_exists(service_dict_, _com_name_)) {
          vire::com::manager & com =
            datatools::grab<vire::com::manager>(service_dict_, _com_name_);
          set_com(com);
        }
      }

      DT_THROW_IF(!has_com(), std::logic_error,
                  "Gate service has no access to com service!");

      if (_login_.empty()) {
        _login_ = "viresvrgate";
      }
          
      if (_password_.empty()) {
        _password_ = "viresvrgate";
      }
     
      if (_client_login_.empty()) {
        _client_login_ = "vireclientgate";
      }
          
      if (_client_password_.empty()) {
        _client_password_ = "vireclientgate";
      }
      
      _at_init_();
      
      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    // virtual
    int gate::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Gate is not initialized!");
      _initialized_ = false;
      _at_reset_();
      _set_defaults_();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }
      
    void gate::print_tree(std::ostream & out_,
                          const boost::property_tree::ptree & options_) const
    {
      base_print_options popts;
      popts.configure_from(options_);
      std::ostringstream outs;
      this->base_service::print_tree(outs,
                                     base_print_options::force_inheritance(options_));
             
      outs << popts.indent << tag
           << "Login : "
           << "'" << this->_login_ << "'" << std::endl;
        
      outs << popts.indent << tag
           << "Password : "
           << "'" << this->_password_ << "'" << std::endl;
        
      outs << popts.indent << tag
           << "Max number of connections : "
           << this->_max_number_of_connections_ << std::endl;
        
      outs << popts.indent << tag
           << "Com : "
           << std::boolalpha << has_com() << std::endl;
       
      outs << popts.indent << tag
           << "Gate hub : ";
      if (_gate_hub_ == nullptr) {
        outs << "<none>";
      } else {
        outs << "'" << _gate_hub_->get_name() << "'";
      }
      outs << std::endl;

      if (!popts.inherit) {
        outs << popts.indent << inherit_tag(popts.inherit)
             << "End."
             << std::endl;
      }

      out_ << outs.str();
      return;
    }

  } // namespace cmsserver

} // namespace vire
