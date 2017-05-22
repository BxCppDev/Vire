//! \file  vire/com/event_emitter_plug.h
//! \brief Vire com event emitter plug
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

#ifndef VIRE_COM_EVENT_EMITTER_PLUG_H
#define VIRE_COM_EVENT_EMITTER_PLUG_H

// Standard library:
#include <cstdint>
#include <mutex>
#include <memory>

// This project:
#include <vire/com/base_plug.h>
#include <vire/utility/base_payload.h>

namespace vire {

  namespace utility {

    class base_event;

  }

  namespace com {

    //! \brief User credentials data
    class event_emitter_plug : public vire::com::base_plug
    {
    public:

      enum send_event_error_type {
        SEND_EVENT_OK               = 0, ///< Payload is ok
        SEND_EVENT_INVALID_CATEGORY = 1, ///< Payload is not an event
        SEND_EVENT_NO_PROTOCOL      = 2, ///< Undefined protocol
        SEND_EVENT_NO_DESTINATION   = 3, ///< Undefined destination
        SEND_EVENT_TIMEOUT          = 4  ///< Timeout
      };

      //! Default constructor
      event_emitter_plug(const manager & mgr_);

      //! Destructor
      virtual ~event_emitter_plug();

      //! Check if relay is set
      bool has_relay() const;

      //! Return the relay
      const std::string & get_relay() const;

      //! Set the relay
      void set_relay(const std::string &);

      //! Send an event (hardcoded relay)
      int send_event(const vire::utility::base_event & event_payload_);

      //! Send an event through a relay (exchange)
      int send_event_through_relay(const vire::utility::base_event & event_payload_,
                                   const std::string & relay_,
                                   const std::string & source_key_);

      //! Send an event to a specific logical address (queue)
      int send_event_to_address(const vire::utility::base_event & event_payload_,
                                const std::string & address_,
                                const std::string & source_key_);

    private:

      // Configuration
      std::string _relay_; //!< Identifier of the relay

      // Working
      std::mutex _send_event_mutex_;

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_EVENT_EMITTER_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
