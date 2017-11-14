//! \file  vire/com/event_listener_plug.h
//! \brief Vire com event listener plug
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

#ifndef VIRE_COM_EVENT_LISTENER_PLUG_H
#define VIRE_COM_EVENT_LISTENER_PLUG_H

// Standard library:
#include <cstdint>
#include <mutex>
#include <memory>

// This project:
#include <vire/com/base_plug.h>
#include <vire/utility/base_payload.h>

namespace vire {

  namespace com {

    //! \brief User credentials data
    class event_listener_plug
      : public base_plug
    {
    public:

      enum received_event_error_type {
        RECEIVED_EVENT_SUCCESS   = 0, ///< Success
        RECEIVED_EVENT_FAILURE   = 1, ///< Generic error
        RECEIVED_EVENT_NO_EVENT  = 2, ///< No event is available
        RECEIVED_EVENT_TIMEOUT   = 3  ///< Timeout
      };

      //! Default constructor
      event_listener_plug(domain & dom_,
                          const std::string & name_);

      //! Destructor
      virtual ~event_listener_plug();

      // //! Check if a new event payload object is available
      // bool has_event() const;

      //! Collect/pop the next available event payload object
      int collect_event(vire::utility::payload_ptr_type & event_payload_,
                        std::string & mailbox_name_,
                        std::string & topic_);

      //! Return the number of received events
      std::size_t get_received_events_counter() const;

    private:

      void _increment_received_events_counter_();

    private:

      // Working:
      std::size_t _received_events_counter_= 0;
      std::mutex _received_event_mutex_;

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_EVENT_LISTENER_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
