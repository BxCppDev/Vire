//! \file vire/resource/role.cc
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
#include <vire/resource/role.h>

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
#include <vire/resource/resource.h>
#include <vire/resource/enumerated_resource_selector.h>
#include <vire/resource/regex_resource_selector.h>
#include <vire/resource/ranged_resource_selector.h>
#include <vire/resource/general_expression_resource_selector.h>
#include <vire/resource/by_access_resource_selector.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace resource {

    // static
    const int32_t role::INVALID_ID;

    void role::_copy_from_(const role & role_)
    {
      this->datatools::enriched_base::operator=(role_);
      _initialized_ = role_._initialized_;
      _id_ = role_._id_;
      _path_ = role_._path_;
      _functional_resource_selector_handle_ = role_._functional_resource_selector_handle_;
      _distributable_resource_selector_handle_ = role_._distributable_resource_selector_handle_;
      _build_scope_selector();
      _resource_manager_ = role_._resource_manager_;
      _allowed_users_ = role_._allowed_users_;
      _allowed_groups_ = role_._allowed_groups_;
      if (_cached_scope_resource_ids_.get() != nullptr) {
        _cached_scope_resource_ids_.reset(new std::set<int32_t>(*_cached_scope_resource_ids_.get()));
      }
      if (_cached_distributable_resource_ids_.get() != nullptr) {
        _cached_distributable_resource_ids_.reset(new std::set<int32_t>(*_cached_distributable_resource_ids_.get()));
      }
      if (_cached_functional_resource_ids_.get() != nullptr) {
        _cached_functional_resource_ids_.reset(new std::set<int32_t>(*_cached_functional_resource_ids_.get()));
      }
      return;
    }

    role::role()
    {
      _initialized_ = false;
      _resource_manager_ = nullptr;
      _set_defaults();
      return;
    }

    role::role(const role & role_) : ::datatools::enriched_base(role_)
    {
      _initialized_ = false;
      _resource_manager_ = nullptr;
      _set_defaults();
      role::_copy_from_(role_);
      return;
    }

    role & role::operator=(const role & role_)
    {
      if (this == &role_) return *this;
      this->role::_copy_from_(role_);
      return *this;
    }

    role::~role()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool role::is_valid() const
    {
      return has_id() && has_path() && has_functional_resource_selector();
    }

    bool role::has_id() const
    {
      return _id_ != INVALID_ID;
    }

    void role::_set_id(int32_t id_)
    {
      _id_ = id_;
      return;
    }

    void role::set_id(int32_t id_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Role '" << get_name() << "' is already initialized!");
      // if (has_id()) {
      //   DT_THROW_IF(is_initialized(), std::logic_error,
      //               "Role is already initialized with ID=[" << _id_ << "]!");
      // }
      DT_THROW_IF(id_ == INVALID_ID,
                  std::logic_error,
                  "Invalid role ID [" << id_ << "]!");
      _set_id(id_);
      return;
    }

    int32_t role::get_id() const
    {
      return _id_;
    }

    void role::reset_id()
    {
      _id_ = INVALID_ID;
      return;
    }

    bool role::is_universe() const
    {
      return _id_ == UNIVERSE_ID;
    }

    bool role::is_registered() const
    {
      return _id_ > UNIVERSE_ID;
    }

    bool role::is_dynamic() const
    {
      return _id_ < INVALID_ID;
    }

    bool role::has_path() const
    {
      return !_path_.empty();
    }

    void role::set_path(const std::string & path_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Role '" << get_name() << "' is already initialized!");
      _path_ = path_;
      return;
    }

    const std::string & role::get_path() const
    {
      return _path_;
    }

    bool role::has_resource_path(const std::string & resource_path_, uint32_t flags_) const
    {
      bool checked = false;
      if (!checked && (flags_ & RESOURCE_SET_FUNCTIONAL) && has_functional_resource_selector()) {
        cuts::i_cut & active_selector = const_cast<cuts::i_cut &>(_functional_resource_selector_handle_.get());
        active_selector.set_user_data(resource_path_);
        checked = (active_selector.process() == cuts::SELECTION_ACCEPTED);
      }
      if (!checked && (flags_ & RESOURCE_SET_DISTRIBUTABLE) && has_distributable_resource_selector()) {
        cuts::i_cut & active_selector = const_cast<cuts::i_cut &>(_distributable_resource_selector_handle_.get());
        active_selector.set_user_data(resource_path_);
        checked = (active_selector.process() == cuts::SELECTION_ACCEPTED);
      }
      return checked;
    }

    bool role::has_resource_id(int32_t resource_id_, uint32_t flags_) const
    {
      bool checked = false;
      if (!checked && (flags_ & RESOURCE_SET_FUNCTIONAL) && has_functional_resource_selector()) {
        cuts::i_cut & active_selector = const_cast<cuts::i_cut &>(_functional_resource_selector_handle_.get());
        active_selector.set_user_data<int32_t>(resource_id_);
        checked = (active_selector.process() == cuts::SELECTION_ACCEPTED);
      }
      if (!checked && (flags_ & RESOURCE_SET_DISTRIBUTABLE) && has_distributable_resource_selector()) {
        cuts::i_cut & active_selector = const_cast<cuts::i_cut &>(_distributable_resource_selector_handle_.get());
        active_selector.set_user_data<int32_t>(resource_id_);
        checked = (active_selector.process() == cuts::SELECTION_ACCEPTED);
      }
      return checked;
     }

    bool role::has_cached_scope_resource_ids() const
    {
      return _cached_scope_resource_ids_.get() != nullptr;
    }

    bool role::has_cached_functional_resource_ids() const
    {
      return _cached_functional_resource_ids_.get() != nullptr;
    }

    bool role::has_cached_distributable_resource_ids() const
    {
      return _cached_distributable_resource_ids_.get() != nullptr;
    }

    void role::build_cached()
    {
      build_cached_functional_resource_ids();
      build_cached_distributable_resource_ids();
      build_cached_scope_resource_ids();
      return;
    }

    void role::build_cached_scope_resource_ids()
    {
      if (_cached_scope_resource_ids_.get() == nullptr) {
        DT_THROW_IF(!has_resource_manager(), std::logic_error, "Cannot build cache without a resource manager!");
        _cached_scope_resource_ids_.reset(new std::set<int32_t>);
        build_set_of_resource_ids(*this,
                                  role::RESOURCE_SET_SCOPE,
                                  _resource_manager_->get_set_of_resource_ids(),
                                  *_cached_scope_resource_ids_.get());
      }
      return;
    }

    void role::build_cached_functional_resource_ids()
    {
      if (_cached_functional_resource_ids_.get() == nullptr) {
        DT_THROW_IF(!has_resource_manager(), std::logic_error, "Cannot build cache without a resource manager!");
        _cached_functional_resource_ids_.reset(new std::set<int32_t>);
        build_set_of_resource_ids(*this,
                                  role::RESOURCE_SET_FUNCTIONAL,
                                  _resource_manager_->get_set_of_resource_ids(),
                                  *_cached_functional_resource_ids_.get());
      }
      return;
    }

    void role::build_cached_distributable_resource_ids()
    {
      if (_cached_distributable_resource_ids_.get() == nullptr) {
        DT_THROW_IF(!has_resource_manager(), std::logic_error, "Cannot build cache without a resource manager!");
        _cached_distributable_resource_ids_.reset(new std::set<int32_t>);
        build_set_of_resource_ids(*this,
                                  role::RESOURCE_SET_DISTRIBUTABLE,
                                  _resource_manager_->get_set_of_resource_ids(),
                                  *_cached_distributable_resource_ids_.get());
      }
      return;
    }

    // static
    void role::build_set_of_resource_ids(const role & r_,
                                         uint32_t flags_,
                                         const std::set<int32_t> & universe_,
                                         std::set<int32_t> & resource_ids_)
    {
      for (auto resource_id : universe_) {
        if (flags_ & role::RESOURCE_SET_FUNCTIONAL) {
          if (r_.has_resource_id(resource_id, role::RESOURCE_SET_FUNCTIONAL)) {
            resource_ids_.insert(resource_id);
          }
        }
        if (flags_ & role::RESOURCE_SET_DISTRIBUTABLE) {
          if (r_.has_resource_id(resource_id, role::RESOURCE_SET_DISTRIBUTABLE)) {
            resource_ids_.insert(resource_id);
          }
        }
      }
      return;
    }

    const std::set<int32_t> & role::get_set_of_scope_resource_ids() const
    {
      bool build_it = false;
      if (!has_cached_scope_resource_ids()) {
        build_it = true;
      }
      if (build_it) {
        role * mutable_this = const_cast<role*>(this);
        mutable_this->build_cached_scope_resource_ids();
      }
      return *_cached_scope_resource_ids_.get();
    }

    const std::set<int32_t> & role::get_set_of_functional_resource_ids() const
    {
      bool build_it = false;
      if (!has_cached_functional_resource_ids()) {
        build_it = true;
      }
      if (build_it) {
        role * mutable_this = const_cast<role*>(this);
        mutable_this->build_cached_functional_resource_ids();
      }
      return *_cached_functional_resource_ids_.get();
    }

    const std::set<int32_t> & role::get_set_of_distributable_resource_ids() const
    {
      bool build_it = false;
      if (!has_cached_distributable_resource_ids()) {
        build_it = true;
      }
      if (build_it) {
        role * mutable_this = const_cast<role*>(this);
        mutable_this->build_cached_distributable_resource_ids();
      }
      return *_cached_distributable_resource_ids_.get();
    }

    bool role::has_distributable_resource_selector() const
    {
      return _distributable_resource_selector_handle_.has_data();
    }

    bool role::has_functional_resource_selector() const
    {
      return _functional_resource_selector_handle_.has_data();
    }

    void role::reset_distributable_resource_selector_handle()
    {
      _distributable_resource_selector_handle_.reset();
      if (has_cached_distributable_resource_ids()) {
        _cached_distributable_resource_ids_.reset();
      }
      if (has_cached_scope_resource_ids()) {
        _cached_scope_resource_ids_.reset();
      }
      return;
    }

    void role::reset_functional_resource_selector_handle()
    {
      _functional_resource_selector_handle_.reset();
      if (has_cached_functional_resource_ids()) {
        _cached_functional_resource_ids_.reset();
      }
      if (has_cached_scope_resource_ids()) {
        _cached_scope_resource_ids_.reset();
      }
      return;
    }

    void role::set_distributable_resource_selector_handle(cuts::cut_handle_type & distributable_resource_selector_handle_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Role '" << get_name() << "' is already initialized!");
      DT_THROW_IF(!distributable_resource_selector_handle_.has_data(),
                  std::logic_error,
                  "Distributable resource selector handle is not set!");
      DT_THROW_IF(!distributable_resource_selector_handle_.get().is_initialized(),
                  std::logic_error,
                  "Distributable resource selector is not initialized!");
      if (! distributable_resource_selector_handle_.get().has_description() ||
          distributable_resource_selector_handle_.get().get_description() != distributable_resource_selector_handle_.grab().get_type_id()) {
        distributable_resource_selector_handle_.grab().set_description(distributable_resource_selector_handle_.grab().get_type_id());
      }
      _distributable_resource_selector_handle_ = distributable_resource_selector_handle_;
      return;
    }

    void role::set_functional_resource_selector_handle(cuts::cut_handle_type & functional_resource_selector_handle_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Role '" << get_name() << "' is already initialized!");
      DT_THROW_IF(!functional_resource_selector_handle_.has_data(),
                  std::logic_error,
                  "Functional resource selector handle is not set!");
      DT_THROW_IF(!functional_resource_selector_handle_.get().is_initialized(),
                  std::logic_error,
                  "Functional resource selector is not initialized!");
      if (! functional_resource_selector_handle_.get().has_description() ||
          functional_resource_selector_handle_.get().get_description() != functional_resource_selector_handle_.grab().get_type_id()) {
        functional_resource_selector_handle_.grab().set_description(functional_resource_selector_handle_.grab().get_type_id());
      }
      _functional_resource_selector_handle_ = functional_resource_selector_handle_;
      return;
    }

    const cuts::cut_handle_type & role::get_functional_resource_selector_handle() const
    {
      return _functional_resource_selector_handle_;
    }

    cuts::cut_handle_type & role::grab_functional_resource_selector_handle()
    {
      // DT_THROW_IF(is_initialized(),
      //             std::logic_error,
      //             "Role '" << get_name() << "' is already initialized!");
      return _functional_resource_selector_handle_;
    }

    const cuts::i_cut & role::get_functional_resource_selector() const
    {
      DT_THROW_IF(!has_functional_resource_selector(), std::logic_error,
                  "Missing functional resource selector in role '" << get_name() << "'!");
      return _functional_resource_selector_handle_.get();
    }

    const cuts::cut_handle_type & role::get_distributable_resource_selector_handle() const
    {
      return _distributable_resource_selector_handle_;
    }

    cuts::cut_handle_type & role::grab_distributable_resource_selector_handle()
    {
      // DT_THROW_IF(is_initialized(),
      //             std::logic_error,
      //             "Role '" << get_name() << "' is already initialized!");
      return _distributable_resource_selector_handle_;
    }

    const cuts::i_cut & role::get_distributable_resource_selector() const
    {
      DT_THROW_IF(!has_functional_resource_selector(), std::logic_error,
                  "Missing distributable resource selector in role '" << get_name() << "'!");
      return _distributable_resource_selector_handle_.get();
    }

    bool role::has_resource_manager() const
    {
      // DT_LOG_TRACE(get_logging_priority(), "Resource manager = [@" << _resource_manager_ << "]");
      return _resource_manager_ != nullptr;
    }

    void role::set_resource_manager(const manager & rmgr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Role '" << get_name() << "' is already initialized!");
      _resource_manager_ = &rmgr_;
      return;
    }

    const manager & role::get_resource_manager() const
    {
      DT_THROW_IF(!has_resource_manager(), std::logic_error,
                  "No resource manager in role '" << get_name() << "'!");
      return *_resource_manager_;
    }

    void role::add_allowed_user(int32_t uid_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Role '" << get_name() << "' is already initialized!");
      DT_THROW_IF(has_allowed_user(uid_), std::logic_error,
                  "Role '" << get_name() << "' already allows user with identifier [" << uid_ << "]!");
      _allowed_users_.insert(uid_);
      return;
    }

    bool role::has_allowed_user(int32_t uid_) const
    {
      return _allowed_users_.count(uid_) == 1;
    }

    void role::remove_allowed_user(int32_t uid_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Role '" << get_name() << "' is already initialized!");
      DT_THROW_IF(!has_allowed_user(uid_), std::logic_error,
                  "Role '" << get_name() << "' does not allow user with identifier [" << uid_ << "]!");
      _allowed_users_.erase(uid_);
      return;
    }

    void role::add_allowed_group(int32_t gid_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Role '" << get_name() << "' is already initialized!");
      DT_THROW_IF(has_allowed_group(gid_), std::logic_error,
                  "Role '" << get_name() << "' already allows user group with identifier [" << gid_ << "]!");
      _allowed_groups_.insert(gid_);
      return;
    }

    bool role::has_allowed_group(int32_t gid_) const
    {
      return _allowed_groups_.count(gid_) == 1;
    }

    void role::remove_allowed_group(int32_t gid_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Role '" << get_name() << "' is already initialized!");
      DT_THROW_IF(!has_allowed_group(gid_), std::logic_error,
                  "Role '" << get_name() << "' does not allow user group with identifier [" << gid_ << "]!");
      _allowed_groups_.erase(gid_);
      return;
    }

    const std::set<int32_t> & role::get_allowed_users() const
    {
      return _allowed_users_;
    }

    const std::set<int32_t> & role::get_allowed_groups() const
    {
      return _allowed_groups_;
    }

    void role::_set_defaults()
    {
      set_logging_priority(datatools::logger::PRIO_FATAL);
      _id_ = INVALID_ID;
      return;
    }

    bool role::is_initialized() const
    {
      return _initialized_;
    }

    void role::initialize_simple()
    {
      datatools::properties dummy;
      initialize(dummy);
      return;
    }

    void role::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Role '" << get_name() << "' is already initialized!");

      this->datatools::enriched_base::initialize(config_, false);

      // DT_THROW_IF(! has_resource_manager(), std::logic_error,
      //             "Role '" << get_name() << "' does not have a resource manager!");

      if (! has_id()) {
        if (config_.has_key("id")) {
          set_id(config_.fetch_integer("id"));
        }
      }

      if (! has_path()) {
        if (config_.has_key("path")) {
          set_path(config_.fetch_string("path"));
        }
      }

      DT_THROW_IF(! has_path(), std::logic_error, "Missing role path!");

      if (! has_functional_resource_selector()) {
        // std::cerr << "DEVEL: role::initialize: " << "Attempt to set the selector for functional resources..." << std::endl;
        std::string selector_type_id;
        if (config_.has_key("functional_resource.selector_type_id")) {
          selector_type_id = config_.fetch_string("functional_resource.selector_type_id");
        }
        // if (selector_type_id.empty()) {
        //   // Default to vire::resource::enumerated_resource_selector :
        //   selector_type_id = "vire::resource::enumerated_resource_selector";
        // }

        if (! selector_type_id.empty()) {
          datatools::properties selector_config;
          config_.export_and_rename_starting_with(selector_config, "functional_resource.selector.", "");

          // Selector factory:
          cuts::i_cut::factory_register_type & facreg = resource_selector_factory_register();
          DT_THROW_IF(! facreg.has(selector_type_id),
                      std::logic_error,
                      "Cannot find resource selector/cut factory with ID '"
                      << selector_type_id << "' for role with role ID=[" << get_id() << "]!");
          const cuts::i_cut::factory_register_type::factory_type & the_factory
            = facreg.get(selector_type_id);
          cuts::cut_handle_type resource_selector_handle(the_factory());
          std::ostringstream resname_oss;
          resname_oss << get_id();
          resource_selector_handle.grab().set_name(resname_oss.str());
          // Store the cut/selector type id in the description string:
          resource_selector_handle.grab().set_description(selector_type_id);
          {
            // Resource selector inherited from 'base_resource_selector' needs a resource manager:
            base_resource_selector * brs = dynamic_cast<base_resource_selector *>(&resource_selector_handle.grab());
            if (brs != nullptr) {
              DT_THROW_IF(!has_resource_manager(), std::logic_error, "Missing resource manager for functional selector '" << resname_oss.str() << "'!");
              brs->set_resource_mgr(get_resource_manager());
            }
          }

          {
            // Build the dictionary of resource selector cut entries from the dictionary of resources:
            cuts::cut_handle_dict_type selectors_dict;
            /*
              if (_resources_mgr_.is_initialized()) {
              resources_dict_type & res
              = const_cast<role_dict_type &>(resource_mgr_.get_roles());
              for (auto r : res) {
              // int32_t a_gid = ug.first;
              //  std::ostringstream a_gid_name;
              //  a_gid_name << a_gid;
              //  {
              //  cuts::cut_entry_type new_cut_entry;
              //  selectors_dict[a_gid_name.str()] = new_cut_entry;
              //  }
              cuts::cut_entry_type & cut_entry = selectors_dict.find(a_gid_name.str())->second;
              cut_entry.set_cut_name(a_gid_name.str());
              cut_entry.set_cut_id(selector_type_id);
              cut_entry.grab_cut_handle() = ug.second.grab().grab_resource_selector_handle();
              }
              }
            */
            resource_selector_handle.grab().initialize_without_service(selector_config, selectors_dict);
          }
          set_functional_resource_selector_handle(resource_selector_handle);
          // get_functional_resource_selector().tree_dump(std::cerr, "functional_resource_selector: ", "DEVEL: ");
        }
      }

      if (! has_distributable_resource_selector()) {
        // std::cerr << "DEVEL: role:initialize: " << "Attempt to set the selector for distributable resources..." << std::endl;
        std::string selector_type_id;
        if (config_.has_key("distributable_resource.selector_type_id")) {
          selector_type_id = config_.fetch_string("distributable_resource.selector_type_id");
        }

        // if (selector_type_id.empty()) {
        //   // Default to vire::resource::enumerated_resource_selector :
        //   selector_type_id = "vire::resource::enumerated_resource_selector";
        // }

        if (!selector_type_id.empty()) {
          datatools::properties selector_config;
          config_.export_and_rename_starting_with(selector_config, "distributable_resource.selector.", "");

          // Selector factory:
          cuts::i_cut::factory_register_type & facreg = resource_selector_factory_register();
          DT_THROW_IF(! facreg.has(selector_type_id),
                      std::logic_error,
                      "Cannot find resource selector/cut factory with ID '"
                      << selector_type_id << "' for  role with role ID=[" << get_id() << "]!");
          const cuts::i_cut::factory_register_type::factory_type & the_factory
            = facreg.get(selector_type_id);
          cuts::cut_handle_type resource_selector_handle(the_factory());
          std::ostringstream resname_oss;
          resname_oss << get_id();
          resource_selector_handle.grab().set_name(resname_oss.str());
          // Store the cut/selector type id in the description string:
          resource_selector_handle.grab().set_description(selector_type_id);
          {
            // Resource selector inherited from 'base_resource_selector' needs a resource manager:
            base_resource_selector * brs = dynamic_cast<base_resource_selector *>(&resource_selector_handle.grab());
            if (brs != nullptr) {
              DT_THROW_IF(!has_resource_manager(), std::logic_error, "Missing resource manager for distributable selector '" << resname_oss.str() << "'!");
              brs->set_resource_mgr(get_resource_manager());
            }
          }
          // if (has_resource_manager()) {
          //   base_resource_selector * brs = dynamic_cast<base_resource_selector *>(&resource_selector_handle.grab());
          //   if (brs != nullptr) {
          //     brs->set_resource_mgr(get_resource_manager());
          //   }
          // }

          {
            // Build the dictionary of resource selector cut entries from the dictionary of resources:
            cuts::cut_handle_dict_type selectors_dict;
            /*
              if (_resources_mgr_.is_initialized()) {
              resources_dict_type & res
              = const_cast<role_dict_type &>(resource_mgr_.get_roles());
              for (auto r : res) {
              // int32_t a_gid = ug.first;
              //  std::ostringstream a_gid_name;
              //  a_gid_name << a_gid;
              //  {
              //  cuts::cut_entry_type new_cut_entry;
              //  selectors_dict[a_gid_name.str()] = new_cut_entry;
              //  }
              cuts::cut_entry_type & cut_entry = selectors_dict.find(a_gid_name.str())->second;
              cut_entry.set_cut_name(a_gid_name.str());
              cut_entry.set_cut_id(selector_type_id);
              cut_entry.grab_cut_handle() = ug.second.grab().grab_resource_selector_handle();
              }
              }
            */
            resource_selector_handle.grab().initialize_without_service(selector_config, selectors_dict);
          }
          set_distributable_resource_selector_handle(resource_selector_handle);
        }
      }

      DT_THROW_IF(! has_functional_resource_selector() && ! has_distributable_resource_selector(),
                  std::logic_error, "Sterile role ! At least one of the functional/distributable resource selectors is needed!");

      // std::cerr << "DEVEL: role::initialize: _allowed_users_.size() = " << _allowed_users_.size() << std::endl;
      if (_allowed_users_.size() == 0) {
        if (config_.has_key("allowed_users")) {
          std::vector<int> uids;
          config_.fetch("allowed_users", uids);
          for (auto uid : uids) {
            add_allowed_user(uid);
          }
        }
      }

      // std::cerr << "DEVEL: role::initialize: _allowed_groups_.size() = " << _allowed_groups_.size() << std::endl;
      if (_allowed_groups_.size() == 0) {
        if (config_.has_key("allowed_groups")) {
          std::vector<int> gids;
          config_.fetch("allowed_groups", gids);
          for (auto gid : gids) {
            add_allowed_group(gid);
          }
        }
      }

      DT_THROW_IF(_allowed_users_.size() + _allowed_groups_.size() == 0,
                  std::logic_error, "Missing allowed user/group!");

      bool build_cache = false;
      if (config_.has_key("build_cache")) {
        build_cache = config_.fetch_boolean("build_cache");
      }

      // Now perform special initialization operations:

      // Build the scope resource selector handle:
      _build_scope_selector();

      if (build_cache) {
        build_cached();
        build_cached();
        build_cached();
      }

      _initialized_ = true;
      return;
    }

    void role::_build_scope_selector()
    {
      // std::cerr << "DEVEL: role::_build_scope_selector: Build the scope resource selector handle..." << std::endl;
      if (has_functional_resource_selector() && has_distributable_resource_selector()) {
        // std::cerr << "DEVEL: role::_build_scope_selector: ... from functional and distributable resource selectors..." << std::endl;
        cuts::or_cut * union_cut = new cuts::or_cut;
        cuts::cut_handle_type cut1 = grab_functional_resource_selector_handle();
        cuts::cut_handle_type cut2 = grab_distributable_resource_selector_handle();
        union_cut->set_cuts(cut1, cut2);
        _scope_resource_selector_handle_.reset(union_cut);
        union_cut->initialize_simple();
      } else if (has_functional_resource_selector()) {
        // std::cerr << "DEVEL: role::_build_scope_selector: ... from functional resource selectors only..." << std::endl;
        _scope_resource_selector_handle_ = grab_functional_resource_selector_handle();
      } else if (has_distributable_resource_selector()) {
        // std::cerr << "DEVEL: role::_build_scope_selector: ... from distributable resource selectors only..." << std::endl;
        _scope_resource_selector_handle_ = grab_distributable_resource_selector_handle();
      }
      return;
    }

    void role::export_to_config(datatools::properties & config_,
                                uint32_t flags_,
                                const std::string & prefix_) const
    {
      bool devel = false;
      // devel = true;
      this->datatools::enriched_base::export_to_config(config_, flags_, prefix_);

      if (flags_ & ROLE_XC_ID) {
        config_.store_integer(prefix_ + "id", get_id(), "The role identifier");
      }

      if (flags_ & ROLE_XC_PATH) {
        config_.store_string(prefix_ + "path", get_path(), "The role path");
      }

      if (flags_ & ROLE_XC_ALLOWED_USERS) {
        if (devel) std::cerr << "DEVEL: " << "vire::resource::role::export_to_config: " << "ROLE_XC_ALLOWED_USERS..." << std::endl;
        std::vector<int> uids;
        for (const auto uid : _allowed_users_) {
          uids.push_back(uid);
        }
        config_.store(prefix_ + "allowed_users", uids, "The list of allowed users");
        if (devel) std::cerr << "DEVEL: " << "vire::resource::role::export_to_config: " << "ROLE_XC_ALLOWED_USERS done." << std::endl;
      }

      if (flags_ & ROLE_XC_ALLOWED_GROUPS) {
        if (devel) std::cerr << "DEVEL: " << "vire::resource::role::export_to_config: " << "ROLE_XC_ALLOWED_GROUPS..." << std::endl;
        std::vector<int> gids;
        for (const auto gid : _allowed_groups_) {
          gids.push_back(gid);
        }
        config_.store(prefix_ + "allowed_groups", gids, "The list of allowed user groups");
        if (devel) std::cerr << "DEVEL: " << "vire::resource::role::export_to_config: " << "ROLE_XC_ALLOWED_GROUPS done." << std::endl;
      }

      if (flags_ & ROLE_XC_FUNCTIONAL_RESOURCE_SELECTOR) {
        if (has_functional_resource_selector()) {
          if (devel) std::cerr << "DEVEL: " << "vire::resource::role::export_to_config: " << "ROLE_XC_FUNCTIONAL_RESOURCE_SELECTOR..." << std::endl;
          config_.store_string(prefix_ + "functional_resource.selector_type_id", get_functional_resource_selector().get_description(),
                               "Type of the functional resource selector");
          datatools::properties selector_config;
          get_functional_resource_selector().export_to_config(config_, 0, prefix_ + "functional_resource.selector.");
          if (devel) std::cerr << "DEVEL: " << "vire::resource::role::export_to_config: " << "ROLE_XC_FUNCTIONAL_RESOURCE_SELECTOR done." << std::endl;
        }
      }

      if (flags_ & ROLE_XC_DISTRIBUTABLE_RESOURCE_SELECTOR) {
        if (has_distributable_resource_selector()) {
          if (devel) std::cerr << "DEVEL: " << "vire::resource::role::export_to_config: " << "ROLE_XC_DISTRIBUTABLE_RESOURCE_SELECTOR..." << std::endl;
          config_.store_string(prefix_ + "distributable_resource.selector_type_id", get_distributable_resource_selector().get_description(),
                               "Type of the distributable resource selector");
          datatools::properties selector_config;
          get_distributable_resource_selector().export_to_config(config_, 0, prefix_ + "distributable_resource.selector.");
          if (devel) std::cerr << "DEVEL: " << "vire::resource::role::export_to_config: " << "ROLE_XC_DISTRIBUTABLE_RESOURCE_SELECTOR done." << std::endl;
         }
      }

      return;
    }

    void role::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Role '" << get_name() << "' is not initialized!");
      _initialized_ = false;
      _cached_scope_resource_ids_.reset();
      _cached_functional_resource_ids_.reset();
      _path_.clear();
      _functional_resource_selector_handle_.reset();
      _distributable_resource_selector_handle_.reset();
      _resource_manager_ = nullptr;
      _allowed_users_.clear();
      _allowed_groups_.clear();
      this->datatools::enriched_base::clear();
      _set_defaults();
      return;
    }

    // static
    cuts::i_cut::factory_register_type & role::resource_selector_factory_register()
    {
      static boost::scoped_ptr<cuts::i_cut::factory_register_type> _usfr;
      if (! _usfr) {
        _usfr.reset(new cuts::i_cut::factory_register_type);
        _usfr->set_label("resource/resource_selector_factory");
        _usfr->registration("cuts::accept_cut",    boost::factory<cuts::accept_cut*>());
        _usfr->registration("cuts::not_cut",       boost::factory<cuts::not_cut*>());
        _usfr->registration("cuts::and_cut",       boost::factory<cuts::and_cut*>());
        _usfr->registration("cuts::or_cut",        boost::factory<cuts::or_cut*>());
        _usfr->registration("cuts::nor_cut",       boost::factory<cuts::nor_cut*>());
        _usfr->registration("cuts::xor_cut",       boost::factory<cuts::xor_cut*>());
        _usfr->registration("cuts::nand_cut",      boost::factory<cuts::nand_cut*>());
        _usfr->registration("cuts::multi_and_cut", boost::factory<cuts::multi_and_cut*>());
        _usfr->registration("cuts::multi_or_cut",  boost::factory<cuts::multi_or_cut*>());
        _usfr->registration("cuts::multi_xor_cut", boost::factory<cuts::multi_xor_cut*>());
        _usfr->registration("vire::resource::enumerated_resource_selector",
                            boost::factory<vire::resource::enumerated_resource_selector*>());
        _usfr->registration("vire::resource::regex_resource_selector",
                            boost::factory<vire::resource::regex_resource_selector*>());
        _usfr->registration("vire::resource::general_expression_resource_selector",
                            boost::factory<vire::resource::general_expression_resource_selector*>());
        _usfr->registration("vire::resource::ranged_resource_selector",
                            boost::factory<vire::resource::ranged_resource_selector*>());
        _usfr->registration("vire::resource::by_access_resource_selector",
                            boost::factory<vire::resource::by_access_resource_selector*>());
      }
      return *_usfr.get();
    }

    void role::tree_dump(std::ostream & out_,
                         const std::string & title_,
                         const std::string & indent_,
                         bool inherit_) const
    {
       this->datatools::enriched_base::tree_dump(out_, title_, indent_, true);

       out_ << indent_ << i_tree_dumpable::tag
            << "Role ID : ";
       if (has_id()) {
         out_ << '[' << _id_ << ']';
       } else {
         out_ << "<none>";
       }
       out_ << std::endl;

       out_ << indent_ << i_tree_dumpable::tag
            << "Path : ";
       if (has_path()) {
         out_ << "'" << _path_ << "'";
       } else {
         out_ << "<none>";
       }
       out_ << std::endl;

       out_ << indent_ << i_tree_dumpable::tag
            << "Functional resource selector : ";
       if (has_functional_resource_selector()) {
         out_ << '[' << &_functional_resource_selector_handle_.get() << ']';
       } else {
         out_ << "<none>";
       }
       out_ << std::endl;
       if (has_functional_resource_selector()) {
         std::ostringstream indent2;
         indent2 << indent_ << i_tree_dumpable::skip_tag;
         _functional_resource_selector_handle_.get().tree_dump(out_, "", indent2.str());
       }

       out_ << indent_ << i_tree_dumpable::tag
            << "Distributable resource selector : ";
       if (has_distributable_resource_selector()) {
         out_ << '[' << &_distributable_resource_selector_handle_.get() << ']';
       } else {
         out_ << "<none>";
       }
       out_ << std::endl;
       if (has_distributable_resource_selector()) {
         std::ostringstream indent2;
         indent2 << indent_ << i_tree_dumpable::skip_tag;
         _distributable_resource_selector_handle_.get().tree_dump(out_, "", indent2.str());
       }

       out_ << indent_ << i_tree_dumpable::tag
            << "Resource manager : ";
       if (has_resource_manager()) {
         out_ << '[' << _resource_manager_ << ']';
       } else {
         out_ << "<none>";
       }
       out_ << std::endl;

       out_ << indent_ << i_tree_dumpable::tag
            << "Allowed users : ";
       if (_allowed_users_.size()) {
         out_ << '[' << _allowed_users_.size() << ']';
       } else {
         out_ << "<none>";
       }
       out_ << std::endl;

       out_ << indent_ << i_tree_dumpable::tag
            << "Allowed groups : ";
       if (_allowed_groups_.size()) {
         out_ << '[' << _allowed_groups_.size() << ']';
       } else {
         out_ << "<none>";
       }
       out_ << std::endl;

       out_ << indent_ << i_tree_dumpable::tag
            << "Cached functional resource identifiers : ";
       if (has_cached_functional_resource_ids()) {
         out_ << '[' << get_set_of_functional_resource_ids().size() << ']';
       } else {
         out_ << "<none>";
       }
       out_ << std::endl;

       out_ << indent_ << i_tree_dumpable::tag
            << "Cached distributable resource identifiers : ";
       if (has_cached_distributable_resource_ids()) {
         out_ << '[' << get_set_of_distributable_resource_ids().size() << ']';
       } else {
         out_ << "<none>";
       }
       out_ << std::endl;

       out_ << indent_ << i_tree_dumpable::tag
            << "Cached scope resource identifiers : ";
       if (has_cached_scope_resource_ids()) {
         out_ << '[' << get_set_of_scope_resource_ids().size() << ']';
       } else {
         out_ << "<none>";
       }
       out_ << std::endl;

       out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
            << "Valid : " << is_valid() << std::endl;

       return;
    }

  } // namespace resource

} // namespace vire
