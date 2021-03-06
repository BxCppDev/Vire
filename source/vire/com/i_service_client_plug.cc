//! \file vire/com/i_service_client_plug.cc
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
#include <vire/com/i_service_client_plug.h>

// Third party:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/clhep_units.h>

// This project:
#include <vire/message/message.h>
#include <vire/message/body_layout.h>
#include <vire/message/message_header.h>
#include <vire/message/message_body.h>
#include <vire/time/utils.h>
#include <vire/com/access_hub.h>
#include <vire/com/access_profile.h>
#include <vire/com/domain.h>
#include <vire/com/utils.h>

namespace vire {

  namespace com {

    i_service_client_plug::i_service_client_plug(const std::string & name_,
                                                 const access_hub & parent_,
                                                 const domain & domain_,
                                                 const datatools::logger::priority logging_)
      : base_plug(name_, parent_, domain_, logging_)
    {
      domain_category_type domCat = get_domain().get_category();
      access_category_type accessCat = get_parent().get_profile().get_category();
   
      if (domCat == DOMAIN_CATEGORY_GATE) {
        DT_THROW_IF(accessCat != ACCESS_CATEGORY_CLIENT_GATE,
                    std::logic_error,
                    "Invalid context for service client plug '" << name_ << "'!");
        _mailbox_name_ = vire::com::mailbox_gate_service_name();
      } else if (domCat == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        DT_THROW_IF(accessCat != ACCESS_CATEGORY_CLIENT_SYSTEM,
                    std::logic_error,
                    "Invalid context for service client plug '" << name_ << "'!");
        _mailbox_name_ = vire::com::mailbox_client_system_service_name();
      } else if (domCat == DOMAIN_CATEGORY_MONITORING
                 || domCat == DOMAIN_CATEGORY_CONTROL) {
        DT_THROW_IF(accessCat != ACCESS_CATEGORY_CLIENT_CMS
                    && accessCat != ACCESS_CATEGORY_SERVER_CMS,
                    std::logic_error,
                    "Invalid context for service client plug '" << name_ << "'!");
        _mailbox_name_ = vire::com::mailbox_cms_service_name();
      } else if (domCat == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        if (accessCat == ACCESS_CATEGORY_SUBCONTRACTOR) {
          _mailbox_name_ = vire::com::mailbox_subcontractor_system_vireserver_service_name();
        } else if (accessCat == ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
          _mailbox_name_ = vire::com::mailbox_subcontractor_system_subcontractor_service_name();
        } else {
          DT_THROW(std::logic_error,
                   "Invalid context for service client plug '" << name_ << "'!");
        }
      }    
      DT_THROW_IF(_mailbox_name_.empty(),
                  std::logic_error,
                  "Missing mailbox name in service client plug '" << name_ << "'!");
      return;
    }

    i_service_client_plug::~i_service_client_plug()
    {
      return;
    }
    
    plug_category_type i_service_client_plug::get_category() const
    {
      return PLUG_SERVICE_CLIENT;
    }

    const std::string & i_service_client_plug::get_mailbox_name() const
    {
      return _mailbox_name_;
    }
    
    com_status
    i_service_client_plug::send_receive_async(const address & async_address_,
                                              const address & address_,
                                              const vire::utility::const_payload_ptr_type & request_payload_,
                                              vire::utility::const_payload_ptr_type & response_payload_,
                                              const double timeout_)
    {
      com_status status = _send_receive_(async_address_, address_, request_payload_, response_payload_, timeout_);
      return status;
    }

    com_status
    i_service_client_plug::send_receive(const address & address_,
                                        const vire::utility::const_payload_ptr_type & request_payload_,
                                        vire::utility::const_payload_ptr_type & response_payload_,
                                        const double timeout_)
    {
      address empty;
      com_status status = _send_receive_(empty, address_, request_payload_, response_payload_, timeout_);
      return status;
    }

    com_status
    i_service_client_plug::_send_receive_(const address & async_address_,
                                          const address & address_,
                                          const vire::utility::const_payload_ptr_type & request_payload_,
                                          vire::utility::const_payload_ptr_type & response_payload_,
                                          const double timeout_)
    {
      datatools::logger::priority logging = datatools::logger::PRIO_FATAL;
      logging = datatools::logger::PRIO_DEBUG; // Hack debug
      domain_category_type domCat = get_domain().get_category();
      access_category_type accessCat = get_parent().get_profile().get_category();
      com_status status = COM_SUCCESS;
      bool async = false;
      if (async_address_.is_complete()) {
        DT_THROW_IF(!async_address_.is_private(),
                    std::logic_error,
                    "Unsupported non private async address category!");
        async = true;
      }
       
      if (domCat == DOMAIN_CATEGORY_GATE
          || domCat == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        if (!address_.is_protocol()) {
          return COM_UNAVAILABLE;
        }
      }
 
      if (domCat == DOMAIN_CATEGORY_CONTROL
          || domCat == DOMAIN_CATEGORY_MONITORING) {
        if (!address_.is_resource()) {
          return COM_UNAVAILABLE;
        }
      }
  
      if (domCat == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        if (address_.is_device()) {
          return COM_UNAVAILABLE;
        }
        if (accessCat == ACCESS_CATEGORY_SUBCONTRACTOR
            && address_.is_resource()) {
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
      h.set_asynchronous(async);
      if (async) {
        h.set_async_address(async_address_.get_value());
      }
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
        raw_msg_request.metadata.store(message_id_key(),
                                       msg_request.get_header().get_message_id().to_string());
      }
      if (async) {
        ///// RIEN A FAIRE: raw_msg_request.metadata.store(async_address_key(), async_address_.value());
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
        if (datatools::logger::is_debug(logging)) {
          std::cerr << "\n**************************************** " << std::endl;
        }
      } 
      return status;
    }

  } // namespace com

} // namespace vire
