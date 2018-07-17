//! \file vire/rabbitmq/service_client_plug.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/rabbitmq/service_client_plug.h>

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
#include <vire/com/actor.h>
#include <vire/rabbitmq/utils.h>

namespace vire {

  namespace rabbitmq {

    namespace bxrabbitmq = ::rabbitmq;
      
    struct service_client_plug::pimpl_type
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
   
    service_client_plug::service_client_plug(const std::string & name_,
                                             const vire::com::actor & parent_,
                                             const vire::com::domain & domain_,
                                             const std::string & mailbox_name_,
                                             const datatools::logger::priority logging_)
      : i_service_client_plug(name_,parent_, domain_, mailbox_name_, logging_)
    {
      _construct_();
      return;
    }

    service_client_plug::~service_client_plug()
    {
      _destroy_();
      return;
    }

    void service_client_plug::_construct_()
    {
      _pimpl_.reset(new pimpl_type);
      const datatools::properties & com_aux = get_parent().get_com().get_auxiliaries();
      DT_THROW_IF(!com_aux.has_key("rabbitmq.server_host"),
                  std::logic_error,
                  "No RabbitMQ server host!");
      DT_THROW_IF(!com_aux.has_key("rabbitmq.server_port"),
                  std::logic_error,
                  "No RabbitMQ server port!");
      _pimpl_->conn_params.host = com_aux.fetch_string("rabbitmq.server_host");
      _pimpl_->conn_params.port = com_aux.fetch_positive_integer("rabbitmq.server_port");
      _pimpl_->conn_params.vhost = get_domain().get_name();
      _pimpl_->conn_params.login = get_parent().get_name();
      _pimpl_->conn_params.passwd = get_parent().get_password();
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
      // publisher_confirm = false;
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
      return;
    }

    void service_client_plug::_destroy_()
    {
      _pimpl_.reset();
      return;
    }

    // virtual
    vire::com::com_status service_client_plug::_at_send_receive_(const vire::com::address & address_,
                                                                 const vire::com::raw_message_type & raw_request_,
                                                                 vire::com::raw_message_type & raw_response_,
                                                                 const float timeout_sec_)
    {
      vire::com::com_status status = vire::com::COM_FAILURE;
      bxrabbitmq::basic_properties prop_out;
      std::string consumer_tag = "";
      bool no_ack = true;
      bool exclusive = false;
      std::string routing_key;
      if (!::vire::rabbitmq::convert(address_, routing_key)) {
        return status;
      }
      _pimpl_->channel->basic_consume(_pimpl_->q_par.name, consumer_tag, no_ack, exclusive);
      prop_out.set_reply_to(_pimpl_->q_par.name);
      prop_out.set_user_id(get_parent().get_name());
      prop_out.set_correlation_id("corid_" + std::to_string(std::rand()));
      if (raw_request_.metadata.has_key(vire::com::message_id_key())) {
        prop_out.set_message_id(raw_request_.metadata.fetch_string(vire::com::message_id_key()));
      }
      std::string msgrequest(raw_request_.buffer.data(), raw_request_.buffer.size());
      _pimpl_->channel->basic_publish(get_mailbox_name(), routing_key, msgrequest, prop_out);
      std::string msgresponse;
      while (true) {
        std::string routing_key;
        bxrabbitmq::basic_properties prop_in;
        uint64_t delivery;
        bxrabbitmq::consume_status_type mqstatus
          = _pimpl_->channel->consume_message(msgresponse, routing_key, prop_in, delivery, timeout_sec_);
        raw_response_.metadata.store(vire::com::address_key(), routing_key);
        if (mqstatus == bxrabbitmq::CONSUME_OK) {
          if (not prop_in.has_correlation_id ()) continue;
          if (prop_in.get_correlation_id () == prop_out.get_correlation_id ()) {
            status = vire::com::COM_SUCCESS;
            raw_response_.buffer = vire::com::raw_message_type::buffer_type(msgresponse.begin(),
                                                                            msgresponse.end());
            break;
          }
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
    
  } // namespace rabbitmq

} // namespace vire
