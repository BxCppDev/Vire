//! \file  vire/rabbitmq/manager_service.h
//! \brief Vire CMS RabbitMQ manager service
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
#include <vire/rabbitmq/vhost.h>

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

      const std::list<user> & get_system_users() const;

      void add_system_user(const user & u_);

      void add_system_user(const std::string & login_,
                           const std::string & password_,
                           const vire::com::actor_category_type & category_);

      const ::rabbitmq::rabbit_mgr & get_manager() const;

      ::rabbitmq::rabbit_mgr & grab_manager();

      bool has_vhost(const std::string & name_,
                     const vire::com::domain_category_type category_ = vire::com::DOMAIN_CATEGORY_INVALID) const;
      
      void add_vhost(const vhost &);
      
      void remove_vhost(const std::string &);
      
      const vhost & get_vhost(const std::string & name_) const;
     
      bool has_user(const std::string & login_,
                    const vire::com::actor_category_type category_ = vire::com::ACTOR_CATEGORY_INVALID) const;
      
      void add_user(const user & user_);
      
      void remove_user(const std::string & login_);
      
      const user & get_user(const std::string & login_) const;

      void fetch_vhosts(std::set<std::string> & names_,
                        const vire::com::domain_category_type category_ = vire::com::DOMAIN_CATEGORY_INVALID) const;
      
      void fetch_users(std::set<std::string> & logins_,
                       const vire::com::actor_category_type category_ = vire::com::ACTOR_CATEGORY_INVALID) const;

    private:

      void _set_defaults_();
      void _setup_vire_cms_();
      void _setup_vire_cms_users_();
      void _setup_vire_cms_domains_();
      void _setup_vire_cms_domains_clients_gate_(const std::string & vhost_name);
      void _setup_vire_cms_domains_control_(const std::string & vhost_name_);
      void _setup_vire_cms_domains_monitoring_(const std::string & vhost_name_);
      void _setup_vire_cms_domains_subcontractor_system_(const std::string & vhost_name_);
      void _setup_vire_cms_domains_client_system_(const std::string & vhost_name_);
      void _destroy_vire_cms_domains_client_system_(const std::string & vhost_name_);
      void _force_destroy_vire_cms_();

      /// Check if a vhost with given name is created in the RabbitMQ server scope
      bool _has_vhost_(const std::string & name_) const;
      
      /// Check if an user with given name is created in the RabbitMQ server scope
      bool _has_user_(const std::string & login_) const;
      
      /// Check if an exchange with given name is created in a given vhost in the RabbitMQ server scope
      bool _has_exchange_(const std::string & vhost_, const std::string & exchange_name_) const;

      /// Check if a queue with given name is created in a given vhost in the RabbitMQ server scope
      bool _has_queue_(const std::string & vhost_, const std::string & queue_name_) const;
   
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
      
      std::list<user> _system_users_;  //!< List of system users
      std::string     _server_cms_name_;  //!< Login of the unique server CMS user
      std::string     _server_gate_name_; //!< Login of the unique server gate user
      std::string     _client_gate_name_; //!< Login of the unique client gate user
      
      // Working data:
      std::map<std::string, user>  _users_;  //!< List of managed users
      std::map<std::string, vhost> _vhosts_; //!< List of managed vhosts
      std::string                  _vhost_gate_name_;
      std::string                  _vhost_control_name_;
      std::string                  _vhost_monitoring_name_;
 
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
