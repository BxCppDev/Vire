//! \file vire/user/manager.cc
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

// Ourselves:
#include <vire/user/manager.h>

// Third party:
// - Boost:
#include <boost/filesystem/operations.hpp>
#include <boost/system/error_code.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/multi_properties.h>

// This project:
#include <vire/user/enumerated_user_selector.h>

namespace vire {

  namespace user {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(manager, "vire::user::manager");

    // static
    const int32_t manager::DEFAULT_STANDARD_USER_MIN_UID;
    const int32_t manager::DEFAULT_STANDARD_GROUP_MIN_GID;

    manager::manager(uint32_t flags_)
      : datatools::base_service("Users", "User manager service", "")
    {
      _initialized_ = false;
      _set_defaults();
      if (flags_ & LOG_TRACE) {
        set_logging_priority(datatools::logger::PRIO_TRACE);
      }
      if (flags_ & DONT_LOAD_TABLES) {
        set_dont_load_tables(true);
      }
      if (flags_ & DONT_STORE_TABLES) {
        set_dont_store_tables(true);
      }
      if (flags_ & DONT_BACKUP_TABLES) {
        set_dont_backup_tables(true);
      }
      return;
    }

    manager::~manager()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    int32_t manager::get_standard_user_min_uid() const
    {
      return _standard_user_min_uid_;
    }

    int32_t manager::get_standard_group_min_gid() const
    {
      return _standard_group_min_gid_;
    }

    void manager::set_standard_user_min_uid(int32_t muid_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "User manager is already initialized!");
      DT_THROW_IF(muid_ <= user::ADMIN_UID, std::domain_error,
                  "Invalid standard user minimium UID!");
      _standard_user_min_uid_ = muid_;
      return;
    }

