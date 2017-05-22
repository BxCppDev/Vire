//! \file  vire/utility/rw_access-reflect.h
//! \brief Introspection for readable/writable access type
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

#ifndef VIRE_UTILITY_RW_ACCESS_REFLECT_H
#define VIRE_UTILITY_RW_ACCESS_REFLECT_H

// Ourselves:
#include <vire/utility/rw_access.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_export.h>
#include <datatools/detail/reflection_macros.h>

DR_TYPE_EXPORT(::vire::utility::rw_access_type, "vire::utility::rw_access_type");

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::utility::rw_access_type, tag_ __attribute__((unused)) )
      {
        DR_ENUM_DECLARE(::vire::utility::rw_access_type)
          .value("invalid", ::vire::utility::RW_INVALID)
          .value("none",    ::vire::utility::RW_NONE)
          .value("ro",      ::vire::utility::RW_READABLE)
          .value("wo",      ::vire::utility::RW_WRITABLE)
          .value("rw",      ::vire::utility::RW_READABLE_WRITABLE)
          ;
        return;
      }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_UTILITY_RW_ACCESS_REFLECT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
