//! \file  vire/rabbitmq/manager_service.h
//! \brief Vire CMS RabbitMQ manager service
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

#ifndef VIRE_RABBITMQ_MANAGER_SERVICE_H
#define VIRE_RABBITMQ_MANAGER_SERVICE_H

// Standard Library:
#include <string>
#include <map>
#include <vector>
#include <memory>

// Third party:
// - Bayeux:
#include <bayeux/datatools/base_service.h>
#include <bayeux/datatools/properties.h>

// This package:
#include <vire/rabbitmq/user.h>

namespace rabbitmq {
  class rabbit_mgr;
}

namespace vire {

  namespace rabbitmq {

    //! \brief RabbitMQ manager service used by the Vire CMS server
    class manager_service
      : public ::datatools::base_service
    {
    public:

      static const std::string & vire_cms_path();

      static const std::string & vire_cms_exchange_service_suffix();

      static const std::string & vire_cms_exchange_event_suffix();

      //! Default constructor
      manager_service(uint32_t flags_ = 0);

      //! Destructor
      virtual ~manager_service();

      //! Check if the service is initialized
      virtual bool is_initialized() const;

      //! Initialize the service using a list of properties with access to a dictionary of services
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the service
      virtual int reset();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      void set_destroy_all_at_reset(const bool);

      bool is_destroy_all_at_reset() const;

      bool has_server_host() const;

      const std::string & get_server_host() const;

      void set_server_host(const std::string &);

      bool has_server_port() const;

      int get_server_port() const;

      void set_server_port(const int);

      bool has_vhost_name_prefix() const;

      const std::string & get_vhost_name_prefix() const;

      void set_vhost_name_prefix(const std::string &);

      bool has_admin_login() const;

      const std::string & get_admin_login() const;

      void set_admin_login(const std::string &);

      bool has_admin_password() const;

      const std::string & get_admin_password() const;

      void set_admin_password(const std::string &);

      const ::rabbitmq::rabbit_mgr & get_manager() const;

      ::rabbitmq::rabbit_mgr & grab_manager();

      bool has_vhost(const std::string &) const;
      // void add_vhost(const std::string &) const;
      // void delete_vhost(const std::string &) const;
      bool has_user(const std::string &) const;
      bool has_exchange(const std::string & vhost_, const std::string &) const;
      bool has_queue(const std::string & vhost_, const std::string &) const;

      void add_subcontractor_system_domain(const std::string & domain_name_);

      // Management of permanent users:
      void add_static_user(const user & user_);
      void add_server_user(const user & user_);
      void add_subcontractor_user(const user & user_);
      bool has_static_user(const std::string & login_) const;
      bool has_server_user() const;
      void fetch_static_users(std::set<std::string> & logins_) const;
      bool fetch_server_user(std::string & login_) const;
      void fetch_subcontractor_users(std::set<std::string> & logins_) const;
      bool is_server_user(const std::string & login_) const;
      bool is_subcontractor_user(const std::string & login_) const;
      const user & get_static_user(const std::string & login_) const;

      // Management of temporary client connection users:
      bool has_client_user(const std::string & login_) const;
      void add_client_user(const std::string & login_,
                           const std::string & password_,
                           bool use_monitoring_ = true,
                           bool use_control_ = false);
      void remove_client_user(const std::string & login_);
      const user & get_client_user(const std::string & login_) const;
      void fetch_client_users(std::set<std::string> & logins_) const;

      // Management of dynamic real users:
      bool has_real_user(const std::string & login_) const;
      void add_real_user(const std::string & login_, const std::string & password_);
      void remove_real_user(const std::string & login_);
      const user & get_real_user(const std::string & login_) const;
      void fetch_real_users(std::set<std::string> & logins_) const;
      void change_real_user_password(const std::string & login_,
                                     const std::string & password_);
      void sync_real_users();

    private:

      void _set_defaults_();
      void _setup_vire_cms_();
      void _setup_vire_cms_users_();
      void _setup_vire_cms_domains_();
      void _setup_vire_cms_domains_clients_gate_();
      void _setup_vire_cms_domains_control_();
      void _setup_vire_cms_domains_monitoring_();
      void _setup_vire_cms_domains_subcontractors_system_();
      void _setup_vire_cms_domains_subcontractor_system_(const std::string & name_);
      void _setup_vire_cms_domains_client_system_(const std::string & name_);
      void _destroy_vire_cms_domains_client_system_(const std::string & name_);
      void _force_destroy_vire_cms_();

      // Add/remove dynamic real users permissions for the gate domain
      void _setup_vire_cms_domains_clients_gate_add_real_user(const std::string & name_);
      void _setup_vire_cms_domains_clients_gate_remove_real_user(const std::string & name_);
      void _destroy_vire_cms_real_users_();

    private:

      // Management:
      bool _initialized_ = false;          //!< Initialization flag
      bool _destroy_all_at_reset_ = false; //!< Flag to destroy all persistent material (vhosts/users) at reset

      // Configuration:
      std::string _vhost_name_prefix_; //!< Virtual host name prefix
      std::string _server_host_;       //!< Server host
      int         _server_port_ = -1;  //!< Server port
      std::string _admin_login_;       //!< Administrator login
      std::string _admin_password_;    //!< Administrator password
      std::string _com_service_name_;
      std::string _gate_service_name_;
      std::string _users_service_name_;
      std::string _auth_service_name_;

      std::set<std::string>       _static_domains_; //!< List of managed static domains/vhosts
      std::map<std::string, user> _static_users_;   //!< List of static users (1 server + N subcontractors)

      // Working data:
      std::map<std::string, user> _real_users_;    //!< List of real users for gate access (sync with external database/LDAP)
      std::map<std::string, user> _client_users_;  //!< List of temporary client users (only for session connections)

      // Private implementation:
      class pimpl_type;
      std::unique_ptr<pimpl_type> _pimpl_;

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(manager_service)

    };

  } // namespace rabbitmq

} // namespace vire

// OCD support:
#include <datatools/ocd_macros.h>
// @arg vire::rabbitmq::manager_service the name of the class with OCD support
DOCD_CLASS_DECLARATION(vire::rabbitmq::manager_service)

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::rabbitmq::manager_service);
#endif // Q_MOC_RUN

#endif // VIRE_RABBITMQ_MANAGER_SERVICE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
