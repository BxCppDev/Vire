//! \file  vire/com/utils.h
//! \brief Vire com utilities
//
// Copyright (c) 2016-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_UTILS_H
#define VIRE_COM_UTILS_H

// Standard library:
#include <vector>

// Third party:
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/utility/model_identifier.h>

namespace vire {

  namespace com {

    enum com_status {
      COM_OK          = 0, //!< Success
      COM_SUCCESS     = 0, //!< Success
      COM_FAILURE     = 1, //!< Generic failure
      COM_UNAVAILABLE = 2, //!< Unavailable resource error
      COM_PERMISSION  = 3, //!< Permission error
      COM_TIMEOUT     = 4  //!< Timeout error
    };

    //! \brief Access categories
    enum access_category_type {
      ACCESS_CATEGORY_INVALID                     =  0, ///< Invalid access category
      ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM = 10, ///< Server subcontractor system category (in CMS serve)
      ACCESS_CATEGORY_SERVER_CLIENT_SYSTEM        = 11, ///< Server client system category (in CMS server)
      ACCESS_CATEGORY_SERVER_GATE                 = 12, ///< Server gate system category (in CMS server, unique user)
      ACCESS_CATEGORY_SERVER_CMS                  = 13, ///< Server CMS category (in CMS server, unique user)
      ACCESS_CATEGORY_CLIENT_SYSTEM               = 20, ///< Client system category (in CMS clients)
      ACCESS_CATEGORY_CLIENT_CMS                  = 21, ///< Client CMS category (in CMS server or clients)
      ACCESS_CATEGORY_CLIENT_GATE                 = 22, ///< Server gate system category (in CMS server, unique user)
      ACCESS_CATEGORY_SUBCONTRACTOR               = 30, ///< Subcontractor category (in CMS subcontractors)
    };

    const std::set<access_category_type> & access_categories_with_unique_user();
    
    bool is_unique_user(const access_category_type);
    
    //! Return the label associated to an access category
    std::string to_string(const access_category_type);
       
    //! Return the access category type associated to a label
    bool from_string(const std::string &, access_category_type &);

    bool access_category_requires_target(const access_category_type);
 
    bool access_category_is_persistant(const access_category_type);

    const std::string & server_cms_access_label();
    const std::string & server_gate_access_label();
    const std::string & client_gate_access_label();
    const std::string & server_subcontractor_system_access_label();
    const std::string & server_client_system_access_label();

    //! Build a standard access login from prefix and access profile label
    void build_system_login(const std::string & prefix_,
                            const std::string & access_label_,
                            std::string & login_);

    //! Return a standard access login from prefix and login base
    std::string make_system_login(const std::string & prefix_,
                                  const std::string & access_label_);
      
    //! \brief Domain categories
    enum domain_category_type {
      DOMAIN_CATEGORY_INVALID              = 0, ///< Invalid domain category
      DOMAIN_CATEGORY_GATE                 = 1, ///< Gate category
      DOMAIN_CATEGORY_CLIENT_SYSTEM        = 2, ///< Client system category
      DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM = 3, ///< Subcontractor system category
      DOMAIN_CATEGORY_CONTROL              = 4, ///< Control category
      DOMAIN_CATEGORY_MONITORING           = 5  ///< Monitoring category
    };

    const std::set<domain_category_type> & domain_categories_with_unique_domain();
   
    bool is_unique_domain(const domain_category_type);

    //! Return the label associated to a domain category
    std::string to_string(const domain_category_type);
       
    //! Return the domain category type associated to a label
    bool from_string(const std::string &, domain_category_type &);

    /// \brief Raw message record
    ///
    /// This record contains :
    /// - a dictionnary of simple properties to store
    ///   metadata about a message,
    /// - a raw buffer of characters to encode the message payload
    struct raw_message_type
    {
      typedef std::vector<char> buffer_type;

      //! Clear the raw message internals
      void reset();

      datatools::properties metadata; //!< List of metadata (key/value pairs)
      buffer_type           buffer;   //!< Encoded message buffer

    };

    const std::string & domain_gate_label();
    const std::string & domain_system_label();
    const std::string & domain_control_label();
    const std::string & domain_monitoring_label();

    const std::string & private_mailbox_key(); ///< Corresponds to "reply-to" basic_properties in RabbitMQ
    const std::string & message_id_key();
    const std::string & correlation_id_key();
    const std::string & address_key();
    const std::string & system_connection_key();

    const std::string & alarm_event_monitoring_label();
    const std::string & log_event_monitoring_label();
    const std::string & pubsub_event_monitoring_label();
    
    enum plug_category_type {
      PLUG_INVALID        = 0,
      PLUG_EVENT_EMITTER  = 1,
      PLUG_EVENT_LISTENER = 2,
      PLUG_SERVICE_CLIENT = 3,
      PLUG_SERVICE_SERVER = 4
    };

    std::string to_string(const plug_category_type);

    const std::string & mailbox_gate_service_name();
    const std::string & mailbox_client_system_service_name();
    const std::string & mailbox_cms_service_name();
    const std::string & mailbox_subcontractor_system_vireserver_service_name();
    const std::string & mailbox_subcontractor_system_subcontractor_service_name();
    const std::string & mailbox_system_vireserver_event_name();
    const std::string & mailbox_system_subcontractor_event_name();
    const std::string & mailbox_monitoring_log_event_name();
    const std::string & mailbox_monitoring_alarm_event_name();
    const std::string & mailbox_monitoring_pubsub_event_name();

    /// \brief Subcontractor management information record
    struct subcontractor_info
    {
      std::string id;                 //!< Unique identifier of the subcontractor
      std::string description;        //!< Description of the subcontractor
      std::string user_login;         //!< User login of the subcontractor
      std::string user_password;      //!< User password of the subcontractor
      std::string sys_svr_login;      //!< Server side system access login of the subcontractor
      std::string sys_svr_password;   //!< Server side system access password of the subcontractor
      bool        persistent = true;  //!< Persitence flag for transport resources associated to the subcontractor
      std::string system_domain_name; //!< Name of the system domain associated to the subcontractor
      vire::utility::model_identifier system_transport_driver_type_id; //!< Transport driver of the system domain associated to the subcontractor
      vire::utility::model_identifier system_encoding_driver_type_id;  //!< Encoding driver of the system domain associated to the subcontractor
    };
  
    /// \brief Client management information record
    struct client_info
    {
      std::string id;                   //!< Unique identifier of the client
      std::string description;          //!< Description of the client
      std::string sys_user_login;       //!< System user login of the client
      std::string sys_user_password;    //!< System user password of the client
      std::string sys_svr_login;        //!< System server access login of the client
      std::string sys_svr_password;     //!< System server access password of the client
      std::string cms_user_login;       //!< CMS user login of the client
      std::string cms_user_password;    //!< CMS user password of the client
      bool        with_control = false; //!< Flag for access to control domain
      std::string system_domain_name;   //!< Name of the system domain associated to the client
      vire::utility::model_identifier system_transport_driver_type_id; //!< Transport driver of the system domain associated to the client
      vire::utility::model_identifier system_encoding_driver_type_id;  //!< Encoding driver of the system domain associated to the client
    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
