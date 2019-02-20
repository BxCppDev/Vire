/// \file vire/rabbitmq/service_server_plug.cc
//
// Copyright (c) 2019 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2019 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//
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
#include <vire/rabbitmq/service_server_plug.h>

// Standard library:
#include <cstdlib>

// Third Party:
// - BxRabbitMQ
#include <bayeux/rabbitmq/basic_properties.h>
#include <bayeux/rabbitmq/parameters.h>
#include <bayeux/rabbitmq/connection.h>
#include <bayeux/rabbitmq/channel.h>

// This project:
#include <vire/message/message.h>
#include <vire/com/manager.h>
#include <vire/com/domain.h>
#include <vire/com/access_hub.h>
#include <vire/com/access_profile.h>
#include <vire/rabbitmq/utils.h>

namespace vire {

  namespace rabbitmq {

    namespace bxrabbitmq = ::rabbitmq;
      
    struct service_server_plug::pimpl_type
    {
      pimpl_type()
      {
        return;
      }
      
      bxrabbitmq::connection_parameters       conn_params;
      std::unique_ptr<bxrabbitmq::connection> conn;
      bxrabbitmq::channel *                   channel = nullptr;
      bxrabbitmq::queue_parameters            q_par;
    };   
   
    service_server_plug::service_server_plug(const std::string & name_,
                                             const vire::com::access_hub & parent_,
                                             const vire::com::domain & domain_,
                                             const datatools::logger::priority logging_)
      : i_service_server_plug(name_,parent_, domain_, logging_)
    {
      _construct_();
      return;
    }

    service_server_plug::~service_server_plug()
    {
      _destroy_();
      return;
    }

    void service_server_plug::_construct_()
    {
      _pimpl_.reset(new pimpl_type);
      const datatools::properties & com_aux = get_parent().get_profile().get_com().get_auxiliaries();
      DT_THROW_IF(!com_aux.has_key(server_host_key()),
                  std::logic_error,
                  "No RabbitMQ server host!");
      DT_THROW_IF(!com_aux.has_key(server_port_key()),
                  std::logic_error,
                  "No RabbitMQ server port!");
      _pimpl_->conn_params.host = com_aux.fetch_string(server_host_key());
      _pimpl_->conn_params.port = com_aux.fetch_positive_integer(server_port_key());
      _pimpl_->conn_params.vhost = get_domain().get_name();
      _pimpl_->conn_params.login = get_parent().get_profile().get_name();
      _pimpl_->conn_params.passwd = get_parent().get_profile().get_password();
      datatools::logger::priority logging = datatools::logger::PRIO_DEBUG;
      if (datatools::logger::is_debug(logging)) {
        DT_LOG_DEBUG(logging, "RabbitMQ server parameters: ");
        DT_LOG_DEBUG(logging, "  - host   = " << _pimpl_->conn_params.host);
        DT_LOG_DEBUG(logging, "  - port   = " << _pimpl_->conn_params.port);
        DT_LOG_DEBUG(logging, "  - vhost  = " << _pimpl_->conn_params.vhost);
        DT_LOG_DEBUG(logging, "  - login  = " << _pimpl_->conn_params.login);
        DT_LOG_DEBUG(logging, "  - passwd = " << _pimpl_->conn_params.passwd);
      }
      bool publisher_confirm = true;
      publisher_confirm = false;
      _pimpl_->conn.reset(new bxrabbitmq::connection(_pimpl_->conn_params, publisher_confirm));
      DT_THROW_IF(!_pimpl_->conn->is_ok(),
                  std::logic_error,
                  "Failed to connect to RabbitMQ server!");
      _pimpl_->channel = &_pimpl_->conn->grab_channel();
      _pimpl_->q_par.name = "";
      _pimpl_->q_par.exclusive = true;
      _pimpl_->channel->queue_declare(_pimpl_->q_par);
      if (datatools::logger::is_debug(logging)) {
        DT_LOG_DEBUG(logging, "  - queue = " <<_pimpl_->q_par.name);
      }
      ///_set_private_address(_pimpl_->q_par.name);
      return;
    }

    void service_server_plug::_destroy_()
    {
      _pimpl_.reset();
      return;
    }

