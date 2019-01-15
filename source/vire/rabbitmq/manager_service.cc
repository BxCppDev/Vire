//! \file vire/rabbitmq/manager_service.cc
//
// Copyright (c) 2017-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
//                            Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/rabbitmq/manager_service.h>

// Standard Library:
#include <mutex>
#include <thread>

// Third party:
#include <boost/algorithm/string.hpp>
#include <bayeux/rabbitmq/rabbit_mgr.h>
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/service_tools.h>

// This package:
#include <vire/rabbitmq/permissions_utils.h>
#include <vire/com/domain_builder.h>

namespace vire {

  namespace rabbitmq {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(manager_service, "vire::rabbitmq::manager_service")

    //! PIMPL-ized internal working data:
    class manager_service::pimpl_type
    {
    public:
      std::mutex mgr_mutex;
      std::unique_ptr<::rabbitmq::rabbit_mgr> mgr;
    };

    // static
    const std::string & manager_service::vire_cms_path()
    {
      static const std::string _vcp("/cms/vire");
      return _vcp;
    }

    // static
    const std::string & manager_service::vire_cms_exchange_service_suffix()
    {
      static const std::string _s(".service");
      return _s;
    }

    // static
    const std::string & manager_service::vire_cms_exchange_event_suffix()
    {
      static const std::string _s(".event");
      return _s;
    }

    void manager_service::set_destroy_all_at_reset(const bool d_)
    {
      _destroy_all_at_reset_ = d_;
      return;
    }

    bool manager_service::is_destroy_all_at_reset() const
    {
      return _destroy_all_at_reset_;
    }

    void manager_service::_set_defaults_()
    {
      _server_port_ = -1;
      return;
    }

    bool manager_service::has_system_user_name_prefix() const
    {
      return !_system_user_name_prefix_.empty();
    }

    const std::string & manager_service::get_system_user_name_prefix() const
    {
      return _system_user_name_prefix_;
    }

    void manager_service::set_system_user_name_prefix(const std::string & sunp_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ manager service is already initialized!");
      DT_THROW_IF(sunp_.size() < 1, std::logic_error,
                  "Invalid system user host name prefix!");
      // DT_THROW_IF(sunp_[0] != '/', std::logic_error,
      //             "Invalid virtual host name prefix '" << vnp_ << "'!");
      // DT_THROW_IF(!vire::com::domain_builder::validate_domain_name_prefix(sunp_),
      //             std::logic_error,
      //             "Invalid virtual host name prefix '" << vnp_ << "'!");                  
      _system_user_name_prefix_ = sunp_;
      return;
    }

    bool manager_service::has_vhost_name_prefix() const
    {
      return !_vhost_name_prefix_.empty();
    }

    const std::string & manager_service::get_vhost_name_prefix() const
    {
      return _vhost_name_prefix_;
    }

    void manager_service::set_vhost_name_prefix(const std::string & vnp_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ manager service is already initialized!");
      DT_THROW_IF(vnp_.size() < 1, std::logic_error,
                  "Invalid virtual host name prefix!");
      DT_THROW_IF(vnp_[0] != '/', std::logic_error,
                  "Invalid virtual host name prefix '" << vnp_ << "'!");
      DT_THROW_IF(!vire::com::domain_builder::validate_domain_name_prefix(vnp_),
                  std::logic_error,
                  "Invalid virtual host name prefix '" << vnp_ << "'!");                  
      _vhost_name_prefix_ = vnp_;
      return;
    }

    bool manager_service::has_server_host() const
    {
      return !_server_host_.empty();
    }

    const std::string & manager_service::get_server_host() const
    {
      return _server_host_;
    }

    void manager_service::set_server_host(const std::string & host_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ manager service is already initialized!");
      _server_host_ = host_;
      return;
    }

    bool manager_service::has_server_port() const
    {
      return _server_port_ > 0;
    }

    int manager_service::get_server_port() const
    {
      return _server_port_;
    }

    void manager_service::set_server_port(const int port_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ manager service is already initialized!");
      DT_THROW_IF(port_ == 0, std::logic_error,
                  "RabbitMQ manager service does not allow port = 0!");
      int port = port_;
      if (port < 0) port = -1;
      _server_port_ = port;
      return;
    }

    bool manager_service::has_admin_login() const
    {
      return !_admin_login_.empty();
    }

    const std::string & manager_service::get_admin_login() const
    {
      return _admin_login_;
    }

    void manager_service::set_admin_login(const std::string & host_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ manager service is already initialized!");
      _admin_login_ = host_;
      return;
    }

    bool manager_service::has_admin_password() const
    {
      return !_admin_password_.empty();
    }

    const std::string & manager_service::get_admin_password() const
    {
      return _admin_password_;
    }

    void manager_service::set_admin_password(const std::string & host_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ manager service is already initialized!");
      _admin_password_ = host_;
      return;
    }

    const manager_service::system_user_dict_type & manager_service::get_system_users() const
    {
      return _system_users_;
    }

    manager_service::system_user_dict_type & manager_service::_grab_system_users_()
    {
      if (_system_users_.size() == 0) {
        _init_system_users_();
      }
      return _system_users_;
    }
   
    void manager_service::_init_system_users_()
    {
      DT_THROW_IF(!has_system_user_name_prefix(),
                  std::logic_error,
                  "Missing system user name prefix!");
      user server_cms_user(_system_user_name_prefix_ + "vireservercms",
                           "vireservercms",
                           vire::com::ACTOR_CATEGORY_SERVER_CMS);
      add_system_user(server_cms_user);
      
      user server_gate_user(_system_user_name_prefix_ + "vireservergate",
                            "vireservergate",
                            vire::com::ACTOR_CATEGORY_SERVER_GATE);
      add_system_user(server_gate_user);
      
      user client_gate_user(_system_user_name_prefix_ + "vireclientgate",
                            "vireclientgate",
                            vire::com::ACTOR_CATEGORY_CLIENT_GATE);
      add_system_user(client_gate_user);
      
      user server_subcontractor_system_user(_system_user_name_prefix_ + "vireserverscsys",
                                            "vireserverscsys",
                                            vire::com::ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM);
      add_system_user(server_subcontractor_system_user);
      
      user server_client_system_user(_system_user_name_prefix_ + "vireserverclientsys",
                                     "vireserverclientsys",
                                     vire::com::ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM);
      add_system_user(server_client_system_user);
      
      return;
    }
    
    void manager_service::set_system_user_password(const vire::com::actor_category_type category_,
                                                   const std::string & password_)
    {
      system_user_dict_type::iterator found = _grab_system_users_().find(category_);
      DT_THROW_IF(found == _grab_system_users_().end(), std::logic_error,
                  "Not system user of category '" << vire::com::to_string(category_) << "' is defined!");
      user & sysuser = found->second;
      sysuser.set_password(password_);
      return;
    }

    bool manager_service::has_system_user(const vire::com::actor_category_type category_) const
    {
      system_user_dict_type::const_iterator found = _system_users_.find(category_);
      return found != _system_users_.end();
    }

    const user & manager_service::get_system_user(const vire::com::actor_category_type category_) const
    {
      system_user_dict_type::const_iterator found = _system_users_.find(category_);
      DT_THROW_IF(found == _system_users_.end(), std::logic_error,
                  "Not system user of category '" << vire::com::to_string(category_) << "' is defined!");
      return found->second;
    }
    
    void manager_service::add_system_user(const user & sysuser_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ manager service is already initialized! Cannot add system users anymore!");     
      DT_THROW_IF(!sysuser_.is_complete(), std::logic_error,
                  "Cannot add an incomplete system user!");
      DT_LOG_DEBUG(get_logging_priority(), "Adding management for system user '" << sysuser_.get_login() << "'...");
      if (sysuser_.get_category() == vire::com::ACTOR_CATEGORY_CLIENT_SYSTEM
          or sysuser_.get_category() == vire::com::ACTOR_CATEGORY_SUBCONTRACTOR) {
        DT_THROW(std::logic_error,
                 "Cannot add a system user of category '" << vire::com::to_string(sysuser_.get_category()) << "'!");
      }
      _system_users_[sysuser_.get_category()] = sysuser_;
      return;
    }
    
