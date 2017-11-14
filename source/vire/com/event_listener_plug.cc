//! \file vire/com/event_listener_plug.cc
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
#include <vire/com/event_listener_plug.h>

namespace vire {

  namespace com {

    event_listener_plug::event_listener_plug(domain & dom_,
                                             const std::string & name_)
      : base_plug(dom_, name_, PLUG_EVENT_EMITTER)
    {
      return;
    }

    event_listener_plug::~event_listener_plug()
    {
      return;
    }

    std::size_t event_listener_plug::get_received_events_counter() const
    {
      return _received_events_counter_;
    }

    int event_listener_plug::collect_event(vire::utility::payload_ptr_type & event_payload_,
                                           std::string & mailbox_name_,
                                           std::string & topic_)
    {
      std::lock_guard<std::mutex> lock(_received_event_mutex_);
      event_payload_.reset();
      mailbox_name_.clear();
      topic_.clear();


      return RECEIVED_EVENT_SUCCESS;
    }

    // bool event_listener_plug::has_event() const
    // {
    //   return true;
    // }

  } // namespace com

} // namespace vire
