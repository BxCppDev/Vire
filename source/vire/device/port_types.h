//! \file  vire/device/port_types.h
//! \brief Vire port types
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_DEVICE_PORT_TYPES_H
#define VIRE_DEVICE_PORT_TYPES_H

// Standard library
#include <string>

// Third party
// - Bayeux/datatools
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

namespace vire {

  namespace device {

    //! \brief Port type identifier
    enum port_type {
      PORT_UNDEFINED  =   -1, //!< Undefined port type
      PORT_GENERIC    =    0, //!< Generic port
      PORT_PLUG       = 1000, //!< Generic plug
      PORT_METHOD     = 2000, //!< Generic method
    };

    //! Return the port type associated to a port label
    //!
    //! @arg label_ a human readable character string representing the type of the port
    //! @return the type identifier of the port
    port_type get_port_type(const std::string & label_);

    //! Return the port label associated to a port type
    //!
    //! @arg type_ the type identifier of the port
    //! @return a human readable character string associated to the type of port
    std::string get_port_type_label(port_type);

    //! Check if two ports have matching types
    //!
    //! @arg pt1_ the type identifier of the first port
    //! @arg pt2_ the type identifier of the second port
    //! @return true if both ports have compatible types
    bool match_port_type(port_type pt1_, port_type pt2_);

  } // end of namespace device

} // end of namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::port_type' enum:
DR_TYPE_INIT(::vire::device::port_type)
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_PORT_TYPES_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
