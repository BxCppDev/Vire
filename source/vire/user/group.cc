//! \file vire/user/group.cc
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
#include <vire/user/group.h>

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>
// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/factory.h>
// - Bayeux/cuts:
#include <cuts/i_cut.h>
#include <cuts/accept_cut.h>
#include <cuts/not_cut.h>
#include <cuts/and_cut.h>
#include <cuts/or_cut.h>
#include <cuts/xor_cut.h>
#include <cuts/nor_cut.h>
#include <cuts/nand_cut.h>
#include <cuts/multi_and_cut.h>
#include <cuts/multi_or_cut.h>
#include <cuts/multi_xor_cut.h>

// This project:
#include <vire/user/user.h>
#include <vire/user/enumerated_user_selector.h>
#include <vire/user/manager.h>

namespace vire {

  namespace user {

    DATATOOLS_SERIALIZATION_IMPLEMENTATION_ADVANCED(group, "vire::user::group")

    // static
    const std::string & group::admin_group()
    {
      static const std::string _l("admin");
      return _l;
    }

    group::group()
    {
      _set_defaults();
      return;
    }

    group::~group()
    {
      reset();
      return;
    }

    bool group::is_valid() const
    {
      return has_valid_gid() && has_name() && has_user_selector();
    }

    bool group::has_valid_gid() const
    {
      return _gid_ != INVALID_GID;
    }

    void group::set_gid(int32_t gid_)
    {
      DT_THROW_IF(gid_ < INVALID_GID,
                  std::logic_error,
                  "Invalid GID [" << gid_ << "]!");
      _gid_ = gid_;
      return;
    }

    int32_t group::get_gid() const
    {
      return _gid_;
    }

    bool group::has_uid(int32_t uid_) const
    {
      cuts::i_cut & active_selector = const_cast<cuts::i_cut &>(_users_selector_handle_.get());
      active_selector.set_user_data<int32_t>(uid_);
      return active_selector.process() == cuts::SELECTION_ACCEPTED;
    }

    void group::build_set_of_uids(const std::set<int32_t> & universe_,
                                  std::set<int32_t> & uids_) const
    {
      for (auto uid : universe_) {
        if (has_uid(uid)) {
          uids_.insert(uid);
        }
      }
      return;
    }

    std::set<int32_t> group::set_of_uids(const std::set<int32_t> & universe_) const
    {
      std::set<int32_t> uids;
      build_set_of_uids(universe_, uids);
      return uids;
    }

    bool group::has_user_selector() const
    {
      return _users_selector_handle_;
    }

    void group::set_user_selector_handle(cuts::cut_handle_type & user_selector_handle_)
    {
      if (!user_selector_handle_.grab().is_initialized()) {
        if (! user_selector_handle_.grab().has_description() ||
            user_selector_handle_.grab().get_description() != user_selector_handle_.grab().get_type_id()) {
          user_selector_handle_.grab().set_description(user_selector_handle_.grab().get_type_id());
        }
      }
      _users_selector_handle_ = user_selector_handle_;
      return;
    }

    const cuts::cut_handle_type & group::get_user_selector_handle() const
    {
      return _users_selector_handle_;
    }

    cuts::cut_handle_type & group::grab_user_selector_handle()
    {
      return _users_selector_handle_;
    }

    const cuts::i_cut & group::get_user_selector() const
    {
      return _users_selector_handle_.get();
    }

    void group::_set_defaults()
    {
      set_logging_priority(datatools::logger::PRIO_FATAL);
      _gid_ = INVALID_GID;
      return;
    }

    void group::check() const
    {
      DT_THROW_IF(!has_valid_gid(), std::logic_error, "Missing a valid GID!");
      DT_THROW_IF(!has_name(), std::logic_error, "User group with GID=[" << _gid_ << "] has no name!");
      DT_THROW_IF(!has_user_selector(), std::logic_error, "User group with GID=[" << _gid_ << "] has no user selector!");
      return;
    }

    // static
    cuts::i_cut::factory_register_type & group::user_selector_factory_register()
    {
      static boost::scoped_ptr<cuts::i_cut::factory_register_type> _usfr;
      if (! _usfr) {
        _usfr.reset(new cuts::i_cut::factory_register_type);
        _usfr->set_label("group/user_selector_factory");
        _usfr->registration<cuts::accept_cut>("cuts::accept_cut");
        _usfr->registration<cuts::not_cut>("cuts::not_cut");
        _usfr->registration<cuts::and_cut>("cuts::and_cut");
        _usfr->registration<cuts::or_cut>("cuts::or_cut");
        _usfr->registration<cuts::nor_cut>("cuts::nor_cut");
        _usfr->registration<cuts::xor_cut>("cuts::xor_cut");
        _usfr->registration<cuts::nand_cut>("cuts::nand_cut");
        _usfr->registration<cuts::multi_and_cut>("cuts::multi_and_cut");
        _usfr->registration<cuts::multi_or_cut>("cuts::multi_or_cut");
        _usfr->registration<cuts::multi_xor_cut>("cuts::multi_xor_cut");
        _usfr->registration<vire::user::enumerated_user_selector>("vire::user::enumerated_user_selector");
      }
      return *_usfr.get();
    }

