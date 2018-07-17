//! \file  vire/com/utils.h
//! \brief Vire com utilities
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

    //! \brief Actor categories
    enum actor_category_type {
      ACTOR_CATEGORY_INVALID                     =  0, ///< Invalid actor category
      ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM = 10, ///< Server subcontractor system category (in CMS server)
      ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM        = 11, ///< Server client system category (in CMS server)
      ACTOR_CATEGORY_SERVER_GATE                 = 12, ///< Server gate system category (in CMS server, unique user)
      ACTOR_CATEGORY_SERVER_CMS                  = 13, ///< Server CMS category (in CMS server, unique user)
      ACTOR_CATEGORY_CLIENT_SYSTEM               = 20, ///< Client system category (in CMS clients)
      ACTOR_CATEGORY_CLIENT_CMS                  = 21, ///< Client CMS category (in CMS server or clients)
      ACTOR_CATEGORY_CLIENT_GATE                 = 22, ///< Server gate system category (in CMS server, unique user)
      ACTOR_CATEGORY_SUBCONTRACTOR               = 30, ///< Subcontractor category (in CMS subcontractors)
    };

    const std::set<actor_category_type> & actor_categories_with_unique_user();
    
    bool is_unique_user(const actor_category_type);
    
    //! Return the label associated to an actor category
    std::string to_string(const actor_category_type);
       
    //! Return the actor category type associated to a label
    bool from_string(const std::string &, actor_category_type &);

    bool actor_category_requires_target(const actor_category_type);
 
    bool actor_category_is_persistant(const actor_category_type);

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

  } // namespace com

} // namespace vire

#endif // VIRE_COM_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