    void manager_service::add_system_user(const std::string & login_,
                                          const std::string & password_,
                                          const vire::com::actor_category_type category_)
    {
      user sysuser(login_, password_, category_);
      add_system_user(sysuser);
      return;
    }

    bool manager_service::has_subcontractor(const std::string & sc_id_) const
    {
      return _subcontractor_infos_.count(sc_id_);
    }

    const manager_service::sc_dict_type & manager_service::get_subcontractor_infos() const
    {
      return _subcontractor_infos_;
    }

    void manager_service::remove_subcontractor(const std::string & sc_id_)
    {
       DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "RabbitMQ manager service is initialized! Cannot remove subcontractor '"
                  << sc_id_ << "'!");
       sc_dict_type::iterator found = _subcontractor_infos_.find(sc_id_);
       DT_THROW_IF(found == _subcontractor_infos_.end(),
                   std::logic_error,
                   "No subcontractor entry '" << sc_id_ << "'!");
       _subcontractor_infos_.erase(sc_id_);
       return;
    }

    const manager_service::subcontractor_info &
    manager_service::get_subcontractor_info(const std::string & sc_id_) const
    {
      sc_dict_type::const_iterator found = _subcontractor_infos_.find(sc_id_);
      DT_THROW_IF(found == _subcontractor_infos_.end(),
                  std::logic_error,
                  "No subcontractor entry '" << sc_id_ << "'!");
      return found->second;
    }

    const manager_service::subcontractor_info &
    manager_service::get_subcontractor_info_per_vhost(const std::string & vhost_name_) const
    {
      const manager_service::subcontractor_info * sc_entry = nullptr;
      for (const auto & sc_pair : _subcontractor_infos_) {
        if (sc_pair.second.system_vhost_name == vhost_name_) {
          sc_entry = &sc_pair.second;
          break;
        }
      }
      DT_THROW_IF(sc_entry == nullptr,
                  std::logic_error,
                  "No subcontractor entry with system vhost '" << vhost_name_ << "'!");
      return *sc_entry;
    }

    const manager_service::subcontractor_info &
    manager_service::get_subcontractor_info_per_user(const std::string & user_login_) const
    {
      const manager_service::subcontractor_info * sc_entry = nullptr;
      for (const auto & sc_pair : _subcontractor_infos_) {
        if (sc_pair.second.user_login == user_login_) {
          sc_entry = &sc_pair.second;
          break;
        }
      }
      DT_THROW_IF(sc_entry == nullptr,
                  std::logic_error,
                  "No subcontractor entry with user '" << user_login_ << "'!");
      return *sc_entry;
    }
  
    void manager_service::add_subcontractor(const subcontractor_info & sc_info_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "RabbitMQ manager service is initialized! Cannot add subcontractor '"
                  << sc_info_.id << "'!");
      DT_THROW_IF(!has_vhost_name_prefix(), std::logic_error, "Missing vhost name prefix!");
      subcontractor_info sc_info   = sc_info_;
      std::string sc_id            = sc_info.id;
      std::string sc_user_login    = sc_info.user_login;
      std::string sc_user_password = sc_info.user_password;
      // Build the subcontractor's system vhost name:
      sc_info.system_vhost_name =
        vire::com::domain_builder::build_cms_subcontractor_system_name(get_vhost_name_prefix(),
                                                                       sc_info.id);

      DT_THROW_IF(sc_id.empty(), std::logic_error, "Missing subcontractor identifier");
      DT_THROW_IF(sc_user_login.empty(), std::logic_error, "Missing user login for subcontractor '" << sc_id << "'!");
      DT_THROW_IF(sc_user_password.empty(), std::logic_error, "Missing user password for subcontractor '" << sc_id << "'!");

