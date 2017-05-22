// vire/user/by_tag_resource_selector.cc - Implementation of a selector of resources with regular expression on resource paths
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

// Ourselves
#include <vire/resource/by_tag_resource_selector.h>

// Third party:
// - Boost:
#include <boost/regex.hpp>

// This project:
#include <vire/resource/utils.h>
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace resource {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(by_tag_resource_selector, "vire::resource::by_tag_resource_selector");

    by_tag_resource_selector::by_tag_resource_selector(datatools::logger::priority logger_priority_)
      : base_resource_selector(logger_priority_)
    {
      _tag_   = INVALID_STR;
      _value_ = INVALID_STR;
      return;
    }

    by_tag_resource_selector::~by_tag_resource_selector()
    {
      if (is_initialized()) {
        this->by_tag_resource_selector::reset();
      }
      return;
    }

    int by_tag_resource_selector::_accept()
    {
      int result = cuts::SELECTION_REJECTED;
      DT_LOG_TRACE(_logging, "Entering...");
      if (is_user_data<std::string>()) {
        const std::string & str_data = get_user_data<std::string>();
        const resource & r  = get_resource_mgr().get_resource_by_path (str_data);
        if (r.get_auxiliaries().has_key(_tag_)) {
           if (r.get_auxiliaries().fetch_string(_tag_) == _value_) {
              result = cuts::SELECTION_ACCEPTED;
           }
        }
      } else if (is_user_data<resource>()) {
        const resource & r = get_user_data<resource>();
        if (r.get_auxiliaries().has_key(_tag_)) {
           if (r.get_auxiliaries().fetch_string(_tag_) == _value_) {
              result = cuts::SELECTION_ACCEPTED;
           }
        }
      } else {
         result = cuts::SELECTION_INAPPLICABLE;
      }
      DT_LOG_TRACE(_logging, "Exiting.");
      return result;
    }

    void by_tag_resource_selector::initialize(const datatools::properties & config_,
                                             datatools::service_manager & /* service_manager_ */,
                                             cuts::cut_handle_dict_type & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource selector '" << get_name() << "' is already initialized !");

      this->base_resource_selector::_base_init(config_);

      // Check:
      DT_THROW_IF(!has_role_tag(), std::logic_error, "Role tag is not set!");

      _set_initialized(true);
      return;
    }

    void by_tag_resource_selector::reset()
    {
      _set_initialized(false);
      _tag_   = INVALID_STR;
      _value_ = INVALID_STR;
      this->base_resource_selector::_base_reset();
      return;
    }

    const bool by_tag_resource_selector::has_role_tag() const
    {
      return _tag_.compare (INVALID_STR) and _value_.compare (INVALID_STR);
    }

    void by_tag_resource_selector::set_role_tag(const std::string tag_, const std::string value_)
    {
      std::size_t zero = tag_.find (VALID_PREFIX);
      if (zero == 0) {
         _tag_   = tag_;
         _value_ = value_;
      }
      return;
    }

    const std::string by_tag_resource_selector::get_role_tag() const
    {
      return _tag_;
    }

    const std::string by_tag_resource_selector::get_tag_value() const
    {
      return _value_;
    }

    void by_tag_resource_selector::export_to_config(datatools::properties & config_,
                                                        uint32_t flags_,
                                                        const std::string & prefix_) const
    {
      this->cuts::i_cut::export_to_config(config_, flags_, prefix_);

      config_.store(prefix_ + "role_tag", _tag_, "The requested role tag for resources");
      config_.store(prefix_ + "tag_value", _value_, "The requested tag value for resources");

      return;
    }

    // virtual
    void by_tag_resource_selector::tree_dump(std::ostream & out_,
                                                 const std::string & title_,
                                                 const std::string & indent_,
                                                 bool inherit_) const
    {
      this->base_resource_selector::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Requested access : ";
      if (has_role_tag()) {
        out_ << "'" << _tag_ << " " << _value_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // namespace resource

} // namespace vire
