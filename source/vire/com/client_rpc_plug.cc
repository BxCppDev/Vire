//! \file vire/com/client_rpc_plug.cc
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/com/client_rpc_plug.h>

// This project:
#include <vire/message/message.h>
#include <vire/message/message_header.h>
#include <vire/message/message_identifier.h>
#include <vire/message/body_layout.h>
#include <vire/message/message_body.h>
#include <vire/time/utils.h>
#include <vire/utility/base_event.h>
#include <vire/com/utils.h>

namespace vire {

  namespace com {

    client_rpc_plug::client_rpc_plug(const manager & mgr_)
      : base_plug(mgr_)
    {
      return;
    }

    client_rpc_plug::~client_rpc_plug()
    {
      return;
    }

    int client_rpc_plug::send_receive(const vire::utility::base_request & request_payload_,
                                      vire::utility::base_response & response_payload_,
                                      const std::string & async_rendez_vous_id_ = "")
    {
      std::lock_guard<std::mutex> lock(_send_mutex_);

      bool async_rpc = false;
      if (!async_rendez_vous_id_.empty()) {
        async_rpc = true;
      }
      // Build the message:
      vire::message::message msg;

      // Build the header:
      vire::message::message_header & msg_header = msg.grab_header();
      vire::message::message_identifier msg_id;
      msg_id.set_emitter(get_full_id());
      msg_id.set_number(get_sent_messages_counter());
      msg_header.set_message_id(msg_id);
      vire::utility::model_identifier body_layout_id(vire::message::body_layout::name(),
                                                     vire::message::body_layout::current_version());
      msg_header.set_timestamp(vire::time::now());
      msg_header.set_asynchronous(async_rpc);
      msg_header.set_body_layout_id(body_layout_id);
      if (async_rpc) {
        msg_header.add_metadata("vire.rpc.response.async-rendez-vous", async_rendez_vous_id_);
      }

      // Build the body:
      vire::message::message_body & msg_body = msg.grab_body();
      msg_body.set_payload(request_payload_);

      _increment_sent_messages_counter();
      return 0;
    }

  } // namespace com

} // namespace vire
