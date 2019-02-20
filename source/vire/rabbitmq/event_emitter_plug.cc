//! \file vire/rabbitmq/event_emitter_plug.cc
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
#include <vire/rabbitmq/event_emitter_plug.h>

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
      
    struct event_emitter_plug::pimpl_type
    {
      pimpl_type()
      {
        return;
      }
      
      bxrabbitmq::connection_parameters       conn_params;
      std::unique_ptr<bxrabbitmq::connection> conn;
      bxrabbitmq::channel *                   channel = nullptr;
    };   
    
    event_emitter_plug::event_emitter_plug(const std::string & name_,
                                           const vire::com::access_hub & parent_,
                                           const vire::com::domain & domain_,
                                           const std::string & mailbox_name_,
                                           const datatools::logger::priority logging_)
      : i_event_emitter_plug(name_,parent_, domain_, mailbox_name_, logging_)
    {
      _construct_();
      return;
    }

    event_emitter_plug::~event_emitter_plug()
    {
      _destroy_();
      return;
    }

    void event_emitter_plug::_construct_()
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
      bool publisher_confirm = false;
      DT_LOG_DEBUG(logging, "Creating RabbitMQ sonnection...");
      try {
        _pimpl_->conn.reset(new bxrabbitmq::connection(_pimpl_->conn_params,
                                                       publisher_confirm));
      } catch (std::exception & x) {
        DT_THROW(std::logic_error,
                 "Could not create a connection to the RabbitMQ server at '" << _pimpl_->conn_params.host << ":"
                 << _pimpl_->conn_params.port << "' entering vhost '" << _pimpl_->conn_params.vhost << "'"
                 << " with user '" << _pimpl_->conn_params.login << "@" << _pimpl_->conn_params.passwd << "' : " 
                 << x.what());
      }
      DT_THROW_IF(!_pimpl_->conn->is_ok(),
                  std::logic_error,
                  "Failed to connect to the RabbitMQ server!");
      _pimpl_->channel = &_pimpl_->conn->grab_channel();
      return;
    }

    void event_emitter_plug::_destroy_()
    {
      _pimpl_.reset();
      return;
    }
    
    vire::com::com_status event_emitter_plug::_at_send_event_(const std::string & exchange_name_,
                                                              const vire::com::address & address_,
                                                              const vire::com::raw_message_type & raw_event_)
    {
      vire::com::com_status status = vire::com::COM_FAILURE;
      std::string routing_key;
      if (!::vire::rabbitmq::convert(address_, routing_key)) {
        return status;
      }
      bxrabbitmq::basic_properties prop_out;
      prop_out.set_user_id(get_parent().get_name());
      prop_out.set_correlation_id("corid_" + std::to_string(std::rand()));
      if (raw_event_.metadata.has_key(vire::com::message_id_key())) {
        prop_out.set_message_id(raw_event_.metadata.fetch_string(vire::com::message_id_key()));
      }
      std::string msgevent(raw_event_.buffer.data(), raw_event_.buffer.size());
      _pimpl_->channel->basic_publish(exchange_name_, routing_key, msgevent, prop_out);

      return status;
    }
   
  } // namespace com

} // namespace vire