      _subcontractor_infos_[sc_id] = sc_info;
      return;
    }

    void manager_service::_setup_vire_cms_subcontractors_()
    {
      for (const auto & sc_pair : _subcontractor_infos_) {
        const std::string & sc_id = sc_pair.first;
        _setup_vire_cms_subcontractor_(sc_id);
      }
      return;
    }

    void manager_service::_dismantle_vire_cms_subcontractors_()
    {
      for (const auto & sc_pair : _subcontractor_infos_) {
        const std::string & sc_id = sc_pair.first;
        const subcontractor_info & sc_info = sc_pair.second;
        _dismantle_vire_cms_subcontractor_(sc_id);
      }
      return;
    }

    void manager_service::_dismantle_vire_cms_subcontractor_(const std::string & sc_id_)
    {
      sc_dict_type::iterator found = _subcontractor_infos_.find(sc_id_);
      DT_THROW_IF(found == _subcontractor_infos_.end(),
                  std::logic_error,
                  "No subcontractor entry '" << sc_id_ << "'!");
      const subcontractor_info & sc_info = found->second;
      if (has_vhost(sc_info.system_vhost_name)) {
        remove_vhost(sc_info.system_vhost_name);
      }
      if (has_user(sc_info.user_login)) {
        remove_user(sc_info.user_login);
      }
      return;
    }

    void manager_service::_setup_vire_cms_subcontractor_(const std::string & sc_id_)
    {
      sc_dict_type::const_iterator found = _subcontractor_infos_.find(sc_id_);
      DT_THROW_IF(found == _subcontractor_infos_.end(),
                  std::logic_error,
                  "No subcontractor entry '" << sc_id_ << "'!");
      const subcontractor_info & sc_info = found->second;

      {
        // Subcontractor side:
        vire::rabbitmq::user sc_user(sc_info.user_login,
                                     sc_info.user_password,
                                     vire::com::ACTOR_CATEGORY_SUBCONTRACTOR);
        if (!has_user(sc_user.get_login())) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Add subcontractor user '" << sc_user.get_login() << "'...");
          add_user(sc_user);
        }
      }
      
      {
        // Subcontractor system vhost:
        vire::rabbitmq::vhost vh(sc_info.system_vhost_name,
                                 vire::com::DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM);
        if (!has_vhost(vh.get_name())) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Add subcontractor system vhost '" << vh.get_name() << "'...");
          add_vhost(vh);
        }
      }

      return;
    }
    
    manager_service::manager_service(uint32_t flags_)
    {
      _initialized_ = false;
      DT_LOG_TRACE(get_logging_priority(), "Creating PIMPL...");
      _pimpl_.reset(new pimpl_type);
      _set_defaults_();
      return;
    }

    manager_service::~manager_service()
    {
      if (is_initialized()) {
        reset();
      }
      if (_pimpl_) {
        _pimpl_.reset();
      }
      return;
    }

    void manager_service::print_tree(std::ostream & out_,
                                     const boost::property_tree::ptree & options_) const
    {
      i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);
      std::ostringstream outs;

      this->datatools::base_service::print_tree(outs,
                                                base_print_options::force_inheritance(options_));

      outs << popts.indent << tag
           << "System user name prefix : "
           << "'" << _system_user_name_prefix_ << "'" << std::endl;
  
      outs << popts.indent << tag
           << "Virtual host name prefix : "
           << "'" << _vhost_name_prefix_ << "'" << std::endl;

      outs << popts.indent << tag
           << "Server host : "
           << "'" << _server_host_ << "'" << std::endl;

      outs << popts.indent << tag
           << "Server port : "
           << _server_port_ << std::endl;

      outs << popts.indent << tag
           << "Administrator login : "
           << "'" << _admin_login_ << "'" << std::endl;

      outs << popts.indent << tag
           << "Administrator password : "
           << "'" << _admin_password_ << "'" << std::endl;

      outs << popts.indent << tag
           << "System users : ["
           << _system_users_.size() << ']' << std::endl;
      {
        std::size_t us_counter = 0;
        for (const auto & su_pair : _system_users_) {
          outs << popts.indent << skip_tag;
          if (us_counter + 1 != _system_users_.size()) {
            outs << tag;
          } else {
            outs << last_tag;
          }
          outs << "User [" << vire::com::to_string(su_pair.second.get_category()) << "] : " <<  su_pair.second.get_login();
          outs << std::endl;
          us_counter++;
        }
      }
 
      outs << popts.indent << tag
           << "Subcontractors : ["
           << _subcontractor_infos_.size() << ']' << std::endl;
      {
        std::size_t sc_counter = 0;
        for (const auto & sc_pair : _subcontractor_infos_) {
          const subcontractor_info & sc_info = sc_pair.second;
          outs << popts.indent << skip_tag;
          if (sc_counter + 1 != _subcontractor_infos_.size()) {
            outs << tag;
          } else {
            outs << last_tag;
          }
          outs << "Subcontractor '" << sc_pair.first << "' : login='" <<  sc_info.user_login << "'";
          outs << std::endl;
          sc_counter++;
        }
      }
 
      outs << popts.indent << tag
           << "Vhosts : "
           << _vhosts_.size() << std::endl;

      outs << popts.indent << tag
           << "Users : "
           << _users_.size() << std::endl;

      outs << popts.indent << tag
           << "Destroy all at reset : "
           << std::boolalpha << _destroy_all_at_reset_ << std::endl;

      outs << popts.indent << inherit_tag(popts.inherit)
           << "Pimpl : "
           << std::boolalpha << (_pimpl_.get() != nullptr) << std::endl;
    
      out_ << outs.str();
      return;
    }

    // virtual
    bool manager_service::is_initialized() const
    {
      return _initialized_;
    }

    // virtual
    int manager_service::initialize(const datatools::properties & config_,
                                    datatools::service_dict_type & services_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ manager service is already initialized!");

      // Load config:
      this->::datatools::base_service::common_initialize(config_);

      // // RabbitMQ server parameters:
      // if (!has_server_host()) {
      //        if (config_.has_key("server_host")) {
      //          std::string server_host = config_.fetch_string("server_host");
      //     set_server_host(server_host);
      //   }
      // }

      // if (!has_server_port()) {
      //   if (config_.has_key("server_port")) {
      //     int server_port = config_.fetch_positive_integer("server_port");
      //     set_server_port(server_port);
      //   }
      // }

      // if (!has_admin_login()) {
      //   if (config_.has_key("admin_login")) {
      //     std::string admin_login = config_.fetch_string("admin_login");
      //     set_admin_login(admin_login);
      //   }
      // }

      // if (!has_admin_password()) {
      //   if (config_.has_key("admin_password")) {
      //     std::string admin_password = config_.fetch_string("admin_password");
      //     set_admin_password(admin_password);
      //   }
      // }
      
      if (_server_host_.empty()) {
        DT_LOG_DEBUG(get_logging_priority(), "Set default RabbitMQ management driver host...");
        set_server_host("localhost");
      }
     
      if (_server_port_ <= 0) {
        DT_LOG_DEBUG(get_logging_priority(), "Set default RabbitMQ management driver port...");
        set_server_port(::rabbitmq::rabbit_mgr::SSL_PORT);
      }
    
      DT_THROW_IF(_admin_login_.empty(), std::logic_error, "Missing admin login!");
      DT_THROW_IF(_admin_password_.empty(), std::logic_error, "Missing admin password!");

      
      DT_LOG_DEBUG(get_logging_priority(), "server_host           : " << _server_host_);
      DT_LOG_DEBUG(get_logging_priority(), "server_port           : " << _server_port_);
      DT_LOG_DEBUG(get_logging_priority(), "admin_login           : " << _admin_login_);
      DT_LOG_DEBUG(get_logging_priority(), "admin_password        : " << _admin_password_);

      // Initialize RabbitMQ management driver:
      DT_LOG_DEBUG(get_logging_priority(), "Instantiating the RabbitMQ management driver...");
      _pimpl_->mgr.reset(new ::rabbitmq::rabbit_mgr(_server_host_,
                                                    _server_port_,
                                                    _admin_login_,
                                                    _admin_password_));

      // // CMS specific:
      // if (!has_system_user_name_prefix()) {
      //   if (config_.has_key("system_user_name_prefix")) {
      //     std::string vnp = config_.fetch_string("system_user_name_prefix");
      //     set_vhost_name_prefix(vnp);
      //   }
      // }
      // if (!has_vhost_name_prefix()) {
      //   if (config_.has_key("vhost_name_prefix")) {
      //     std::string vnp = config_.fetch_string("vhost_name_prefix");
      //     set_vhost_name_prefix(vnp);
      //   }
      // }

      DT_THROW_IF(!has_system_user_name_prefix(),
                  std::logic_error,
                  "Missing system user name prefix!");

      DT_THROW_IF(!has_vhost_name_prefix(),
                  std::logic_error,
                  "Missing vhost name prefix!");
      DT_LOG_DEBUG(get_logging_priority(), "system_user_name_prefix : '" << _system_user_name_prefix_ << "'");
      DT_LOG_DEBUG(get_logging_priority(), "vhost_name_prefix       : '" << _vhost_name_prefix_ << "'");

      // Set system users:
      DT_LOG_DEBUG(get_logging_priority(), "Setup system users...");
      if (_system_users_.size() == 0) {
        _init_system_users_();
      }

      // Vire CMS system:
      DT_LOG_DEBUG(get_logging_priority(), "Setup Vire CMS system...");
      _setup_vire_cms_system_users_();
      _setup_vire_cms_system_domains_();

      // Subcontractors:
      {
        std::set<std::string> subcontractors;
        if (config_.has_key("subcontractors")) {
          config_.fetch("subcontractors", subcontractors);
        }
        for (const std::string & sc_id : subcontractors) {
          subcontractor_info sc_info;
          DT_LOG_DEBUG(get_logging_priority(), "Adding management for subcontractor '" << sc_id << "'...");
          sc_info.id = sc_id;
          {
            std::ostringstream desc_key_ss;
            desc_key_ss << "subcontractors." << sc_id << ".description";
            if (config_.has_key(desc_key_ss.str())) {
              sc_info.description = config_.fetch_string(desc_key_ss.str());
            }
          }
          {
            std::ostringstream user_login_key_ss;
            user_login_key_ss << "subcontractors." << sc_id << ".user_login";
            if (config_.has_key(user_login_key_ss.str())) {
              sc_info.user_login = config_.fetch_string(user_login_key_ss.str());
            }
          }
          {
            std::ostringstream user_password_key_ss;
            user_password_key_ss << "subcontractors." << sc_id << ".user_password";
            if (config_.has_key(user_password_key_ss.str())) {
              sc_info.user_password = config_.fetch_string(user_password_key_ss.str());
            }
          }
          {
            std::ostringstream persistent_key_ss;
            persistent_key_ss << "subcontractors." << sc_id << ".persistent";
            if (config_.has_key(persistent_key_ss.str())) {
              sc_info.persistent = config_.fetch_boolean(persistent_key_ss.str());
            }
          }
          add_subcontractor(sc_info);
        }
      }

      DT_LOG_DEBUG(get_logging_priority(), "Setup Vire CMS subcontractors...");
      _setup_vire_cms_subcontractors_();

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    int manager_service::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error, "RabbitMQ manager service is not initialized!");
      _initialized_ = false;

      // Remove Vire client users:
      {
        std::set<std::string> logins;
        fetch_users(logins, vire::com::ACTOR_CATEGORY_CLIENT_SYSTEM);
        for (const auto & login : logins) {
          remove_user(login);
        }
      }
       
      {
        std::set<std::string> logins;
        fetch_users(logins, vire::com::ACTOR_CATEGORY_CLIENT_CMS);
        for (const auto & login : logins) {
          remove_user(login);
        }
      }

      // Remove Vire client system vhosts:
      {
        std::set<std::string> names;
        fetch_vhosts(names, vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM);
        for (const auto & name : names) {
          DT_LOG_DEBUG(get_logging_priority(), "Removing client system host '" << name << "'...");
          remove_vhost(name);
        }
      }

      // Remove subcontractors:
      DT_LOG_DEBUG(get_logging_priority(), "Dismantling subcontractors domains and users...");
      _dismantle_vire_cms_subcontractors_();
      _subcontractor_infos_.clear();

      // System:
      if (is_destroy_all_at_reset()) {
        DT_LOG_DEBUG(get_logging_priority(), "Dismantling system domains and users...");
        _dismantle_vire_cms_system_domains_();
        _dismantle_vire_cms_system_users_();
      }
      
      _users_.clear();
      _vhosts_.clear();
      _pimpl_->mgr.reset();
      _vhost_gate_name_.clear();
      _vhost_control_name_.clear();
      _vhost_monitoring_name_.clear();
      _system_users_.clear();
      _admin_login_.clear();
      _admin_password_.clear();
      _server_port_ = -1;
      _server_host_.clear();
      _vhost_name_prefix_.clear();
      _system_user_name_prefix_.clear();
 
      _set_defaults_();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    const ::rabbitmq::rabbit_mgr & manager_service::get_manager() const
    {
      DT_THROW_IF(!_pimpl_->mgr.get(), std::logic_error,
                  "RabbitMQ manager is not instantiated!");
      return *(_pimpl_->mgr.get());
    }

    ::rabbitmq::rabbit_mgr & manager_service::grab_manager()
    {
      DT_THROW_IF(!_pimpl_->mgr.get(), std::logic_error,
                  "RabbitMQ manager is not instantiated!");
      return *(_pimpl_->mgr.get());
    }

    bool manager_service::_has_vhost_(const std::string & name_) const
    {
      ::rabbitmq::vhost::list vhosts;
      ::rabbitmq::error_response err;
      manager_service * mutable_this = const_cast<manager_service*>(this);
      mutable_this->grab_manager().list_vhosts(vhosts, err);
      for (const auto & v : vhosts) {
        if (v.name == name_) return true;
      }
      return false;      
    }

    void manager_service::fetch_vhosts(std::set<std::string> & names_,
                                       const vire::com::domain_category_type category_) const
    {
      names_.clear();
      for (const auto & p : _vhosts_) {
        bool add_it = false;
        if (category_ != vire::com::DOMAIN_CATEGORY_INVALID) {
          const vhost & vh = p.second;
          if (vh.get_category() == category_) {
            add_it = true;
          }
        } else {
          add_it = true;
        }
        if (add_it) {
          names_.insert(p.first);
        }
      }
      return;
    }
 
    void manager_service::fetch_users(std::set<std::string> & logins_,
                                      const vire::com::actor_category_type category_) const
    {
      logins_.clear();
      for (const auto & p : _users_) {
        bool add_it = false;
        if (category_ != vire::com::ACTOR_CATEGORY_INVALID) {
          const user & u = p.second;
          if (u.get_category() == category_) {
            add_it = true;
          }
        } else {
          add_it = true;
        }
        if (add_it) {
          logins_.insert(p.first);
        }
      }
      return;
    }
    
    bool manager_service::has_vhost(const std::string & name_,
                                    const vire::com::domain_category_type category_) const
    {
      std::map<std::string, vhost>::const_iterator found = _vhosts_.find(name_);
      if (found != _vhosts_.end()) {
        const vhost & vh = found->second;
        if (category_ != vire::com::DOMAIN_CATEGORY_INVALID) {
          return vh.get_category() == category_;
        }
        return true;
      }
      return false;
    }

    const vhost & manager_service::get_vhost(const std::string & name_) const
    {
      std::map<std::string, vhost>::const_iterator found = _vhosts_.find(name_);
      DT_THROW_IF(found == _vhosts_.end(),
                  std::logic_error,
                  "No vhost named '" << name_ << "'!");
      return found->second;
    }

    void manager_service::add_vhost(const vhost & vh_)
    {      
      DT_THROW_IF(!vh_.is_complete(), std::logic_error, "Vhost is not complete!");
      if (is_initialized()) {
        DT_THROW_IF(vh_.get_category() != vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM,
                    std::logic_error,
                    "Cannot add non system client vhost!");
      } else {
        DT_THROW_IF(vh_.get_category() == vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM,
                    std::logic_error,
                    "Cannot add system client vhost!");
      }
      for (auto cat : vire::com::domain_categories_with_unique_domain()) {
        if (vh_.get_category() == cat) {
          std::set<std::string> vhs;
          fetch_vhosts(vhs, cat);
          DT_THROW_IF(vhs.size(), std::logic_error,
                      "Vhost of category '"
                      << vire::com::to_string(vh_.get_category())
                      << "' already exists!");
        }
      }
      DT_THROW_IF(has_vhost(vh_.get_name()), std::logic_error, "Vhost '" << vh_.get_name() << "' already exists!");
      DT_LOG_DEBUG(get_logging_priority(), "Adding management for vhost '" << vh_.get_name() << "'...");
      _vhosts_[vh_.get_name()] = vh_;
      if (_vhost_gate_name_.empty() && vh_.get_category() == vire::com::DOMAIN_CATEGORY_GATE) {
        _vhost_gate_name_ = vh_.get_name();
      }
      if (_vhost_control_name_.empty() && vh_.get_category() == vire::com::DOMAIN_CATEGORY_CONTROL) {
        _vhost_control_name_ = vh_.get_name();
      }
      if (_vhost_control_name_.empty() && vh_.get_category() == vire::com::DOMAIN_CATEGORY_MONITORING) {
        _vhost_monitoring_name_ = vh_.get_name();
      }
      // RabbitMQ backend:
      if (!_has_vhost_(vh_.get_name())) {
        DT_LOG_DEBUG(get_logging_priority(), "Creating RabbitMQ vhost '" << vh_.get_name() << "'...");
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().add_vhost(vh_.get_name(), err),
                    std::logic_error,
                    "Cannot create RabbitMQ vhost '" << vh_.get_name() << "': "
                    + err.error + ": " + err.reason + "!");
        if (vh_.get_category() == vire::com::DOMAIN_CATEGORY_CONTROL) {
          _setup_vire_cms_domains_control_(vh_.get_name());
        }
        if (vh_.get_category() == vire::com::DOMAIN_CATEGORY_MONITORING) {
          _setup_vire_cms_domains_monitoring_(vh_.get_name());
        }
        if (vh_.get_category() == vire::com::DOMAIN_CATEGORY_GATE) {
          _setup_vire_cms_domains_clients_gate_(vh_.get_name());
        }
        if (vh_.get_category() == vire::com::DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
          const subcontractor_info & sc_info = get_subcontractor_info_per_vhost(vh_.get_name());
          _setup_vire_cms_domains_subcontractor_system_(sc_info.id);
        }
        if (vh_.get_category() == vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM) {
          const client_info & cl_info = get_client_info_per_vhost(vh_.get_name());
          _setup_vire_cms_domains_client_system_(cl_info.id);
        }
      }
      return;
    }

    void manager_service::remove_vhost(const std::string & name_)
    {
      DT_THROW_IF(!has_vhost(name_), std::logic_error,
                  "No management for vhost named '" << name_ << "' exists!");
      const vhost & vh = get_vhost(name_);
      bool delete_it = true;
      if (!_destroy_all_at_reset_
          and vh.get_category() == vire::com::DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        const subcontractor_info & sc_info = get_subcontractor_info_per_vhost(name_);
        if (sc_info.persistent) {
          delete_it = false;
        }
      }
      if (delete_it) {
        // Only delete temporary vhost related to Vire client:
        if (_has_vhost_(vh.get_name())) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Deleting RabbitMQ vhost '" << vh.get_name() << "'...");
         ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().delete_vhost(vh.get_name(), err),
                      std::logic_error,
                      "Cannot delete RabbitMQ vhost '" << vh.get_name() << "': "
                      + err.error + ": " + err.reason + "!");
        }
      }
      DT_LOG_DEBUG(get_logging_priority(),
                   "Removing management for vhost '" << vh.get_name() << "'...");
      _vhosts_.erase(name_);
      return;
    }

    bool manager_service::_has_exchange_(const std::string & vhost_,
                                         const std::string & name_) const
    {
      ::rabbitmq::exchange::list exchanges;
      ::rabbitmq::error_response err;
      manager_service * mutable_this = const_cast<manager_service*>(this);
      mutable_this->grab_manager().list_exchanges(vhost_, exchanges, err);
      for (const auto & e : exchanges) {
        if (e.name == name_) return true;
      }
      return false;
    }

    bool manager_service::_has_queue_(const std::string & vhost_,
                                      const std::string & name_) const
    {
      ::rabbitmq::queue::list queues;
      ::rabbitmq::error_response err;
      manager_service * mutable_this = const_cast<manager_service*>(this);
      mutable_this->grab_manager().list_queues(vhost_, queues, err);
      for (const auto & q : queues) {
        if (q.name == name_) return true;
      }
      return false;
    }

    bool manager_service::_has_user_(const std::string & name_) const
    {
      ::rabbitmq::user::list users;
      ::rabbitmq::error_response err;
      manager_service * mutable_this = const_cast<manager_service*>(this);
      mutable_this->grab_manager().list_users(users, err);
      for (const auto & u : users) {
        if (u.name == name_) return true;
      }
      return false;
    }

    bool manager_service::has_user(const std::string & name_,
                                   const vire::com::actor_category_type category_) const
    {
      std::map<std::string, user>::const_iterator found = _users_.find(name_);
      if (found != _users_.end()) {
        const user & u = found->second;
        if (category_ != vire::com::ACTOR_CATEGORY_INVALID) {
          return u.get_category() == category_;
        }
        return true;
      }
      return false;
    }

    const user & manager_service::get_user(const std::string & login_) const
    {
      std::map<std::string, user>::const_iterator found = _users_.find(login_);
      DT_THROW_IF(found == _users_.end(),
                  std::logic_error,
                  "No user '" << login_ << "'!");
      return found->second;
    }

    void manager_service::add_user(const user & user_)
    {      
      DT_THROW_IF(!user_.is_complete(), std::logic_error, "User is not complete!");
      for (auto cat : vire::com::actor_categories_with_unique_user()) {
        if (user_.get_category() == cat) {
          std::set<std::string> users;
          fetch_users(users, cat);
          DT_THROW_IF(users.size(), std::logic_error,
                      "User of category '"
                      << vire::com::to_string(user_.get_category())
                      << "' already exists!");
        }
      }
      DT_THROW_IF(has_user(user_.get_login()), std::logic_error,
                  "Management for user '" << user_.get_login() << "' already exists!");
      DT_LOG_DEBUG(get_logging_priority(), "Adding management for user '" << user_.get_login() << "'...");
      _users_[user_.get_login()] = user_;
      // Backend:
      if (!_has_user_(user_.get_login())) {
        DT_LOG_DEBUG(get_logging_priority(), "Adding RabbitMQ user '" << user_.get_login() << "'...");
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().add_user(user_.get_login(), user_.get_password(), err),
                    std::logic_error,
                    "Cannot create RabbitMQ user '" << user_.get_login() << "': "
                    + err.error + ": " + err.reason + "!");
      }
      return;
    }

    void manager_service::remove_user(const std::string & login_)
    {
      DT_THROW_IF(!has_user(login_), std::logic_error, "No management for user '" << login_ << "' exists!");
      const user & u = get_user(login_);
      bool delete_it = true;
      if (!_destroy_all_at_reset_
          and u.get_category() == vire::com::ACTOR_CATEGORY_SUBCONTRACTOR) {
        const subcontractor_info & sc_info = get_subcontractor_info_per_user(login_);
        if (sc_info.persistent) {
          delete_it = false;
        }
      }
      if (delete_it) {
        // Only delete temporary users related to Vire client:
        if (_has_user_(u.get_login())) {
          DT_LOG_DEBUG(get_logging_priority(), "Deleting RabbitMQ client user '" << u.get_login() << "'...");
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().delete_user(u.get_login(), err),
                      std::logic_error,
                      "Cannot delete RabbitMQ client user '" << u.get_login() << "': "
                      + err.error + ": " + err.reason + "!");
        }
      }
      DT_LOG_DEBUG(get_logging_priority(), "Removing management for user '" << u.get_login() << "'...");
      _users_.erase(login_);
      return;
    }

    bool manager_service::has_client(const std::string & client_id_) const
    {
      return _client_infos_.count(client_id_) > 0;
    }

    const manager_service::client_info &
    manager_service::get_client_info(const std::string & cl_id_) const
    {
      client_dict_type::const_iterator found = _client_infos_.find(cl_id_);
      DT_THROW_IF(found == _client_infos_.end(),
                  std::logic_error,
                  "No client entry '" << cl_id_ << "'!");
      return found->second;
    }

    const manager_service::client_info &
    manager_service::get_client_info_per_vhost(const std::string & vhost_name_) const
    {
      const manager_service::client_info * cl_entry = nullptr;
      for (const auto & cl_pair : _client_infos_) {
        if (cl_pair.second.system_vhost_name == vhost_name_) {
          cl_entry = &cl_pair.second;
          break;
        }
      }
      DT_THROW_IF(cl_entry == nullptr,
                  std::logic_error,
                  "No client entry with system vhost '" << vhost_name_ << "'!");
      return *cl_entry;
    }
 
    void manager_service::_setup_vire_cms_client_(const std::string & client_id_)
    {
      client_dict_type::const_iterator found = _client_infos_.find(client_id_);
      DT_THROW_IF(found == _client_infos_.end(),
                  std::logic_error,
                  "No client entry '" << client_id_ << "'!");
      const client_info & cl_info = found->second;

      {
        // Client system side:
        vire::rabbitmq::user cl_user(cl_info.sys_user_login,
                                     cl_info.sys_user_password,
                                     vire::com::ACTOR_CATEGORY_CLIENT_SYSTEM);
        if (!has_user(cl_user.get_login())) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Add system client user '" << cl_user.get_login() << "'...");
          add_user(cl_user);
        }
      }
 
      {
        // Client CMS side:
        vire::rabbitmq::user cl_user(cl_info.cms_user_login,
                                     cl_info.cms_user_password,
                                     vire::com::ACTOR_CATEGORY_CLIENT_CMS);
        if (!has_user(cl_user.get_login())) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Add CMS client user '" << cl_user.get_login() << "'...");
          add_user(cl_user);
        }
      }
        
      {
        // Client system vhost:
        vire::rabbitmq::vhost vh(cl_info.system_vhost_name,
                                 vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM);
        if (!has_vhost(vh.get_name())) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Add client system vhost '" << vh.get_name() << "'...");
          add_vhost(vh);
        }
      }
    
      return;
    }
   
    void manager_service::create_client(const client_info & client_info_)
    {
      DT_THROW_IF(!is_initialized(),
                  std::logic_error,
                  "RabbitMQ manager service is not initialized! Cannot add client '"
                  << client_info_.id << "'!");
      DT_THROW_IF(client_info_.id.empty(), std::logic_error, "Missing client identifier");
      DT_LOG_DEBUG(get_logging_priority(),
                   "Creating management client system user '" << client_info_.sys_user_login << "'...");
      DT_THROW_IF(client_info_.sys_user_login.empty(), std::logic_error,
                  "Missing system user login for client '" << client_info_.id << "'!");
      DT_THROW_IF(client_info_.sys_user_password.empty(), std::logic_error,
                  "Missing system user password for client '" << client_info_.id << "'!");
      DT_LOG_DEBUG(get_logging_priority(),
                   "Creating management client CMS user '" << client_info_.sys_user_login << "'...");
      DT_THROW_IF(client_info_.cms_user_login.empty(), std::logic_error,
                  "Missing CMS user login for client '" << client_info_.id << "'!");
      DT_THROW_IF(client_info_.cms_user_password.empty(), std::logic_error,
                  "Missing CMS user password for client '" << client_info_.id << "'!");
      client_info cl_info = client_info_;
      cl_info.system_vhost_name =
        vire::com::domain_builder::build_cms_client_system_name(get_vhost_name_prefix(),
                                                                cl_info.id);
      _client_infos_[cl_info.id] = cl_info;
 
      // Backend ops:
      _setup_vire_cms_client_(cl_info.id);
      
      DT_LOG_DEBUG(get_logging_priority(), "Created management client user with ID '" << client_info_.id << "'.");
      return;
    }

    void manager_service::change_client_passwords(const std::string & client_id_,
                                                  const std::string & sys_user_password_,
                                                  const std::string & cms_user_password_)
    {
      DT_THROW_IF(!is_initialized(),
                  std::logic_error,
                  "RabbitMQ manager service is not initialized! Cannot remove client '"
                  << client_id_ << "'!");
      client_dict_type::iterator found = _client_infos_.find(client_id_);
      DT_THROW_IF(found == _client_infos_.end(),
                  std::logic_error,
                  "No client entry '" << client_id_ << "'!");
      client_info & cl_info = found->second;
      DT_LOG_DEBUG(get_logging_priority(), "Changing password for system client user '" << cl_info.sys_user_login << "'...");
      cl_info.sys_user_password = sys_user_password_;
      DT_LOG_DEBUG(get_logging_priority(), "Changing password for CMS client user '" << cl_info.cms_user_login << "'...");
      cl_info.cms_user_password = cms_user_password_;

      // Backend ops:
      {
        ::rabbitmq::error_response err;
        if (!grab_manager().change_user_password(cl_info.cms_user_login,
                                                 cl_info.cms_user_password,
                                                 err)) {
          DT_THROW(std::logic_error,
                   "Cannot change password for RabbitMQ CMS client user '" << cl_info.cms_user_login << "': "
                   + err.error + ": " + err.reason + "!");
        }
        
        if (!grab_manager().change_user_password(cl_info.sys_user_login,
                                                 cl_info.sys_user_password,
                                                 err)) {
          DT_THROW(std::logic_error,
                   "Cannot change password for RabbitMQ system client user '" << cl_info.sys_user_login << "': "
                   + err.error + ": " + err.reason + "!");
        }
      }
      
      DT_LOG_DEBUG(get_logging_priority(), "Password changed for system client user '" << cl_info.sys_user_login << "'.");
      DT_LOG_DEBUG(get_logging_priority(), "Password changed for CMS client user '" << cl_info.cms_user_login << "'.");
      return;
    }

    void manager_service::destroy_client(const std::string & client_id_)
    {
      DT_THROW_IF(!is_initialized(),
                  std::logic_error,
                  "RabbitMQ manager service is not initialized! Cannot remove client '"
                  << client_id_ << "'!");
      client_dict_type::iterator found = _client_infos_.find(client_id_);
      DT_THROW_IF(found == _client_infos_.end(),
                  std::logic_error,
                  "No client entry '" << client_id_ << "'!");
      client_info & cl_info = found->second;
      DT_LOG_DEBUG(get_logging_priority(), "Destroying management client user with ID  '" << cl_info.id << "'...");

      /// Backend ops
      
      
      DT_LOG_DEBUG(get_logging_priority(), "Destroyed management client user with ID '" << cl_info.id << "'.");
      _client_infos_.erase(client_id_);
      return;
    }
    
    void manager_service::_dismantle_vire_cms_system_users_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      for (const auto & su_pair : _system_users_) {
        const user & sysuser = su_pair.second;
        if (has_user(sysuser.get_login())) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Removing system user '" << sysuser.get_login() << "' to RabbitMQ server...");
          remove_user(sysuser.get_login());
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
 
    void manager_service::_setup_vire_cms_system_users_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      for (const auto & su_pair : _system_users_) {
        const user & sysuser = su_pair.second;
        if (!has_user(sysuser.get_login())) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Adding system user '" << sysuser.get_login() << "' to RabbitMQ server...");
          add_user(sysuser);
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_dismantle_vire_cms_system_domains_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (has_vhost(_vhost_gate_name_)) {
        vhost vh(_vhost_gate_name_, vire::com::DOMAIN_CATEGORY_GATE);
        DT_LOG_DEBUG(get_logging_priority(),
                     "Removing system vhost '" << vh.get_name() << "' to RabbitMQ server...");
        remove_vhost(vh.get_name());
      }
      
      if (has_vhost(_vhost_control_name_)) {
        vhost vh(_vhost_control_name_, vire::com::DOMAIN_CATEGORY_CONTROL);
        DT_LOG_DEBUG(get_logging_priority(),
                     "Removing system vhost '" << vh.get_name() << "' to RabbitMQ server...");
        remove_vhost(vh.get_name());
      }
      
      if (has_vhost(_vhost_monitoring_name_)) {
        vhost vh(_vhost_monitoring_name_, vire::com::DOMAIN_CATEGORY_MONITORING);
        DT_LOG_DEBUG(get_logging_priority(),
                     "Removing system vhost '" << vh.get_name() << "' to RabbitMQ server...");
        remove_vhost(vh.get_name());
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_system_domains_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_LOG_DEBUG(get_logging_priority(), "Building Vire CMS domains/vhosts...");
      
      _vhost_gate_name_ = vire::com::domain_builder::build_cms_clients_gate_name(_vhost_name_prefix_);
      if (!has_vhost(_vhost_gate_name_)) {
        vhost vh(_vhost_gate_name_, vire::com::DOMAIN_CATEGORY_GATE);
        DT_LOG_DEBUG(get_logging_priority(),
                     "Adding system vhost '" << vh.get_name() << "' to RabbitMQ server...");
        add_vhost(vh);
      }
      
      _vhost_control_name_ = vire::com::domain_builder::build_cms_control_name(_vhost_name_prefix_);
      if (!has_vhost(_vhost_control_name_)) {
        vhost vh(_vhost_control_name_, vire::com::DOMAIN_CATEGORY_CONTROL);
        DT_LOG_DEBUG(get_logging_priority(),
                     "Adding system vhost '" << vh.get_name() << "' to RabbitMQ server...");
        add_vhost(vh);
      }
      
      _vhost_monitoring_name_ = vire::com::domain_builder::build_cms_monitoring_name(_vhost_name_prefix_);
      if (!has_vhost(_vhost_monitoring_name_)) {
        vhost vh(_vhost_monitoring_name_, vire::com::DOMAIN_CATEGORY_MONITORING);
        DT_LOG_DEBUG(get_logging_priority(),
                     "Adding system vhost '" << vh.get_name() << "' to RabbitMQ server...");
        add_vhost(vh);
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_subcontractor_system_(const std::string & sc_id_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      // Find the subcontractor user associated to this subcontractor identifier:
      const subcontractor_info & sc_info = get_subcontractor_info(sc_id_);
      std::string sc_user_login = sc_info.user_login;

      // System subcontractor domain/vhost:
      {
        const vhost & vh = get_vhost(sc_info.system_vhost_name);
      
        DT_LOG_DEBUG(get_logging_priority(),
                     "Populating system subcontractor domain '" << vh.get_name() << "'...");
      
        // Set administrator permissions:
        {
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().set_permissions(get_admin_login(),
                                                      vh.get_name(),
                                                      ".*", ".*", ".*",
                                                      err),
                      std::logic_error,
                      "Cannot set '" << get_admin_login() << "' permissions for '"
                      << vh.get_name() << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        }

      
        // Fill the vhost with exchanges:
        static const std::set<std::string> exchanges = {"vireserver.service",
                                                        "vireserver.event",
                                                        "subcontractor.service",
                                                        "subcontractor.event"};
        for (auto exchange : exchanges) {
          DT_LOG_DEBUG(get_logging_priority(), "Declare exchange '" << exchange << "'...");
          if (!_has_exchange_(vh.get_name(), exchange)) {
            // Create exchange:
            {
              ::rabbitmq::error_response err;
              DT_THROW_IF(!grab_manager().exchange_declare(exchange,
                                                           vh.get_name(),
                                                           "topic",
                                                           true,
                                                           false,
                                                           false,
                                                           err),
                          std::logic_error,
                          "Cannot create the '" << exchange << "' exchange in '"
                          << vh.get_name() << "' virtual host: "
                          << err.error << ": " << err.reason + "!");
            }
          }
        }

        // Permissions:
        {
          // Server side:
          const user & sc_sys_user = get_system_user(vire::com::ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM);
          DT_LOG_DEBUG(get_logging_priority(),
                       "Setting permissions for server subcontractor system user '" << sc_sys_user.get_login()
                       << "' in the '"
                       << vh.get_name() << "' vhost...");
          ::rabbitmq::permissions perms;
          perms.user  = sc_sys_user.get_login();
          perms.vhost = vh.get_name();
          permissions::add_exchange_service_server_perms(perms, "vireserver.service");
          permissions::add_exchange_service_client_perms(perms, "subcontractor.service");
          permissions::add_exchange_event_producer_perms(perms, "vireserver.event");
          permissions::add_exchange_event_listener_perms(perms, "subcontractor.event");
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                      std::logic_error,
                      "Cannot set server subcontractor system user '" << sc_sys_user.get_login() << "' permissions for '"
                      << vh.get_name() << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        }
          
        {
          // Subcontractor side:
          DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for subcontractor user '" << sc_user_login
                       << "' in the '"
                       << vh.get_name() << "' vhost...");
          const user & sc_user = get_user(sc_user_login);
          ::rabbitmq::permissions perms;
          perms.user  = sc_user.get_login();
          perms.vhost = vh.get_name();
          permissions::add_exchange_service_client_perms(perms, "vireserver.service");
          permissions::add_exchange_service_server_perms(perms, "subcontractor.service");
          permissions::add_exchange_event_listener_perms(perms, "vireserver.event");
          permissions::add_exchange_event_producer_perms(perms, "subcontractor.event");
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                      std::logic_error,
                      "Cannot set subcontractor user '" << sc_user_login << "' permissions for '"
                      << vh.get_name() << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        }
      }

      // Monitoring domain/vhost:
      {
        const vhost & vh = get_vhost(_vhost_monitoring_name_);
          
        {
          // Subcontractor side:
          DT_LOG_DEBUG(get_logging_priority(),
                       "Setting permissions for subcontractor user '" << sc_user_login
                       << "' in the '"
                       << vh.get_name() << "' vhost...");
          const user & sc_user = get_user(sc_user_login);
          ::rabbitmq::permissions perms;
          perms.user  = sc_user.get_login();
          perms.vhost = vh.get_name();
          permissions::add_exchange_service_server_perms(perms, "resource_request.service");
          permissions::add_exchange_event_producer_perms(perms, "alarm.event");
          permissions::add_exchange_event_producer_perms(perms, "log.event");
          permissions::add_exchange_event_producer_perms(perms, "pubsub.event");
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                      std::logic_error,
                      "Cannot set subcontractor user '" << sc_user_login << "' permissions for '"
                      << vh.get_name() << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        }
        
      }
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_monitoring_(const std::string & vhost_name_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      const vhost & vh = get_vhost(vhost_name_);
      DT_LOG_DEBUG(get_logging_priority(), "Populating monitoring domain '" << vhost_name_ << "'...");

      // Set administrator permissions:
      {
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().set_permissions(get_admin_login(),
                                                    vh.get_name(),
                                                    ".*", ".*", ".*",
                                                    err),
                    std::logic_error,
                    "Cannot set '" << get_admin_login() << "' permissions for '"
                    << vh.get_name() << "' virtual host: "
                    << err.error << ": " << err.reason << "!");
      }
      
      // Fill the vhost with exchanges:
      static const std::set<std::string> exchanges = {"resource_request.service",
                                                      "log.event",
                                                      "alarm.event",
                                                      "pubsub.event"};
      for (auto exchange : exchanges) {
        DT_LOG_DEBUG(get_logging_priority(), "Building monitor exchange '" << exchange << "'...");
        if (_has_exchange_(vh.get_name(), exchange)) continue;
        // Create exchange:
        {
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().exchange_declare(exchange,
                                                       vh.get_name(),
                                                       "topic",
                                                       true,
                                                       false,
                                                       false,
                                                       err),
                      std::logic_error,
                      "Cannot create the '" << exchange << "' exchange in '"
                      << vh.get_name() << "' vhost: "
                      << err.error << ": " << err.reason + "!");
        }
      }
      
      // Permissions:
      {
        const user & u = get_system_user(vire::com::ACTOR_CATEGORY_SERVER_CMS);
        const std::string & login = u.get_login();
        DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for user '" << login
                     << "' in the '"
                     << vh.get_name() << "' vhost...");
        ::rabbitmq::permissions perms;
        perms.user  = login;
        perms.vhost = vh.get_name();
        permissions::add_exchange_service_server_perms(perms, "resource_request.service");
        permissions::add_exchange_service_client_perms(perms, "resource_request.service");
        permissions::add_exchange_event_producer_perms(perms, "log.event");
        permissions::add_exchange_event_listener_perms(perms, "log.event");
        permissions::add_exchange_event_producer_perms(perms, "alarm.event");
        permissions::add_exchange_event_listener_perms(perms, "alarm.event");
        permissions::add_exchange_event_producer_perms(perms, "pubsub.event");
        permissions::add_exchange_event_listener_perms(perms, "pubsub.event");
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                    std::logic_error,
                    "Cannot set user '" << login << "' permissions for '"
                    << vh.get_name() << "' vhost: "
                    << err.error << ": " << err.reason << "!");
      }
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_control_(const std::string & vhost_name_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      const vhost & vh = get_vhost(vhost_name_);
      
      // Set administrator permissions:
      {
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().set_permissions(get_admin_login(),
                                                    vh.get_name(),
                                                    ".*", ".*", ".*",
                                                    err),
                    std::logic_error,
                    "Cannot set '" << get_admin_login() << "' permissions for '"
                    << vh.get_name() << "' virtual host: "
                    << err.error << ": " << err.reason << "!");
      }
      
      // Fill the vhost with exchange:
      std::string service_exchange = "resource_request.service";
      if (!_has_exchange_(vh.get_name(), service_exchange)) {
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().exchange_declare(service_exchange,
                                                     vh.get_name(),
                                                     "topic",
                                                     true,
                                                     false,
                                                     false,
                                                     err),
                    std::logic_error,
                    "Cannot create the '" << service_exchange << "' exchange in '"
                    << vh.get_name() << "' virtual host: "
                    + err.error + ": " + err.reason + "!");
      } // Exchange
        
      // Permissions:
      {
        const user & u = get_system_user(vire::com::ACTOR_CATEGORY_SERVER_CMS);
        const std::string & login = u.get_login();
        const std::string & exchange = service_exchange;
        DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for user '" << login
                     << "' in the '"
                     << vh.get_name() << "' vhost...");
        ::rabbitmq::permissions perms;
        perms.user  = login;
        perms.vhost = vh.get_name();
        permissions::add_exchange_service_server_perms(perms, "resource_request.service");
        permissions::add_exchange_service_client_perms(perms, "resource_request.service");
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                    std::logic_error,
                    "Cannot set user '" << login << "' permissions ofor '"
                    << vh.get_name() << "' vhost: "
                    << err.error << ": " << err.reason << "!");
      }
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
    
    void manager_service::_setup_vire_cms_domains_clients_gate_(const std::string & vhost_name_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      const vhost & vh = get_vhost(vhost_name_);
      DT_LOG_DEBUG(get_logging_priority(),
                   "Populating clients gate domain '" << vhost_name_ << "'...");

      
      // Set administrator permissions:
      {
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().set_permissions(get_admin_login(),
                                                    vh.get_name(),
                                                    ".*", ".*", ".*",
                                                    err),
                    std::logic_error,
                    "Cannot set '" << get_admin_login() << "' permissions for '"
                    << vh.get_name() << "' virtual host: "
                    << err.error << ": " << err.reason << "!");
      }
      
      // Fill the vhost with exchanges:
      {
        std::string service_exchange = "gate.service";
        if (!_has_exchange_(vh.get_name(), service_exchange)) {
          // Create exchange:
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().exchange_declare(service_exchange,
                                                       vh.get_name(),
                                                       "topic",
                                                       true,
                                                       false,
                                                       false,
                                                       err),
                      std::logic_error,
                      "Cannot create the '" << service_exchange << "' exchange"
                      << " in the '" << vh.get_name() << "' virtual host: "
                      << err.error << ": " + err.reason << "!");
        }
      }

      // Permissions:
      {
        // Server side:
        const user & u = get_system_user(vire::com::ACTOR_CATEGORY_SERVER_GATE);
        const std::string & login = u.get_login();
        DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for user '" << login
                     << "' in the '"
                     << vh.get_name() << "' vhost...");
        ::rabbitmq::permissions perms;
        perms.user  = login;
        perms.vhost = vh.get_name();
        permissions::add_exchange_service_server_perms(perms, "gate.service");
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                    std::logic_error,
                    "Cannot set user '" << login << "' permissions for '"
                    << vh.get_name() << "' vhost: "
                    << err.error << ": " << err.reason << "!");
      }
       
      {
        // Client side:
        const user & u = get_system_user(vire::com::ACTOR_CATEGORY_CLIENT_GATE);
        const std::string & login = u.get_login();
        DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for user '" << login
                     << "' in the '"
                     << vh.get_name() << "' vhost...");
        ::rabbitmq::permissions perms;
        perms.user  = login;
        perms.vhost = vh.get_name();
        permissions::add_exchange_service_client_perms(perms, "gate.service");
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                    std::logic_error,
                    "Cannot set user '" << login << "' permissions for '"
                    << vh.get_name() << "' vhost: "
                    << err.error << ": " << err.reason << "!");
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_client_system_(const std::string & cl_id_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      // Find the client user associated to this client identifier:
      const client_info & cl_info = get_client_info(cl_id_);
      std::string cl_sys_user_login = cl_info.sys_user_login;
      std::string cl_cms_user_login = cl_info.cms_user_login;

      // System client domain/vhost:
      {
        const vhost & vh = get_vhost(cl_info.system_vhost_name);
      
        DT_LOG_DEBUG(get_logging_priority(),
                     "Populating system client domain '" << vh.get_name() << "'...");
      
        // Set administrator permissions:
        {
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().set_permissions(get_admin_login(),
                                                      vh.get_name(),
                                                      ".*", ".*", ".*",
                                                      err),
                      std::logic_error,
                      "Cannot set '" << get_admin_login() << "' permissions for '"
                      << vh.get_name() << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        }
      
        // Fill the vhost with exchanges:
        static const std::set<std::string> exchanges = {"vireserver.service",
                                                        "vireserver.event"};

        for (auto exchange : exchanges) {
          DT_LOG_DEBUG(get_logging_priority(), "Declare exchange '" << exchange << "'...");
          if (!_has_exchange_(vh.get_name(), exchange)) {
            // Create exchange:
            {
              ::rabbitmq::error_response err;
              DT_THROW_IF(!grab_manager().exchange_declare(exchange,
                                                           vh.get_name(),
                                                           "topic",
                                                           true,
                                                           false,
                                                           false,
                                                           err),
                          std::logic_error,
                          "Cannot create the '" << exchange << "' exchange in '"
                          << vh.get_name() << "' virtual host: "
                          << err.error << ": " << err.reason + "!");
            }
          }
        }

        // Permissions:
        {
          // Server side:
          const user & cl_sys_user = get_system_user(vire::com::ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM);
          DT_LOG_DEBUG(get_logging_priority(),
                       "Setting permissions for server client system user '" << cl_sys_user.get_login()
                       << "' in the '"
                       << vh.get_name() << "' vhost...");
          ::rabbitmq::permissions perms;
          perms.user  = cl_sys_user.get_login();
          perms.vhost = vh.get_name();
          permissions::add_exchange_service_server_perms(perms, "vireserver.service");
          permissions::add_exchange_event_producer_perms(perms, "vireserver.event");
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                      std::logic_error,
                      "Cannot set server client system user '" << cl_sys_user.get_login() << "' permissions for '"
                      << vh.get_name() << "' virtual host: "
                      << err.error << ": " << err.reason << "!");

        }
           
        {
          // Client side:
          DT_LOG_DEBUG(get_logging_priority(),
                       "Setting permissions for client system user '" << cl_sys_user_login
                       << "' in the '"
                       << vh.get_name() << "' vhost...");
          const user & cl_sys_user = get_user(cl_sys_user_login);
          ::rabbitmq::permissions perms;
          perms.user  = cl_sys_user.get_login();
          perms.vhost = vh.get_name();
          permissions::add_exchange_service_client_perms(perms, "vireserver.service");
          permissions::add_exchange_event_listener_perms(perms, "vireserver.event");
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                      std::logic_error,
                      "Cannot set client system user '"
                      << cl_sys_user.get_login() << "' permissions for '"
                      << vh.get_name() << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        }

        
      }

      // Monitoring domain/vhost:
      {
        const vhost & vh = get_vhost(_vhost_monitoring_name_);
          
        {
          // Client CMS side:
          DT_LOG_DEBUG(get_logging_priority(),
                       "Setting permissions for client CMS user '" << cl_cms_user_login
                       << "' in the '"
                       << vh.get_name() << "' vhost...");
          const user & u = get_user(cl_cms_user_login);
          ::rabbitmq::permissions perms;
          perms.user  = u.get_login();
          perms.vhost = vh.get_name();
          permissions::add_exchange_service_client_perms(perms, "resource_request.service");
          permissions::add_exchange_event_listener_perms(perms, "log.event");
          permissions::add_exchange_event_listener_perms(perms, "alarm.event");
          permissions::add_exchange_event_listener_perms(perms, "pubsub.event");
           ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                      std::logic_error,
                      "Cannot set client CMS user '" << u.get_login() << "' permissions for '"
                      << vh.get_name() << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        }
      }
    
 
      // Control domain/vhost:
      if (cl_info.with_control) {
        const vhost & vh = get_vhost(_vhost_control_name_);
          
        {
          // Client CMS side:
          DT_LOG_DEBUG(get_logging_priority(),
                       "Setting permissions for client CMS user '" << cl_cms_user_login
                       << "' in the '"
                       << vh.get_name() << "' vhost...");
          const user & u = get_user(cl_cms_user_login);
          ::rabbitmq::permissions perms;
          perms.user  = u.get_login();
          perms.vhost = vh.get_name();
          permissions::add_exchange_service_client_perms(perms, "resource_request.service");
           ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                      std::logic_error,
                      "Cannot set client CMS user '" << u.get_login() << "' permissions for '"
                      << vh.get_name() << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        }
      }
        
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_destroy_vire_cms_domains_client_system_(const std::string & login_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      /*
      const user & client_user = get_client_user(login_);
      ::rabbitmq::error_response err;
      {
        std::string vhost_name = _vhost_name_prefix_ + vire_cms_path() + "/clients/system/"
          + client_user.get_login();
        if (has_vhost(vhost_name)) {
          delete_vhost(vhost_name);
          // ::rabbitmq::error_response err;
          // DT_THROW_IF(!grab_manager().delete_vhost(vhost, err),
          //             std::logic_error,
          //             "Cannot delete the '" << vhost << "' virtual host: "
          //             << err.error << ": " << err.reason << "!");
        } else {
          DT_LOG_DEBUG(get_logging_priority(), "Virtual host '" << vhost_name << "' does not exist.");
        }
      }
      */
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

  } // namespace rabbitmq

} // namespace vire
