//! \file  vire/device/port_utils.h
//! \brief Vire port utilities
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

#ifndef VIRE_DEVICE_PORT_UTILS_H
#define VIRE_DEVICE_PORT_UTILS_H

// Standard library
#include <string>

// Third party
// - Bayeux/datatools
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

namespace vire {

  namespace device {

    //! \brief Port I/O type identifier
    enum port_io_type {
      PORT_IO_UNDEFINED =   -1, //!< Undefined port type
      PORT_IO_GENERIC   =    0, //!< Generic I/O type
      PORT_IO_IN        =    1, //!< Input port
      PORT_IO_OUT       =    2, //!< Output port
      PORT_IO_IN_OUT    =    3, //!< Input/output port
    };

    //! Return the port I/O type associated to a port I/O label
    port_io_type get_port_io_type(const std::string & label_);

    //! Return the port I/O label associated to a port I/O type
    std::string get_port_io_type_label(port_io_type);

    //! \brief Device type identifier
    enum port_interface_type {
      PORT_INTERFACE_UNDEFINED = -1, //!< Undefined port interface
      PORT_INTERFACE_INSIDE    =  1, //!< Port interface to inside
      PORT_INTERFACE_OUTSIDE   =  2, //!< Port interface to outside
    };

    //! Return the port interface type associated to a port interface label
    port_interface_type get_port_interface_type(const std::string & label_);

    //! Return the port interface label associated to a port interface type
    std::string get_port_interface_type_label(port_interface_type);

  } // end of namespace device

} // end of namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::port_io_type' enum:
DR_TYPE_INIT(::vire::device::port_io_type)
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_PORT_TYPES_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
