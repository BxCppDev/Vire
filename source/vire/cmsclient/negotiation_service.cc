// /vire/cmsclient/negotiation_service.cc
//
// Copyright (c) 2019 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsclient/negotiation_service.h>

// This project:
#include <vire/cmsclient/client.h>
#include <vire/com/access_profile.h>

namespace vire {

  namespace cmsclient {

    /* Auto-registration of this service class in a central service Db */
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(negotiation_service,
                                             "vire::cmsclient::negotiation_service")

    negotiation_service::negotiation_service()
    {
      return;
    }

    negotiation_service::~negotiation_service()
    {
      if (this->is_initialized()) this->negotiation_service::reset();
      return;
    }

    void negotiation_service::set_com(vire::com::manager & com_)
    {
      _com_ = &com_;
      return;
    }

    void negotiation_service::set_client(const client & client_)
    {
      _client_ = &client_;
      return;
    }

    bool negotiation_service::has_gate_user_login() const
    {
      return !_gate_user_login_.empty();
    }

    void negotiation_service::set_gate_user_login(const std::string & login_)
    {
      _gate_user_login_ = login_;
      return;
    }

    const std::string & negotiation_service::get_gate_user_login() const
    {
      return _gate_user_login_;
    }

    bool negotiation_service::has_gate_user_password() const
    {
      return !_gate_user_password_.empty();
    }

    void negotiation_service::set_gate_user_password(const std::string & password_)
    {
      _gate_user_password_ = password_;
      return;
    }

    const std::string & negotiation_service::get_gate_user_password() const
    {
      return _gate_user_password_;
    }

    bool negotiation_service::is_initialized() const
    {
      return _initialized_;
    }

    int negotiation_service::initialize(const datatools::properties & config_,
                                        datatools::service_dict_type & services_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Negotiation service is already initialized!");

      DT_LOG_DEBUG(get_logging_priority(), "Number of services : " << services_.size());
      for (const auto & p : services_) {
        DT_LOG_DEBUG(get_logging_priority()," key='" << p.first << "'");
      }

      if (_com_ == nullptr) {
        std::string com_name = client::com_service_name();
        vire::com::manager & com
          = datatools::grab<vire::com::manager>(services_, com_name);
        set_com(com);
      }

      if (!has_gate_user_login()) {
        if (config_.has_key("gate_user_login")) {
          set_gate_user_login(config_.fetch_string("gate_user_login"));
        }
      }

      if (!has_gate_user_password()) {
        if (config_.has_key("gate_user_password")) {
          set_gate_user_password(config_.fetch_string("gate_user_password"));
        }
      }

      _at_init_();

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return 0;
    }

    int negotiation_service::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Negotiation service is not initialized!");
      _initialized_ = false;
      _at_reset_();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return 0;
    }

    void negotiation_service::_at_init_()
    {
      DT_THROW_IF(_client_ == nullptr, std::logic_error,
                  "No handle to CMS client!");
      DT_THROW_IF(_com_ == nullptr, std::logic_error,
                  "No handle to com service!");
      DT_THROW_IF(!has_gate_user_login(), std::logic_error,
                  "Missing gate user login!");
      DT_THROW_IF(!has_gate_user_password(), std::logic_error,
                  "Missing gate user password!");

      // Create access profile:
      DT_LOG_DEBUG(get_logging_priority(),
                   "Creating access profile '"
                   << vire::com::to_string(vire::com::ACCESS_CATEGORY_CLIENT_GATE) << "'");
      vire::com::access_profile & client_gate_access_profile =
        _com_->create_access_profile(_gate_user_login_,
                                    _gate_user_password_,
                                    vire::com::ACCESS_CATEGORY_CLIENT_GATE);
      {
        std::string hub_name = "connection";
        DT_LOG_DEBUG(get_logging_priority(),
                     "Creating access hub '" << hub_name << "'");
        _client_gate_hub_ = 
          client_gate_access_profile.create_access_hub(hub_name, "all", false);
      }
      return;
    }

    void negotiation_service::_at_reset_()
    {
      _client_gate_hub_.reset();
      _client_ = nullptr;
      _com_ = nullptr;
      _gate_user_login_.clear();
      _gate_user_password_.clear();
      return;
    }
    
    void negotiation_service::print_tree(std::ostream & out_,
                                         const boost::property_tree::ptree & options_) const
    {
      i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);
      
      this->::datatools::base_service::print_tree(out_,
                                                  base_print_options::force_inheritance(options_));
                                   
      out_ << popts.indent << i_tree_dumpable::tag
           << "Client : ";
      if (_client_ != nullptr) {
        out_ << "@" << _client_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
                                   
      out_ << popts.indent << i_tree_dumpable::tag
           << "Com service : ";
      if (_com_ != nullptr) {
        out_ << "@" << _com_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
                                  
      out_ << popts.indent << i_tree_dumpable::tag
           << "Gate user login : "
           << "'" << _gate_user_login_ << "'" << std::endl;
                                   
      out_ << popts.indent << i_tree_dumpable::tag
           << "Gate user password : "
           << "'" << _gate_user_password_ << "'" << std::endl;
                                  
      out_ << popts.indent << i_tree_dumpable::inherit_tag(popts.inherit)
           << "Client gate hub : ";
      if (_client_gate_hub_.get()) {
        out_ << "@" << _client_gate_hub_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      
      return;
    }

  } // namespace cmsclient

} // namespace vire
