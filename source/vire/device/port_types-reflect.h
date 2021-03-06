//! \file  vire/device/port_types.h
//! \brief Vire port types reflection support
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

#ifndef VIRE_DEVICE_PORT_TYPES_REFLECT_H
#define VIRE_DEVICE_PORT_TYPES_REFLECT_H

// Ourselves:
#include <vire/device/port_types.h>

// Standard Library:
#include <iostream>

// Third party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

// Export the introspection code for the 'vire::device::port_type' enum:
DR_TYPE_EXPORT(::vire::device::port_type, "vire::device::port_type")

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::device::port_type, tag_)
      {
        DR_ENUM_DECLARE(::vire::device::port_type)
          .DR_ENUM_PAIR("undefined",    ::vire::device::PORT_UNDEFINED)
          .DR_ENUM_PAIR("generic",      ::vire::device::PORT_GENERIC)
          .DR_ENUM_PAIR("connector",    ::vire::device::PORT_CONNECTOR)
          ;
        return;
      }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_DEVICE_PORT_TYPES_REFLECT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