    void service_server_plug::_at_add_subscription_(const vire::com::subscription_info & subinfo_)
    {
      const std::string & mailbox_name = subinfo_.mailbox_name;
      const vire::com::address & addr = subinfo_.addr;
      bxrabbitmq::exchange_parameters x_par;
      x_par.name = mailbox_name; // Exchange name
      x_par.type = "topic";
      std::string topic;
      if (!convert(addr, topic)) {
        DT_THROW(std::logic_error, "Invalid address -> binding key conversion from '" << addr
                 << "' in service server plug '" << get_name() << "'!");
      }
      _pimpl_->channel->queue_bind(_pimpl_->q_par.name, x_par.name, topic);
      if (addr.is_device()) {
        // Also bind all resources associated to this device:
        topic += ".#";
        _pimpl_->channel->queue_bind(_pimpl_->q_par.name, x_par.name, topic);
      }
      return;
    }
    
    vire::com::com_status
    service_server_plug::_at_receive_next_request_(vire::com::raw_message_type & raw_request_,
                                                   vire::com::address & response_address_)
    {
      vire::com::com_status status = vire::com::COM_FAILURE;
      std::string consumer_tag = "";
      bool no_ack = true;
      bool exclusive = false;
      _pimpl_->channel->basic_consume(_pimpl_->q_par.name, consumer_tag, no_ack, exclusive);
      while (true) {
        std::string msgevent;
        std::string routing_key;
        bxrabbitmq::basic_properties prop_in;
        uint64_t delivery_tag;
        float timeout_sec = -1.0F;
        bxrabbitmq::consume_status_type mqstatus
          = _pimpl_->channel->consume_message(msgevent, routing_key, prop_in, delivery_tag, timeout_sec);
        if (mqstatus == bxrabbitmq::CONSUME_OK) {
          if (not prop_in.has_correlation_id() or not prop_in.has_reply_to()) {
            _pimpl_->channel->basic_ack(delivery_tag);
            // Or: XXX
            // bool requeue = false; // requeue the delivery ?
            // _pimpl_->channel->basic_reject(delivery_tag, requeue);
            continue;
          }
          // Extract the response address (private queue name):
          response_address_.set(vire::com::ADDR_CATEGORY_PRIVATE, prop_in.get_reply_to());
          // Populate metadata:
          raw_request_.metadata.store(vire::com::private_mailbox_key(), response_address_.get_value());
          raw_request_.metadata.store(vire::com::address_key(),         routing_key);
          raw_request_.metadata.store(vire::com::correlation_id_key(),  prop_in.get_correlation_id());
          if (prop_in.has_message_id()) {
            raw_request_.metadata.store(vire::com::message_id_key(),    prop_in.get_message_id());
          }
          // Populate buffer:
          raw_request_.buffer = vire::com::raw_message_type::buffer_type(msgevent.begin(),
                                                                         msgevent.end());
          status = vire::com::COM_SUCCESS;
          break;
        } else {
          if (mqstatus == bxrabbitmq::CONSUME_TIMEOUT) {
            status = vire::com::COM_TIMEOUT;
          } else {
            status = vire::com::COM_FAILURE;
          }
          break;
        }
      }
      
      return status;
    }
      
    vire::com::com_status
    service_server_plug::_at_send_back_response_(const vire::com::address & response_address_,
                                                 const vire::com::raw_message_type & raw_response_) 
    {
      vire::com::com_status status = vire::com::COM_FAILURE;
      std::string routing_key;
      if (!::vire::rabbitmq::convert(response_address_, routing_key)) {
        return status;
      }
      bxrabbitmq::basic_properties prop_out;
      prop_out.set_user_id(get_parent().get_name());
      if (raw_response_.metadata.has_key(vire::com::correlation_id_key())) {
        prop_out.set_correlation_id(raw_response_.metadata.fetch_string(vire::com::correlation_id_key()));
      } else {
        /// XXX 
        /// prop_out.set_correlation_id("corid_" + std::to_string(std::rand()));
      }
      if (raw_response_.metadata.has_key(vire::com::message_id_key())) {
        prop_out.set_message_id(raw_response_.metadata.fetch_string(vire::com::message_id_key()));
      }
      std::string exchange_name;
      if (raw_response_.metadata.has_key(vire::com::private_mailbox_key())) {
        exchange_name = raw_response_.metadata.fetch_string(vire::com::private_mailbox_key());
      }
      std::string msgevent(raw_response_.buffer.data(), raw_response_.buffer.size());
      _pimpl_->channel->basic_publish(exchange_name, routing_key, msgevent, prop_out);
      
      return status;
    }
   
  } // namespace rabbitmq

} // namespace vire
