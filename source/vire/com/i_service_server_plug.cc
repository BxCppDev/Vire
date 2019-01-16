//! \file vire/com/i_service_server_plug.cc
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
#include <vire/com/i_service_server_plug.h>

// Third party:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/clhep_units.h>

// This project:
#include <vire/message/message.h>
#include <vire/message/body_layout.h>
#include <vire/message/message_header.h>
#include <vire/message/message_body.h>
#include <vire/time/utils.h>
#include <vire/com/actor.h>
#include <vire/com/domain.h>

namespace vire {

  namespace com {

    i_service_server_plug::i_service_server_plug(const std::string & name_,
                                                 const actor & parent_,
                                                 const domain & domain_,
                                                 const datatools::logger::priority logging_)
      : base_plug(name_, parent_, domain_, logging_)
    {
      if (get_domain().get_category() == DOMAIN_CATEGORY_GATE) {
        DT_THROW_IF(parent_.get_category() != ACTOR_CATEGORY_SERVER_GATE,
                    std::logic_error,
                    "Invalid context for service server plug '" << name_ << "'!");
        _mailbox_name_ = vire::com::mailbox_gate_service_name();
      } else if (get_domain().get_category() == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        DT_THROW_IF(parent_.get_category() != ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM,
                    std::logic_error,
                    "Invalid context for service server plug '" << name_ << "'!");
        _mailbox_name_ = vire::com::mailbox_client_system_service_name();
      } else if (get_domain().get_category() == DOMAIN_CATEGORY_CONTROL) {
        DT_THROW_IF(parent_.get_category() != ACTOR_CATEGORY_SERVER_CMS,
                    std::logic_error,
                    "Invalid context for service server plug '" << name_ << "'!");
        _mailbox_name_ = vire::com::mailbox_cms_service_name();
      } else if (get_domain().get_category() == DOMAIN_CATEGORY_MONITORING) {
        DT_THROW_IF(parent_.get_category() != ACTOR_CATEGORY_SUBCONTRACTOR
                    && parent_.get_category() != ACTOR_CATEGORY_SERVER_CMS,
                    std::logic_error,
                    "Invalid context for service server plug '" << name_ << "'!");
        _mailbox_name_ = vire::com::mailbox_cms_service_name();
      } else if (get_domain().get_category() == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        if (parent_.get_category() == ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
          _mailbox_name_ = vire::com::mailbox_subcontractor_system_vireserver_service_name();
        } else if (parent_.get_category() == ACTOR_CATEGORY_SUBCONTRACTOR) {
          _mailbox_name_ = vire::com::mailbox_subcontractor_system_subcontractor_service_name();
        } else {
          DT_THROW(std::logic_error,
                   "Invalid context for service server plug '" << name_ << "'!");
        }
      }
      DT_THROW_IF(_mailbox_name_.empty(),
                  std::logic_error,
                  "Missing mailbox name in service server plug '" << name_ << "'!");
      return;
    }

    i_service_server_plug::~i_service_server_plug()
    {
      return;
    }
    
    plug_category_type i_service_server_plug::get_category() const
    {
      return PLUG_SERVICE_SERVER;
    }

    const std::string & i_service_server_plug::get_mailbox_name() const
    {
      return _mailbox_name_;
    }

    void i_service_server_plug::add_subscription(const subscription_info & subinfo_)
    {
      // DT_THROW_IF(_allowed_mailboxes_.count(subinfo_.mailbox_name) == 0,
      //             std::logic_error,
      //             "Subscription mailbox '" << subinfo_.mailbox_name << "' is not allowed in service server '" << get_name() << "'!");

      domain_category_type domCat = get_domain().get_category();
      actor_category_type actorCat = get_parent().get_category();

      if (domCat == DOMAIN_CATEGORY_GATE) {
        
      }
 
      // XXX
      // _subscriptions_.push_back(subinfo_);
      //_at_add_subscription_(subinfo_);
     
      return;
    }
    
    const subscription_info_list & i_service_server_plug::get_subscriptions() const
    {
      return _subscriptions_;
    }

    // com_status
    // i_service_server_plug::receive_send(const address & address_,
    //                                     const vire::utility::const_payload_ptr_type & request_payload_,
    //                                     vire::utility::const_payload_ptr_type & response_payload_)
    // {
    //   return _receive_send_(address_, request_payload_, response_payload_);
    // }

