// vire/user/enumerated_resource_selector.cc - Implementation of a selector of resources with explicit list of resource identifiers
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
#include <vire/resource/enumerated_resource_selector.h>

// This project:
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace resource {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(enumerated_resource_selector, "vire::resource::enumerated_resource_selector");

    enumerated_resource_selector::enumerated_resource_selector(datatools::logger::priority logger_priority_)
      : base_resource_selector(logger_priority_)
    {
      return;
    }

    enumerated_resource_selector::~enumerated_resource_selector()
    {
      if (is_initialized()) {
        this->enumerated_resource_selector::reset();
      }
      return;
    }

    int enumerated_resource_selector::_accept()
    {
      int result = cuts::SELECTION_ACCEPTED;
      DT_LOG_TRACE(_logging, "Entering...");
      if (is_user_data<std::string>()) {
        const std::string & rpath = get_user_data<std::string>();
        // std::cerr << "DEVEL: enumerated_resource_selector::_accept: "
        //           << "rpath='" << rpath << "'" << std::endl;
        if (_resource_paths_.count(rpath) == 0) {
          result = cuts::SELECTION_REJECTED;
        }
      } else  if (is_user_data<resource>()) {
        const resource & r = get_user_data<resource>();
        if (_resource_paths_.count(r.get_path()) == 0) {
          result = cuts::SELECTION_REJECTED;
        }
      } else if (is_user_data<int32_t>()) {
        int32_t candidate_rid = get_user_data<int32_t>();
        // std::cerr << "DEVEL: enumerated_resource_selector::_accept: "
        //           << "candidate_rid=[" << candidate_rid << ']' << std::endl;
        if (candidate_rid == resource::INVALID_ID) {
          result = cuts::SELECTION_REJECTED;
        } else {
          DT_THROW_IF(!has_resource_mgr(), std::logic_error,
                      "Missing resource manager needed for resource path->ID resolution!");
          DT_THROW_IF(!get_resource_mgr().has_resource_by_id(candidate_rid), std::logic_error,
                      "Resource manager has no resource with ID=[" << candidate_rid << "]!");
          const resource & r = get_resource_mgr().get_resource_by_id(candidate_rid);
          if (_resource_paths_.count(r.get_path()) == 0) {
            result = cuts::SELECTION_REJECTED;
          }
        }
      } else {
        result = cuts::SELECTION_INAPPLICABLE;
      }
      // std::cerr << "DEVEL: enumerated_resource_selector::_accept: "
      //           << "result=[" << result << "]" << std::endl;

      DT_LOG_TRACE(_logging, "Exiting.");
      return result;
    }

    void enumerated_resource_selector::initialize(const datatools::properties & config_,
                                                  datatools::service_manager & /* service_manager_ */,
                                                  cuts::cut_handle_dict_type & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource selector '" << get_name() << "' is already initialized !");

      this->base_resource_selector::_base_init(config_);

      if (has_resource_mgr()) {
        if (config_.has_key("resources")) {
          std::vector<std::string> rpaths;
          config_.fetch("resources", rpaths);
          for (auto rpath : rpaths) {
            add_path(rpath);
          }
        }
      }

      _set_initialized(true);
      return;
    }

    void enumerated_resource_selector::reset()
    {
      _set_initialized(false);
      _resource_paths_.clear();
      this->base_resource_selector::_base_reset();
      return;
    }

    bool enumerated_resource_selector::has_path(const std::string & rpath_) const
    {
      return _resource_paths_.count(rpath_) == 1;
    }

    void enumerated_resource_selector::add_path(const std::string & rpath_)
    {
      DT_THROW_IF(has_path(rpath_), std::logic_error,
                  "Resource with path='" << rpath_ << "' already belongs to this enumeration!");
      if (has_resource_mgr()) {
        // std::cerr << "DEVEL: add_path: " << "Found a res. mgr..." << std::endl;
        DT_THROW_IF(!get_resource_mgr().has_resource_by_path(rpath_),
                    std::logic_error,
                    "No resource with path='" << rpath_ << "' exists in the resource manager!");
      }
      // std::cerr << "DEVEL: add_path: Resource path '" << rpath_ << "' was added in the selector..." << std::endl;
      _resource_paths_.insert(rpath_);
      return;
    }

    void enumerated_resource_selector::remove_path(const std::string & rpath_)
    {
      DT_THROW_IF(!has_path(rpath_), std::logic_error,
                  "Resource with path='" << rpath_ << "' does not belong to this enumeration!")
      _resource_paths_.erase(_resource_paths_.find(rpath_));
      return;
    }

    const std::set<std::string> & enumerated_resource_selector::get_resource_paths() const
    {
      return _resource_paths_;
    }

    void enumerated_resource_selector::export_to_config(datatools::properties & config_,
                                                        uint32_t flags_,
                                                        const std::string & prefix_) const
    {
      this->cuts::i_cut::export_to_config(config_, flags_, prefix_);

      std::vector<std::string> rpaths;
      for (const auto rpath : _resource_paths_) {
        rpaths.push_back(rpath);
      }
      config_.store(prefix_ + "resources", rpaths, "The list of resource path");

      return;
    }

    // virtual
    void enumerated_resource_selector::tree_dump(std::ostream & out_,
                                                 const std::string & title_,
                                                 const std::string & indent_,
                                                 bool inherit_) const
    {
      this->base_resource_selector::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Resource paths : ";
      if (_resource_paths_.size()) {
        out_ << _resource_paths_.size();
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // namespace resource

} // namespace vire
