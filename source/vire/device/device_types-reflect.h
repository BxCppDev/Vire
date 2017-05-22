//! \file  vire/device/device_types-reflect.h
//! \brief Vire device types
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

#ifndef VIRE_DEVICE_DEVICE_TYPES_REFLECT_H
#define VIRE_DEVICE_DEVICE_TYPES_REFLECT_H

// Ourselves
#include <vire/device/device_types.h>

// Standard Library
#include <iostream>

// Third party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

// Export the introspection code for the 'vire::device::device_type' enum:
DR_TYPE_EXPORT(::vire::device::device_type, "vire::device::device_type")

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::device::device_type, tag_)
      {
        DR_ENUM_DECLARE(::vire::device::component_type)
          .DR_ENUM_PAIR("undefined",    ::vire::device::TYPE_UNDEFINED)
          .DR_ENUM_PAIR("generic",      ::vire::device::TYPE_GENERIC)
          .DR_ENUM_PAIR("setup",        ::vire::device::TYPE_SETUP)
          .DR_ENUM_PAIR("rack",         ::vire::device::TYPE_RACK)
          .DR_ENUM_PAIR("crate",        ::vire::device::TYPE_CRATE)
          .DR_ENUM_PAIR("module",       ::vire::device::TYPE_MODULE)
          .DR_ENUM_PAIR("module_board", ::vire::device::TYPE_MODULE_BOARD)
          .DR_ENUM_PAIR("module_mezzanine_board", ::vire::device::TYPE_MODULE_MEZZANINE_BOARD)
          .DR_ENUM_PAIR("chip",         ::vire::device::TYPE_CHIP)
          .DR_ENUM_PAIR("chip_pld",     ::vire::device::TYPE_CHIP_PLD)
          .DR_ENUM_PAIR("chip_asic",    ::vire::device::TYPE_CHIP_ASIC)
          .DR_ENUM_PAIR("chip_eprom",   ::vire::device::TYPE_CHIP_EPROM)
          .DR_ENUM_PAIR("wire",         ::vire::device::TYPE_WIRE)
          .DR_ENUM_PAIR("cable",        ::vire::device::TYPE_CABLE)
          .DR_ENUM_PAIR("flat_ribbon_cable", ::vire::device::TYPE_FLAT_RIBBON_CABLE)
          .DR_ENUM_PAIR("cable_strand", ::vire::device::TYPE_CABLE_STRAND)
          .DR_ENUM_PAIR("firmware",     ::vire::device::TYPE_FIRMWARE)
          .DR_ENUM_PAIR("software",     ::vire::device::TYPE_SOFTWARE)
          .DR_ENUM_PAIR("datapoint",    ::vire::device::TYPE_DATAPOINT)
          .DR_ENUM_PAIR("register",     ::vire::device::TYPE_REGISTER)
          ;
        return;
      }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_DEVICE_DEVICE_TYPES_REFLECT_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
