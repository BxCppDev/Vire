//! \file  vire/resource/resource-reflect.h
//! \brief Vire resource reflection support
//! \details
//
// Copyright (c) 2015 by Francois Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_RESOURCE_RESOURCE_REFLECT_H
#define VIRE_RESOURCE_RESOURCE_REFLECT_H

// Ourselves:
#include <vire/resource/resource.h>

// Export the introspection code :
DR_CLASS_EXPORT(::vire::resource::resource, "vire::resource::resource")

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_CLASS_IMPLEMENT_REFLECTION_HEAD(::vire::resource::resource, /*tag_*/)
  {

    DR_CLASS_DECLARE (::vire::resource::resource)
      .DR_CLASS_BASE(::datatools::enriched_base)
      .DR_CLASS_CONSTRUCTOR()
      .DR_CLASS_PROPERTY_GETTER_SETTER("rid",
               ::vire::resource::resource::get_id,
               ::vire::resource::resource::set_id)
      .DR_CLASS_PROPERTY_GETTER_SETTER("path",
               ::vire::resource::resource::get_path,
               ::vire::resource::resource::set_path)
      .DR_CLASS_PROPERTY_GETTER_SETTER("access",
               ::vire::resource::resource::get_access,
               ::vire::resource::resource::set_access)
      .DR_CLASS_PROPERTY_GETTER_SETTER("max_number_of_subscribers",
               ::vire::resource::resource::get_max_number_of_subscribers,
               ::vire::resource::resource::set_max_number_of_subscribers)
      .DR_CLASS_METHOD_CONST("has_id",
           ::vire::resource::resource,
           ::vire::resource::resource::has_id,
           bool)
      .DR_CLASS_METHOD_CONST("has_path",
           ::vire::resource::resource,
           ::vire::resource::resource::has_path,
           bool)
      .DR_CLASS_METHOD_CONST("has_max_number_of_subscribers",
           ::vire::resource::resource,
           ::vire::resource::resource::has_max_number_of_subscribers,
           bool)
      .DR_CLASS_METHOD_CONST("has_resource_instance",
           ::vire::resource::resource,
           ::vire::resource::resource::has_resource_instance,
           bool)
      .DR_CLASS_METHOD_CONST("is_number_of_subscribers_unlimited",
           ::vire::resource::resource,
           ::vire::resource::resource::is_number_of_subscribers_unlimited,
           bool)
      .DR_CLASS_METHOD_CONST("is_number_of_subscribers_singleton",
           ::vire::resource::resource,
           ::vire::resource::resource::is_number_of_subscribers_singleton,
           bool)
      .DR_CLASS_METHOD_CONST("is_number_of_subscribers_limited",
           ::vire::resource::resource,
           ::vire::resource::resource::is_number_of_subscribers_limited,
           bool)
      .DR_CLASS_METHOD_CONST("has_access",
           ::vire::resource::resource,
           ::vire::resource::resource::has_access,
           bool)
      .DR_CLASS_METHOD_CONST("is_readable",
           ::vire::resource::resource,
           ::vire::resource::resource::is_readable,
           bool)
      .DR_CLASS_METHOD_CONST("is_writable",
           ::vire::resource::resource,
           ::vire::resource::resource::is_writable,
           bool)
      .DR_CLASS_METHOD_CONST("is_initialized",
           ::vire::resource::resource,
           ::vire::resource::resource::is_initialized,
           bool)
      .DR_CLASS_VOID_METHOD1("initialize",
           ::vire::resource::resource,
           ::vire::resource::resource::initialize,
           const ::datatools::properties &)
      .DR_CLASS_VOID_METHOD("initialize_simple",
          ::vire::resource::resource,
          ::vire::resource::resource::initialize_simple)
      .DR_CLASS_VOID_METHOD("reset",
          ::vire::resource::resource,
          ::vire::resource::resource::reset)
      .DR_CLASS_METHOD_CONST("is_valid",
           ::vire::resource::resource,
           ::vire::resource::resource::is_valid,
           bool)
      ;
    return;
  }

    } // end of namespace introspection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_RESOURCE_RESOURCE_REFLECT_H
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
