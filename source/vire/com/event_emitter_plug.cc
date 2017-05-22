//! \file vire/com/event_emitter_plug.cc
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
#include <vire/com/event_emitter_plug.h>

// Standard library:
#include <string>
#include <map>
#include <vector>

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

    event_emitter_plug::event_emitter_plug(const manager & mgr_)
      : base_plug(mgr_)
    {
      return;
    }

    event_emitter_plug::~event_emitter_plug()
    {
      return;
    }

    int event_emitter_plug::send_event(const vire::utility::base_event & event_payload_)
    {
      std::lock_guard<std::mutex> lock(_send_event_mutex_);
      // if (!has_protocol_driver()) {
      //   return SEND_EVENT_NO_PROTOCOL;
      // }

      // Build the message:
      vire::message::message msg;

      // Build the header:
      vire::message::message_header & msg_header = msg.grab_header();
      vire::message::message_identifier msg_id;
      msg_id.set_emitter(get_full_key());
      msg_id.set_number(get_sent_messages_counter());
      msg_header.set_message_id(msg_id);
      vire::utility::model_identifier body_layout_id(vire::message::body_layout::name(),
                                                     vire::message::body_layout::current_version());
      msg_header.set_timestamp(vire::time::now());
      msg_header.set_asynchronous(true);
      msg_header.set_body_layout_id(body_layout_id);
      msg_header.add_metadata("signature", "dummy");

      // Build the body:
      vire::message::message_body & msg_body = msg.grab_body();
      msg_body.set_payload(event_payload_);

      // XXX
      // grab_protocol_driver().send(msg);

      _increment_sent_messages_counter();
      return SEND_EVENT_OK;
    }

    /*
    int event_emitter_plug::send_event(vire::utility::const_payload_ptr_type event_payload_)
    {
      std::lock_guard<std::mutex> lock(_send_event_mutex_);
      if (!event_payload_->is_event()) {
        return SEND_EVENT_INVALID_CATEGORY;
      }
      if (!has_protocol_driver()) {
        return SEND_EVENT_NO_PROTOCOL;
      }

      // Build the message:
      vire::message::message msg;
      msg_metadata_type msg_metadata;

      // Build the header:
      vire::message::message_header & msg_header = msg.grab_header();
      vire::message::message_identifier msg_id;
      msg_id.set_emitter(get_full_id());
      msg_id.set_number(get_sent_messages_counter());
      msg_header.set_message_id(msg_id);
      vire::utility::model_identifier format_id(vire::message::basic_format::name(),
                                                vire::message::basic_format::current_version());
      msg_header.set_format_id(format_id);
      msg_header.set_timestamp(vire::time::now());
      // Build the body:
      vire::message::message_body & msg_body = msg.grab_body();
      msg_body.set_payload(event_payload_);
      vire::utility::model_identifier payload_type_id(event_payload_->get_serial_tag());
      msg_body.set_payload_type_id(payload_type_id);

      // XXX
      // grab_protocol_driver().send(msg_data, msg_metadata);

      _increment_sent_messages_counter();
      return SEND_EVENT_OK;
    }
    */

  } // namespace com

} // namespace vire
