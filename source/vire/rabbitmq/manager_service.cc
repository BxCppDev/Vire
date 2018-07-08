//! \file vire/rabbitmq/manager_service.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
      DT_THROW_IF(vnp_[vnp_.size() - 1] == '/', std::logic_error,
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
           << "Static domains : "
           << _static_domains_.size() << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Static users : "
           << _static_users_.size() << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Client users : "
           << _client_users_.size() << std::endl;

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

      if (_static_domains_.size() == 0) {
        DT_LOG_DEBUG(get_logging_priority(), "Adding default static domains...");
        _static_domains_.insert("monitoring");
        _static_domains_.insert("control");
        _static_domains_.insert("clients/gate");
        _static_domains_.insert("subcontractors/system/*");
      }

      std::size_t nb_of_servers = 0;
      if (has_server_user()) {
        nb_of_servers = 1;
      }
      //if (_static_users_.size() == 0)
      {
        std::set<std::string> susers;
        if (config_.has_key("static_users")) {
          config_.fetch("static_users", susers);
        }

        for (const auto & sulogin : susers) {
          user sys_user;
          sys_user.set_login(sulogin);
          datatools::properties user_config;
          config_.export_and_rename_starting_with(user_config, "static_users." + sulogin + ".", "");
          sys_user.initialize(user_config);
          if (sys_user.is_server()) {
            DT_THROW_IF(nb_of_servers != 0,
                        std::logic_error,
                        "Only one user with server category is allowed!");
            nb_of_servers++;
          }
          add_static_user(sys_user);
        }
      }

      if (!has_server_user()) {
        user server_user("vireserver", "vireserver", user::CATEGORY_SERVER);
        add_server_user(server_user);
      }

      if (_server_port_ <= 0) {
        set_server_port(::rabbitmq::rabbit_mgr::SSL_PORT);
      }

      DT_LOG_DEBUG(get_logging_priority(), "vhost_name_prefix     : " << _vhost_name_prefix_);
      DT_LOG_DEBUG(get_logging_priority(), "server_host           : " << _server_host_);
      DT_LOG_DEBUG(get_logging_priority(), "server_port           : " << _server_port_);
      DT_LOG_DEBUG(get_logging_priority(), "admin_login           : " << _admin_login_);
      DT_LOG_DEBUG(get_logging_priority(), "admin_password        : " << _admin_password_);
      for (auto d : _static_domains_) {
        DT_LOG_DEBUG(get_logging_priority(), "static_domain         : " << d);
      }
      for (auto u : _static_users_) {
        DT_LOG_DEBUG(get_logging_priority(), "static_user           : " << u.second.get_login());
      }

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
      std::set<std::string> client_logins;
      fetch_client_users(client_logins);
      for (const auto & client : client_logins) {
        remove_client_user(client);
      }

      if (is_destroy_all_at_reset()) {
        _force_destroy_vire_cms_();
      }

      _pimpl_->mgr.reset();

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

    bool manager_service::has_static_user(const std::string & login_) const
    {
      return _static_users_.count(login_) == 1;
    }

    bool manager_service::has_server_user() const
    {
      for (const auto & pu : _static_users_) {
        const user & su = pu.second;
        if (su.is_server()) return true;
      }
      return false;
    }

    const user & manager_service::get_static_user(const std::string & login_) const
    {
      std::map<std::string, user>::const_iterator found = _static_users_.find(login_);
      DT_THROW_IF(found == _static_users_.end(),
                  std::logic_error,
                  "There is no static user '" << login_ << "'");
      return found->second;
    }

    bool manager_service::fetch_server_user(std::string & login_) const
    {
      login_.clear();
      for (const auto & pu : _static_users_) {
        const user & su = pu.second;
        if (su.is_server()) {
          login_ = su.get_login();
          return true;
        }
      }
      return false;
    }

    bool manager_service::is_subcontractor_user(const std::string & login_) const
    {
      for (const auto & pu : _static_users_) {
        const user & su = pu.second;
        if (su.get_login() != login_) continue;
        if (su.is_subcontractor()) return true;
      }
      return false;
    }

    bool manager_service::is_server_user(const std::string & login_) const
    {
      for (const auto & pu : _static_users_) {
        const user & su = pu.second;
        if (su.get_login() != login_) continue;
        if (su.is_server()) return true;
      }
      return false;
    }

    void manager_service::fetch_static_users(std::set<std::string> & logins_) const
    {
      logins_.clear();
      for (const auto & pu : _static_users_) {
        const user & su = pu.second;
        logins_.insert(su.get_login());
      }
      return;
    }

    void manager_service::fetch_subcontractor_users(std::set<std::string> & logins_) const
    {
      logins_.clear();
      for (const auto & pu : _static_users_) {
        const user & su = pu.second;
        if (su.is_subcontractor()) {
          logins_.insert(su.get_login());
        }
      }
      return;
    }

    void manager_service::fetch_system_users(std::set<std::string> & logins_) const
    {
      logins_.clear();
      for (const auto & pu : _static_users_) {
        const user & su = pu.second;
        if (su.is_system()) {
          logins_.insert(su.get_login());
        }
      }
      return;
    }

    void manager_service::add_server_user(const user & user_)
    {
      DT_THROW_IF(!user_.is_server(),
                  std::logic_error,
                  "Not a server user!");
      DT_THROW_IF(has_server_user(),
                  std::logic_error,
                  "A server user already exists!");
      add_static_user(user_);
      return;
    }

    void manager_service::add_subcontractor_user(const user & user_)
    {
      DT_THROW_IF(!user_.is_subcontractor(),
                  std::logic_error,
                  "Not a subcontractor user!");
      add_static_user(user_);
      return;
    }

    void manager_service::add_system_user(const user & user_)
    {
      DT_THROW_IF(!user_.is_system(),
                  std::logic_error,
                  "Not a system user!");
      add_static_user(user_);
      return;
    }

    void manager_service::add_static_user(const user & user_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ manager service is already initialized!");
      DT_THROW_IF(!user_.is_complete(),
                  std::logic_error,
                  "User is not completed user!");
      DT_THROW_IF(user_.is_client(),
                  std::logic_error,
                  "Cannot add a static client user!");
      DT_THROW_IF(_static_users_.count(user_.get_login()) > 0,
                  std::logic_error,
                  "User '" << user_.get_login() << "' already exists!");
      DT_THROW_IF(user_.is_server() && has_server_user(),
                  std::logic_error,
                  "A server user already exists!");
      _static_users_[user_.get_login()] = user_;
      return;
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

    bool manager_service::has_vhost(const std::string & name_) const
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

    // void manager_service::add_vhost(const std::string & name_) const
    // {
    //   ::rabbitmq::error_response err;
    //   grab_manager().add_vhost(name_, err);
    //   if (err
    //   return;
    // }

    // void manager_service::delete_vhost(const std::string & name_) const
    // {
    //   return;
    // }

    bool manager_service::has_user(const std::string & name_) const
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

    bool manager_service::has_exchange(const std::string & vhost_,
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

    bool manager_service::has_queue(const std::string & vhost_,
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

      // Destroy the static users:
      for (const auto & psu : _static_users_) {
        const user & su = psu.second;
        if (has_user(su.get_login())) {
          DT_LOG_DEBUG(get_logging_priority(), "Removing user '" << su.get_login() << "'...");
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().delete_user(su.get_login(), err),
                      std::logic_error,
                      "Cannot create user '" << su.get_login() << "': "
                      + err.error + ": " + err.reason + "!");
        }
      }

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
      for (const auto & psu : _static_users_) {
        const user & su = psu.second;
        if (!has_user(su.get_login())) {
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().add_user(su.get_login(),
                                               su._password_,
                                               err),
                      std::logic_error,
                      "Cannot create user '" << su.get_login() << "': "
                      + err.error + ": " + err.reason + "!");
        }
      }

      {
        DT_LOG_DEBUG(get_logging_priority(), "Vire CMS users:");
        ::rabbitmq::user::list users;
        ::rabbitmq::error_response err;
        manager_service * mutable_this = const_cast<manager_service*>(this);
        mutable_this->grab_manager().list_users(users, err);
        for (const auto & u : users) {
          if (has_static_user(u.name)) {
            DT_LOG_DEBUG(get_logging_priority(), " - User: '" << u.name << "'");
          }
        }
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_subcontractors_system_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (_static_domains_.count("subcontractors/system/*")) {
        std::set<std::string> subnames;
        fetch_subcontractor_users(subnames);
        for (const auto & subname : subnames) {
          const user & scu = get_static_user(subname);
           DT_LOG_DEBUG(get_logging_priority(), "Adding system subcontractor domain '" << subname
                       << "' associated to subcontractor user '" << subname << "'...");
          _setup_vire_cms_domains_subcontractor_system_(subname);
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_subcontractor_system_(const std::string & name_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_LOG_DEBUG(get_logging_priority(), "Adding system subcontractor domain '" << name_ << "'...");
      ::rabbitmq::error_response err;
      const user & sys_sc_user = get_static_user(name_);
      std::string vhost = _vhost_name_prefix_ + vire_cms_path() + "/subcontractors/system/" + name_; 
      DT_LOG_DEBUG(get_logging_priority(), "Building system subcontractor domain...");
      if (!has_vhost(vhost)) {
        DT_THROW_IF(!grab_manager().add_vhost(vhost, err),
                    std::logic_error,
                    "Cannot create the '" << vhost << "' virtual host: "
                    << err.error << ": " << err.reason << "!");
      } else {
        DT_LOG_DEBUG(get_logging_priority(), "Virtual host '" << vhost << "' already exists.");
      }
      // Set administrator permissions:
      DT_THROW_IF(!grab_manager().set_permissions(get_admin_login(),
                                                  vhost,
                                                  ".*", ".*", ".*",
                                                  err),
                    std::logic_error,
                  "Cannot set '" << get_admin_login() << "' permissions for '"
                  << vhost << "' virtual host: "
                  << err.error << ": " << err.reason << "!");
      // Fill the vhost with exchanges:
      static const std::set<std::string> exchanges = {"vireserver.service",
                                                      "vireserver.event",
                                                      "subcontractor.service",
                                                      "subcontractor.event"};
      for (auto exchange : exchanges) {
        DT_LOG_DEBUG(get_logging_priority(), "Building monitor exchange '" << exchange << "'...");
        if (has_exchange(vhost, exchange)) continue;
        DT_THROW_IF(!grab_manager().exchange_declare(exchange,
                                                     vhost,
                                                     "topic",
                                                     true,
                                                     false,
                                                     false,
                                                     err),
                    std::logic_error,
                    "Cannot create the '" << exchange << "' exchange in '"
                    << vhost << "' virtual host: "
                    << err.error << ": " << err.reason + "!");
        std::set<std::string> sys_logins;
        std::string server_user;
        fetch_server_user(server_user);
        sys_logins.insert(server_user);
        sys_logins.insert(name_);
        for (const auto & sys_login : sys_logins) {
          DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for static user '" << sys_login
                       << "' and exchange '" << exchange << "' in the '"
                       << vhost << "' virtual host...");
          ::rabbitmq::permissions perms;
          perms.user  = sys_login;
          perms.vhost = vhost;
          const user & sys_user = get_static_user(sys_login);
          if (sys_user.is_server()) {
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
          }
          if (sys_user.is_subcontractor()) {
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
          }
          DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                      std::logic_error,
                      "Cannot set user '" << sys_login << "' permissions on exchange '" << exchange
                      << "' for '"
                      << vhost << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_LOG_DEBUG(get_logging_priority(), "Building Vire CMS domains/vhosts...");
      _setup_vire_cms_domains_clients_gate_();
      _setup_vire_cms_domains_control_();
      _setup_vire_cms_domains_monitoring_();
      _setup_vire_cms_domains_subcontractors_system_();

      {
        DT_LOG_DEBUG(get_logging_priority(), "Vire CMS domains/vhosts are built:");
        ::rabbitmq::vhost::list vhosts;
        ::rabbitmq::error_response err;
        manager_service * mutable_this = const_cast<manager_service*>(this);
        mutable_this->grab_manager().list_vhosts(vhosts, err);
        for (const auto & v : vhosts) {
          if (boost::algorithm::starts_with(v.name, _vhost_name_prefix_)) {
            DT_LOG_DEBUG(get_logging_priority(), " - Virtual host: '" << v.name << "'");
          }
        }
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_monitoring_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      ::rabbitmq::error_response err;
      if (_static_domains_.count("monitoring")) {
        DT_LOG_DEBUG(get_logging_priority(), "Building monitoring...");
        std::string vhost = _vhost_name_prefix_ + vire_cms_path() + "/monitoring";
        if (!has_vhost(vhost)) {
          DT_THROW_IF(!grab_manager().add_vhost(vhost, err),
                      std::logic_error,
                      "Cannot create the '" << vhost << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        } else {
          DT_LOG_DEBUG(get_logging_priority(), "Virtual host '" << vhost << "' already exists.");
        }
        // Set administrator permissions:
        DT_THROW_IF(!grab_manager().set_permissions(get_admin_login(),
                                                    vhost,
                                                    ".*", ".*", ".*",
                                                    err),
                    std::logic_error,
                    "Cannot set '" << get_admin_login() << "' permissions for '"
                    << vhost << "' virtual host: "
                    << err.error << ": " << err.reason << "!");
        // Fill the vhost with exchanges:
        static const std::set<std::string> exchanges = {"resource_request.service",
                                                        "log.event",
                                                        "alarm.event","pubsub.event"};
        for (auto exchange : exchanges) {
          DT_LOG_DEBUG(get_logging_priority(), "Building monitor exchange '" << exchange << "'...");
          if ( has_exchange(vhost, exchange)) continue;
          DT_THROW_IF(!grab_manager().exchange_declare(exchange,
                                                       vhost,
                                                       "topic",
                                                       true,
                                                       false,
                                                       false,
                                                       err),
                      std::logic_error,
                      "Cannot create the '" << exchange << "' exchange in '"
                      << vhost << "' virtual host: "
                      << err.error << ": " << err.reason + "!");
          // Permissions:
          std::set<std::string> sys_logins;
          fetch_static_users(sys_logins);
          for (const auto & sys_login : sys_logins) {
            DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for static user '" << sys_login
                         << "' and exchange '" << exchange << "' in the '"
                         << vhost << "' virtual host...");
            ::rabbitmq::permissions perms;
            perms.user  = sys_login;
            perms.vhost = vhost;
            const user & sys_user = get_static_user(sys_login);
            if (boost::algorithm::ends_with(exchange, vire_cms_exchange_service_suffix())) {
              permissions::add_exchange_service_server_perms(perms, exchange);
              if (sys_user.is_server()) {
                permissions::add_exchange_service_client_perms(perms, exchange);
              }
            } else if (boost::algorithm::ends_with(exchange, vire_cms_exchange_event_suffix())) {
              permissions::add_exchange_event_producer_perms(perms, exchange);
              if (sys_user.is_server()) {
                permissions::add_exchange_event_listener_perms(perms, exchange);
              }
            }
            DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                        std::logic_error,
                        "Cannot set user '" << sys_login << "' permissions on exchange '" << exchange
                        << "' for '"
                        << vhost << "' virtual host: "
                        << err.error << ": " << err.reason << "!");
          }
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_control_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      ::rabbitmq::error_response err;
      if (_static_domains_.count("control")) {
        DT_LOG_DEBUG(get_logging_priority(), "Building control...");
        std::string vhost = _vhost_name_prefix_ + vire_cms_path() + "/control";
        if (!has_vhost(vhost)) {
          DT_THROW_IF(!grab_manager().add_vhost(vhost, err),
                      std::logic_error,
                      "Cannot create the '" << vhost << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        } else {
          DT_LOG_DEBUG(get_logging_priority(), "Virtual host '" << vhost << "' already exists.");
        }
        // Set administrator permissions:
        DT_THROW_IF(!grab_manager().set_permissions(get_admin_login(),
                                                    vhost,
                                                    ".*", ".*", ".*",
                                                    err),
                    std::logic_error,
                    "Cannot set '" << get_admin_login() << "' permissions for '"
                    << vhost << "' virtual host: "
                    << err.error << ": " << err.reason << "!");
        // Fill the vhost with exchange:
        std::string service_exchange = "resource_request.service";
        if (!has_exchange(vhost, service_exchange)) {
          DT_THROW_IF(!grab_manager().exchange_declare(service_exchange,
                                                       vhost,
                                                       "topic",
                                                       true,
                                                       false,
                                                       false,
                                                       err),
                      std::logic_error,
                      "Cannot create the '" << service_exchange << "' exchange in '"
                      << vhost << "' virtual host: "
                      + err.error + ": " + err.reason + "!");
          // Permissions:
          std::string server_login;
          fetch_server_user(server_login);
          {
            const std::string & sys_login = server_login;
            const std::string & exchange = service_exchange;
            DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for static user '" << sys_login
                         << "' and exchange '" << exchange << "' in the '"
                         << vhost << "' virtual host...");
            ::rabbitmq::permissions perms;
            perms.user  = sys_login;
            perms.vhost = vhost;
            permissions::add_exchange_service_server_perms(perms, exchange);
            DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                        std::logic_error,
                        "Cannot set user '" << sys_login << "' permissions on exchange '" << exchange
                        << "' for '"
                        << vhost << "' virtual host: "
                        << err.error << ": " << err.reason << "!");
          }
        }
      } // end of control
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_clients_gate_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      ::rabbitmq::error_response err;

      // Gate:
      if (_static_domains_.count("clients/gate")) {
        DT_LOG_DEBUG(get_logging_priority(), "Building gate...");
        std::string vhost = _vhost_name_prefix_ + vire_cms_path() + "/clients/gate";
        if (!has_vhost(vhost)) {
          DT_THROW_IF(!grab_manager().add_vhost(vhost, err),
                      std::logic_error,
                      "Cannot create the '" << vhost << "' virtual host: "
                      << err.error << ": " << err.reason + "!");
        } else {
          DT_LOG_DEBUG(get_logging_priority(), "Virtual host '" << vhost << "' already exists.");
        }
        // Set administrator permissions:
        DT_THROW_IF(!grab_manager().set_permissions(get_admin_login(),
                                                    vhost,
                                                    ".*", ".*", ".*",
                                                    err),
                    std::logic_error,
                    "Cannot set '" << get_admin_login() << "' permissions for '"
                    << vhost << "' virtual host: "
                    << err.error << ": " << err.reason << "!");
        // Fill the vhost with exchanges:
        std::string service_exchange = "gate.service";
        if (!has_exchange(vhost, service_exchange)) {
          DT_THROW_IF(!grab_manager().exchange_declare(service_exchange,
                                                       vhost,
                                                       "topic",
                                                       true,
                                                       false,
                                                       false,
                                                       err),
                      std::logic_error,
                      "Cannot create the '" << service_exchange << "' exchange"
                      << " in the '" << vhost << "' virtual host: "
                      << err.error << ": " + err.reason << "!");
          // Permissions:
          std::string server_login;
          fetch_server_user(server_login);
          {
            const std::string & sys_login = server_login;
            const std::string & exchange = service_exchange;
            DT_LOG_DEBUG(get_logging_priority(), "Setting permissions for static user '" << sys_login
                         << "' and exchange '" << exchange << "' in the '"
                         << vhost << "' virtual host...");
            ::rabbitmq::permissions perms;
            perms.user  = sys_login;
            perms.vhost = vhost;
            permissions::add_exchange_service_server_perms(perms, exchange);
            DT_THROW_IF(!grab_manager().set_permissions(perms, err),
                        std::logic_error,
                        "Cannot set user '" << sys_login << "' permissions on exchange '" << exchange
                        << "' for '"
                        << vhost << "' virtual host: "
                        << err.error << ": " << err.reason << "!");
          }
        }
      } // end of gate

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    const user & manager_service::get_client_user(const std::string & login_) const
    {
      std::map<std::string, user>::const_iterator found = _client_users_.find(login_);
      DT_THROW_IF(found == _client_users_.end(),
                  std::logic_error,
                  "There is no client user '" << login_ << "'");
      return found->second;
    }

    bool manager_service::has_client_user(const std::string & login_) const
    {
      return _client_users_.count(login_) == 1;
    }

    void manager_service::fetch_client_users(std::set<std::string> & logins_) const
    {
      logins_.clear();
      for (const auto & pu : _client_users_) {
        const user & su = pu.second;
        logins_.insert(su.get_login());
      }
      return;
    }

    void manager_service::add_client_user(const std::string & login_,
                                          const std::string & password_,
                                          bool use_monitoring_,
                                          bool use_control_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "RabbitMQ manager service is not initialized!");
      DT_THROW_IF(has_user(login_), std::logic_error,
                  "User '" << login_ << "' already exists!");
      DT_THROW_IF(has_client_user(login_), std::logic_error,
                  "Client user '" << login_ << "' already exists!");
      user client_user(login_, password_, user::CATEGORY_CLIENT);
      client_user.use_monitoring = use_monitoring_;
      client_user.use_control = use_control_;
      if (!has_user(client_user.get_login())) {
        ::rabbitmq::error_response err;
        if (!grab_manager().add_user(client_user.get_login(),
                                     client_user._password_,
                                     err)) {
          DT_THROW(std::logic_error,
                   "Cannot create rabbitmq client user '" << login_ << "': "
                   + err.error + ": " + err.reason + "!");
        }
      }
      _setup_vire_cms_domains_client_system_(login_);
      _client_users_[login_] = client_user;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::remove_client_user(const std::string & login_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(! has_client_user(login_), std::logic_error,
                  "Client user '" << login_ << "' does not exist!");
      _client_users_.erase(login_);
      _destroy_vire_cms_domains_client_system_(login_);
      if (has_user(login_)) {
        ::rabbitmq::error_response err;
        if (!grab_manager().delete_user(login_, err)) {
          DT_THROW(std::logic_error,
                   "Cannot delete rabbitmq client user '" << login_ << "': "
                   + err.error + ": " + err.reason + "!");
        }
      }
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

    void manager_service::_destroy_vire_cms_domains_client_system_(const std::string & login_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      const user & client_user = get_client_user(login_);
      ::rabbitmq::error_response err;
      {
        std::string vhost = _vhost_name_prefix_ + vire_cms_path() + "/clients/system/"
          + client_user.get_login();
        if (has_vhost(vhost)) {
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().delete_vhost(vhost, err),
                      std::logic_error,
                      "Cannot delete the '" << vhost << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        } else {
          DT_LOG_DEBUG(get_logging_priority(), "Virtual host '" << vhost << "' does not exist.");
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager_service::_setup_vire_cms_domains_client_system_(const std::string & name_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());

      DT_LOG_DEBUG(get_logging_priority(), "Adding system client domain '" << name_ << "'...");
      const user & client_user = get_client_user(name_);
      {
        std::string vhost = get_vhost_name_prefix() + vire_cms_path() + "/clients/system/"
          + client_user.get_login();
        DT_LOG_DEBUG(get_logging_priority(), "Building system client domain...");
        if (!has_vhost(vhost)) {
          ::rabbitmq::error_response err;
          DT_THROW_IF(!grab_manager().add_vhost(vhost, err),
                      std::logic_error,
                      "Cannot create the '" << vhost << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        } else {
          DT_LOG_DEBUG(get_logging_priority(), "Virtual host '" << vhost << "' already exists.");
        }
      }

      if (client_user.use_control) {
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

      if (client_user.use_monitoring) {
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
      std::string vhost = get_vhost_name_prefix() + vire_cms_path() + "/clients/system/"
        + client_user.get_login();
      static const std::set<std::string> exchanges = {"vireserver.service",
                                                      "vireserver.event"};
      for (auto exchange : exchanges) {
        DT_LOG_DEBUG(get_logging_priority(), "Building client exchange '" << exchange << "'...");
        if (has_exchange(vhost, exchange)) continue;
        ::rabbitmq::error_response err;
        DT_THROW_IF(!grab_manager().exchange_declare(exchange,
                                                     vhost,
                                                     "topic",
                                                     true,
                                                     false,
                                                     false,
                                                     err),
                    std::logic_error,
                    "Cannot create the '" << exchange << "' exchange in '"
                    << vhost << "' virtual host: "
                    << err.error << ": " << err.reason + "!");
        DT_THROW_IF(!grab_manager().set_permissions(get_admin_login(),
                                                    vhost,
                                                    ".*", ".*", ".*",
                                                    err),
                    std::logic_error,
                    "Cannot set '" << get_admin_login() << "' permissions for '"
                    << vhost << "' virtual host: "
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
                       << vhost << "' virtual host...");
          ::rabbitmq::permissions perms;
          perms.user  = u;
          perms.vhost = vhost;
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
                      << vhost << "' virtual host: "
                      << err.error << ": " << err.reason << "!");
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

  } // namespace rabbitmq

} // namespace vire
