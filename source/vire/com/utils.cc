//! \file vire/com/utils.cc
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
#include <vire/com/utils.h>

namespace vire {

  namespace com {

    void raw_message_type::reset()
    {
      buffer.clear();
      metadata.clear();
      return;
    }

    std::string to_string(const plug_category_type cat_)
    {
      switch(cat_) {
      case PLUG_EVENT_EMITTER: return std::string("event_emitter");
      case PLUG_EVENT_LISTENER: return std::string("event_listener");
      case PLUG_SERVICE_CLIENT: return std::string("service_client");
      case PLUG_SERVICE_SERVER: return std::string("service_server");
      default: return std::string();
      }
    }

    const std::string & message_id_key()
    {
      static const std::string _k("message_id");
      return _k;
    }
    
    const std::string & correlation_id_key()
    {
      static const std::string _k("correlation_id");
      return _k;
    }
    
    const std::string & address_key()
    {
      static const std::string _k("address");
      return _k;
    }

    const std::string & system_connection_key()
    {
      static const std::string _k("connection");
      return _k;
    }

    const std::string & domain_gate_label()
    {
      static const std::string _l("gate");
      return _l;
    }

    const std::string & domain_system_label()
    {
      static const std::string _l("system");
      return _l;
    }

    const std::string & domain_control_label()
    {
      static const std::string _l("control");
      return _l;
    }

    const std::string & domain_monitoring_label()
    {
      static const std::string _l("monitoring");
      return _l;
    }

    std::string to_string(const actor_category_type category_)
    {
      switch(category_) {
      case ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM: return "server-subcontractor-system";
      case ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM: return "server-client-system";
      case ACTOR_CATEGORY_SERVER_GATE: return "server-gate";
      case ACTOR_CATEGORY_SERVER_CMS: return "server-cms";
      case ACTOR_CATEGORY_CLIENT_SYSTEM : return "client-system";
      case ACTOR_CATEGORY_CLIENT_CMS : return "client-cms";
      case ACTOR_CATEGORY_CLIENT_GATE : return "client-gate";
      case ACTOR_CATEGORY_SUBCONTRACTOR : return "subcontractor";
      default:
        break;
      }
      return "";
    }

    const std::set<actor_category_type> & actor_categories_with_unique_user()
    {
      static std::set<actor_category_type> _s;
      if (_s.size()) {
        _s.insert(ACTOR_CATEGORY_SERVER_CMS);
        _s.insert(ACTOR_CATEGORY_SERVER_GATE);
        _s.insert(ACTOR_CATEGORY_CLIENT_GATE);
      }
      return _s;
    }

    bool is_unique_user(const actor_category_type category_)
    {
      return actor_categories_with_unique_user().count(category_);
    }

    bool from_string(const std::string & label_, actor_category_type & category_)
    {
      category_ = ACTOR_CATEGORY_INVALID;
      if (label_ == to_string(ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM)) {
        category_ = ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM;
      }
      if (label_ == to_string(ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM)) {
        category_ = ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM;
      }
      if (label_ == to_string(ACTOR_CATEGORY_SERVER_GATE)) {
        category_ = ACTOR_CATEGORY_SERVER_GATE;
      }
      if (label_ == to_string(ACTOR_CATEGORY_SERVER_CMS)) {
        category_ = ACTOR_CATEGORY_SERVER_CMS;
      }
      if (label_ == to_string(ACTOR_CATEGORY_CLIENT_SYSTEM)) {
        category_ = ACTOR_CATEGORY_CLIENT_SYSTEM;
      }
      if (label_ == to_string(ACTOR_CATEGORY_CLIENT_CMS)) {
        category_ = ACTOR_CATEGORY_CLIENT_CMS;
      }
      if (label_ == to_string(ACTOR_CATEGORY_CLIENT_GATE)) {
        category_ = ACTOR_CATEGORY_CLIENT_GATE;
      }
      if (label_ == to_string(ACTOR_CATEGORY_SUBCONTRACTOR)) {
        category_ = ACTOR_CATEGORY_SUBCONTRACTOR;
      }
      return category_ != ACTOR_CATEGORY_INVALID;
    }

