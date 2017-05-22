//! \file  vire/user/user-reflect.h
//! \brief Vire user reflection support
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

#ifndef VIRE_USER_USER_REFLECT_H
#define VIRE_USER_USER_REFLECT_H

// Ourselves:
#include <vire/user/user.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>
#include <datatools/enriched_base.h>

// Export the introspection code :
DR_CLASS_EXPORT(::vire::user::user, "vire::user::user")

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_CLASS_IMPLEMENT_REFLECTION_HEAD(::vire::user::user, tag_ __attribute__((unused)) )
      {
        DR_CLASS_DECLARE (::vire::user::user)
          // .tag("pi", camp::Value(3.14149))
          // .tag("NaN", DR_VALUE(std::numeric_limits<double>::quiet_NaN()))
          // .tag("pi", DR_VALUE(3.14149))
          // .tag("NaN", DR_VALUE(std::numeric_limits<double>::quiet_NaN()))
          .DR_CLASS_BASE(::datatools::enriched_base)
          .DR_CLASS_CONSTRUCTOR()
          .DR_CLASS_PROPERTY_GETTER_SETTER("uid",
                                           ::vire::user::user::get_uid,
                                           ::vire::user::user::set_uid)
          .tag("Description", "The unique identifier of the user")
          .DR_CLASS_METHOD_CONST("is_valid",
                                 ::vire::user::user,
                                 ::vire::user::user::is_valid,
                                 bool)
          .DR_CLASS_METHOD_CONST("has_valid_uid",
                                 ::vire::user::user,
                                 ::vire::user::user::has_valid_uid,
                                 bool)
          .DR_CLASS_METHOD_CONST("is_administrator",
                                 ::vire::user::user,
                                 ::vire::user::user::is_administrator,
                                 bool)
          .DR_CLASS_VOID_METHOD("initialize_simple",
                                ::vire::user::user,
                                ::vire::user::user::initialize_simple)
          .DR_CLASS_VOID_METHOD1("initialize",
                                 ::vire::user::user,
                                 ::vire::user::user::initialize,
                                 const ::datatools::properties &)
          .tag("Description", "The main initialization method")
          .tag("Arg[0]", "A set of configuration properties")
          .DR_CLASS_VOID_METHOD3("export_to_config",
                                 ::vire::user::user,
                                 ::vire::user::user::export_to_config,
                                 ::datatools::properties &,
                                 uint32_t,
                                 const std::string &)
          .DR_CLASS_VOID_METHOD("reset",
                                ::vire::user::user,
                                ::vire::user::user::reset)
          .DR_CLASS_VOID_METHOD("check",
                                ::vire::user::user,
                                ::vire::user::user::check)
          ;
        return;
      }

    } // end of namespace introspection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_USER_USER_REFLECT_H
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