    /*
    com_status
    i_service_server_plug::_receive_send_(const address & address_,
                                          const vire::utility::const_payload_ptr_type & request_payload_,
                                          vire::utility::const_payload_ptr_type & response_payload_)
    {
      datatools::logger::priority logging = get_logging();
      com_status status = COM_SUCCESS;
      status = COM_UNAVAILABLE;

      const i_encoding_driver & encoder = get_domain().get_encoding_driver();
      raw_message_type raw_msg_event;
      status = _at_receive_request_(raw_msg_event);
      if (status == COM_SUCCESS) {
        // Message response:
        vire::message::message msg_event;
        // Decode the raw buffer:
        encoder.decode(raw_msg_event, msg_event);
        if (! msg_event.get_header().is_request()) {
          status = COM_FAILURE;
        } else {
          // Process returned metadata:
          request_payload_ = msg_event.get_body().get_payload();
        }
      }


      
      return status;
    }
    */
    
    /*
    com_status
    i_service_server_plug::receive_send(const address & address_,
                                        const vire::utility::const_payload_ptr_type & request_payload_,
                                        vire::utility::const_payload_ptr_type & response_payload_)
    {
      datatools::logger::priority logging = datatools::logger::PRIO_FATAL;
      logging = datatools::logger::PRIO_DEBUG; // Hack debug
      com_status status = COM_SUCCESS;

      if (get_domain().get_category() == DOMAIN_CATEGORY_GATE
          || get_domain().get_category() == DOMAIN_CATEGORY_SERVER_SYSTEM) {
        if (!address_.is_protocol()) {
          return COM_UNAVAILABLE;
        }
      }
 
      if (get_domain().get_category() == DOMAIN_CATEGORY_CONTROL
          || get_domain().get_category() == DOMAIN_CATEGORY_MONITORING) {
        if (!address_.is_resource()) {
          return COM_UNAVAILABLE;
        }
      }
  
      if (get_domain().get_category() == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        if (address_.is_device()) {
          return COM_UNAVAILABLE;
        }
      }
       
      response_payload_.reset();
     
      // Message request:
      vire::message::message msg_request;
      // Header:
      vire::message::message_header & h = msg_request.grab_header();
      vire::message::message_identifier msg_id(this->get_name(), this->_pop_next_message_id());
      h.set_message_id(msg_id);
      h.set_timestamp(vire::time::now());
      h.set_category(vire::message::MESSAGE_REQUEST);
      h.set_asynchronous(false);
      vire::utility::model_identifier body_layout_id;
      body_layout_id.set_name(vire::message::body_layout::name());
      body_layout_id.set_version(vire::message::body_layout::current_version());
      h.set_body_layout_id(body_layout_id);
      // Body:
      vire::message::message_body & b = msg_request.grab_body();
      b.set_payload(request_payload_);

      const i_encoding_driver & encoder = get_domain().get_encoding_driver();

      float timeout_sec = -1.0F; 
      if (datatools::is_valid(timeout_) && timeout_ > 0.0) {
        timeout_sec = float(timeout_ / CLHEP::second);
      }
      
      raw_message_type raw_msg_request;
      // Encode the raw buffer:
      encoder.encode(msg_request, raw_msg_request);

      if (datatools::logger::is_debug(logging)) {
        std::cerr << "********** RAW REQUEST BUFFER ********** " << std::endl;
        for (char byte : raw_msg_request.buffer)  {
          if (std::isprint(byte)) {
            std::cerr << byte;
          } else {
            std::cerr << '?';
          }
        }
        std::cerr << "\n************************************** " << std::endl;
       }
      
      // Populate raw metadata:
      if (msg_request.get_header().get_message_id().is_valid()) {
        raw_msg_request.metadata.store(message_id_key(), msg_request.get_header().get_message_id().to_string());
      }

      raw_message_type raw_msg_response;
      status = _at_send_receive_(address_, raw_msg_request, raw_msg_response, timeout_sec);
      if (status == COM_SUCCESS) {
        if (datatools::logger::is_debug(logging)) {
          std::cerr << "********** RPC RESPONSE SUCCESS ********** " << std::endl;
        }
        // Message response:
        vire::message::message msg_response;
        // Decode the raw buffer:
        encoder.decode(raw_msg_response, msg_response);
        // Process returned metadata:
        response_payload_ = msg_response.get_body().get_payload();
      } else {
        if (datatools::logger::is_debug(logging)) {
          std::cerr << "\n**************************************** " << std::endl;
        }
      }
      return status;
    }
    */
    
  } // namespace com

} // namespace vire
