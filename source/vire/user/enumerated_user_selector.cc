// vire/user/enumerated_user_selector.cc - Implementation of a selector of users with explicit list of user identifiers
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

// Ourselves
#include <vire/user/enumerated_user_selector.h>

// This project:
#include <vire/user/user.h>
#include <vire/user/group.h>
#include <vire/user/manager.h>

namespace vire {

  namespace user {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(enumerated_user_selector, "vire::user::enumerated_user_selector");

    enumerated_user_selector::enumerated_user_selector(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_)
    {
      _user_mgr_ = 0;
      return;
    }

    enumerated_user_selector::~enumerated_user_selector()
    {
      if (is_initialized()) {
        this->enumerated_user_selector::reset();
      }
      return;
    }

    bool enumerated_user_selector::has_user_mgr() const
    {
      return _user_mgr_ != 0;
    }

    void enumerated_user_selector::set_user_mgr(manager & umgr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is initialized and locked!");
      DT_THROW_IF(!umgr_.is_initialized(), std::logic_error,
                  "User manager for cut '" << get_name() << "' is not initialized!");
      _user_mgr_ = &umgr_;
      return;
    }

    void enumerated_user_selector::reset_user_mgr()
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is initialized and locked!");
      _user_mgr_ = 0;
      return;
    }

    int enumerated_user_selector::_accept()
    {
      int result = cuts::SELECTION_ACCEPTED;
      DT_LOG_TRACE(_logging, "Entering...");
      int32_t candidate_uid = user::INVALID_UID;
      if (this->cuts::i_cut::is_user_data<int32_t>()) {
        candidate_uid = this->cuts::i_cut::get_user_data<int32_t>();
      } else if (this->cuts::i_cut::is_user_data<user>()) {
        candidate_uid = this->cuts::i_cut::get_user_data<user>().get_uid();
      }
      if (candidate_uid <= user::INVALID_UID) {
        result = cuts::SELECTION_REJECTED;
      } else if (_uids_.count(candidate_uid) == 0)  {
        result = cuts::SELECTION_REJECTED;
      }
      DT_LOG_TRACE(_logging, "Exiting.");
      return result;
    }

    void enumerated_user_selector::initialize(const datatools::properties & config_,
                                              datatools::service_manager & /* service_manager_ */,
                                              cuts::cut_handle_dict_type & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized !");

      _common_initialize(config_);

      /*
      // Not implemented:
      vire::user_service * userv_ptr = 0;
      if (service_manager_.is_a<vire::user::service>("Users")) {
      vire::user::service & userv = service_manager_.grab<vire::user::service>("Users");
      set_user_mgr(userv->grab_user_manager());
      }
      */

      if (_uids_.empty()) {
        if (config_.has_key("uids")) {
          std::vector<int32_t> uids;
          config_.fetch("uids", uids);
          for (auto uid : uids) {
            add_uid(uid);
          }
        }
      }

      _set_initialized(true);
      return;
    }

    void enumerated_user_selector::reset()
    {
      _set_initialized(false);
      _uids_.clear();
      reset_user_mgr();
      this->cuts::i_cut::_reset();
      return;
    }

    bool enumerated_user_selector::has_uid(int32_t uid_) const
    {
      return _uids_.count(uid_) == 1;
    }

    void enumerated_user_selector::add_uid(int32_t uid_)
    {
      DT_THROW_IF(has_uid(uid_), std::logic_error,
                  "User with UID=[" << uid_ << "] already belongs to this enumeration!");
      if (has_user_mgr()) {
        DT_THROW_IF(!_user_mgr_->has_user_by_uid(uid_),
                    std::logic_error,
                    "No user with UID=[" << uid_ << "] exists in the user manager!");
      }
      _uids_.insert(uid_);
      return;
    }

    void enumerated_user_selector::remove_uid(int32_t uid_)
    {
      DT_THROW_IF(!has_uid(uid_), std::logic_error,
                  "User with UID=[" << uid_ << "] does not belong to this enumeration!")
        _uids_.erase(_uids_.find(uid_));
      return;
    }

    const std::set<int32_t> & enumerated_user_selector::get_uids() const
    {
      return _uids_;
    }

    void enumerated_user_selector::export_to_config(datatools::properties & config_,
                                                    uint32_t flags_,
                                                    const std::string & prefix_) const
    {
      this->cuts::i_cut::export_to_config(config_, flags_, prefix_);

      std::vector<int32_t> uids;
      for (auto uid : _uids_) {
        uids.push_back(uid);
      }
      config_.store(prefix_ + "uids", uids, "The list of user identifiers (UIDs)");

      return;
    }

    // virtual
    void enumerated_user_selector::tree_dump(std::ostream & out_,
                                             const std::string & title_,
                                             const std::string & indent_,
                                             bool inherit_) const
    {
      this->cuts::i_cut::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "User manager : ";
      if (has_user_mgr()) {
        out_ << _user_mgr_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "UIDs : ";
      if (_uids_.size()) {
        out_ << _uids_.size();
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // namespace user

} // namespace vire
