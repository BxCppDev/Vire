//! \file    vire/user/manager-reflect.h
//! \brief   Vire user manager reflection support
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

#ifndef VIRE_USER_MANAGER_REFLECT_H
#define VIRE_USER_MANAGER_REFLECT_H

// Ourselves:
#include <vire/user/manager.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>
#include <datatools/detail/reflection_export.h>

// This project:
#include <vire/user/user.h>
#include <vire/user/group.h>

// Export the introspection code :
DR_CLASS_EXPORT (::vire::user::manager, "vire::user::manager")

namespace datatools {
  namespace detail {
    namespace reflection {

      DR_CLASS_IMPLEMENT_REFLECTION_HEAD(::vire::user::manager, tag_ __attribute__((unused)) )
      {
        DR_CLASS_DECLARE(::vire::user::manager)
          .DR_CLASS_BASE(::datatools::base_service)
          .DR_CLASS_CONSTRUCTOR()
          .DR_CLASS_PROPERTY_GETTER_SETTER("standard_user_min_uid",
                                           ::vire::user::manager::get_standard_user_min_uid,
                                           ::vire::user::manager::set_standard_user_min_uid)
          .DR_CLASS_PROPERTY_GETTER_SETTER("standard_group_min_gid",
                                           ::vire::user::manager::get_standard_group_min_gid,
                                           ::vire::user::manager::set_standard_group_min_gid)
          .DR_CLASS_PROPERTY_GETTER_SETTER("users_table_path",
                                           ::vire::user::manager::get_users_table_path,
                                           ::vire::user::manager::set_users_table_path)
          .DR_CLASS_PROPERTY_GETTER_SETTER("groups_table_path",
                                           ::vire::user::manager::get_groups_table_path,
                                           ::vire::user::manager::set_groups_table_path)
          .DR_CLASS_PROPERTY_GETTER_SETTER("dont_backup_tables",
                                           ::vire::user::manager::dont_backup_tables,
                                           ::vire::user::manager::set_dont_backup_tables)
          .DR_CLASS_PROPERTY_GETTER_SETTER("dont_load_tables",
                                           ::vire::user::manager::dont_load_tables,
                                           ::vire::user::manager::set_dont_load_tables)
          .DR_CLASS_PROPERTY_GETTER_SETTER("dont_store_tables",
                                           ::vire::user::manager::dont_store_tables,
                                           ::vire::user::manager::set_dont_store_tables)
          .DR_CLASS_METHOD_CONST("is_initialized",
                                 ::vire::user::manager,
                                 ::vire::user::manager::is_initialized,
                                 bool)
          .DR_CLASS_METHOD_CONST("has_users_table_path",
                                 ::vire::user::manager,
                                 ::vire::user::manager::has_users_table_path,
                                 bool)
          .DR_CLASS_METHOD_CONST("has_groups_table_path",
                                 ::vire::user::manager,
                                 ::vire::user::manager::has_groups_table_path,
                                 bool)
          .DR_CLASS_METHOD1_CONST("has_user_by_uid",
                                  ::vire::user::manager,
                                  ::vire::user::manager::has_user_by_uid,
                                  bool,
                                  int32_t)
          .DR_CLASS_METHOD1_CONST("has_group_by_gid",
                                  ::vire::user::manager,
                                  ::vire::user::manager::has_group_by_gid,
                                  bool,
                                  int32_t)
          .DR_CLASS_METHOD1_CONST("has_user_by_name",
                                  ::vire::user::manager,
                                  ::vire::user::manager::has_user_by_name,
                                  bool,
                                  const std::string &)
          .DR_CLASS_METHOD1_CONST("has_group_by_name",
                                  ::vire::user::manager,
                                  ::vire::user::manager::has_group_by_name,
                                  bool,
                                  const std::string &)
          .DR_CLASS_METHOD1_CONST("get_group_by_gid",
                                  ::vire::user::manager,
                                  ::vire::user::manager::get_group_by_gid,
                                  const ::vire::user::group &,
                                  int32_t
                                  )
          .DR_CLASS_METHOD1_CONST("get_user_by_uid",
                                  ::vire::user::manager,
                                  ::vire::user::manager::get_user_by_uid,
                                  const ::vire::user::user &,
                                  int32_t
                                  )
          .DR_CLASS_METHOD1("grab_user_by_uid",
                            ::vire::user::manager,
                            ::vire::user::manager::grab_user_by_uid,
                            ::vire::user::user &,
                            int32_t)
          .DR_CLASS_METHOD1("grab_group_by_gid",
                            ::vire::user::manager,
                            ::vire::user::manager::grab_group_by_gid,
                            ::vire::user::group &,
                            int32_t)
          .DR_CLASS_METHOD1_CONST("get_user_by_name",
                                  ::vire::user::manager,
                                  ::vire::user::manager::get_user_by_name,
                                  const ::vire::user::user &,
                                  const std::string &)
          .DR_CLASS_METHOD1_CONST("get_group_by_name",
                                  ::vire::user::manager,
                                  ::vire::user::manager::get_group_by_name,
                                  const ::vire::user::group &,
                                  const std::string &)
          .DR_CLASS_METHOD1("grab_user_by_name",
                            ::vire::user::manager,
                            ::vire::user::manager::grab_user_by_name,
                            ::vire::user::user &,
                            const std::string &)
          .DR_CLASS_METHOD1("grab_group_by_name",
                            ::vire::user::manager,
                            ::vire::user::manager::grab_group_by_name,
                            ::vire::user::group &,
                            const std::string &)
          .DR_CLASS_METHOD2("create_new_group",
                            ::vire::user::manager,
                            ::vire::user::manager::create_new_group,
                            ::vire::user::group &,
                            const datatools::properties &,
                            int32_t
                            )
          .DR_CLASS_METHOD2("create_new_user",
                            ::vire::user::manager,
                            ::vire::user::manager::create_new_user,
                            ::vire::user::user &,
                            const datatools::properties &,
                            int32_t
                            )
          .DR_CLASS_METHOD2("create_new_user_by_name",
                            ::vire::user::manager,
                            ::vire::user::manager::create_new_user_by_name,
                            ::vire::user::user &,
                            const std::string &,
                            int32_t
                            )
          .DR_CLASS_METHOD2("create_new_group_by_name",
                            ::vire::user::manager,
                            ::vire::user::manager::create_new_group_by_name,
                            ::vire::user::group &,
                            const std::string &,
                            int32_t
                            )
          .DR_CLASS_VOID_METHOD1("remove_user_by_uid",
                                 ::vire::user::manager,
                                 ::vire::user::manager::remove_user_by_uid,
                                 int32_t
                                 )
          .DR_CLASS_VOID_METHOD1("remove_group_by_gid",
                                 ::vire::user::manager,
                                 ::vire::user::manager::remove_group_by_gid,
                                 int32_t
                                 )
          .DR_CLASS_VOID_METHOD1("remove_user_by_name",
                                 ::vire::user::manager,
                                 ::vire::user::manager::remove_user_by_name,
                                 const std::string &
                                 )
          .DR_CLASS_VOID_METHOD1("remove_group_by_name",
                                 ::vire::user::manager,
                                 ::vire::user::manager::remove_group_by_name,
                                 const std::string &
                                 )
          .DR_CLASS_VOID_METHOD("remove_all_users",
                                ::vire::user::manager,
                                ::vire::user::manager::remove_all_users
                                )
          .DR_CLASS_VOID_METHOD("remove_all_groups",
                                ::vire::user::manager,
                                ::vire::user::manager::remove_all_groups
                                )
          .DR_CLASS_VOID_METHOD2("add_user_in_group",
                                 ::vire::user::manager,
                                 ::vire::user::manager::add_user_in_group,
                                 int32_t, int32_t
                                 )
          .DR_CLASS_VOID_METHOD2("remove_user_from_group",
                                 ::vire::user::manager,
                                 ::vire::user::manager::remove_user_from_group,
                                 int32_t, int32_t
                                 )
          .DR_CLASS_METHOD1_CONST("is_user_valid",
                                  ::vire::user::manager,
                                  ::vire::user::manager::is_user_valid,
                                  bool,
                                  int32_t)
          .DR_CLASS_METHOD1_CONST("is_group_valid",
                                  ::vire::user::manager,
                                  ::vire::user::manager::is_group_valid,
                                  bool,
                                  int32_t)
          .DR_CLASS_METHOD1_CONST("is_group_with_enumeration",
                                  ::vire::user::manager,
                                  ::vire::user::manager::is_group_with_enumeration,
                                  bool,
                                  int32_t)
          .DR_CLASS_METHOD_CONST("get_users",
                                 ::vire::user::manager,
                                 ::vire::user::manager::get_users,
                                 const ::vire::user::user_dict_type &)
          .DR_CLASS_METHOD_CONST("get_groups",
                                 ::vire::user::manager,
                                 ::vire::user::manager::get_groups,
                                 const ::vire::user::group_dict_type &)
           .DR_CLASS_VOID_METHOD("reset",
                                ::vire::user::manager,
                                ::vire::user::manager::reset)
          // .DR_CLASS_VOID_METHOD1_CONST("build_list_of_uids",
          //                              ::vire::user::manager,
          //                              ::vire::user::manager::build_list_of_uids,
          //                              std::vector<int32_t> &
          //                              )
          //                                 camp::ValueMapper<std::vector<int32_t> &>
          //                       )
          // .DR_CLASS_METHOD_CONST("list_of_uids",
          //                        ::vire::user::manager,
          //                        ::vire::user::manager::list_of_uids,
          //                        std::vector<int32_t>
          //                        //camp::ValueMapper<std::vector<int32_t> >
          //                        )
          // .DR_CLASS_METHOD_CONST("list_of_usernames",
          //                        ::vire::user::manager,
          //                        ::vire::user::manager::list_of_usernames,
          //                        std::vector<std::string>
          //                        )
          ;
        return;
      }

    } // end of namespace introspection
  } // end of namespace detail
} // end of namespace datatools

#endif // VIRE_USER_MANAGER_REFLECT_H
/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
