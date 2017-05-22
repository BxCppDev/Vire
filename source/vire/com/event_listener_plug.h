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

// This project:
#include <vire/com/base_plug.h>

namespace vire {

  namespace com {

   //! \brief User credentials data
    class event_listener_plug : public vire::com::base_plug
    {
    public:

      //! Default constructor
      event_listener_plug(const manager & mgr_);

      //! Destructor
      virtual ~event_listener_plug();

      //! Check if a new event payload object is available
      bool has_event() const;

      // //! Pop an event payload object
      // int pop_event(vire::utility::base_event & event_payload_);

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_EVENT_LISTENER_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
