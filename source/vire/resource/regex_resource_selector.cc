// vire/user/regex_resource_selector.cc - Implementation of a selector of resources with regular expression on resource paths
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
#include <vire/resource/regex_resource_selector.h>

// Third party:
// - Boost:
#include <boost/regex.hpp>

// This project:
#include <vire/resource/resource.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace resource {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(regex_resource_selector, "vire::resource::regex_resource_selector");

    struct regex_resource_selector::work
    {
      boost::scoped_ptr<boost::regex> hregex; ///< Regex handle
    };

    regex_resource_selector::regex_resource_selector(datatools::logger::priority logger_priority_)
      : base_resource_selector(logger_priority_)
    {
      return;
    }

    regex_resource_selector::~regex_resource_selector()
    {
      if (is_initialized()) {
        this->regex_resource_selector::reset();
      }
      return;
    }

    int regex_resource_selector::_accept()
    {
      int result = cuts::SELECTION_ACCEPTED;
      DT_LOG_TRACE(_logging, "Entering...");
      if (is_user_data<std::string>()) {
        const std::string & rpath = get_user_data<std::string>();
        if (!has_path(rpath)) {
          result = cuts::SELECTION_REJECTED;
        }
      } else  if (is_user_data<resource>()) {
        const resource & r = get_user_data<resource>();
        // std::cerr << "DEVEL: regex_resource_selector::_accept: "
        //           << "r='" << r.get_name() << "'" << std::endl;
        if (!has_path(r.get_path())) {
          result = cuts::SELECTION_REJECTED;
        }
      }
      DT_LOG_TRACE(_logging, "Exiting.");
      return result;
    }

    void regex_resource_selector::initialize(const datatools::properties & config_,
                                             datatools::service_manager & /* service_manager_ */,
                                             cuts::cut_handle_dict_type & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource selector '" << get_name() << "' is already initialized !");

      this->base_resource_selector::_base_init(config_);

      if (!has_resource_mgr()) {
      /*
      // Not implemented:
      vire::resource_service * resourcev_ptr = 0;
      if (service_manager_.is_a<vire::resource::service>("Resources")) {
      vire::resource::service & resourcev = service_manager_.grab<vire::resource::service>("Resources");
      set_resource_mgr(resourcev->get_resource_manager());
      }
      */
      }

      // if (has_resource_mgr()) {
      //   if (_rids_.empty()) {
      //     if (config_.has_key("resources.regex")) {
      //       std::string regexp;
      //       config_.fetch("resources.regex", regexp);
      //       _resource_mgr_->build_set_of_rids_from_path_regexp(regexp, _rids_);
      //     }
      //   }
      // }

      if (has_resource_mgr()) {
        if (config_.has_key("regex")) {
          std::string regex_paths = config_.fetch_string("regex");
          set_regex_paths(regex_paths);
        }
      }

      // Initialize internal data:
      _work_.reset(new work);
      _work_->hregex.reset(new boost::regex(_regex_paths_));

      _set_initialized(true);
      return;
    }

    void regex_resource_selector::reset()
    {
      _set_initialized(false);
      _work_->hregex.reset();
      _work_.reset();
      _regex_paths_.clear();
      this->base_resource_selector::_base_reset();
      return;
    }

    bool regex_resource_selector::has_path(const std::string & rpath_) const
    {
      if (_work_.get() == 0) return false;
      return boost::regex_match(rpath_, *_work_->hregex);
    }

    bool regex_resource_selector::has_regex_paths() const
    {
      return ! _regex_paths_.empty();
    }

    void regex_resource_selector::set_regex_paths(const std::string & regex_)
    {
      _regex_paths_= regex_;
      return;
    }

    const std::string & regex_resource_selector::get_regex_paths() const
    {
      return _regex_paths_;
    }

    void regex_resource_selector::export_to_config(datatools::properties & config_,
                                                        uint32_t flags_,
                                                        const std::string & prefix_) const
    {
      this->cuts::i_cut::export_to_config(config_, flags_, prefix_);

      config_.store(prefix_ + "regex", _regex_paths_, "The regular expression on resource paths");

      return;
    }

    // virtual
    void regex_resource_selector::tree_dump(std::ostream & out_,
                                                 const std::string & title_,
                                                 const std::string & indent_,
                                                 bool inherit_) const
    {
      this->base_resource_selector::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Regex paths : ";
      if (has_regex_paths()) {
        out_ << "'" << _regex_paths_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // namespace resource

} // namespace vire
