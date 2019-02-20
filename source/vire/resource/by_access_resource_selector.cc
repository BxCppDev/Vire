// vire/user/by_access_resource_selector.cc - Implementation of a selector of resources with regular expression on resource paths
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
#include <vire/resource/by_access_resource_selector.h>

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
    CUT_REGISTRATION_IMPLEMENT(by_access_resource_selector, "vire::resource::by_access_resource_selector");

    by_access_resource_selector::by_access_resource_selector(datatools::logger::priority logger_priority_)
      : base_resource_selector(logger_priority_)
    {
      _access_ = ::vire::utility::RW_INVALID;
      return;
    }

    by_access_resource_selector::~by_access_resource_selector()
    {
      if (is_initialized()) {
        this->by_access_resource_selector::reset();
      }
      return;
    }

    int by_access_resource_selector::_accept()
    {
      int result = cuts::SELECTION_INAPPLICABLE;
      DT_LOG_TRACE(_logging, "Entering...");
      if (is_user_data<std::string>()) {
        const std::string & str_data = get_user_data<std::string>();
        DT_THROW_IF(!has_resource_mgr(), std::logic_error,
                    "No resource manager is set!");
        const resource & r  = get_resource_mgr().get_resource_by_path (str_data);
        //const resource & r get_resource_by_path (str_data);
        if (r.get_access() == _access_) {
           result = cuts::SELECTION_ACCEPTED;
        } else {
           result = cuts::SELECTION_REJECTED;
        }
        //std::cerr << "DEVEL: by_access_resource_selector::_accept: "
        //           << "str_user_data='" << str_user_data << "'" << std::endl;
      } else if (is_user_data<int32_t>()) {
        int32_t rid = get_user_data<int32_t>();
        const resource & r  = get_resource_mgr().get_resource_by_id (rid);
        DT_THROW_IF(!has_resource_mgr(), std::logic_error,
                    "No resource manager is set!");
        if (r.get_access() == _access_) {
           result = cuts::SELECTION_ACCEPTED;
        } else {
           result = cuts::SELECTION_REJECTED;
        }
      } else if (is_user_data<resource>()) {
        const resource & r = get_user_data<resource>();
        DT_THROW_IF(!r.is_valid(), std::logic_error,
                    "Resource is not valid!");
        if (r.get_access() == _access_) {
          result = cuts::SELECTION_ACCEPTED;
        } else {
          result = cuts::SELECTION_REJECTED;
        }
      }
      DT_LOG_TRACE(_logging, "Exiting.");
      return result;
    }

    void by_access_resource_selector::initialize(const datatools::properties & config_,
                                             datatools::service_manager & /* service_manager_ */,
                                             cuts::cut_handle_dict_type & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource selector '" << get_name() << "' is already initialized !");

      this->base_resource_selector::_base_init(config_);

      if (!has_access()) {
        if (config_.has_key("access")) {
          std::string access_label = config_.fetch_string("access");
          set_access_by_label(access_label);
        }
      }

      // Check:
      DT_THROW_IF(!has_access(), std::logic_error, "No access is set!");

      _set_initialized(true);
      return;
    }

    void by_access_resource_selector::reset()
    {
      _set_initialized(false);
      _access_ = ::vire::utility::RW_INVALID;
      this->base_resource_selector::_base_reset();
      return;
    }

    bool by_access_resource_selector::has_resource(const resource & r_) const
    {
      return has_access() && r_.get_access() == _access_;
    }

    bool by_access_resource_selector::has_access() const
    {
      return _access_ != ::vire::utility::RW_INVALID;
    }

    void by_access_resource_selector::set_access(::vire::utility::rw_access_type access_)
    {
      _access_= access_;
      return;
    }

    void by_access_resource_selector::set_access_by_label(const std::string & access_label_)
    {
      ::vire::utility::rw_access_type rw_access = ::vire::utility::RW_INVALID;
      DT_THROW_IF(!::vire::utility::from_string(access_label_, rw_access),
                  std::logic_error,
                  "Invalid R/W access label '" << access_label_ << "'!");
      _access_ = rw_access;
      return;
    }

    void by_access_resource_selector::reset_access()
    {
      _access_ = ::vire::utility::RW_INVALID;
      return;
    }

    ::vire::utility::rw_access_type by_access_resource_selector::get_access() const
    {
      return _access_;
    }

    void by_access_resource_selector::export_to_config(datatools::properties & config_,
                                                        uint32_t flags_,
                                                        const std::string & prefix_) const
    {
      this->cuts::i_cut::export_to_config(config_, flags_, prefix_);

      config_.store(prefix_ + "access", ::vire::utility::to_string(_access_), "The requested access for resources");

      return;
    }

    // virtual
    void by_access_resource_selector::tree_dump(std::ostream & out_,
                                                 const std::string & title_,
                                                 const std::string & indent_,
                                                 bool inherit_) const
    {
      this->base_resource_selector::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Requested access : ";
      if (has_access()) {
        out_ << "'" << vire::utility::to_string(_access_) << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // namespace resource

} // namespace vire
