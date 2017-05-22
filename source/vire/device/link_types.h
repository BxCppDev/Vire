//! \file  vire/device/link_types.h
//! \brief Vire link types
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

#ifndef VIRE_DEVICE_LINK_TYPES_H
#define VIRE_DEVICE_LINK_TYPES_H

// Standard library
#include <string>

// Third party
// - Bayeux/datatools
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

namespace vire {

  namespace device {

    //! \brief Link type identifier
    enum link_type {
      LINK_UNDEFINED  =   -1, //!< Undefined link type
      LINK_GENERIC    =    0, //!< Generic link
      LINK_PIPE       = 1000, //!< Generic pipe
      LINK_CABLE      = 2000, //!< Generic cable
      LINK_TRACE      = 3000, //!< Generic conductive trace (PCB)
      LINK_WIRE       = 4000, //!< Generic conductive wire (ASIC)
      LINK_BUS        = 5000, //!< Generic data bus
      LINK_DEPENDENCY = 10000, //!< Generic dependency
    };

    //! Return the link type associated to a link label
    //!
    //! @arg label_ a human readable character string representing the type of the link
    //! @return the type identifier of the link
    link_type get_link_type(const std::string & label_);

    //! Return the link label associated to a link type
    //!
    //! @arg type_ the type identifier of the link
    //! @return a human readable character string associated to the type of link
    std::string get_link_type_label(link_type type_);

    //! Check if two links have matching types
    //!
    //! @arg lt1_ the type identifier of the first link
    //! @arg lt2_ the type identifier of the second link
    //! @return true if both links have compatible types
    bool match_link_type(link_type lt1_, link_type lt2_);

  } // end of namespace device

} // end of namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::link_type' enum:
DR_TYPE_INIT(::vire::device::link_type)
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_LINK_TYPES_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
