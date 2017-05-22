//! \file  vire/cms/status-reflect.h
//! \brief Vire status reflection
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

#ifndef VIRE_CMS_STATUS_REFLECT_H
#define VIRE_CMS_STATUS_REFLECT_H

// Ourselves
#include <vire/cms/status.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

// Export the introspection code for the 'vire::cms::status' enum:
DR_TYPE_EXPORT(:vire::cms::status, "vire::cms::status")

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_TYPE_IMPLEMENT_REFLECTION_HEAD(::vire::cms::status, tag_)
      {
        DR_ENUM_DECLARE(::vire::cms::status)
          .DR_ENUM_PAIR("void",     ::vire::cms::status::VOID)
          .DR_ENUM_PAIR("failure",  ::vire::cms::status::FAILURE)
          .DR_ENUM_PAIR("pending",  ::vire::cms::status::PENDING)
          .DR_ENUM_PAIR("disabled", ::vire::cms::status::DISABLED)
          .DR_ENUM_PAIR("missing",  ::vire::cms::status::MISSING)
          ;
        return;
      }

    } // end of namespace reflection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_CMS_STATUS_REFLECT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
