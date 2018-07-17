//! \file vire/rabbitmq/manager_service.cc
//
// Copyright (c) 2017-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

    const std::list<user> & manager_service::get_system_users() const
    {
      return _system_users_;
    }
   
    void manager_service::add_system_user(const user & sysuser_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ manager service is already initialized!");     
      DT_THROW_IF(!sysuser_.is_complete(), std::logic_error, "Cannot add an incomplete system user!");
      for (const user & su : _system_users_) {
        if (vire::com::is_unique_user(sysuser_.get_category()) &&  su.get_category() == sysuser_.get_category()) {
          DT_THROW(std::logic_error, "RabbitMQ manager service already has an user of category '"
                   << vire::com::to_string(sysuser_.get_category()) << "'!");
        }
        if (sysuser_.get_category() == vire::com::ACTOR_CATEGORY_SERVER_CMS) {
          _server_cms_name_ = sysuser_.get_login();
        }
        if (sysuser_.get_category() == vire::com::ACTOR_CATEGORY_SERVER_GATE) {
          _server_gate_name_ = sysuser_.get_login();
        }
        if (sysuser_.get_category() == vire::com::ACTOR_CATEGORY_CLIENT_GATE) {
          _client_gate_name_ = sysuser_.get_login();
        }
      }
      _system_users_.push_back(sysuser_);
      return;
    }
    
    void manager_service::add_system_user(const std::string & login_,
                                          const std::string & password_,
                                          const vire::com::actor_category_type & category_)
    {
      user sysuser(login_, password_, category_);
      add_system_user(sysuser);
      return;
    }

    manager_service::manager_service(uint32_t flags_)
    {
      _initialized_ = false;
      _set_defaults_();
      return;
    }

    manager_service::~manager_service()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void manager_service::tree_dump(std::ostream & out_,
                                    const std::string & title_,
                                    const std::string & indent_,
                                    bool inherit_) const
    {
      this->datatools::base_service::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Virtual host name prefix : "
           << "'" << _vhost_name_prefix_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Server host : "
           << "'" << _server_host_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Server port : "
           << _server_port_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Administrator login : "
           << "'" << _admin_login_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Administrator password : "
           << "'" << _admin_password_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "System users : "
           << _system_users_.size() << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Vhosts : "
           << _vhosts_.size() << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Users : "
           << _users_.size() << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Destroy all at reset : "
           << std::boolalpha << _destroy_all_at_reset_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Pimpl : "
           << std::boolalpha << (_pimpl_.get() != nullptr) << std::endl;

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

      DT_LOG_TRACE(get_logging_priority(), "Creating PIMPL...");
      _pimpl_.reset(new pimpl_type);

      if (!has_vhost_name_prefix()) {
        if (config_.has_key("vhost_name_prefix")) {
          std::string vnp = config_.fetch_string("vhost_name_prefix");
          set_vhost_name_prefix(vnp);
        }
      }

      if (!has_server_host()) {
        if (config_.has_key("server_host")) {
          std::string server_host = config_.fetch_string("server_host");
          set_server_host(server_host);
        }
      }

      if (!has_server_port()) {
        if (config_.has_key("server_port")) {
          int server_port = config_.fetch_positive_integer("server_port");
          set_server_port(server_port);
        }
      }

      if (!has_admin_login()) {
        if (config_.has_key("admin_login")) {
          std::string admin_login = config_.fetch_string("admin_login");
          set_admin_login(admin_login);
        }
      }

      if (!has_admin_password()) {
        if (config_.has_key("admin_password")) {
          std::string admin_password = config_.fetch_string("admin_password");
          set_admin_password(admin_password);
        }
      }

      {
        std::set<std::string> susers;
        if (config_.has_key("system_users")) {
          config_.fetch("system_users", susers);
        }
        for (const auto & login : susers) {
          user sys_user;
          sys_user.set_login(login);
          datatools::properties user_config;
          config_.export_and_rename_starting_with(user_config, "users." + login + ".", "");
          sys_user.initialize(user_config);
          add_system_user(sys_user);
        }
      }

      // Checks:

      if (_server_port_ <= 0) {
        set_server_port(::rabbitmq::rabbit_mgr::SSL_PORT);
      }

      if (_system_users_.size() == 0) {
        // Throw ???
      }
      
      // if (_server_cms_name_.empty()) {
      //   user server_cms_user("vireservercms", "vireservercms", vier::com::ACTOR_CATEGORY_SERVER_CMS);
      //   add_system_user(server_cms_user);
      // }
      // if (_server_gate_name_.empty()) {
      //   user server_gate_user("vireservergate", "vireservergate", vier::com::ACTOR_CATEGORY_SERVER_GATE);
      //   add_system_user(server_gate_user);
      // }
      // if (_client_gate_name_.empty()) {
      //   user client_gate_user("vireclientgate", "vireclientgate", vier::com::ACTOR_CATEGORY_CLIENT_GATE);
      //   add_system_user(client_gate_user);
      // }

      DT_LOG_DEBUG(get_logging_priority(), "vhost_name_prefix     : " << _vhost_name_prefix_);
      DT_LOG_DEBUG(get_logging_priority(), "server_host           : " << _server_host_);
      DT_LOG_DEBUG(get_logging_priority(), "server_port           : " << _server_port_);
      DT_LOG_DEBUG(get_logging_priority(), "admin_login           : " << _admin_login_);
      DT_LOG_DEBUG(get_logging_priority(), "admin_password        : " << _admin_password_);

      // Initialize:
      DT_LOG_TRACE(get_logging_priority(), "Instantiating the RabbitMQ management driver...");
      _pimpl_->mgr.reset(new ::rabbitmq::rabbit_mgr(_server_host_,
                                                    _server_port_,
                                                    _admin_login_,
                                                    _admin_password_));

      DT_LOG_TRACE(get_logging_priority(), "Setup Vire CMS...");
      _setup_vire_cms_();

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
        fetch_users(logins, vire::com::ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM);
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

      if (is_destroy_all_at_reset()) {
        _force_destroy_vire_cms_();
      }
      _users_.clear();
      _vhosts_.clear();
      _pimpl_->mgr.reset();
      _vhost_gate_name_.clear();
      _vhost_control_name_.clear();
      _vhost_monitoring_name_.clear();
      _server_cms_name_.clear();
      _server_gate_name_.clear();
      _client_gate_name_.clear();
      _system_users_.clear();
      _admin_login_.clear();
      _admin_password_.clear();
      _vhost_name_prefix_.clear();
      _server_port_ = -1;
      _server_host_.clear();

      _pimpl_.reset();
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

    void manager_service::fetch_vhosts(std::set<std::string> & names_, const vire::com::domain_category_type category_) const
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
 
    void manager_service::fetch_users(std::set<std::string> & logins_, const vire::com::actor_category_type category_) const
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
    
    bool manager_service::has_vhost(const std::string & name_, const vire::com::domain_category_type category_) const
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
      for (auto cat : vire::com::domain_categories_with_unique_domain()) {
        if (vh_.get_category() == cat) {
          std::set<std::string> vhs;
          fetch_vhosts(vhs, cat);
          DT_THROW_IF(vhs.size(), std::logic_error,
                      "Vhost of category '"
                      << vire::com::to_string(vh_.get_category())
                      << "' already exist!");
        }
      }
      DT_THROW_IF(has_vhost(vh_.get_name()), std::logic_error, "Vhost '" << vh_.get_name() << "' already exists!");
      DT_LOG_DEBUG(get_logging_priority(), "Adding virtual host '" << vh_.get_name() << "'...");
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
      if (!_has_vhost_(vh_.get_name())) {
        DT_LOG_DEBUG(get_logging_priority(), "Creating RabbitMQ virtual host '" << vh_.get_name() << "'...");
         ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().add_vhost(vh_.get_name(), err),
                    std::logic_error,
                    "Cannot create RabbitMQ vhost '" << vh_.get_name() << "': "
                    + err.error + ": " + err.reason + "!");
        if (vh_.get_category() == vire::com::DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
          _setup_vire_cms_domains_subcontractor_system_(vh_.get_name());
        }
        if (vh_.get_category() == vire::com::DOMAIN_CATEGORY_CONTROL) {
          _setup_vire_cms_domains_control_(vh_.get_name());
        }
        if (vh_.get_category() == vire::com::DOMAIN_CATEGORY_MONITORING) {
          _setup_vire_cms_domains_monitoring_(vh_.get_name());
        }
        if (vh_.get_category() == vire::com::DOMAIN_CATEGORY_GATE) {
          _setup_vire_cms_domains_clients_gate_(vh_.get_name());
        }
        if (vh_.get_category() == vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM) {
          _setup_vire_cms_domains_client_system_(vh_.get_name());
        }
      }
      return;
    }

    void manager_service::remove_vhost(const std::string & name_)
    {
      DT_THROW_IF(!has_vhost(name_), std::logic_error, "No vhost named '" << name_ << "' exists!");
      const vhost & vh = get_vhost(name_);
      if (vh.get_category() == vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        // Only delete temporary vhost related to Vire client:
        if (_has_vhost_(vh.get_name())) {
          DT_LOG_DEBUG(get_logging_priority(), "Deleting RabbitMQ vhost '" << vh.get_name() << "'...");
         ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().delete_vhost(vh.get_name(), err),
                      std::logic_error,
                      "Cannot delete RabbitMQ vhost '" << vh.get_name() << "': "
                      + err.error + ": " + err.reason + "!");
        }
      }
      DT_LOG_DEBUG(get_logging_priority(), "Removing virtual host '" << vh.get_name() << "'...");
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

    bool manager_service::has_user(const std::string & name_, const vire::com::actor_category_type category_) const
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
                      << "' already exist!");
        }
      }
      DT_THROW_IF(has_user(user_.get_login()), std::logic_error, "User '" << user_.get_login() << "' already exists!");
      DT_LOG_DEBUG(get_logging_priority(), "Adding user '" << user_.get_login() << "'...");
      _users_[user_.get_login()] = user_;
      if (!_has_user_(user_.get_login())) {
        DT_LOG_DEBUG(get_logging_priority(), "Adding RabbitMQ user '" << user_.get_login() << "'...");
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().add_user(user_.get_login(), user_.get_password(), err),
                    std::logic_error,
                    "Cannot create RabbitMQ user '" << user_.get_login() << "': "
                    + err.error + ": " + err.reason + "!");

        // Set permissions
      }
      return;
    }

    void manager_service::remove_user(const std::string & login_)
    {
      DT_THROW_IF(!has_user(login_), std::logic_error, "No user '" << login_ << "' exists!");
      const user & u = get_user(login_);
      if (u.get_category() == vire::com::ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM
          || u.get_category() == vire::com::ACTOR_CATEGORY_CLIENT_SYSTEM
          || u.get_category() == vire::com::ACTOR_CATEGORY_CLIENT_CMS) {
        // Only delete temporary users related to Vire client:
        if (_has_user_(u.get_login())) {
          DT_LOG_DEBUG(get_logging_priority(), "Deleting RabbitMQ user '" << u.get_login() << "'...");
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().delete_user(u.get_login(), err),
                      std::logic_error,
                      "Cannot delete RabbitMQ user '" << u.get_login() << "': "
                      + err.error + ": " + err.reason + "!");
        }
      }
      DT_LOG_DEBUG(get_logging_priority(), "Removing user '" << u.get_login() << "'...");
      _users_.erase(login_);
      return;
    }

    void manager_service::_force_destroy_vire_cms_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());

      {
        // Destroy the static domains/vhosts:
        ::rabbitmq::vhost::list vhosts;
        ::rabbitmq::error_response err;
        manager_service * mutable_this = const_cast<manager_service*>(this);
        mutable_this->grab_manager().list_vhosts(vhosts, err);
        for (const auto & vhost : vhosts) {
          if (boost::algorithm::starts_with(vhost.name, _vhost_name_prefix_ )) {
            DT_LOG_DEBUG(get_logging_priority(), "Removing virtual host '" << vhost.name << "'...");
            if (has_vhost(vhost.name)) {
              DT_THROW_IF(!grab_manager().delete_vhost(vhost.name, err),
                          std::logic_error,
                          "Could remove virtual host '" << vhost.name << "': "
                          + err.error + ": " + err.reason + "!");
            }
          }
        }
      }

      // // Destroy the users:
      // for (const auto & psu : _users_) {
      //   const user & su = psu.second;
      //   if (_has_user_(su.get_login())) {
      //     DT_LOG_DEBUG(get_logging_priority(), "Removing user '" << su.get_login() << "'...");
      //     ::rabbitmq::error_response err;
      //     DT_THROW_IF(!grab_manager().delete_user(su.get_login(), err),
      //                 std::logic_error,
      //                 "Cannot delete RabbitMQ '" << su.get_login() << "': "
      //                 + err.error + ": " + err.reason + "!");
      //   }
      // }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      _setup_vire_cms_users_();
      _setup_vire_cms_domains_();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_users_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      for (const user & su : _system_users_) {
        if (!has_user(su.get_login())) {
          DT_LOG_DEBUG(get_logging_priority(), "Adding system user '" << su.get_login() << "'...");
          add_user(su);
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_LOG_DEBUG(get_logging_priority(), "Building Vire CMS domains/vhosts...");
      std::string vhost_gate_name = vire::com::domain_builder::build_cms_clients_gate_name(_vhost_name_prefix_);
      if (!has_vhost(vhost_gate_name)) {
        vhost vh(vhost_gate_name, vire::com::DOMAIN_CATEGORY_GATE);
        add_vhost(vh);
      }
      std::string vhost_control_name = vire::com::domain_builder::build_cms_control_name(_vhost_name_prefix_);
      if (!has_vhost(vhost_control_name)) {
        vhost vh(vhost_control_name, vire::com::DOMAIN_CATEGORY_CONTROL);
        add_vhost(vh);
      }
      std::string vhost_monitoring_name = vire::com::domain_builder::build_cms_monitoring_name(_vhost_name_prefix_);
      if (!has_vhost(vhost_monitoring_name)) {
        vhost vh(vhost_monitoring_name, vire::com::DOMAIN_CATEGORY_MONITORING);
        add_vhost(vh);
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_subcontractor_system_(const std::string & vhost_name_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      const vhost & vh = get_vhost(vhost_name_);
      DT_LOG_DEBUG(get_logging_priority(), "Populating system subcontractor domain '" << vhost_name_ << "'...");
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
          // Permissions:
          {
            std::set<std::string> logins;
            fetch_users(logins, vire::com::ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM);
            for (const auto & login : logins) {
              DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for user '" << login
                           << "' and exchange '" << exchange << "' in the '"
                           << vh.get_name() << "' virtual host...");
              ::rabbitmq::permissions perms;
              perms.user  = login;
              perms.vhost = vh.get_name();
              const user & u = get_user(login);
              if (exchange == "vireserver.service") {
                permissions::add_exchange_service_server_perms(perms, exchange);
              }
              if (exchange == "subcontractor.service") {
                permissions::add_exchange_service_client_perms(perms, exchange);
              }
              if (exchange == "vireserver.event") {
                permissions::add_exchange_event_producer_perms(perms, exchange);
              }
              if (exchange == "subcontractor.event") {
                permissions::add_exchange_event_listener_perms(perms, exchange);
              }
              ::rabbitmq::error_response err;
              DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                          std::logic_error,
                          "Cannot set user '" << login << "' permissions on exchange '" << exchange
                          << "' for '"
                          << vh.get_name() << "' virtual host: "
                          << err.error << ": " << err.reason << "!");
            }
          }
          
          {
            std::set<std::string> logins;
            fetch_users(logins, vire::com::ACTOR_CATEGORY_SUBCONTRACTOR);
            for (const auto & login : logins) {
              DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for user '" << login
                           << "' and exchange '" << exchange << "' in the '"
                           << vh.get_name() << "' virtual host...");
              ::rabbitmq::permissions perms;
              perms.user  = login;
              perms.vhost = vh.get_name();
              const user & u = get_user(login);
              if (exchange == "vireserver.service") {
                permissions::add_exchange_service_client_perms(perms, exchange);
              }
              if (exchange == "subcontractor.service") {
                permissions::add_exchange_service_server_perms(perms, exchange);
              }
              if (exchange == "vireserver.event") {
                permissions::add_exchange_event_listener_perms(perms, exchange);
              }
              if (exchange == "subcontractor.event") {
                permissions::add_exchange_event_producer_perms(perms, exchange);
              }
              ::rabbitmq::error_response err;
              DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                          std::logic_error,
                          "Cannot set user '" << login << "' permissions on exchange '" << exchange
                          << "' for '"
                          << vh.get_name() << "' virtual host: "
                          << err.error << ": " << err.reason << "!");
            }
          }
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
                      << vh.get_name() << "' virtual host: "
                      << err.error << ": " << err.reason + "!");
        }
        // Permissions:
        {
          std::set<std::string> logins;
          fetch_users(logins, vire::com::ACTOR_CATEGORY_SERVER_CMS);
          for (const auto & login : logins) {
            DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for user '" << login
                         << "' and exchange '" << exchange << "' in the '"
                         << vh.get_name() << "' virtual host...");
            ::rabbitmq::permissions perms;
            perms.user  = login;
            perms.vhost = vh.get_name();
            const user & u = get_user(login);
            if (boost::algorithm::ends_with(exchange, vire_cms_exchange_service_suffix())) {
              permissions::add_exchange_service_server_perms(perms, exchange);
              permissions::add_exchange_service_client_perms(perms, exchange);
            } else if (boost::algorithm::ends_with(exchange, vire_cms_exchange_event_suffix())) {
              permissions::add_exchange_event_producer_perms(perms, exchange);
              permissions::add_exchange_event_listener_perms(perms, exchange);
            }
            ::rabbitmq::error_response err;
            DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                        std::logic_error,
                        "Cannot set user '" << login << "' permissions on exchange '" << exchange
                        << "' for '"
                        << vh.get_name() << "' virtual host: "
                        << err.error << ": " << err.reason << "!");
          }
        }
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
        // Exchange:
        {
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
          std::set<std::string> logins;
          fetch_users(logins, vire::com::ACTOR_CATEGORY_SERVER_CMS);
          for (const auto & login : logins) {{
              const std::string & exchange = service_exchange;
              DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for user '" << login
                           << "' and exchange '" << exchange << "' in the '"
                           << vh.get_name() << "' virtual host...");
              ::rabbitmq::permissions perms;
              perms.user  = login;
              perms.vhost = vh.get_name();
              permissions::add_exchange_service_server_perms(perms, exchange);
              ::rabbitmq::error_response err;
              DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                          std::logic_error,
                          "Cannot set user '" << login << "' permissions on exchange '" << exchange
                          << "' for '"
                          << vh.get_name() << "' virtual host: "
                          << err.error << ": " << err.reason << "!");
            }
          }
        } // Permissions:
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
    
    void manager_service::_setup_vire_cms_domains_clients_gate_(const std::string & vhost_name_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      const vhost & vh = get_vhost(vhost_name_);
      DT_LOG_DEBUG(get_logging_priority(), "Populating clients gate domain '" << vhost_name_ << "'...");

      
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
          {
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
          // Permissions:
          {
            // Server side:
            std::set<std::string> logins;
            fetch_users(logins, vire::com::ACTOR_CATEGORY_SERVER_GATE);
            for (const auto & login : logins) {
              const std::string & exchange = service_exchange;
              DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for user '" << login
                           << "' and exchange '" << exchange << "' in the '"
                           << vh.get_name() << "' virtual host...");
              ::rabbitmq::permissions perms;
              perms.user  = login;
              perms.vhost = vh.get_name();
              permissions::add_exchange_service_server_perms(perms, exchange);
              ::rabbitmq::error_response err;
              DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                          std::logic_error,
                          "Cannot set user '" << login << "' permissions on exchange '" << exchange
                          << "' for '"
                          << vh.get_name() << "' virtual host: "
                          << err.error << ": " << err.reason << "!");
            }
          }
          {
            // Client side:
            std::set<std::string> logins;
            fetch_users(logins, vire::com::ACTOR_CATEGORY_CLIENT_GATE);
            for (const auto & login : logins) {
              const std::string & exchange = service_exchange;
              DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for user '" << login
                           << "' and exchange '" << exchange << "' in the '"
                           << vh.get_name() << "' virtual host...");
              ::rabbitmq::permissions perms;
              perms.user  = login;
              perms.vhost = vh.get_name();
              permissions::add_exchange_service_client_perms(perms, exchange);
              ::rabbitmq::error_response err;
              DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                          std::logic_error,
                          "Cannot set user '" << login << "' permissions on exchange '" << exchange
                          << "' for '"
                          << vh.get_name() << "' virtual host: "
                          << err.error << ": " << err.reason << "!");
            }
          }
        }
      } 

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_client_system_(const std::string & vhost_name_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      /*
      const vhost & vh = get_vhost(vhost_name_);

      DT_LOG_DEBUG(get_logging_priority(), "Adding system client domain '" << name_ << "'...");
      std::string client_id;
      DT_THROW_IF!(domain_builder::extract_client_id(get_vhost_name_prefix(), vhost_name_, client_id),
                   std::logic_error,
                   "Invalid client system vhost name '" << vhost_name_ << "'!");
      const user & client_user = get_user(client_id);
      vhost vh(vhost_name_, vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM);
      {
        ::rabbitmq::permissions perms;
        perms.user  = client_user.get_login();
        perms.vhost = get_vhost_name_prefix() + vire_cms_path() + "/control";
        permissions::add_exchange_service_client_perms(perms, "resource_request.service");
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                    std::logic_error,
                    "Cannot set user '" << client_user.get_login()
                    << "' permissions for '"
                    << perms.vhost << "' virtual host: "
                    << err.error << ": " << err.reason << "!");
      }

      {
        ::rabbitmq::permissions perms;
        perms.user  = client_user.get_login();
        perms.vhost = get_vhost_name_prefix() + vire_cms_path() + "/monitoring";
        permissions::add_exchange_service_client_perms(perms, "resource_request.service");
        permissions::add_exchange_event_listener_perms(perms, "alarm.event");
        permissions::add_exchange_event_listener_perms(perms, "log.event");
        permissions::add_exchange_event_listener_perms(perms, "pubsub.event");
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                    std::logic_error,
                    "Cannot set user '" << client_user.get_login()
                    << "' permissions for '"
                    << perms.vhost << "' virtual host: "
                    << err.error << ": " << err.reason << "!");
      }

      // Fill the vhost with exchanges:
      vhost vh(get_vhost_name_prefix() + vire_cms_path() + "/clients/system/" + client_user.get_login(), vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM);
      static const std::set<std::string> exchanges = {"vireserver.service",
                                                      "vireserver.event"};
      for (auto exchange : exchanges) {
        DT_LOG_DEBUG(get_logging_priority(), "Building client exchange '" << exchange << "'...");
        if (has_exchange(vh.get_name(), exchange)) continue;
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
        DT_THROW_IF(!grab_manager().set_permissions(get_admin_login(),
                                                    vh.get_name(),
                                                    ".*", ".*", ".*",
                                                    err),
                    std::logic_error,
                    "Cannot set '" << get_admin_login() << "' permissions for '"
                    << vh.get_name() << "' virtual host: "
                    << err.error << ": " << err.reason << "!");
        // Permissions:
        std::set<std::string> users;
        std::string server_user_login;
        fetch_server_user(server_user_login);
        users.insert(server_user_login);
        users.insert(client_user.get_login());
        for (const auto & u : users) {
          DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for user '" << u
                       << "' and exchange '" << exchange << "' in the '"
                       << vh.get_name() << "' virtual host...");
          ::rabbitmq::permissions perms;
          perms.user  = u;
          perms.vhost = vh.get_name();
          if (u == server_user_login) {
            if (exchange == "vireserver.service") {
              permissions::add_exchange_service_server_perms(perms, exchange);
            }
            if (exchange == "vireserver.event") {
              permissions::add_exchange_event_producer_perms(perms, exchange);
            }
          } else {
            if (exchange == "vireserver.service") {
              permissions::add_exchange_service_client_perms(perms, exchange);
            }
            if (exchange == "vireserver.event") {
              permissions::add_exchange_event_listener_perms(perms, exchange);
            }
          }
          DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                      std::logic_error,
                      "Cannot set user '" << perms.user << "' permissions on exchange '" << exchange
                      << "' for '"
                      << vh.get_name() << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        }
      }
      */
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

    // void manager_service::change_real_user_password(const std::string & login_,
    //                                                 const std::string & password_)
    // {
    //   DT_LOG_TRACE_ENTERING(get_logging_priority());
    //   std::map<std::string, user>::iterator found = _real_users_.find(login_);
    //   DT_THROW_IF(found == _real_users_.end(), std::logic_error,
    //               "Real user '" << login_ << "' does not exist!");
    //   user & real_user = found->second;
    //   real_user.set_password(password_);
    //   // NOT IMPLEMENTED YET!
    //   // if (has_user(real_user.get_login())) {
    //   //   ::rabbitmq::error_response err;
    //   //   if (!grab_manager().change_user_password(real_user.get_login(),
    //   //                                                 real_user.password,
    //   //                                                 err)) {
    //   //          DT_THROW(std::logic_error,
    //   //              "Cannot change password for real user '" << login_ << "': "
    //   //                   + err.error + ": " + err.reason + "!");
    //   //   }
    //   // }
    //   DT_LOG_TRACE_EXITING(get_logging_priority());
    // }

  } // namespace rabbitmq

} // namespace vire
