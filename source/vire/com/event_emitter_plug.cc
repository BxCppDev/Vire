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

// This project:
#include <vire/message/message.h>
#include <vire/message/message_header.h>
#include <vire/message/message_identifier.h>
#include <vire/message/body_layout.h>
#include <vire/message/message_body.h>
#include <vire/time/utils.h>
#include <vire/com/utils.h>

namespace vire {

  namespace com {

    event_emitter_plug::event_emitter_plug(domain & dom_,
                                           const std::string & name_)
      : base_plug(dom_, name_, PLUG_EVENT_EMITTER)
    {
      return;
    }

    event_emitter_plug::~event_emitter_plug()
    {
      return;
    }

    void event_emitter_plug::_increment_sent_events_counter_()
    {
      _sent_events_counter_++;
      return;
    }

    std::size_t event_emitter_plug::get_sent_events_counter() const
    {
      return _sent_events_counter_;
    }

    int event_emitter_plug::send_event(const vire::utility::payload_ptr_type & event_payload_,
                                       const std::string & mailbox_name_,
                                       const std::string & topic_)
    {
      std::lock_guard<std::mutex> lock(_send_event_mutex_);

      // Build the message:
      vire::message::message msg;

      // Build the header:
      vire::message::message_header & msg_header = msg.grab_header();

      vire::message::message_identifier msg_id;
      msg_id.set_emitter(get_name());
      msg_id.set_number(get_sent_events_counter());
      msg_header.set_message_id(msg_id);

      vire::utility::model_identifier body_layout_id(vire::message::body_layout::name(),
                                                     vire::message::body_layout::current_version());
      msg_header.set_body_layout_id(body_layout_id);

      msg_header.set_timestamp(vire::time::now());
      msg_header.set_category(vire::message::MESSAGE_EVENT);
      //msg_header.set_asynchronous(true);
      //msg_header.add_metadata("signature", "dummy");

      // Build the body:
      vire::message::message_body & msg_body = msg.grab_body();
      msg_body.set_payload(*event_payload_.get());

      // XXX
      // grab_protocol_driver().send(msg);

      _increment_sent_events_counter_();
      return SEND_EVENT_SUCCESS;
    }

  } // namespace com

} // namespace vire