    void manager::set_standard_group_min_gid(int32_t mgid_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "User manager is already initialized!");
      DT_THROW_IF(mgid_ <= group::ADMIN_GID, std::domain_error,
                  "Invalid standard user group minimium GID!");
      _standard_group_min_gid_ = mgid_;
      return;
    }

    bool manager::has_users_table_path() const
    {
      return !_users_table_path_.empty();
    }

    bool manager::has_groups_table_path() const
    {
      return !_groups_table_path_.empty();
    }

    const std::string & manager::get_users_table_path() const
    {
      return _users_table_path_;
    }

    const std::string & manager::get_groups_table_path() const
    {
      return _groups_table_path_;
    }

    void manager::set_users_table_path(const std::string & ut_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "User manager is already initialized!");
      _users_table_path_ = ut_;
      return;
    }

    void manager::set_groups_table_path(const std::string & gt_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "User manager is already initialized!");
      _groups_table_path_ = gt_;
      return;
    }

    bool manager::dont_load_tables() const
    {
      return _dont_load_tables_;
    }

    bool manager::is_load_tables() const
    {
      return _dont_load_tables_ == false;
    }

    bool manager::dont_store_tables() const
    {
      return _dont_store_tables_;
    }

    bool manager::is_store_tables() const
    {
      return _dont_store_tables_ == false;
    }

    bool manager::dont_backup_tables() const
    {
      return _dont_backup_tables_;
    }

    bool manager::is_backup_tables() const
    {
      return _dont_backup_tables_ == false;
    }

    void manager::set_dont_load_tables(bool flag_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "User manager is already initialized!");
      _dont_load_tables_ = flag_;
      DT_LOG_TRACE(get_logging_priority(), "Set dont_load_tables = " << _dont_load_tables_);
      return;
    }

    void manager::set_dont_store_tables(bool flag_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "User manager is already initialized!");
      _dont_store_tables_ = flag_;
      DT_LOG_TRACE(get_logging_priority(), "Set dont_store_tables = " << _dont_store_tables_);
      return;
    }

    void manager::set_dont_backup_tables(bool flag_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "User manager is already initialized!");
      _dont_backup_tables_ = flag_;
      DT_LOG_TRACE(get_logging_priority(), "Set dont_backup_tables = " << _dont_backup_tables_);
      return;
    }

    /*
    bool manager::is_backup_tables() const
    {
      return _backup_tables_;
    }

    void manager::set_dont_backup_tables(bool flag_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "User manager is already initialized!");
      _dont_backup_tables_ = flag_;
      return;
    }
    */

    bool manager::has_user_by_uid(int32_t uid_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      return _has_user_by_uid(uid_);
    }

    bool manager::has_group_by_gid(int32_t gid_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      return _has_group_by_gid(gid_);
    }

    bool manager::_has_user_by_uid(int32_t uid_) const
    {
      return _users_.find(uid_) != _users_.end();
    }

    bool manager::_has_group_by_gid(int32_t gid_) const
    {
      return _groups_.find(gid_) != _groups_.end();
    }

    bool manager::has_user_by_name(const std::string & name_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      return _has_user_by_name(name_);
    }

    bool manager::has_group_by_name(const std::string & gname_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      return _has_group_by_name(gname_);
    }

    bool manager::_has_user_by_name(const std::string & name_) const
    {
      for (auto u: _users_) {
        if (u.second.get().get_name() == name_) return true;
      }
      return false;
    }

    bool manager::_has_group_by_name(const std::string & gname_) const
    {
      for (auto g: _groups_) {
        if (g.second.get().get_name() == gname_) return true;
      }
      return false;
    }

    const user & manager::get_user_by_uid(int32_t uid_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      user_dict_type::const_iterator found = _users_.find(uid_);
      DT_THROW_IF (found == _users_.end(), std::logic_error,
                   "Cannot find user with UID=[" << uid_ << "]!");
      return found->second.get();
    }

    const group & manager::get_group_by_gid(int32_t gid_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      group_dict_type::const_iterator found = _groups_.find(gid_);
      DT_THROW_IF (found == _groups_.end(), std::logic_error,
                   "Cannot find user group with GID=[" << gid_ << "]!");
      return found->second.get();
    }

    user & manager::grab_user_by_uid(int32_t uid_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      user_dict_type::iterator found = _users_.find(uid_);
      DT_THROW_IF (found == _users_.end(), std::logic_error,
                   "Cannot find user with UID=[" << uid_ << "]!");
      return found->second.grab();
    }

    group & manager::grab_group_by_gid(int32_t gid_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      group_dict_type::iterator found = _groups_.find(gid_);
      DT_THROW_IF (found == _groups_.end(), std::logic_error,
                   "Cannot find user group with GID=[" << gid_ << "]!");
      return found->second.grab();
    }

    const user & manager::get_user_by_name(const std::string & name_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      DT_THROW_IF(!has_user_by_name(name_),
                  std::logic_error,
                  "User with name '" << name_ << "' does not exist!");
      for (const auto & u : _users_) {
        if (u.second.get().get_name() == name_) return u.second.get();
      }
      DT_THROW(std::logic_error, "User with name '" << name_ << "' does not exist!");
    }

    const group & manager::get_group_by_name(const std::string & gname_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      DT_THROW_IF(!has_group_by_name(gname_),
                  std::logic_error,
                  "Group with name '" << gname_ << "' does not exist!");
      for (const auto & ug : _groups_) {
        if (ug.second.get().get_name() == gname_) return ug.second.get();
      }
      DT_THROW(std::logic_error, "Group with name '" << gname_ << "' does not exist!");
    }

    user & manager::grab_user_by_name(const std::string & name_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      DT_THROW_IF(!has_user_by_name(name_),
                  std::logic_error,
                  "User with name '" << name_ << "' does not exist!");
      for (auto & u : _users_) {
        if (u.second.get().get_name() == name_) return u.second.grab();
      }
      DT_THROW(std::logic_error, "User with name '" << name_ << "' does not exist!");
    }

    group & manager::grab_group_by_name(const std::string & gname_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      DT_THROW_IF(!has_group_by_name(gname_),
                  std::logic_error,
                  "Group with name '" << gname_ << "' does not exist!");
      for (auto & ug : _groups_) {
        if (ug.second.get().get_name() == gname_) return ug.second.grab();
      }
      DT_THROW(std::logic_error, "Group with name '" << gname_ << "' does not exist!");
    }

    user & manager::create_new_user(const datatools::properties & user_config_, int32_t uid_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      return _create_new_user(user_config_, uid_);
    }

    user & manager::create_new_user_by_name(const std::string & user_name_,
                                            int32_t uid_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      datatools::properties user_config;
      user_config.store_string("name", user_name_);
      return _create_new_user(user_config, uid_);
    }

    group & manager::create_new_group(const datatools::properties & group_config_,
                                      int32_t gid_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      return _create_new_group(group_config_, gid_);
    }

    group & manager::create_new_group_by_name(const std::string & group_name_,
                                              int32_t gid_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      datatools::properties group_config;
      group_config.store_string("name", group_name_);
      return _create_new_group(group_config, gid_);
    }

    user & manager::_create_new_user(const datatools::properties & user_config_, int32_t uid_)
    {
      int32_t uid = uid_;
      if (uid != user::INVALID_UID) {
        DT_THROW_IF(_has_user_by_uid(uid), std::logic_error,
                    "User with UID=[" << uid << "] already exists!");
      } else {
        uid = _candidate_uid();
      }
      {
        user_handle_type new_user;
        new_user.reset(new user);
        new_user.grab().set_uid(uid);
        new_user.grab().initialize(user_config_);
        DT_THROW_IF(_has_user_by_name(new_user.get().get_name()), std::logic_error,
                    "User '" << new_user.get().get_name() << "' already exists!");
        _users_[uid] = new_user;
      }
      return _users_.find(uid)->second.grab();
    }

    group & manager::_create_new_group(const datatools::properties & group_config_,
                                       int32_t gid_)
    {
      DT_LOG_DEBUG(get_logging_priority(), "Requested GID = [" << gid_ << "]");
      int32_t gid = gid_;
      if (gid != group::INVALID_GID) {
        DT_THROW_IF(_has_group_by_gid(gid), std::logic_error,
                    "Group with GID=[" << gid << "] already exists!");
      } else {
        gid = _candidate_gid();
      }
      DT_LOG_DEBUG(get_logging_priority(), "Candidate GID = [" << gid << "]");
      {
        group_handle_type new_group;
        new_group.reset(new group);
        new_group.grab().set_gid(gid);
        new_group.grab().initialize(group_config_, *this);
        DT_THROW_IF(_has_group_by_name(new_group.get().get_name()), std::logic_error,
                    "Group with name '" << new_group.get().get_name() << "' already exists!");
        _groups_[gid] = new_group;
        DT_LOG_DEBUG(get_logging_priority(), "Group '" << new_group.get().get_name() << "' with GID = [" << gid << "] is created!");
      }
      return _groups_.find(gid)->second.grab();
    }

    void manager::remove_user_by_uid(int32_t uid_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      _remove_user(uid_);
      return;
    }

    void manager::remove_group_by_gid(int32_t gid_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      group_dict_type::iterator found = _groups_.find(gid_);
      DT_THROW_IF(found == _groups_.end(), std::logic_error,
                  "Group with GID=[" << gid_ << "] does not exist!");
      _groups_.erase(found);
      return;
    }

    void manager::remove_user_by_name(const std::string & name_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      int32_t removed_uid = user::INVALID_UID;
      for (user_dict_type::iterator i = _users_.begin();
           i != _users_.end();
           i++) {
        if (i->second.get().get_name() == name_) {
          removed_uid = i->first;
          break;
        }
      }
      DT_THROW_IF(removed_uid == user::INVALID_UID,
                  std::logic_error,
                  "User with name '" << name_ << "' does not exist!");
      _remove_user(removed_uid);
      return;
    }

    void manager::remove_group_by_name(const std::string & gname_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      int32_t removed_gid = group::INVALID_GID;
      for (group_dict_type::iterator i = _groups_.begin();
           i != _groups_.end();
           i++) {
        if (i->second.get().get_name() == gname_) {
          removed_gid = i->first;
          break;
        }
      }
      DT_THROW_IF(removed_gid == group::INVALID_GID,
                  std::logic_error,
                  "Group with name '" << gname_ << "' does not exist!");
      _remove_group(removed_gid);
      return;
    }

    void manager::build_list_of_gids_with_user(int32_t uid_,
                                               std::vector<int32_t> & gids_) const
    {
     DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      gids_.clear();
      for (auto g : _groups_) {
        const user & u = get_user_by_uid(uid_);
        if (g.second.get().has_uid(uid_)) {
          gids_.push_back(g.first);
        }
      }
      return;
    }

    void manager::build_list_of_uids_in_group(int32_t gid_,
                                              std::vector<int32_t> & uids_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      uids_.clear();
      for (auto u : _users_) {
        const group & ug = get_group_by_gid(gid_);
        if (ug.has_uid(u.first)) {
          uids_.push_back(u.first);
        }
      }
      return;
    }

    void manager::build_list_of_uids(std::vector<int32_t> & uids_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      uids_.clear();
      uids_.reserve(_users_.size());
      for (auto u : _users_) {
        uids_.push_back(u.first);
      }
      return;
    }

    void manager::build_list_of_gids(std::vector<int32_t> & gids_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      gids_.clear();
      gids_.reserve(_groups_.size());
      for (auto g : _groups_) {
        gids_.push_back(g.first);
      }
      return;
    }

    std::vector<int32_t> manager::list_of_uids() const
    {
      std::vector<int32_t> l;
      build_list_of_uids(l);
      return l;
    }

    std::vector<int32_t> manager::list_of_gids() const
    {
      std::vector<int32_t> l;
      build_list_of_gids(l);
      return l;
    }

    void manager::build_list_of_user_names(std::vector<std::string> & user_names_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      user_names_.clear();
      user_names_.reserve(_users_.size());
      for (auto u : _users_) {
        user_names_.push_back(u.second.get().get_name());
      }
      return;
    }

    void manager::build_list_of_group_names(std::vector<std::string> & group_names_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      group_names_.clear();
      group_names_.reserve(_groups_.size());
      for (auto g : _groups_) {
        group_names_.push_back(g.second.get().get_name());
      }
      return;
    }

    std::vector<std::string> manager::list_of_user_names() const
    {
      std::vector<std::string> l;
      build_list_of_user_names(l);
      return l;
    }

    std::vector<std::string> manager::list_of_group_names() const
    {
      std::vector<std::string> l;
      build_list_of_group_names(l);
      return l;
    }

    void manager::_set_defaults()
    {
      _standard_user_min_uid_ = user::INVALID_UID;
      _standard_group_min_gid_ = group::INVALID_GID;
      _dont_load_tables_ = false;
      _dont_store_tables_ = false;
      _dont_backup_tables_ = false;
      _users_table_path_ = "";
      _groups_table_path_ = "";
      set_logging_priority(datatools::logger::PRIO_FATAL);
      return;
    }

    int32_t manager::_candidate_uid() const
    {
      int32_t last_cuid = user::INVALID_UID;
      for (auto u : _users_) {
        if (u.second.get().get_uid() >= _standard_user_min_uid_) {
          last_cuid = u.second.get().get_uid();
        }
      }
      if (last_cuid == user::INVALID_UID) {
        return _standard_user_min_uid_;
      }
      return ++last_cuid;
    }

    int32_t manager::_candidate_gid() const
    {
      int32_t last_cgid = group::INVALID_GID;
      for (auto g : _groups_) {
        if (g.second.get().get_gid() >= _standard_group_min_gid_) {
          last_cgid = g.second.get().get_gid();
        }
      }
      if (last_cgid == group::INVALID_GID) {
        return _standard_group_min_gid_;
      }
      return ++last_cgid;
    }

    void manager::load_tables(uint32_t load_flags_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      // First load users:
      remove_all_users();
      DT_THROW_IF(!has_users_table_path(),
                  std::logic_error,
                  "No users table path is defined!");
      _load_users_table(_users_table_path_, load_flags_);
      // Then load user groups:
      remove_all_groups();
      DT_THROW_IF(!has_groups_table_path(),
                  std::logic_error,
                  "No user groups table path is defined!");
      _load_groups_table(_groups_table_path_, load_flags_);
      return;
    }

    void manager::store_tables(uint32_t store_flags_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      DT_THROW_IF(!has_users_table_path(),
                  std::logic_error,
                  "No users table path is defined!");
      _store_users_table(_users_table_path_, store_flags_);
      DT_THROW_IF(!has_groups_table_path(),
                  std::logic_error,
                  "No groups table path is defined!");
      _store_groups_table(_groups_table_path_, store_flags_);
      return;
    }

    void manager::remove_all_users()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      _users_.clear();
      return;
    }

    void manager::remove_all_groups()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      _groups_.clear();
      return;
    }

    bool manager::is_user_valid(int32_t uid_) const
    {
      DT_THROW_IF(!has_user_by_uid(uid_), std::logic_error,
                  "User with UID=[" << uid_ << "] does not exist!");
      const user & u = get_user_by_uid(uid_);
      return u.is_valid();
    }

    bool manager::is_group_valid(int32_t gid_) const
    {
      DT_THROW_IF(!has_group_by_gid(gid_), std::logic_error,
                  "Group with GID=[" << gid_ << "] does not exist!");
      const group & ug = get_group_by_gid(gid_);
      return ug.is_valid();
    }

    bool manager::is_group_with_enumeration(int32_t gid_) const
    {
      DT_THROW_IF(!has_group_by_gid(gid_), std::logic_error,
                  "Group with GID=[" << gid_ << "] does not exist!");
      const group & ug = get_group_by_gid(gid_);
      DT_THROW_IF(!ug.has_user_selector(), std::logic_error,
                  "Group with GID=[" << gid_ << "] has no user selector!");
      const cuts::i_cut & usel = ug.get_user_selector();
      if (usel.get_type_id() == "vire::user::enumerated_user_selector") {
        return true;
      }
      return false;
    }

    void manager::add_user_in_group(int32_t uid_, int32_t gid_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      _add_user_in_group(uid_, gid_);
      return;
    }

    void manager::_add_user_in_group(int32_t uid_, int32_t gid_)
    {
      DT_THROW_IF(!has_user_by_uid(uid_), std::logic_error,
                  "User with UID=[" << uid_ << "] does not exist!");
      DT_THROW_IF(!has_group_by_gid(gid_), std::logic_error,
                  "Group with GID=[" << gid_ << "] does not exist!");
      group & ug = grab_group_by_gid(gid_);
      DT_THROW_IF(!ug.has_user_selector(), std::logic_error,
                  "Group with GID=[" << gid_ << "] has no user selector!");
      cuts::i_cut & usel = const_cast<cuts::i_cut &>(ug.get_user_selector());
      DT_THROW_IF(usel.get_type_id() != "vire::user::enumerated_user_selector",
                  std::logic_error,
                  "Group with GID=[" << gid_ << "] has no 'enumerated' user selector!");
      vire::user::enumerated_user_selector & enum_selector
        = dynamic_cast<vire::user::enumerated_user_selector &>(usel);
      enum_selector.add_uid(uid_);
      return;
    }

    void manager::remove_user_from_group(int32_t uid_, int32_t gid_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "User manager is not initialized!");
      _remove_user_from_group(uid_, gid_);
      return;
    }

    void manager::_remove_user(int32_t uid_)
    {
      user_dict_type::iterator found = _users_.find(uid_);
      DT_THROW_IF(found == _users_.end(), std::logic_error,
                  "User with UID=[" << uid_ << "] does not exist!");
      std::vector<int32_t> gids;
      build_list_of_gids_with_user(uid_,gids);
      for (int i = 0; i < (int) gids.size(); i++) {
        int32_t gid = gids[i];
        if (is_group_with_enumeration(gid)) {
          _remove_user_from_group(uid_, gid);
        }
      }
      _users_.erase(found);
      return;
    }

    void manager::_remove_group(int32_t gid_)
    {
      group_dict_type::iterator found = _groups_.find(gid_);
      DT_THROW_IF(found == _groups_.end(), std::logic_error,
                  "Group with GID=[" << gid_ << "] does not exist!");
      _groups_.erase(found);
      return;
    }

    void manager::_remove_user_from_group(int32_t uid_, int32_t gid_)
    {
      DT_THROW_IF(!has_user_by_uid(uid_), std::logic_error,
                  "User with UID=[" << uid_ << "] does not exist!");
      DT_THROW_IF(!has_group_by_gid(gid_), std::logic_error,
                  "Group with GID=[" << gid_ << "] does not exist!");
      group & ug = grab_group_by_gid(gid_);
      DT_THROW_IF(!ug.has_user_selector(), std::logic_error,
                  "Group with GID=[" << gid_ << "] has no user selector!");
      cuts::i_cut & usel = const_cast<cuts::i_cut &>(ug.get_user_selector());
      DT_THROW_IF(usel.get_type_id() != "vire::user::enumerated_user_selector",
                  std::logic_error,
                  "Group with GID=[" << gid_ << "] has no 'enumerated' user selector!");
      vire::user::enumerated_user_selector & enum_selector
        = dynamic_cast<vire::user::enumerated_user_selector &>(usel);
      enum_selector.remove_uid(uid_);
      /*
        if (ug.is_enumerated()) {
        user & u = grab_user_by_uid(uid_);
        u.add_group(gid_);
        user & u = grab_user_by_uid(uid_);
        u.remove_group(gid_);
        }*/
      return;
    }

    bool manager::is_initialized() const
    {
      return _initialized_;
    }

    // virtual
    int manager::initialize(const datatools::properties & config_,
                            datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Manager is already initialized!");

      this->::datatools::base_service::common_initialize(config_);

      // Fetch configuration parameters:
      if (_standard_user_min_uid_ == ::vire::user::user::INVALID_UID) {
        if (config_.has_key("standard_user_min_uid")) {
          set_standard_user_min_uid(config_.fetch_integer("standard_user_min_uid"));
        }
      }

      if (_standard_group_min_gid_ == ::vire::user::group::INVALID_GID) {
        if (config_.has_key("standard_group_min_gid")) {
          set_standard_group_min_gid(config_.fetch_integer("standard_group_min_gid"));
        }
      }

      if (_users_table_path_.empty()) {
        if (config_.has_key("users_table_path")) {
          set_users_table_path(config_.fetch_string("users_table_path"));
        }
      }

      if (_groups_table_path_.empty()) {
        if (config_.has_key("groups_table_path")) {
          set_groups_table_path(config_.fetch_string("groups_table_path"));
        }
      }

      if (config_.has_key("dont_load_tables")) {
        set_dont_load_tables(config_.fetch_boolean("dont_load_tables"));
      }

      if (config_.has_key("dont_store_tables")) {
        set_dont_store_tables(config_.fetch_boolean("dont_store_tables"));
      }

      if (is_store_tables()) {
        if (config_.has_key("dont_backup_tables")) {
          set_dont_backup_tables(config_.fetch_boolean("dont_backup_tables"));
        }
      }

      // Set default parameter values:
      if (_standard_user_min_uid_ == user::INVALID_UID) {
        _standard_user_min_uid_ = DEFAULT_STANDARD_USER_MIN_UID;
      }

      if (_standard_group_min_gid_ == group::INVALID_GID) {
        _standard_group_min_gid_ = DEFAULT_STANDARD_GROUP_MIN_GID;
      }

      // Checks:
      DT_THROW_IF(!has_users_table_path(), std::logic_error,
                  "Missing users table path!");

      DT_THROW_IF(!has_groups_table_path(), std::logic_error,
                  "Missing user groups table path!");

      // Initialization:
      if (boost::filesystem::exists(_users_table_path_)) {
        if (is_load_tables()) {
          DT_LOG_DEBUG(get_logging_priority(), "Loading user table...");
          _load_users_table(_users_table_path_, 0);
          DT_LOG_DEBUG(get_logging_priority(), "Loading user table done.");
        }
      } else {
        DT_LOG_DEBUG(get_logging_priority(), "No user table file '" << _users_table_path_ << "' exists!");
      }

      if (boost::filesystem::exists(_groups_table_path_)) {
        if (is_load_tables()) {
          DT_LOG_DEBUG(get_logging_priority(), "Loading group table...");
          _load_groups_table(_groups_table_path_, 0);
          DT_LOG_DEBUG(get_logging_priority(), "Loading group done.");
        }
      } else {
        DT_LOG_DEBUG(get_logging_priority(), "No group table file '" << _groups_table_path_ << "' exists!");
      }

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());

      return datatools::SUCCESS;
    }

    int manager::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Manager is not initialized!");
      _initialized_ = false;
      if (is_store_tables()) {
        _store_groups_table(_groups_table_path_, 0);
      }
      _groups_.clear();
      if (is_store_tables()) {
        _store_users_table(_users_table_path_, 0);
      }
      _users_.clear();
      _set_defaults();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    // virtual
    void manager::_load_users_table(const std::string & source_, uint32_t /* flags_ */)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      std::string source = source_;
      datatools::fetch_path_with_env(source);
      datatools::multi_properties users_table("uid", "type", "Vire users table");
      users_table.read(source);
      std::vector<std::string> uid_keys;
      users_table.ordered_keys(uid_keys);

      for (auto uid_key : uid_keys) {
        const datatools::multi_properties::entry & user_entry = users_table.get(uid_key);
        const std::string & meta = user_entry.get_meta();
        if (meta != "vire::user::user") continue; // Should we throw ?
        std::istringstream uid_iss(user_entry.get_key());
        int uid;
        uid_iss >> uid;
        DT_THROW_IF(!uid_iss, std::logic_error,
                    "Invalid format for UID key ('" << user_entry.get_key() << "')!");
        const datatools::properties & user_config = user_entry.get_properties();
        const user & u = _create_new_user(user_config, uid);
        DT_LOG_DEBUG(get_logging_priority(), "User '" << u.get_name() << "' is loaded with UID=[" << u.get_uid() << "]");
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    // virtual
    void manager::_load_groups_table(const std::string & source_, uint32_t /* flags_ */)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      std::string source = source_;
      datatools::fetch_path_with_env(source);
      datatools::multi_properties groups_table("gid", "type", "Vire user groups table");
      groups_table.read(source);
      std::vector<std::string> gid_keys;
      groups_table.ordered_keys(gid_keys);

      for (auto gid_key : gid_keys) {
        const datatools::multi_properties::entry & group_entry = groups_table.get(gid_key);
        const std::string & meta = group_entry.get_meta();
        if (meta != "vire::user::group") continue; // Should we throw ?
        std::istringstream gid_iss(group_entry.get_key());
        int gid;
        gid_iss >> gid;
        DT_THROW_IF(!gid_iss, std::logic_error,
                    "Invalid format for GID key ('" << group_entry.get_key() << "')!");
        DT_LOG_DEBUG(get_logging_priority(), "Creating group GID=[" << gid << "]...");
        const datatools::properties & group_config = group_entry.get_properties();
        // group_config.tree_dump(std::cerr, "Group config: ", "[debug] ");
        const group & ug = _create_new_group(group_config, gid);
        DT_LOG_DEBUG(get_logging_priority(), "Group '" << ug.get_name() << "' is loaded with GID=[" << ug.get_gid() << "]");
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    // virtual
    void manager::_store_users_table(const std::string & target_, uint32_t /* flags_ */) const
    {
      std::string target = target_;
      datatools::fetch_path_with_env(target);
      datatools::multi_properties users_table("uid", "type", "Vire users table");

      if (boost::filesystem::exists(target)) {
        DT_LOG_DEBUG(get_logging_priority(), "File '" << target << "' exists...");
        DT_THROW_IF(! boost::filesystem::is_regular_file(target),
                    std::runtime_error,
                    "Target '" << target_ << "' is not a regular file!");
        if (is_backup_tables()) {
          DT_LOG_DEBUG(get_logging_priority(), "Backup users table...");
          std::ostringstream backup_target_oss;
          backup_target_oss << target << "~";
          std::string backup_target = backup_target_oss.str();
          std::string tmp_backup_target = backup_target + ".tmp";
          if (boost::filesystem::exists(backup_target)) {
            DT_LOG_DEBUG(get_logging_priority(), "Backuped users tables '" << backup_target << "' exists...");
            if (boost::filesystem::exists(tmp_backup_target)) {
              boost::filesystem::remove(tmp_backup_target);
            }
            DT_LOG_DEBUG(get_logging_priority(), "Rename in safe tmp file '" << tmp_backup_target << "'...");
            boost::filesystem::rename(backup_target, tmp_backup_target);
          }
          boost::system::error_code ec;
          boost::filesystem::copy(boost::filesystem::path(target),
                                  boost::filesystem::path(backup_target),
                                  ec);
          if (ec) {
            boost::filesystem::rename(tmp_backup_target, backup_target);
            DT_LOG_DEBUG(get_logging_priority(), "Restore the safe tmp file '" << tmp_backup_target << "' to '" << backup_target << "'...");
            DT_THROW(std::runtime_error,
                     "Cannot backup the users table '" << backup_target << "' : "
                     << ec.message() << "!");
          } else {
            if (boost::filesystem::exists(tmp_backup_target)) {
              boost::filesystem::remove(tmp_backup_target);
              DT_LOG_DEBUG(get_logging_priority(), "Remove the safe tmp file '" << tmp_backup_target << "'...");
            }
          }
        } else {
          boost::filesystem::remove(target);
        }
      }

      for (auto iu: _users_) {
        int32_t      uid = iu.first;
        // if (uid == user::ADMIN_UID) {
        //   continue;
        // }
        const user & u   = iu.second.get();
        std::ostringstream uid_iss;
        uid_iss << uid;
        users_table.add(uid_iss.str(), "vire::user::user");
        datatools::multi_properties::entry & user_entry = users_table.grab(uid_iss.str());
        u.export_to_config(user_entry.grab_properties(),
                           datatools::enriched_base::EXPORT_CONFIG_DEFAULT
                           | datatools::enriched_base::EXPORT_CONFIG_NAME
                           );
      }
      // users_table.tree_dump(std::cerr, "Users table: ", "DEVEL: ");
      users_table.write(target);
      return;
    }

    // virtual
    void manager::_store_groups_table(const std::string & target_, uint32_t /* flags_ */) const
    {
      std::string target = target_;
      datatools::fetch_path_with_env(target);
      datatools::multi_properties groups_table("gid", "type", "Vire user groups table");

      if (boost::filesystem::exists(target)) {
        DT_LOG_DEBUG(get_logging_priority(), "File '" << target << "' exists...");
        DT_THROW_IF(! boost::filesystem::is_regular_file(target),
                    std::runtime_error,
                    "Target '" << target_ << "' is not a regular file!");
        if (is_backup_tables()) {
          DT_LOG_DEBUG(get_logging_priority(), "Backup groups table...");
          std::ostringstream backup_target_oss;
          backup_target_oss << target << "~";
          std::string backup_target = backup_target_oss.str();
          std::string tmp_backup_target = backup_target + ".tmp";
          if (boost::filesystem::exists(backup_target)) {
            DT_LOG_DEBUG(get_logging_priority(), "Backuped groups table '" << backup_target << "' exists...");
            if (boost::filesystem::exists(tmp_backup_target)) {
              boost::filesystem::remove(tmp_backup_target);
            }
            DT_LOG_DEBUG(get_logging_priority(), "Rename in safe tmp file '" << tmp_backup_target << "'...");
            boost::filesystem::rename(backup_target, tmp_backup_target);
          }
          boost::system::error_code ec;
          boost::filesystem::copy(boost::filesystem::path(target),
                                  boost::filesystem::path(backup_target),
                                  ec);
          if (ec) {
            boost::filesystem::rename(tmp_backup_target, backup_target);
            DT_LOG_DEBUG(get_logging_priority(), "Restore the safe tmp file '" << tmp_backup_target << "' to '" << backup_target << "'...");
            DT_THROW(std::runtime_error,
                     "Cannot backup the users table '" << backup_target << "' : "
                     << ec.message() << "!");
          } else {
            if (boost::filesystem::exists(tmp_backup_target)) {
              boost::filesystem::remove(tmp_backup_target);
              DT_LOG_DEBUG(get_logging_priority(), "Remove the safe tmp file '" << tmp_backup_target << "'...");
            }
          }
        } else {
          boost::filesystem::remove(target);
        }
      }

      for (auto iug : _groups_) {
        int32_t       gid = iug.first;
        // if (gid == group::ADMIN_GID) {
        //   continue;
        // }
        const group & ug  = iug.second.get();
        std::ostringstream gid_iss;
        gid_iss << gid;
        groups_table.add(gid_iss.str(), "vire::user::group");
        datatools::multi_properties::entry & group_entry = groups_table.grab(gid_iss.str());
        ug.export_to_config(group_entry.grab_properties(),
                            datatools::enriched_base::EXPORT_CONFIG_DEFAULT
                            | datatools::enriched_base::EXPORT_CONFIG_NAME
                            | group::GROUP_XC_SELECTOR
                            );
      }
      // groups_table.tree_dump(std::cerr, "Groups table: ", "DEVEL: ");
      groups_table.write(target);

      return;
    }

    const user_dict_type & manager::get_users() const
    {
      return _users_;
    }

    const group_dict_type & manager::get_groups() const
    {
      return _groups_;
    }

    void manager::tree_dump(std::ostream & out_,
                            const std::string & title_,
                            const std::string & indent_,
                            bool inherit_) const
    {
      this->datatools::base_service::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Min UID for standard users : "
           << _standard_user_min_uid_
           << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Min GID for standard user groups : "
           << _standard_group_min_gid_
           << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Users table path : '"
           << _users_table_path_ << "'"
           << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Groups table path : '"
           << _groups_table_path_ << "'"
           << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Don't load tables : '" << _dont_load_tables_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Don't store tables : '" << _dont_store_tables_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Don't backup tables : '" << _dont_backup_tables_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Groups : ";
      if (_groups_.size()) {
        out_ << std::endl;
        for (group_dict_type::const_iterator i = _groups_.begin();
             i != _groups_.end();
             i++) {
          out_ << indent_ << i_tree_dumpable::skip_tag;
          group_dict_type::const_iterator j = i;
          if (++j == _groups_.end()) {
            out_ << i_tree_dumpable::last_tag;
          } else {
            out_ << i_tree_dumpable::tag;
          }
          out_ << "GID=[" << i->second.get().get_gid() << "] : "
               << "Name='" << i->second.get().get_name() << "' "
               << "Selector='" << i->second.get().get_user_selector().get_type_id() << "' "
               << std::endl;
        }
      } else {
        out_ << "<none>" << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Users : ";
      if (_users_.size()) {
        out_ << std::endl;
        for (user_dict_type::const_iterator i = _users_.begin();
             i != _users_.end();
             i++) {
          out_ << indent_ << i_tree_dumpable::last_skip_tag;
          user_dict_type::const_iterator j = i;
          if (++j == _users_.end()) {
            out_ << i_tree_dumpable::last_tag;
          } else {
            out_ << i_tree_dumpable::tag;
          }
          out_ << "UID=[" << i->second.get().get_uid() << "] : "
               << "Name='" << i->second.get().get_name() << "'"
               << std::endl;
        }
      } else {
        out_ << "<none>" << std::endl;
      }

      return;
    }

  } // namespace user

} // namespace vire

// OCD support:
#include <datatools/object_configuration_description.h>

DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(vire::user::manager, ocd_)
{
  // The class name :
  ocd_.set_class_name("vire::user::manager");

  // The class terse description :
  ocd_.set_class_description("The Vire user manager");

  // The library the class belongs to :
  ocd_.set_class_library("Vire");

  // The class detailed documentation :
  ocd_.set_class_documentation("The Vire user manager service embedded in the Vire Server class");

  datatools::base_service::common_ocd(ocd_);

  {
    datatools::configuration_property_description & cpd
      = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("standard_user_min_uid")
      .set_terse_description("The minimum user identifier for standard users")
      .set_traits(datatools::TYPE_INTEGER)
      .set_mandatory(false)
      // .set_long_description("")
      .set_default_value_integer(vire::user::manager::DEFAULT_STANDARD_USER_MIN_UID)
      .add_example("Set the default value::                       \n"
                   "                                              \n"
                   "  standard_user_min_uid : integer = 1000      \n"
                   "                                              \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd
      = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("standard_group_min_gid")
      .set_terse_description("The minimum user group identifier for standard groups")
      .set_traits(datatools::TYPE_INTEGER)
      .set_mandatory(false)
      // .set_long_description("")
      .set_default_value_integer(vire::user::manager::DEFAULT_STANDARD_GROUP_MIN_GID)
      .add_example("Set the default value::                       \n"
                   "                                              \n"
                   "  standard_group_min_gid : integer = 1000     \n"
                   "                                              \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd
      = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("users_table_path")
      .set_terse_description("The file that stores the user table")
      .set_traits(datatools::TYPE_STRING)
      .set_path(true)
      .set_mandatory(true)
      .add_example("Set the path::                                      \n"
                   "                                                    \n"
                   "  users_table_path : string as path = \"users.lis\" \n"
                   "                                                    \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd
      = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("groups_table_path")
      .set_terse_description("The file that stores the user group table")
      .set_traits(datatools::TYPE_STRING)
      .set_path(true)
      .set_mandatory(true)
      .add_example("Set the path::                                        \n"
                   "                                                      \n"
                   "  groups_table_path : string as path = \"groups.lis\" \n"
                   "                                                      \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd
      = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("dont_load_tables")
      .set_terse_description("Flag to not load user/group table files")
      .set_traits(datatools::TYPE_BOOLEAN)
      .set_mandatory(false)
      .set_default_value_boolean(false)
      .add_example("Set the default value::               \n"
                   "                                      \n"
                   "  dont_load_tables : boolean = true   \n"
                   "                                      \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd
      = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("dont_store_tables")
      .set_terse_description("Flag to not store user/group table files")
      .set_traits(datatools::TYPE_BOOLEAN)
      .set_mandatory(false)
      .set_default_value_boolean(false)
      .add_example("Set the default value::               \n"
                   "                                      \n"
                   "  dont_store_tables : boolean = true  \n"
                   "                                      \n"
                   )
      ;
  }

  {
    datatools::configuration_property_description & cpd
      = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("dont_backup_tables")
      .set_terse_description("Flag to not backup user/group table files")
      .set_traits(datatools::TYPE_BOOLEAN)
      .set_mandatory(false)
      .set_default_value_boolean(false)
      .add_example("Set the default value::               \n"
                   "                                      \n"
                   "  dont_backup_tables : boolean = true \n"
                   "                                      \n"
                   )
      ;
  }

  // Additionnal configuration hints :
  ocd_.set_configuration_hints("Here is a full configuration example in the            \n"
                               "``datatools::properties`` ASCII format::               \n"
                               "                                                       \n"
                               "  standard_user_min_uid : integer = 1000               \n"
                               "  standard_user_min_gid : integer = 1000               \n"
                               "  users_table_path  : string as path = \"users.lis\"   \n"
                               "  groups_table_path : string as path = \"groups.lis\"  \n"
                               "  dont_backup_tables : boolean = true                  \n"
                               "                                                       \n"
                               );

  ocd_.set_validation_support(true);

  // Lock the description:
  ocd_.lock();

  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END()

// Registration macro for the class :
DOCD_CLASS_SYSTEM_REGISTRATION(vire::user::manager,"vire::user::manager")
