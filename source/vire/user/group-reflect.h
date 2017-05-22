//! \file    vire/user/group-reflect.h
//! \brief   Vire user group reflection support
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

#ifndef VIRE_USER_GROUP_REFLECT_H
#define VIRE_USER_GROUP_REFLECT_H

// Ourselves:
#include <vire/user/group.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

// Export the introspection code :
DR_CLASS_EXPORT(::vire::user::group, "vire::user::group")

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_CLASS_IMPLEMENT_REFLECTION_HEAD(::vire::user::group, tag_ __attribute__((unused)) )
      {

        DR_CLASS_DECLARE (::vire::user::group)
          .DR_CLASS_BASE(::datatools::enriched_base)
          .DR_CLASS_CONSTRUCTOR()
          .DR_CLASS_PROPERTY_GETTER_SETTER("gid",
                                           ::vire::user::group::get_gid,
                                           ::vire::user::group::set_gid)
          .DR_CLASS_METHOD_CONST("is_valid",
                                 ::vire::user::group,
                                 ::vire::user::group::is_valid,
                                 bool)
          .DR_CLASS_METHOD_CONST("has_valid_gid",
                                 ::vire::user::group,
                                 ::vire::user::group::has_valid_gid,
                                 bool)
          .DR_CLASS_METHOD1_CONST("has_uid",
                                  ::vire::user::group,
                                  ::vire::user::group::has_uid,
                                  bool,
                                  int32_t)
          .DR_CLASS_VOID_METHOD2_CONST("build_set_of_uids",
                                       ::vire::user::group,
                                       ::vire::user::group::build_set_of_uids,
                                       const std::set<int32_t> &,
                                       std::set<int32_t> &)
          // Not supported because std::set<int32_t> is not a reference:
          // .DR_CLASS_METHOD1_CONST("set_of_uids",
          //                         ::vire::user::group,
          //                         ::vire::user::group::set_of_uids,
          //                         std::set<int32_t>,
          //                         const std::set<int32_t> &)
          .DR_CLASS_METHOD_CONST("has_user_selector",
                                 ::vire::user::group,
                                 ::vire::user::group::has_user_selector,
                                 bool)
          .DR_CLASS_VOID_METHOD("check",
                                ::vire::user::group,
                                ::vire::user::group::check)
          .DR_CLASS_VOID_METHOD("initialize_simple",
                                 ::vire::user::group,
                                 ::vire::user::group::initialize_simple)
          .DR_CLASS_VOID_METHOD1("initialize_standalone",
                                 ::vire::user::group,
                                 ::vire::user::group::initialize_standalone,
                                 const datatools::properties &)
          .DR_CLASS_VOID_METHOD2("initialize",
                                 ::vire::user::group,
                                 ::vire::user::group::initialize,
                                 const datatools::properties &,
                                 ::vire::user::manager &)
          .DR_CLASS_VOID_METHOD3("export_to_config",
                                 ::vire::user::group,
                                 ::vire::user::group::export_to_config,
                                 ::datatools::properties &,
                                 uint32_t,
                                 const std::string &)
          .DR_CLASS_VOID_METHOD("reset",
                                ::vire::user::group,
                                ::vire::user::group::reset)
          ;
        return;
      }

    } // end of namespace introspection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_USER_GROUP_REFLECT_H
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
