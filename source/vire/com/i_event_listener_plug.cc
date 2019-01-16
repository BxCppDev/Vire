//! \file vire/com/i_event_listener_plug.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/com/i_event_listener_plug.h>

// Third party:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/clhep_units.h>

// This project:
#include <vire/com/domain.h>
#include <vire/com/actor.h>
#include <vire/com/utils.h>
#include <vire/time/utils.h>
#include <vire/message/message.h>
#include <vire/message/body_layout.h>
#include <vire/message/message_header.h>
#include <vire/message/message_body.h>

namespace vire {

  namespace com {
   
    void i_event_listener_plug::_populate_allowed_mailboxes_()
    {
      domain_category_type domCat = get_domain().get_category();
      actor_category_type actorCat = get_parent().get_category();

      bool supported = false;

      if (domCat == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        // Only for ACTOR_CATEGORY_CLIENT_SYSTEM:
        _allowed_mailboxes_.insert(mailbox_system_vireserver_event_name());
        supported = true;
      }

      if (domCat == DOMAIN_CATEGORY_MONITORING) {
        _allowed_mailboxes_.insert(mailbox_monitoring_log_event_name());
        _allowed_mailboxes_.insert(mailbox_monitoring_alarm_event_name());
        _allowed_mailboxes_.insert(mailbox_monitoring_pubsub_event_name());
        supported = true;         
      }
 
      if (domCat == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        if (actorCat == ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
          _allowed_mailboxes_.insert(mailbox_system_subcontractor_event_name());
          supported = true;
        }
        if (actorCat == ACTOR_CATEGORY_SUBCONTRACTOR) {
          _allowed_mailboxes_.insert(mailbox_system_vireserver_event_name());
          supported = true;
        }  
      }
 
      DT_THROW_IF(!supported,
                  std::logic_error,
                 "Unsupported combinaison of domain/actor categories for listener plug '" << get_name() << "'!");
      return;
    }

    i_event_listener_plug::i_event_listener_plug(const std::string & name_,
                                                 const actor & parent_,
                                                 const domain & domain_,
                                                 const datatools::logger::priority logging_)
      : base_plug(name_, parent_, domain_, logging_)
    {
      _populate_allowed_mailboxes_();
      return;
    }

    i_event_listener_plug::~i_event_listener_plug()
    {
      return;
    }
  
    const std::set<std::string> & i_event_listener_plug::get_allowed_mailboxes() const
    {
      return _allowed_mailboxes_;
    }
  
    plug_category_type i_event_listener_plug::get_category() const
    {
      return PLUG_EVENT_LISTENER;
    }

    void i_event_listener_plug::add_subscription(const subscription_info & subinfo_)
    {
      DT_THROW_IF(_allowed_mailboxes_.count(subinfo_.mailbox_name) == 0,
                  std::logic_error,
                  "Subscription mailbox '" << subinfo_.mailbox_name << "' is not allowed in event listener '" << get_name() << "'!");

      domain_category_type domCat = get_domain().get_category();
      actor_category_type actorCat = get_parent().get_category();

      if (domCat == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        DT_THROW_IF(!subinfo_.addr.is_protocol(),
                    std::logic_error,
                    "Subscription mailbox '" << subinfo_.mailbox_name << "' use a non supported address type in event listener '" << get_name() << "'!");
      }
  
      if (domCat == DOMAIN_CATEGORY_MONITORING) {
        DT_THROW_IF(subinfo_.addr.is_protocol(),
                    std::logic_error,
                    "Subscription mailbox '" << subinfo_.mailbox_name << "' use a non supported protocol address type in event listener '" << get_name() << "'!");
      }
   
      if (domCat == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        DT_THROW_IF(!subinfo_.addr.is_protocol(),
                    std::logic_error,
                    "Subscription mailbox '" << subinfo_.mailbox_name << "' use a non supported address type in event listener '" << get_name() << "'!");
      }
   
      _subscriptions_.push_back(subinfo_);
      _at_add_subscription_(subinfo_);
      return;
    }
      
    const subscription_info_list &
    i_event_listener_plug::get_subscriptions() const
    {
      return _subscriptions_;
    }

    bool i_event_listener_plug::has_private_address() const
    {
      return _private_address_.is_complete();
    }

    const address & i_event_listener_plug::get_private_address() const
    {
      return _private_address_;
    }
      
    void i_event_listener_plug::_set_private_address(const std::string & address_value_)
    {
      _private_address_ = address(ADDR_CATEGORY_PRIVATE, address_value_);
      return;
    }
      
    com_status i_event_listener_plug::receive_next_event(vire::utility::const_payload_ptr_type & event_payload_)
    {
      datatools::logger::priority logging = get_logging();
      com_status status = COM_SUCCESS;

      const i_encoding_driver & encoder = get_domain().get_encoding_driver();

      raw_message_type raw_msg_event;
      status = _at_receive_next_event_(raw_msg_event);
      if (status == COM_SUCCESS) {
        if (datatools::logger::is_debug(logging)) {
          std::cerr << "********** NEXT EVENT RECEIVED ********** " << std::endl;
        }
        // Message response:
        vire::message::message msg_event;
        // Decode the raw buffer:
        encoder.decode(raw_msg_event, msg_event);
        if (! msg_event.get_header().is_event()) {
          status = COM_FAILURE;
        } else {
          // Process returned metadata:
          event_payload_ = msg_event.get_body().get_payload();
        }
      } else {
        if (datatools::logger::is_debug(logging)) {
          std::cerr << "\n**************************************** " << std::endl;
        }
      }
      return status;
    }

  } // namespace com

} // namespace vire