    bool actor_category_requires_target(const actor_category_type category_)
    {
      switch(category_) {
      case ACTOR_CATEGORY_SERVER_GATE:
      case ACTOR_CATEGORY_SERVER_CMS:
      case ACTOR_CATEGORY_CLIENT_CMS:
      case ACTOR_CATEGORY_CLIENT_GATE:
        return false;
      }
      return true;
    }

    bool actor_category_is_persistant(const actor_category_type category_)
    {
      switch(category_) {
      case ACTOR_CATEGORY_SERVER_GATE:
      case ACTOR_CATEGORY_SERVER_CMS:
      case ACTOR_CATEGORY_SUBCONTRACTOR:
        return true;
     }
      return false;
    }

    std::string to_string(const domain_category_type category_)
    {
      switch(category_) {
      case DOMAIN_CATEGORY_GATE:                 return "vire::com::domain::gate";
      case DOMAIN_CATEGORY_CLIENT_SYSTEM:        return "vire::com::domain::client_system";
      case DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM: return "vire::com::domain::subcontractor_system";
      case DOMAIN_CATEGORY_CONTROL:              return "vire::com::domain::control";
      case DOMAIN_CATEGORY_MONITORING:           return "vire::com::domain::monitoring";
      default:
        break;
      }
      return "";
    }

    const std::set<domain_category_type> & domain_categories_with_unique_domain()
    {
      static std::set<domain_category_type> _s;
      if (_s.size()) {
        _s.insert(DOMAIN_CATEGORY_GATE);
        _s.insert(DOMAIN_CATEGORY_CONTROL);
        _s.insert(DOMAIN_CATEGORY_MONITORING);
      }
      return _s;
    }

    bool is_unique_domain(const domain_category_type category_)
    {
      return domain_categories_with_unique_domain().count(category_);
    }

    bool from_string(const std::string & label_, domain_category_type & category_)
    {
      category_ = DOMAIN_CATEGORY_INVALID;
      if (label_ == to_string(DOMAIN_CATEGORY_GATE)) {
        category_ = DOMAIN_CATEGORY_GATE;
      }
      if (label_ == to_string(DOMAIN_CATEGORY_CLIENT_SYSTEM)) {
        category_ = DOMAIN_CATEGORY_CLIENT_SYSTEM;
      }
      if (label_ == to_string(DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM)) {
        category_ = DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM;
      }
      if (label_ == to_string(DOMAIN_CATEGORY_CONTROL)) {
        category_ = DOMAIN_CATEGORY_CONTROL;
      }
      if (label_ == to_string(DOMAIN_CATEGORY_MONITORING)) {
        category_ = DOMAIN_CATEGORY_MONITORING;
      }
      return category_ != DOMAIN_CATEGORY_INVALID;
    }

    const std::string & alarm_event_monitoring_label()
    {
      static const std::string _l("alarm");
      return _l;
    }

    const std::string & log_event_monitoring_label()
    {
      static const std::string _l("log");
      return _l;
    }

    const std::string & pubsub_event_monitoring_label()
    {
      static const std::string _l("pubsub");
      return _l;
    }
 
    const std::string & mailbox_gate_service_name()
    {
      static const std::string _n("gate.service");
      return _n;
    }
 
    const std::string & mailbox_client_system_service_name()
    {
      static const std::string _n("vireserver.service");
      return _n;
    }
 
    const std::string & mailbox_cms_service_name()
    {
      static const std::string _n("resource_request.service");
      return _n;
    }
 
    const std::string & mailbox_subcontractor_system_vireserver_service_name()
    {
      static const std::string _n("vireserver.service");
      return _n;
    }

    const std::string & mailbox_subcontractor_system_subcontractor_service_name()
    {
      static const std::string _n("subcontractor.service");
      return _n;
    }

    const std::string & mailbox_system_vireserver_event_name()
    {
      static const std::string _n("vireserver.event");
      return _n;
    }

    const std::string & mailbox_system_subcontractor_event_name()
    {
      static const std::string _n("subcontractor.event");
      return _n;
    }

    const std::string & mailbox_monitoring_log_event_name()
    {
      static const std::string _n("log.event");
      return _n;
    }

    const std::string & mailbox_monitoring_alarm_event_name()
    {
      static const std::string _n("alarm.event");
      return _n;
    }

    const std::string & mailbox_monitoring_pubsub_event_name()
    {
      static const std::string _n("pubsub.event");
      return _n;
    }

  } // namespace com

} // namespace vire
