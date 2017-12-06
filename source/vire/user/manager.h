//! \file  vire/user/manager.h
//! \brief Vire user manager
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

#ifndef VIRE_USER_MANAGER_H
#define VIRE_USER_MANAGER_H

// Standard library:
#include <string>
#include <set>
#include <map>

// Third party:
// - Bayeux/datatools:
#include <datatools/bit_mask.h>
#include <datatools/base_service.h>

// This package:
#include <vire/user/user.h>
#include <vire/user/group.h>

namespace vire {

  namespace user {

    //! \brief User manager
    class manager
      : public ::datatools::base_service
    {
    public:

      //! Default value for standard user minimum UID
      static const int32_t DEFAULT_STANDARD_USER_MIN_UID = 1000;

      //! Default value for standard user minimum GID
      static const int32_t DEFAULT_STANDARD_GROUP_MIN_GID = 1000;

      static const std::string & default_service_name();

      /// \brief Setup flags
      enum setup_flags {
        DONT_STORE_TABLES  = datatools::bit_mask::bit00, //!< Do not store tables at reset
        DONT_BACKUP_TABLES = datatools::bit_mask::bit01, //!< Do not backup tables at store
        DONT_LOAD_TABLES   = datatools::bit_mask::bit02, //!< Do not load tables at init
        LOG_TRACE          = datatools::bit_mask::bit03  //!< Activate trace logging
      };

      //! Default constructor
      manager(uint32_t flags_ = 0);

      //! Destructor
      virtual ~manager();

      //! Return the minimum UID for a standard user
      int32_t get_standard_user_min_uid() const;

      //! Set the minimum UID for a standard user
      void set_standard_user_min_uid(int32_t);

      //! Return the minimum GID for a standard user group
      int32_t get_standard_group_min_gid() const;

      //! Set the minimum GID for a standard user group
      void set_standard_group_min_gid(int32_t);

      //! Check the existence of an users table path
      bool has_users_table_path() const;

      //! Check the existence of an user groups table path
      bool has_groups_table_path() const;

      //! Return the path of the users table path
      const std::string & get_users_table_path() const;

      //! Set the path of the users table path
      void set_users_table_path(const std::string &);

      //! Return the path of the user groups table path
      const std::string & get_groups_table_path() const;

      //! Set the path of the user groups table path
      void set_groups_table_path(const std::string &);

      //! Check the flag for not loading tables
      bool dont_load_tables() const;

      //! Check the flag for loading tables
      bool is_load_tables() const;

      //! Check the flag for not storing tables
      bool dont_store_tables() const;

      //! Check the flag for storing tables
      bool is_store_tables() const;

      //! Check the flag for not backuping tables
      bool dont_backup_tables() const;

      //! Check the flag for backuping tables
      bool is_backup_tables() const;

      //! Set the flag for denying tables loading
      void set_dont_load_tables(bool);

      //! Set the flag for denying tables storage
      void set_dont_store_tables(bool);

      //! Set the flag for not backuping tables
      void set_dont_backup_tables(bool);

      //! Check the existence of an user given its UID
      bool has_user_by_uid(int32_t uid_) const;

      //! Check the existence of an user group given its GID
      bool has_group_by_gid(int32_t gid_) const;

      //! Check the existence of an user given its name
      bool has_user_by_name(const std::string & name_) const;

      //! Check the existence of an user group given its gname
      bool has_group_by_name(const std::string & gname_) const;

      //! Return a const reference to an existing user given its UID
      const user & get_user_by_uid(int32_t uid_) const;

      //! Return a const reference to an existing user group given its GID
      const group & get_group_by_gid(int32_t gid_) const;

      //! Return a mutable reference to an existing user given its UID
      user & grab_user_by_uid(int32_t uid_);

      //! Return a mutable reference to an existing group given its GID
      group & grab_group_by_gid(int32_t gid_);

      //! Return a const reference to an existing user given its name
      const user & get_user_by_name(const std::string & name_) const;

      //! Return a const reference to an existing user group given its name
      const group & get_group_by_name(const std::string & gname_) const;

      //! Return a mutable reference to an existing user given its name
      user & grab_user_by_name(const std::string & name_);

      //! Return a mutable reference to an existing user group given its name
      group & grab_group_by_name(const std::string & gname_);

      //! Create a new user given its name and optional UID
      user & create_new_user(const datatools::properties & user_config_,
                             int32_t uid_ = user::INVALID_UID);

      //! Create a new user given its name and optional UID
      user & create_new_user_by_name(const std::string & user_name_,
                                     int32_t uid_ = user::INVALID_UID);

      //! Create a new user group given its properties and optional GID
      group & create_new_group(const datatools::properties & group_config_,
                               int32_t gid_ = group::INVALID_GID);

      //! Create a new user group given its name and optional GID
      group & create_new_group_by_name(const std::string & group_name_,
                                       int32_t gid_ = group::INVALID_GID);

      //! Remove an existing user given its UID
      void remove_user_by_uid(int32_t uid_);

      //! Remove an existing user group given its GID
      void remove_group_by_gid(int32_t gid_);

      //! Remove an existing user given its name
      void remove_user_by_name(const std::string & name_);

      //! Remove an existing user group given its name
      void remove_group_by_name(const std::string & gname_);

      //! Remove all users
      void remove_all_users();

      //! Remove all groups
      void remove_all_groups();

      //! Associate an existing user group to an existing user
      void add_user_in_group(int32_t uid_, int32_t gid_);

      //! Detach an existing user group from an existing user
      void remove_user_from_group(int32_t uid_, int32_t gid_);

      //! Check if an user is valid
      bool is_user_valid(int32_t uid_) const;

      //! Check if an user group is valid
      bool is_group_valid(int32_t gid_) const;

      //! Check if an user group is defined by an enumerated user selector
      bool is_group_with_enumeration(int32_t gid_) const;

      //! Load the tables
      void load_tables(uint32_t load_flags_ = 0);

      //! Store the tables
      void store_tables(uint32_t store_flags_ = 0) const;

      //! Build the list of UIDS
      void build_list_of_uids(std::vector<int32_t> & uids_) const;

      //! Build the list of GIDS
      void build_list_of_gids(std::vector<int32_t> & gids_) const;

      //! Return the list of UIDs
      std::vector<int32_t> list_of_uids() const;

      //! Return the list of GIDs
      std::vector<int32_t> list_of_gids() const;

      //! Build the list of UIDS in group with given GID
      void build_list_of_uids_in_group(int32_t gid_,
                                       std::vector<int32_t> & uids_) const;

      //! Build the list of GIDS with user with given UID
      void build_list_of_gids_with_user(int32_t uid_,
                                        std::vector<int32_t> & gids_) const;

      //! Build the list of user names
      void build_list_of_user_names(std::vector<std::string> & user_names_) const;

      //! Build the list of user group names
      void build_list_of_group_names(std::vector<std::string> & group_names_) const;

      //! Return the list of user names
      std::vector<std::string> list_of_user_names() const;

      //! Return the list of user group names
      std::vector<std::string> list_of_group_names() const;

      //!< Return a non mutable reference to the embedded dictionary of users
      const user_dict_type & get_users() const;

      //!< Return a non mutable reference to the embedded dictionary of user groups
      const group_dict_type & get_groups() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Check if the service is initialized
      virtual bool is_initialized() const;

      //! Initialize the service using a list of properties with access to a dictionary of services
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the service
      virtual int reset();

    protected:

      //! Set default attribute values
      void _set_defaults();

      //! Create a new user
      user & _create_new_user(const datatools::properties & user_config_,
                              int32_t uid_);

      //! Create a new user group given its name and optional GID
      group & _create_new_group(const datatools::properties & group_config_,
                                int32_t gid_);

      //! Associate an existing user in an existing group
      void _add_user_in_group(int32_t uid_, int32_t gid_);

      //! Remove an existing user from an existing user group
      void _remove_user_from_group(int32_t uid_, int32_t gid_);

      //! Remove an existing user
      void _remove_user(int32_t uid_);

      //! Remove an existing group
      void _remove_group(int32_t gid_);

      //! Load users table
      virtual void _load_users_table(const std::string & source_, uint32_t flags_);

      //! Load groups table
      virtual void _load_groups_table(const std::string & source_, uint32_t flags_);

      //! Store users tables
      virtual void _store_users_table(const std::string & target_, uint32_t flags_) const;

      //! Store groups tables
      virtual void _store_groups_table(const std::string & target_, uint32_t flags_) const;

      //! Check the existence of an user given its name
      bool _has_user_by_name(const std::string & name_) const;

      //! Check the existence of an user group given its name
      bool _has_group_by_name(const std::string & gname_) const;

      //! Check the existence of an user given its UID
      bool _has_user_by_uid(int32_t uid_) const;

      //! Check the existence of an user group given its GID
      bool _has_group_by_gid(int32_t gid_) const;

      //! Compute a new (unused) UID
      int32_t _candidate_uid() const;

      //! Compute a new (unused) GID
      int32_t _candidate_gid() const;

    private:

      // Management:
      bool _initialized_; //!< Initialization flag

      // Configuration attributes:
      int32_t        _standard_user_min_uid_;  //!< Minimum UID for a standard user
      int32_t        _standard_group_min_gid_; //!< Minimum GID for a standard group
      std::string    _users_table_path_;       //!< Path to the users table storage
      std::string    _groups_table_path_;      //!< Path to the groups table storage
      bool           _dont_load_tables_;       //!< Flag to load tables at initialization
      bool           _dont_store_tables_;      //!< Flag to store tables at reset
      bool           _dont_backup_tables_;     //!< Flag to not backup tables

      // Working data:
      user_dict_type  _users_;  //!< Dictionary of users
      group_dict_type _groups_; //!< Dictionary of groups
#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(manager);

    };

  } // namespace user

} // namespace vire

// OCD support:
#include <datatools/ocd_macros.h>
// @arg vire::user::manager the name of the class with OCD support
DOCD_CLASS_DECLARATION(vire::user::manager)

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::user::manager);
#endif // Q_MOC_RUN

#endif // VIRE_USER_MANAGER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
