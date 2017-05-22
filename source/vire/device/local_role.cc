// vire/device/local_role.cc - Implementation of Vire device local role support
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
#include <vire/device/local_role.h>

namespace vire {

  namespace device {

    // static
    const std::string & local_role::role_id_key()
    {
      static std::string _k("vire.device.local_role.role_id");
      return _k;
    }

    // static
    const std::string & local_role::role_tags_key()
    {
      static std::string _k("vire.device.local_role.tags");
      return _k;
    }

    // static
    const std::string & local_role::role_expert_id()
    {
      static std::string _x("expert");
      return _x;
    }

    // static
    const std::string & local_role::role_user_id()
    {
      static std::string _u("user");
      return _u;
    }

    // static
    const std::string & local_role::role_id_to_string(local_role_id role_id_)
    {
      static const std::string _bad;
      switch (role_id_) {
      case LR_USER : return role_user_id();
      case LR_EXPERT : return role_expert_id();
      }
      return _bad;
    }

    // static
    local_role::local_role_id local_role::string_to_role_id(const std::string & label_)
    {
      if (label_ == role_user_id()) return LR_USER;
      if (label_ == role_expert_id()) return LR_USER;
      return LR_INVALID;
    }

    local_role::local_role()
    {
      _role_id_ = LR_INVALID;
      return;
    }

    bool local_role::has_role_id() const
    {
      return _role_id_ != LR_INVALID;
    }

    local_role::local_role_id local_role::get_role_id() const
    {
      return _role_id_;
    }

    void local_role::set_role_id(local_role_id id_)
    {
      _role_id_ = id_;
      return;
    }

    bool local_role::is_user() const
    {
      return _role_id_ == LR_USER;
    }

    bool local_role::is_expert() const
    {
      return _role_id_ == LR_EXPERT;
    }

    bool local_role::has_tag(const std::string & tag_id_) const
    {
      return _tags_.count(tag_id_) != 0;
    }

    void local_role::add_tag(const std::string & tag_id_)
    {
      _tags_.insert(tag_id_);
      return;
    }

    bool local_role::has_tags() const
    {
      return _tags_.size();
    }

    bool local_role::is_initialized() const
    {
      return _role_id_ != LR_INVALID;
    }

    void local_role::initialize_simple()
    {
      datatools::properties dummy_properties;
      initialize(dummy_properties);
      return;
    }

    void local_role::initialize(const datatools::properties & config_)
    {
      if (!has_role_id()) {
        if (config_.has_key("role_id")) {
          const std::string & role_id = config_.fetch_string("role_id");
          if (role_id == role_expert_id()) {
            set_role_id(LR_USER);
          } else if (role_id == role_user_id()) {
            set_role_id(LR_EXPERT);
          } else {
            DT_THROW(std::logic_error, "Invalid role id '" << role_id << "'!");
          }
        }
      }

      if (!has_role_id()) {
        set_role_id(LR_DEFAULT);
      }

      if (!has_tags()) {
        if (config_.has_key("tags")) {
          config_.fetch("tags", _tags_);
        }
      }

      return;
    }

    void local_role::reset()
    {
      _tags_.clear();
      _role_id_ = LR_INVALID;
      return;
    }

    void local_role::tree_dump(std::ostream & out_,
                                const std::string & title_,
                                const std::string & indent_,
                                bool inherit_) const
    {
      if (! title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Role ID : ";
      if (has_role_id()) {
        out_ << "'" << role_id_to_string(_role_id_) << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Tags : ";
      if (has_tags()) {
        out_ << "[" << _tags_.size() << "]";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (has_tags()) {
        int count = 0;
        for (const auto & tag : _tags_) {
          out_ << indent_ << datatools::i_tree_dumpable::skip_tag;
          if (++count == _tags_.size()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "Tag : '" << tag << "'" << std::endl;
        }
      }
      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Initialized : " << is_initialized() << std::endl;

      return;
    }

  } // namespace resource

} // namespace vire
