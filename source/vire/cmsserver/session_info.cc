//! \file vire/cmsserver/session_info.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/session_info.h>

// This project:
#include <vire/time/utils.h>
#include <vire/user/user.h>
#include <vire/user/group.h>
#include <vire/user/manager.h>
#include <vire/resource/role.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace cmsserver {

    // static
    const int32_t session_info::INVALID_ID;
    const int32_t session_info::ROOT_ID;
    const int32_t session_info::MIN_BLESSED_ID;
    const int32_t session_info::MAX_USER_ID;

    void session_info::_set_defaults()
    {
      _id_  = INVALID_ID;
      _role_id_ = vire::resource::role::INVALID_ID;
      vire::time::invalidate_time_interval(_when_);
      return;
    }

    session_info::session_info() : _when_(vire::time::invalid_time_interval())
    {
      _set_defaults();
      return;
    }

    session_info::~session_info()
    {
      reset();
      return;
    }

    bool session_info::is_valid() const
    {
      if (!has_id()) return false;
      if (!has_key()) return false;
      if (!has_role_id()) return false;
      if (!has_when()) return false;
      if (!has_usecase_id()) return false;
      return true;
    }

    void session_info::initialize_simple()
    {
      datatools::properties dummy_config;
      _initialize(dummy_config);
      return;
    }

    void session_info::initialize(const vire::user::manager & user_mgr_,
                                  const vire::resource::manager & resource_mgr_)
    {
      datatools::properties dummy_config;
      _initialize(dummy_config, &user_mgr_, &resource_mgr_);
      return;
    }

    void session_info::initialize(const datatools::properties & config_,
                                  const vire::user::manager & user_mgr_,
                                  const vire::resource::manager & resource_mgr_)
    {
      _initialize(config_, &user_mgr_, &resource_mgr_);
      return;
    }

    void session_info::_initialize(const datatools::properties & config_,
                                   const vire::user::manager * /* user_mgr_ */,
                                   const vire::resource::manager * resource_mgr_)
     {
      if (!has_id()) {
        if (config_.has_key("id")) {
          int id = config_.fetch_integer("id");
          set_id(id);
        }
      }

      if (!has_key()) {
        if (config_.has_key("key")) {
          std::string key = config_.fetch_string("key");
          set_key(key);
        }
      }

      if (!has_description()) {
        if (config_.has_key("description")) {
          const std::string & desc = config_.fetch_string("description");
          set_description(desc);
        }
      }

      if (!has_role_id()) {
        int32_t role_id = vire::resource::role::INVALID_ID;
        if (role_id == vire::resource::role::INVALID_ID
            && config_.has_key("role_id")) {
          role_id = config_.fetch_integer("role_id");
          if (resource_mgr_ != nullptr) {
            DT_THROW_IF(!resource_mgr_->has_role_by_id(role_id),
                        std::logic_error,
                        "Role with ID=[" << role_id << "] does not exist!");
          }
        }
        if (resource_mgr_ != nullptr
            && role_id == vire::resource::role::INVALID_ID
            && config_.has_key("role")) {
          const std::string & role_name = config_.fetch_string("role");
          DT_THROW_IF(!resource_mgr_->has_role_by_name(role_name),
                      std::logic_error,
                      "Role '" << role_name << "' does not exist!");
          role_id = resource_mgr_->get_role_by_name(role_name).get_id();
        }
        set_role_id(role_id);
      }

      if (!has_when()) {
        if (config_.has_key("when")) {
          const std::string & when_repr = config_.fetch_string("when");
          boost::posix_time::time_period when = vire::time::invalid_time_interval();
          DT_THROW_IF(!vire::time::parse_time_interval(when_repr, when),
                      std::logic_error,
                      "Invalid time interval representation '" << when_repr << "'!");
          set_when(when);
        }
      }

      if (config_.has_key("usecase")) {
        const std::string & usecase_id = config_.fetch_string("usecase");
        set_usecase_id(usecase_id);
      }

      if (!has_special_functional_cardinalities()) {
        datatools::properties sfc_config;
        config_.export_and_rename_starting_with(sfc_config, "special_functional_cardinalities.", "");
        _special_functional_cardinalities_.initialize(sfc_config, resource_mgr_);
      }

      if (!has_special_distributable_cardinalities()) {
        datatools::properties sdc_config;
        config_.export_and_rename_starting_with(sdc_config, "special_distributable_cardinalities.", "");
        _special_distributable_cardinalities_.initialize(sdc_config, resource_mgr_);
      }

      // if (!is_valid()) {
      //   DT_THROW(std::logic_error, "Session info record is not valid!");
      // }

      return;
    }

    void session_info::reset()
    {
      _description_.clear();
      _special_functional_cardinalities_.clear();
      _special_distributable_cardinalities_.clear();
      vire::time::invalidate_time_interval(_when_);
      _set_defaults();
      return;
    }

    bool session_info::is_blessed() const
    {
      return _id_ >= MIN_BLESSED_ID;
    }

    bool session_info::is_user() const
    {
      return _id_ <= MAX_USER_ID;
    }

    bool session_info::has_description() const
    {
      return !_description_.empty();
    }

    const std::string & session_info::get_description() const
    {
      return _description_;
    }

    bool session_info::has_id() const
    {
      return _id_ != INVALID_ID;
    }

    int32_t session_info::get_id() const
    {
      return _id_;
    }

    bool session_info::has_role_id() const
    {
      return _role_id_ != vire::resource::role::INVALID_ID;
    }

    int32_t session_info::get_role_id() const
    {
      return _role_id_;
    }

    bool session_info::has_key() const
    {
      return !_key_.empty();
    }

    const std::string & session_info::get_key() const
    {
      return _key_;
    }

    bool session_info::has_when() const
    {
      return vire::time::is_valid(_when_);
    }

    const boost::posix_time::time_period & session_info::get_when() const
    {
      return _when_;
    }

    bool session_info::has_usecase_id() const
    {
      return ! _usecase_id_.empty();
    }

    const std::string & session_info::get_usecase_id() const
    {
      return _usecase_id_;
    }

    bool session_info::has_special_functional_cardinalities() const
    {
      return _special_functional_cardinalities_.size() > 0;
    }

    const cardinalities_request_type & session_info::get_special_functional_cardinalities() const
    {
      return _special_functional_cardinalities_;
    }

    bool session_info::has_special_distributable_cardinalities() const
    {
      return _special_distributable_cardinalities_.size() > 0;
    }

    const cardinalities_request_type & session_info::get_special_distributable_cardinalities() const
    {
      return _special_distributable_cardinalities_;
    }

    void session_info::tree_dump(std::ostream & out_,
                                 const std::string & title_,
                                 const std::string & indent_,
                                 bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "ID      : ";
      if (has_id()) {
        out_ << _id_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Description: ";
      if (has_description()) {
        out_ << "'" << _description_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Role ID : ";
      if (has_role_id()) {
        out_ << _role_id_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "When    : ";
      if (has_when()) {
        out_ << boost::posix_time::to_simple_string(_when_);
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Usecase ID : ";
      if (has_usecase_id()) {
        out_ << "'" << _usecase_id_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Special request on functional resource cardinality    : ";
      if (has_special_functional_cardinalities()) {
        out_ << "[" << _special_functional_cardinalities_.size() << "]";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Special request on distributable resource cardinality : ";
      if (has_special_distributable_cardinalities()) {
        out_ << "[" << _special_distributable_cardinalities_.size() << "]";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Valid   : " << std::boolalpha << is_valid() << std::endl;

      return;
    }

    void session_info::set_id(int32_t id_)
    {
      _id_ = id_;
      return;
    }

    void session_info::set_key(const std::string & key_)
    {
      _key_ = key_;
      return;
    }

    void session_info::set_description(const std::string & description_)
    {
      _description_ = description_;
      return;
    }

    void session_info::set_role_id(int32_t rid_)
    {
      _role_id_ = rid_;
      return;
    }

    void session_info::set_when(const boost::posix_time::time_period & when_)
    {
      _when_ = when_;
      return;
    }

    void session_info::set_usecase_id(const std::string & usecase_id_)
    {
      _usecase_id_ = usecase_id_;
      return;
    }

    void session_info::set_special_functional_limited(int32_t resource_id_, std::size_t cardinality_)
    {
      _special_functional_cardinalities_.set_limited_resource(resource_id_, cardinality_);
      return;
    }

    void session_info::set_special_functional_unlimited(int32_t resource_id_)
    {
      _special_functional_cardinalities_.set_unlimited_resource(resource_id_);
      return;
    }

    void session_info::unset_special_functional(int32_t resource_id_)
    {
      _special_functional_cardinalities_.unset_resource(resource_id_);
      return;
    }

    void session_info::set_special_distributable_limited(int32_t resource_id_, std::size_t cardinality_)
    {
      _special_distributable_cardinalities_.set_limited_resource(resource_id_, cardinality_);
      return;
    }

    void session_info::set_special_distributable_unlimited(int32_t resource_id_)
    {
      _special_distributable_cardinalities_.set_unlimited_resource(resource_id_);
      return;
    }

    void session_info::unset_special_distributable(int32_t resource_id_)
    {
      _special_distributable_cardinalities_.unset_resource(resource_id_);
       return;
    }

    // bool session_info::has_role() const
    // {
    //   return _role_ != nullptr || _role_id_ < -1;
    // }

    // const resource::role & session_info::get_role() const
    // {
    //   if (_role_ != nullptr) {
    //     return *_role_;
    //   } else if (_role_id_ < -1) {
    //     return _process_desc_._role_.get();
    //   }
    // }

  } // namespace cmsserver

} // namespace vire
