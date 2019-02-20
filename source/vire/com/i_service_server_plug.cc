//! \file vire/com/i_service_server_plug.cc
//
// Copyright (c) 2019 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2019 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/com/access_hub.h>
#include <vire/com/access_profile.h>
#include <vire/com/domain.h>

namespace vire {

  namespace com {

    i_service_server_plug::i_service_server_plug(const std::string & name_,
                                                 const access_hub & parent_,
                                                 const domain & domain_,
                                                 const datatools::logger::priority logging_)
      : base_plug(name_, parent_, domain_, logging_)
    {
      const domain_category_type domCat = get_domain().get_category();
      const access_category_type  accessCat = get_parent().get_profile().get_category();
      if (domCat == DOMAIN_CATEGORY_GATE) {
        DT_THROW_IF(accessCat != ACCESS_CATEGORY_SERVER_GATE,
                    std::logic_error,
                    "Invalid context for service server plug '" << name_ << "'!");
        _mailbox_name_ = vire::com::mailbox_gate_service_name();
      } else if (domCat == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        DT_THROW_IF(accessCat != ACCESS_CATEGORY_SERVER_CLIENT_SYSTEM,
                    std::logic_error,
                    "Invalid context for service server plug '" << name_ << "'!");
        _mailbox_name_ = vire::com::mailbox_client_system_service_name();
      } else if (domCat == DOMAIN_CATEGORY_CONTROL) {
        DT_THROW_IF(accessCat != ACCESS_CATEGORY_SERVER_CMS,
                    std::logic_error,
                    "Invalid context for service server plug '" << name_ << "'!");
        _mailbox_name_ = vire::com::mailbox_cms_service_name();
      } else if (domCat == DOMAIN_CATEGORY_MONITORING) {
        DT_THROW_IF(accessCat != ACCESS_CATEGORY_SUBCONTRACTOR
                    && accessCat != ACCESS_CATEGORY_SERVER_CMS,
                    std::logic_error,
                    "Invalid context for service server plug '" << name_ << "'!");
        _mailbox_name_ = vire::com::mailbox_cms_service_name();
      } else if (get_domain().get_category() == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        if (accessCat == ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
          _mailbox_name_ = vire::com::mailbox_subcontractor_system_vireserver_service_name();
        } else if (accessCat == ACCESS_CATEGORY_SUBCONTRACTOR) {
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
      DT_THROW_IF(!subinfo_.is_complete(), std::logic_error, "Incomplete subscription!");
      const domain_category_type domCat = get_domain().get_category();
      const access_category_type accessCat = get_parent().get_profile().get_category();

      if (domCat == DOMAIN_CATEGORY_GATE) {
        DT_THROW_IF(!subinfo_.addr.is_protocol(),
                    std::logic_error,
                    "Only protocol subscription address is allowed!");
        DT_THROW_IF(subinfo_.mailbox_name != mailbox_gate_service_name(),
                    std::logic_error,
                    "Subscription mailbox '" << subinfo_.mailbox_name << "' is not allowed in service server '" << get_name() << "'!");
        DT_THROW_IF(accessCat != ACCESS_CATEGORY_SERVER_GATE,
                    std::logic_error,
                    "Mailbox '" << subinfo_.mailbox_name << " cannot be subscribed by access of category '" << to_string(accessCat) << "'!");
      }
 
      if (domCat == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        DT_THROW_IF(!subinfo_.addr.is_protocol(),
                    std::logic_error,
                    "Only protocol subscription address is allowed!");
        
        if (accessCat == ACCESS_CATEGORY_SERVER_CLIENT_SYSTEM) {
          DT_THROW_IF(subinfo_.mailbox_name != mailbox_client_system_service_name(),
                      std::logic_error,
                      "Subscription mailbox '" << subinfo_.mailbox_name << "' is not allowed in service server '" << get_name() << "'!");
        } else if (accessCat == ACCESS_CATEGORY_CLIENT_SYSTEM) {
          DT_THROW_IF(subinfo_.mailbox_name != mailbox_system_vireserver_event_name(),
                      std::logic_error,
                      "Subscription mailbox '" << subinfo_.mailbox_name << "' is not allowed in service server '" << get_name() << "'!");
        }
      }

      if (domCat == DOMAIN_CATEGORY_CONTROL) {
        DT_THROW_IF(!subinfo_.addr.is_device() and !subinfo_.addr.is_resource(),
                    std::logic_error,
                    "Only device and resource subscription address is allowed!");
        DT_THROW_IF(subinfo_.mailbox_name != mailbox_cms_service_name(),
                    std::logic_error,
                    "Subscription mailbox '" << subinfo_.mailbox_name << "' is not allowed in service server '" << get_name() << "'!");
        DT_THROW_IF(accessCat != ACCESS_CATEGORY_SERVER_CMS,
                    std::logic_error,
                    "Mailbox '" << subinfo_.mailbox_name << " cannot be subscribed by access of category '" << to_string(accessCat) << "'!");
      }

      if (domCat == DOMAIN_CATEGORY_MONITORING) {
        DT_THROW_IF(!subinfo_.addr.is_device() and !subinfo_.addr.is_resource(),
                    std::logic_error,
                    "Only device and resource subscription address is allowed!");
        DT_THROW_IF(subinfo_.mailbox_name != mailbox_cms_service_name(),
                    std::logic_error,
                    "Subscription mailbox '" << subinfo_.mailbox_name << "' is not allowed in service server '" << get_name() << "'!"); 
        DT_THROW_IF(accessCat != ACCESS_CATEGORY_SERVER_CMS and accessCat != ACCESS_CATEGORY_SUBCONTRACTOR,
                    std::logic_error,
                    "Mailbox '" << subinfo_.mailbox_name << " cannot be subscribed by access of category '" << to_string(accessCat) << "'!");
      }

      if (domCat == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        if (accessCat == ACCESS_CATEGORY_SUBCONTRACTOR) {
          DT_THROW_IF(subinfo_.mailbox_name != mailbox_subcontractor_system_subcontractor_service_name(),
                      std::logic_error,
                      "Subscription mailbox '" << subinfo_.mailbox_name << "' is not allowed in service server '" << get_name() << "'!");
        } else if (accessCat == ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
          DT_THROW_IF(subinfo_.mailbox_name != mailbox_subcontractor_system_vireserver_service_name(),
                      std::logic_error,
                      "Subscription mailbox '" << subinfo_.mailbox_name << "' is not allowed in service server '" << get_name() << "'!");
        } 
      }
      
      _subscriptions_.push_back(subinfo_);
      _at_add_subscription_(subinfo_);
     
      return;
    }
    
    const subscription_info_list & i_service_server_plug::get_subscriptions() const
    {
      return _subscriptions_;
    }

    com_status i_service_server_plug::receive_next_request(vire::utility::const_payload_ptr_type & request_payload_,
                                                           address & response_address_,
                                                           vire::message::message_identifier & origine_msg_id_,
                                                           address & async_response_address_)
    {
      datatools::logger::priority logging = get_logging();
      com_status status = COM_SUCCESS;
      
      const i_encoding_driver & encoder = get_domain().get_encoding_driver();
      raw_message_type raw_msg_request;
      status = _at_receive_next_request_(raw_msg_request, response_address_);
      if (status == COM_SUCCESS) {
        if (datatools::logger::is_debug(logging)) {
          std::cerr << "********** NEXT REQUEST RECEIVED ********** " << std::endl;
        }
        // Message request:
        vire::message::message msg_request;
        // Decode the raw buffer:
        encoder.decode(raw_msg_request, msg_request);
        if (msg_request.get_header().has_message_id()) {
          origine_msg_id_ = msg_request.get_header().get_message_id();
        }
        if (! msg_request.get_header().is_request()) {
          status = COM_FAILURE;
        } else {
          // Process returned metadata:
          request_payload_ = msg_request.get_body().get_payload();
        }
        if (datatools::logger::is_debug(logging)) {
          std::cerr << "\n**************************************** " << std::endl;
        }
      } else {
      }
      return status;
    }
    
    com_status i_service_server_plug::send_back_response(const address & response_address_,
                                                         const vire::message::message_identifier & in_reply_to_,
                                                         const vire::utility::const_payload_ptr_type & response_payload_)
    {
      datatools::logger::priority logging = get_logging();
      com_status status = COM_SUCCESS;
      status = COM_UNAVAILABLE;
    
      // Message response:
      vire::message::message msg_response;
      
      // Header:
      vire::message::message_header & h = msg_response.grab_header();
      vire::message::message_identifier msg_id(this->get_name(), this->_pop_next_message_id());
      h.set_message_id(msg_id);
      h.set_timestamp(vire::time::now());
      h.set_category(vire::message::MESSAGE_RESPONSE);
      if (in_reply_to_.is_valid()) {
        h.set_in_reply_to(in_reply_to_);
      }
      h.set_asynchronous(false);
      vire::utility::model_identifier body_layout_id;
      body_layout_id.set_name(vire::message::body_layout::name());
      body_layout_id.set_version(vire::message::body_layout::current_version());
      h.set_body_layout_id(body_layout_id);
      
      // Body:
      vire::message::message_body & b = msg_response.grab_body();
      b.set_payload(response_payload_);

      const i_encoding_driver & encoder = get_domain().get_encoding_driver();

      // Raw message record:
      raw_message_type raw_msg_response;
      // Encode the raw buffer:
      encoder.encode(msg_response, raw_msg_response);
      if (datatools::logger::is_debug(logging)) {
        std::cerr << "********** RAW BACK RESPONSE BUFFER ********** " << std::endl;
        for (char byte : raw_msg_response.buffer)  {
          if (std::isprint(byte)) {
            std::cerr << byte;
          } else {
            std::cerr << '?';
          }
        }
        std::cerr << "\n********************************************* " << std::endl;
      }
    
      // Populate raw metadata:
      if (msg_response.get_header().get_message_id().is_valid()) {
        raw_msg_response.metadata.store(vire::com::message_id_key(),
                                        msg_response.get_header().get_message_id().to_string());
      }
      // XXX
      if (msg_response.get_header().has_in_reply_to()) {
        raw_msg_response.metadata.store(vire::com::correlation_id_key(),
                                        in_reply_to_.to_string());
      }
      // if (msg_response.get_header().has_in_reply_to()) {
      //   raw_msg_response.metadata.store(address_id_key(), msg_response.get_header().get_in_reply_to());
      // }
      
      // Send the response through the transport implementation:
      status = _at_send_back_response_(response_address_, raw_msg_response);
     
      return status;
    }
       
  } // namespace com

} // namespace vire
