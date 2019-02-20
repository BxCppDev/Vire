//! \file  vire/rabbitmq/manager_service.h
//! \brief Vire CMS RabbitMQ manager service
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
#include <vire/com/utils.h>

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
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & poptions_ = empty_options()) const override;

      /// Set the flag to destroy all RabbitMQ resources at reset
      void set_destroy_all_at_reset(const bool);

      /// Check if the flag to destroy all RabbitMQ resources at reset is set
      bool is_destroy_all_at_reset() const;

      /// Check if the server host is set
      bool has_server_host() const;

      /// Return the server host
      const std::string & get_server_host() const;

      /// Set the server host
      void set_server_host(const std::string &);

      /// Check if the server port is set
      bool has_server_port() const;

      /// Return the server port
      int get_server_port() const;

      /// Set the server port
      void set_server_port(const int);

      /// Check if the system user name prefix is set
      bool has_system_user_name_prefix() const;

      /// Return the system user name prefix
      const std::string & get_system_user_name_prefix() const;

      /// Set the system user name prefix
      void set_system_user_name_prefix(const std::string &);

      /// Check if the vhost name prefix is set
      bool has_vhost_name_prefix() const;

      /// Return the vhost name prefix
      const std::string & get_vhost_name_prefix() const;

      /// Set the vhost name prefix
      void set_vhost_name_prefix(const std::string &);

      /// Check if the admin login is set
      bool has_admin_login() const;

      /// Return the admin login
      const std::string & get_admin_login() const;

      /// Set the admin login
      void set_admin_login(const std::string &);

      /// Check if the admin password is set
      bool has_admin_password() const;

      /// Return the admin password
      const std::string & get_admin_password() const;

      /// Set the admin password
      void set_admin_password(const std::string &);

      /// \brief Dictionary of system user information records
      typedef std::map<vire::com::access_category_type, user> system_user_dict_type;

      /// Return the dictionary of system users
      const system_user_dict_type & get_system_user_infos() const;

      /// Change the password of a specific system user
      ///
      /// This method must be invoked after the
      /// set_system_user_name_prefix method
      void set_system_user_password(const vire::com::access_category_type category_,
                                    const std::string & password_);

      /// Return the system user of a given category
      const user & get_system_user(const vire::com::access_category_type category_) const;

      /// Check if the system user of a given category is declared
      bool has_system_user(const vire::com::access_category_type category_) const;
      
      /// Declare a system_user
      void declare_system_user(const user & u_);

      /// Declare a system_user
      void declare_system_user(const std::string & login_,
                           const std::string & password_,
                           const vire::com::access_category_type category_);

      /// Return the const RabbitMQ manager
      const ::rabbitmq::rabbit_mgr & get_manager() const;

      /// Return the mutable RabbitMQ manager
      ::rabbitmq::rabbit_mgr & grab_manager();

      /// Check if a management vhost is set
      bool has_vhost(const std::string & name_,
                     const vire::com::domain_category_type category_ = vire::com::DOMAIN_CATEGORY_INVALID) const;
      
      /// Add a management vhost
      void add_vhost(const vhost &);
      
      /// Remove a management vhost
      void remove_vhost(const std::string &);

      /// Return a management vhost
      const vhost & get_vhost(const std::string & name_) const;

      /// Check if a management user is set
      bool has_user(const std::string & login_,
                    const vire::com::access_category_type category_ = vire::com::ACCESS_CATEGORY_INVALID) const;
      
      /// Add a management user
      void add_user(const user & user_);
      
      /// Remove a management user
      void remove_user(const std::string & login_);
      
      /// Return a management user
      const user & get_user(const std::string & login_) const;

      /// Compute the list of management vhosts
      void fetch_vhosts(std::set<std::string> & names_,
                        const vire::com::domain_category_type category_ = vire::com::DOMAIN_CATEGORY_INVALID) const;
      
      /// Compute the list of management users
      void fetch_users(std::set<std::string> & logins_,
                       const vire::com::access_category_type category_ = vire::com::ACCESS_CATEGORY_INVALID) const;

      /// \brief Dictionary of subcontractor information records
      typedef std::map<std::string, vire::com::subcontractor_info> sc_dict_type;

      /// Check if a subcontractor is defined
      bool has_subcontractor(const std::string & sc_id_) const;
      
      /// Add resources for a new subcontractor in the RabbitMQ system
      ///
      /// This implies to add:
      /// - one virtual host (domain)
      /// - one system interface user (server side)
      /// - one user (subcontractor side)
      ///
      void add_subcontractor(const vire::com::subcontractor_info & sc_info_);

      /// Remove subcontractor from the RabbitMQ system
      void remove_subcontractor(const std::string & sc_id_);

      /// Return the subcontractor associated with a given identifier
      const vire::com::subcontractor_info & get_subcontractor_info(const std::string & sc_id_) const;
      
      /// Return the subcontractor associated with a given system vhost
      const vire::com::subcontractor_info & get_subcontractor_info_per_vhost(const std::string & vhost_name_) const;
     
      /// Return the subcontractor associated with a given user
      const vire::com::subcontractor_info & get_subcontractor_info_per_user(const std::string & user_login_) const;
      
      /// Return the dictionary of subcontractor information records
      const sc_dict_type & get_subcontractor_infos() const;

      /// \brief Dictionary of client information records
      typedef std::map<std::string, vire::com::client_info> client_dict_type;

      /// Check if a client is defined
      bool has_client(const std::string & client_id_) const;

      /// Add resources for a new connected client in the RabbitMQ system
      ///
      /// This implies to add:
      /// - one virtual host (client system domain)
      /// - one system interface user (server side)
      /// - one client CMS user (client side)
      ///
      void add_client(const vire::com::client_info & client_info_);

      void change_client_passwords(const std::string & client_id_,
                                   const std::string & sys_user_password_,
                                   const std::string & cms_user_password_);
      
      /// Remove a client from the RabbitMQ system
      void remove_client(const std::string & client_id_);
            
      /// Return the client associated with a given identifier
      const vire::com::client_info & get_client_info(const std::string & cl_id_) const;
            
      /// Return the client associated with a given system vhost
      const vire::com::client_info & get_client_info_per_vhost(const std::string & vhost_name_) const;
 
    private:

      /// Return the dictionary of system users after initialization if needed
      system_user_dict_type & _grab_system_users_infos_();

      /// Set default attributes:
      void _set_defaults_();
      
      void _init_system_users_infos_();
      void _setup_vire_cms_system_users_();
      void _setup_vire_cms_system_domains_();
      void _dismantle_vire_cms_system_users_();
      void _dismantle_vire_cms_system_domains_();

      // Subcontractors:
      // void _setup_vire_cms_subcontractors_();
      void _dismantle_vire_cms_subcontractors_();
      void _setup_vire_cms_subcontractor_(const std::string & sc_id_);
      void _dismantle_vire_cms_subcontractor_(const std::string & sc_id_);
      void _setup_vire_cms_domains_subcontractor_system_(const std::string & sc_id_);

      // Clients:
      void _dismantle_vire_cms_clients_();
      void _setup_vire_cms_client_(const std::string & client_id_);
      void _dismantle_vire_cms_client_(const std::string & client_id_);
      void _setup_vire_cms_domains_client_system_(const std::string & cl_id_);
      void _destroy_vire_cms_domains_client_system_(const std::string & vhost_name_);

      void _setup_vire_cms_domains_clients_gate_(const std::string & vhost_name);
      void _setup_vire_cms_domains_control_(const std::string & vhost_name_);
      void _setup_vire_cms_domains_monitoring_(const std::string & vhost_name_);

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
      std::string _system_user_name_prefix_; //!< System user name prefix (mandatory)
      std::string _vhost_name_prefix_; //!< Virtual host name prefix (mandatory)
      std::string _server_host_;       //!< Server host (mandatory, default: "localhost")
      int         _server_port_ = -1;  //!< Server port (mandatory, default: 15672)
      std::string _admin_login_;       //!< Administrator login (mandatory)
      std::string _admin_password_;    //!< Administrator password (mandatory)

      // System users management:
      system_user_dict_type _system_users_infos_; //!< List of system user information records
      
      // Working data:
      
      std::map<std::string, user>  _users_;  //!< List of managed users
      std::map<std::string, vhost> _vhosts_; //!< List of managed vhosts
      
      // - System domains:
      std::string                  _vhost_gate_name_; //!< Name of the unique gate vhost/domain
      std::string                  _vhost_control_name_; //!< Name of the unique control vhost/domain
      std::string                  _vhost_monitoring_name_; //!< Name of the unique monitoring vhost/domain
      
      // - Subcontractors (vhosts+users)
      sc_dict_type _subcontractor_infos_; //!< Collection of subcontractor information records

      // - Clients (vhosts+users)
      client_dict_type _client_infos_; //!< Collection of client information records
 
      // - Private implementation:
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
