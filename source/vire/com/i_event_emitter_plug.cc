//! \file vire/com/i_event_emitter_plug.cc
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
#include <vire/com/i_event_emitter_plug.h>

// This project:
#include <vire/com/actor.h>
#include <vire/com/domain.h>
#include <vire/time/utils.h>
#include <vire/message/message.h>
#include <vire/message/body_layout.h>
#include <vire/message/message_header.h>
#include <vire/message/message_body.h>

namespace vire {

  namespace com {
    
    void i_event_emitter_plug::_populate_allowed_mailboxes_()
    {
      domain_category_type domCat = get_domain().get_category();
      actor_category_type actorCat = get_parent().get_category();

      bool supported = false;
      if (domCat == DOMAIN_CATEGORY_MONITORING) {
        if (actorCat == ACTOR_CATEGORY_SUBCONTRACTOR
            || actorCat == ACTOR_CATEGORY_SERVER_CMS) {
          _allowed_mailboxes_.insert("log.event");
          _allowed_mailboxes_.insert("alarm.event");
          _allowed_mailboxes_.insert("pubsub.event");
          _default_mailbox_name_ = "log.event";
          supported = true;
        }
      }
 
      if (domCat == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        if (actorCat == ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM) {
          _allowed_mailboxes_.insert("vireserver.event");
          _default_mailbox_name_ = "vireserver.event";
          supported = true;
        }
      }
 
      if (domCat == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        if (actorCat == ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
          _allowed_mailboxes_.insert("vireserver.event");
          _default_mailbox_name_ = "vireserver.event";
          supported = true;
        }
        if (actorCat == ACTOR_CATEGORY_SUBCONTRACTOR) {
          _allowed_mailboxes_.insert("subcontractor.event");
          _default_mailbox_name_ = "subcontractor.event";
          supported = true;
        }
      }
      DT_THROW_IF(!supported,
                  std::logic_error,
                  "Unsupported combinaison of domain/actor categories for emitter plug '" << get_name() << "'!");
      return;
    }

    i_event_emitter_plug::i_event_emitter_plug(const std::string & name_,
                                               const actor & parent_,
                                               const domain & domain_,
                                               const std::string & default_mailbox_name_,
                                               const datatools::logger::priority logging_)
      : base_plug(name_, parent_, domain_, logging_)
    {
      _populate_allowed_mailboxes_();
      if (!default_mailbox_name_.empty()) {
        DT_THROW_IF(_allowed_mailboxes_.count(default_mailbox_name_) == 0,
                    std::logic_error,
                    "Unsupported default mailbox '" << default_mailbox_name_ << "' in emiiter plug '" << name_ << "'!");
        _default_mailbox_name_ = default_mailbox_name_;
      }
      return;
    }

    const std::set<std::string> & i_event_emitter_plug::get_allowed_mailboxes() const
    {
      return _allowed_mailboxes_;
    }
    
    bool i_event_emitter_plug::has_default_mailbox_name() const
    {
      return !_default_mailbox_name_.empty();
    }
 
    const std::string & i_event_emitter_plug::get_default_mailbox_name() const
    {
      return _default_mailbox_name_;
    }

    i_event_emitter_plug::~i_event_emitter_plug()
    {
      return;
    }
   
    plug_category_type i_event_emitter_plug::get_category() const
    {
      return PLUG_EVENT_EMITTER;
    }

    com_status i_event_emitter_plug::send_event(const address & address_,
                                                const vire::utility::const_payload_ptr_type & event_payload_)
    {
      if (!has_default_mailbox_name()) {
        // "No default mailbox name in event emitter plug '" << get_name() << "'!");
        return COM_FAILURE;
      }
      return send_event(get_default_mailbox_name(), address_, event_payload_);
    }

    com_status i_event_emitter_plug::send_event(const std::string & mailbox_name_,
                                                const address & address_,
                                                const vire::utility::const_payload_ptr_type & event_payload_)
    {
      datatools::logger::priority logging = get_logging();
      com_status status = COM_SUCCESS;
      if (mailbox_name_.empty()) {
        // "No target mailbox name is set from emitting plug '" << get_name() << "'!");
        return COM_FAILURE;
      }
      if (_allowed_mailboxes_.count(mailbox_name_) == 0) {
        return COM_PERMISSION;
      }

      if (get_domain().get_category() == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        if (!address_.is_protocol()) {
          return COM_UNAVAILABLE;
        }
      }
      
      if (get_domain().get_category() == DOMAIN_CATEGORY_MONITORING) {
        if (address_.is_protocol()) {
          return COM_UNAVAILABLE;
        }
      }
  
      if (get_domain().get_category() == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        if (!address_.is_protocol()) {
          return COM_UNAVAILABLE;
        }
      }
      
      // Message event:
      vire::message::message msg_event;
      // Header:
      vire::message::message_header & h = msg_event.grab_header();
      vire::message::message_identifier msg_id(this->get_name(), this->_pop_next_message_id());
      h.set_message_id(msg_id);
      h.set_timestamp(vire::time::now());
      h.set_category(vire::message::MESSAGE_EVENT);
      h.set_asynchronous(false);
      vire::utility::model_identifier body_layout_id;
      body_layout_id.set_name(vire::message::body_layout::name());
      body_layout_id.set_version(vire::message::body_layout::current_version());
      h.set_body_layout_id(body_layout_id);
      // Body:
      vire::message::message_body & b = msg_event.grab_body();
      b.set_payload(event_payload_);

      const i_encoding_driver & encoder = get_domain().get_encoding_driver();

      raw_message_type raw_msg_event;
      if (datatools::logger::is_debug(logging)) {
        std::cerr << "********** RAW EVENT BUFFER ********** " << std::endl;
        for (char byte : raw_msg_event.buffer)  {
          if (std::isprint(byte)) {
            std::cerr << byte;
          } else {
            std::cerr << '?';
          }
        }
        std::cerr << "\n************************************ " << std::endl;
      }
    
      // Populate raw metadata:
      if (msg_event.get_header().get_message_id().is_valid()) {
        raw_msg_event.metadata.store(message_id_key(), msg_event.get_header().get_message_id().to_string());
      }
      // Send the event through the transport implementation:
      status = _at_send_event_(mailbox_name_, address_, raw_msg_event);

      return status;
    }

  } // namespace com

} // namespace vire
