//! \file vire/user/user.cc
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
#include <vire/user/user.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/exception.h>

namespace vire {

  namespace user {

    DATATOOLS_SERIALIZATION_IMPLEMENTATION_ADVANCED(user, "vire::user::user")

    // static
    const int32_t user::INVALID_UID;
    const int32_t user::ADMIN_UID;
    const int32_t user::MIN_UID;

    // static
    const std::string & user::admin_user()
    {
      static const std::string _l("admin");
      return _l;
    }

    user::user()
    {
      _set_defaults();
      return;
    }

    user::~user()
    {
      reset();
      return;
    }

    bool user::is_valid() const
    {
      return has_valid_uid() && has_name();
    }

    bool user::has_valid_uid() const
    {
      return _uid_ != INVALID_UID;
    }

    void user::set_uid(int32_t uid_)
    {
      DT_THROW_IF(uid_ < INVALID_UID,
                  std::logic_error,
                  "Invalid UID [" << uid_ << "]!");
      _uid_ = uid_;
      return;
    }

    int32_t user::get_uid() const
    {
      return _uid_;
    }

    bool user::is_administrator() const
    {
      return _uid_ == ADMIN_UID;
    }

    void user::_set_defaults()
    {
      set_logging_priority(datatools::logger::PRIO_FATAL);
      _uid_ = INVALID_UID;
      return;
    }

    void user::check() const
    {
      DT_THROW_IF(!has_valid_uid(), std::logic_error, "Missing a valid UID!");
      DT_THROW_IF(!has_name(), std::logic_error, "User with UID=[#" << _uid_ << "] has no name!");
      return;
    }

    void user::initialize_simple()
    {
      const datatools::properties dummy_config;
      initialize(dummy_config);
      return;
    }

    void user::initialize(const datatools::properties & config_)
    {
      // Fetch the attributes from the base class, preserving those who have been
      // previously set:
      this->datatools::enriched_base::initialize(config_, false);

      if (! has_valid_uid()) {
        if (config_.has_key("uid")) {
          set_uid(config_.fetch_integer("uid"));
        }
      }

      check();
      return;
    }

    void user::export_to_config(datatools::properties & config_,
                                uint32_t flags_,
                                const std::string & prefix_) const
    {
      this->datatools::enriched_base::export_to_config(config_, flags_, prefix_);
      if (flags_ & USER_XC_UID) {
        config_.store_integer(prefix_ + "uid", get_uid());
      }
      return;
    }

    void user::reset()
    {
      this->datatools::enriched_base::clear();
      _set_defaults();
      return;
    }

    void user::tree_dump(std::ostream & out_,
                         const std::string & title_,
                         const std::string & indent_,
                         bool inherit_) const
    {
      this->datatools::enriched_base::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "UID : ";
      if (has_valid_uid()) {
        out_ << _uid_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Valid : " << is_valid() << std::endl;

      return;
    }

  } // namespace user

} // namespace vire
