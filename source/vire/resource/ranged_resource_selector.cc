// vire/user/ranged_resource_selector.cc - Implementation of a selector of resources with explicit list of resource identifiers
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
#include <vire/resource/ranged_resource_selector.h>

// This project:
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace resource {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(ranged_resource_selector, "vire::resource::ranged_resource_selector");

    ranged_resource_selector::ranged_resource_selector(datatools::logger::priority logger_priority_)
      : base_resource_selector(logger_priority_)
    {
      return;
    }

    ranged_resource_selector::~ranged_resource_selector()
    {
      if (is_initialized()) {
        this->ranged_resource_selector::reset();
      }
      return;
    }

    int ranged_resource_selector::_accept()
    {
      int result = cuts::SELECTION_ACCEPTED;
      DT_LOG_TRACE(_logging, "Entering...");
      if (is_user_data<std::string>()) {
        const std::string & rpath = get_user_data<std::string>();
        // std::cerr << "DEVEL: ranged_resource_selector::_accept: "
        //           << "rpath='" << rpath << "'" << std::endl;
        if (!has_path(rpath)) {
          result = cuts::SELECTION_REJECTED;
        }
      } else  if (is_user_data<resource>()) {
        const resource & r = get_user_data<resource>();
        // std::cerr << "DEVEL: ranged_resource_selector::_accept: "
        //           << "r='" << r.get_name() << "'" << std::endl;
        if (!has_path(r.get_path())) {
          result = cuts::SELECTION_REJECTED;
        }
      }
      /* else if (is_user_data<int32_t>()) {
        int32_t candidate_rid = get_user_data<int32_t>();
        // std::cerr << "DEVEL: ranged_resource_selector::_accept: "
        //           << "candidate_rid=[" << candidate_rid << ']' << std::endl;
        if (candidate_rid == resource::INVALID_RID) {
          result = cuts::SELECTION_REJECTED;
        } else {
          if (get_set_of_rids().count(candidate_rid) == 0)  {
            result = cuts::SELECTION_REJECTED;
          }
        }
      }
      */
      // std::cerr << "DEVEL: ranged_resource_selector::_accept: "
      //           << "result=[" << result << "]" << std::endl;

      DT_LOG_TRACE(_logging, "Exiting.");
      return result;
    }

    void ranged_resource_selector::initialize(const datatools::properties & config_,
                                              datatools::service_manager & /* service_manager_ */,
                                              cuts::cut_handle_dict_type & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource selector '" << get_name() << "' is already initialized !");

      this->base_resource_selector::_base_init(config_);

      if (!has_range()) {
        std::string first_path;
        std::string second_path;

        if (config_.has_key("first_path")) {
          first_path = config_.fetch_string("first_path");
        }

        if (config_.has_key("second_path")) {
          second_path = config_.fetch_string("second_path");
        }

        set_range(first_path, second_path);

      }

      _set_initialized(true);
      return;
    }

    void ranged_resource_selector::reset()
    {
      _set_initialized(false);
      reset_range();
      this->base_resource_selector::_base_reset();
      return;
    }

    bool ranged_resource_selector::has_path(const std::string & rpath_) const
    {
      return rpath_ >= _first_path_ && rpath_ <= _second_path_;
    }

    bool ranged_resource_selector::has_range() const
    {
      return !_first_path_.empty() && !_second_path_.empty() && _first_path_ <= _second_path_;
    }

    void ranged_resource_selector::set_range(const std::string & first_path_, const std::string & second_path_)
    {
      DT_THROW_IF(second_path_ < first_path_, std::logic_error,
                  "Path='" << second_path_ << "' < path='" << first_path_<< "'!");
      if (has_resource_mgr()) {
        DT_THROW_IF(!get_resource_mgr().has_resource_by_path(first_path_),
                    std::logic_error,
                    "No first resource with path='" << first_path_ << "' exists in the resource manager!");
        DT_THROW_IF(!get_resource_mgr().has_resource_by_path(first_path_),
                    std::logic_error,
                    "No second resource with path='" << second_path_ << "' exists in the resource manager!");
      }
      _first_path_ = first_path_;
      _second_path_ = second_path_;
      return;
    }

    void ranged_resource_selector::reset_range()
    {
      _first_path_.clear();
      _second_path_.clear();
      return;
    }

    const std::string & ranged_resource_selector::get_first_path() const
    {
      return _first_path_;
    }

    const std::string & ranged_resource_selector::get_second_path() const
    {
      return _second_path_;
    }

    void ranged_resource_selector::export_to_config(datatools::properties & config_,
                                                        uint32_t flags_,
                                                        const std::string & prefix_) const
    {
      this->cuts::i_cut::export_to_config(config_, flags_, prefix_);

      config_.store_string(prefix_ + "first_path", _first_path_, "The first resource path in the range");
      config_.store_string(prefix_ + "second_path", _second_path_, "The second resource path in the range");

      return;
    }

    // virtual
    void ranged_resource_selector::tree_dump(std::ostream & out_,
                                                 const std::string & title_,
                                                 const std::string & indent_,
                                                 bool inherit_) const
    {
      this->base_resource_selector::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "First path : ";
      if (_first_path_.size()) {
        out_ << "'" << _first_path_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;


      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Second path : ";
      if (_second_path_.size()) {
        out_ << "'" << _second_path_ << "'";
      } else {
        out_ << "<none>";
      }

      return;
    }

  } // namespace resource

} // namespace vire