    void group::initialize_simple()
    {
      const datatools::properties dummy_config;
      initialize_standalone(dummy_config);
      return;
    }

    void group::initialize_standalone(const datatools::properties & config_)
    {
      manager dummy_mgr; // not initialized
      initialize(config_, dummy_mgr);
      return;
    }

    void group::initialize(const datatools::properties & config_,
                           manager & user_mgr_)
    {
      this->datatools::enriched_base::initialize(config_, false);

      if (! has_valid_gid()) {
        DT_THROW_IF(!config_.has_key("gid"),
                    std::logic_error,
                    "Missing 'gid' property!");
        set_gid(config_.fetch_integer("gid"));
      }

      if (! has_user_selector()) {
        // DT_THROW_IF(!config_.has_key("selector_type_id"),
        //             std::logic_error,
        //             "Missing 'selector_type_id' property!");

        std::string selector_type_id;
        if (config_.has_key("selector_type_id")) {
          selector_type_id = config_.fetch_string("selector_type_id");
        }
        if (selector_type_id.empty()) {
          // Default to vire::user::enumerated_user_selector :
          selector_type_id = "vire::user::enumerated_user_selector";
        }

        datatools::properties selector_config;
        config_.export_and_rename_starting_with(selector_config, "selector.", "");

        // Selector factory:
        cuts::i_cut::factory_register_type & facreg = user_selector_factory_register();
        DT_THROW_IF(! facreg.has(selector_type_id),
                    std::logic_error,
                    "Cannot find user selector/cut factory with ID '"
                    << selector_type_id << "' for user group with GID=[" << get_gid() << "]!");
        const cuts::i_cut::factory_register_type::factory_type & the_factory
          = facreg.get(selector_type_id);
        cuts::cut_handle_type user_selector_handle(the_factory());
        std::ostringstream usname_oss;
        usname_oss << get_gid();
        user_selector_handle.grab().set_name(usname_oss.str());
        // Store the cut/selector type id in the description string:
        user_selector_handle.grab().set_description(selector_type_id);
        if (selector_type_id == "vire::user::enumerated_user_selector") {
          vire::user::enumerated_user_selector & selector =
            dynamic_cast<vire::user::enumerated_user_selector &>(user_selector_handle.grab());
          if (user_mgr_.is_initialized()) {
            selector.set_user_mgr(user_mgr_);
          }
        }

        {
          // Build the dictionary of user selector cut entries from the dictionary of user groups:
          cuts::cut_handle_dict_type selectors_dict;
          if (user_mgr_.is_initialized()) {
            group_dict_type & groups
              = const_cast<group_dict_type &>(user_mgr_.get_groups());
            for (auto ug : groups) {
              int32_t a_gid = ug.first;
              std::ostringstream a_gid_name;
              a_gid_name << a_gid;
              {
                cuts::cut_entry_type new_cut_entry;
                selectors_dict[a_gid_name.str()] = new_cut_entry;
              }
              cuts::cut_entry_type & cut_entry = selectors_dict.find(a_gid_name.str())->second;
              cut_entry.set_cut_name(a_gid_name.str());
              cut_entry.set_cut_id(selector_type_id);
              cut_entry.grab_cut_handle() = ug.second.grab().grab_user_selector_handle();
            }
          }
          user_selector_handle.grab().initialize_without_service(selector_config, selectors_dict);
        }

        set_user_selector_handle(user_selector_handle);
      }

      check();

      return;
    }

    void group::export_to_config(datatools::properties & config_,
                                 uint32_t flags_,
                                 const std::string & prefix_) const
    {
      this->datatools::enriched_base::export_to_config(config_, flags_, prefix_);

      if (flags_ & GROUP_XC_GID) {
        config_.store_integer(prefix_ + "gid", get_gid());
      }

      if (flags_ & GROUP_XC_SELECTOR) {
        if (has_user_selector()) {
          config_.store_string(prefix_ + "selector_type_id", get_user_selector().get_description(),
                               "Type of the user selector");
          datatools::properties selector_config;
          get_user_selector().export_to_config(config_, 0, prefix_ + "selector.");
        }
      }

      return;
    }

    void group::reset()
    {
      _users_selector_handle_.reset();
      this->datatools::enriched_base::clear();
      _set_defaults();
      return;
    }

    void group::tree_dump(std::ostream & out_,
                               const std::string & title_,
                               const std::string & indent_,
                               bool inherit_) const
    {
      this->datatools::enriched_base::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "GID : ";
      if (has_valid_gid()) {
        out_ << _gid_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "User selector : ";
      if (has_user_selector()) {
        out_ << "'" << get_user_selector().get_name() << "'"
             << " (of type '" << get_user_selector().get_description() << "')";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Validity : " << is_valid() << std::endl;

      return;
    }

  } // namespace user

} // namespace vire
