//! \file vire/cmsserver/use_case_info.cc
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
#include <vire/cmsserver/use_case_info.h>

// This project:
#include <vire/time/utils.h>
#include <vire/user/user.h>
#include <vire/user/group.h>
#include <vire/user/manager.h>
#include <vire/resource/role.h>
#include <vire/resource/manager.h>
#include <vire/cmsserver/resource_cardinality.h>

namespace vire {

  namespace cmsserver {

    // Serialization implementation
    DATATOOLS_SERIALIZATION_IMPLEMENTATION(use_case_info, "vire::cmsserver::use_case_info")

    void use_case_info::_set_defaults_()
    {
      return;
    }

    use_case_info::use_case_info()
    {
      _set_defaults_();
      return;
    }

    use_case_info::~use_case_info()
    {
      reset();
      return;
    }

    bool use_case_info::is_valid() const
    {
      if (!has_name()) return false;
      if (!has_role_definition()) return false;
      if (!has_use_case_type_id()) return false;
      return true;
    }

    void use_case_info::initialize_simple()
    {
      datatools::properties dummy_config;
      _initialize_(dummy_config);
      return;
    }

    void use_case_info::initialize(const vire::user::manager & user_mgr_,
                                   const vire::resource::manager & resource_mgr_)
    {
      datatools::properties dummy_config;
      _initialize_(dummy_config, &user_mgr_, &resource_mgr_);
      return;
    }

    void use_case_info::initialize(const datatools::properties & config_,
                                  const vire::user::manager & user_mgr_,
                                  const vire::resource::manager & resource_mgr_)
    {
      _initialize_(config_, &user_mgr_, &resource_mgr_);
      return;
    }

    void use_case_info::_initialize_(const datatools::properties & config_,
                                    const vire::user::manager * /* user_mgr_ */,
                                    const vire::resource::manager * resource_mgr_)
    {

      if (!has_name()) {
        if (config_.has_key("name")) {
          std::string name = config_.fetch_string("name");
          set_name(name);
        }
      }

      if (!has_description()) {
        if (config_.has_key("description")) {
          const std::string & desc = config_.fetch_string("description");
          set_description(desc);
        }
      }

      if (!has_role_definition()) {
        if (config_.has_key("role_definition")) {
          const std::string & role_def = config_.fetch_string("role_definition");
          set_role_definition(role_def);
        }
      }


      //   if (resource_mgr_ != nullptr)  {

      //     // Search the role by name:
      //     if (_role_definition_.empty() && config_.has_key("role.name")) {
      //       const std::string & role_name = config_.fetch_string("role.name");
      //       DT_THROW_IF(!resource_mgr_->has_role_by_name(role_name),
      //                   std::logic_error,
      //                   "Role '" << role_name << "' does not exist!");
      //       set_role_definition(role_name);
      //     }

      //     // Search the role by ID:
      //     if (_role_definition_.empty() && config_.has_key("role.id")) {
      //       int role_id = config_.fetch_integer("role.id");
      //       DT_THROW_IF(!resource_mgr_->has_role_by_id(role_id),
      //                   std::logic_error,
      //                   "Role ID " << role_id << " does not exist!");
      //       std::string role_name = resource_mgr_->get_role_by_id(role_id).get_name();
      //       set_role_definition(role_name);
      //     }

      //     // Search the role by path:
      //     if (_role_definition_.empty() && config_.has_key("role.path")) {
      //       const std::string & role_path = config_.fetch_string("role.path");
      //       DT_THROW_IF(!resource_mgr_->has_role_by_path(role_path),
      //                   std::logic_error,
      //                   "Role path '" << role_path << "' does not exist!");
      //       std::string role_name = resource_mgr_->get_role_by_path(role_path).get_name();
      //       set_role_definition(role_name);
      //     }

      //   }
      // }

      // if (!has_special_functional_cardinalities()) {
      //   datatools::properties sfc_config;
      //   config_.export_and_rename_starting_with(sfc_config, "special_functional_cardinalities.", "");
      //   resource_cardinality wrapper(_special_functional_cardinalities_);
      //   wrapper.initialize(sfc_config, resource_mgr_);
      // }

      // if (!has_special_distributable_cardinalities()) {
      //   datatools::properties sdc_config;
      //   config_.export_and_rename_starting_with(sdc_config, "special_distributable_cardinalities.", "");
      //   resource_cardinality wrapper(_special_distributable_cardinalities_);
      //   wrapper.initialize(sdc_config, resource_mgr_);
      // }

      if (! has_use_case_type_id()) {
        if (config_.has_key("use_case.type_id")) {
          const std::string & use_case_type_id = config_.fetch_string("use_case.type_id");
          set_use_case_type_id(use_case_type_id);
        }
      }

      if (! has_use_case_config()) {

        datatools::properties use_case_cfg;
        config_.export_and_rename_starting_with(use_case_cfg, "use_case.config.", "");
        if (!use_case_cfg.empty()) {
          set_use_case_config(use_case_cfg);
        } else {
          if (! has_use_case_config_path()) {
            if (config_.has_key("use_case.config_path")) {
              const std::string & use_case_config_path = config_.fetch_string("use_case.config_path");
              set_use_case_config_path(use_case_config_path);
            }
          }
        }

      }

      return;
    }

    void use_case_info::reset()
    {
      reset_use_case_config();
      _use_case_config_path_.clear();
      _use_case_type_id_.clear();
      // _special_functional_cardinalities_.clear();
      // _special_distributable_cardinalities_.clear();
       _role_definition_.clear();
      _description_.clear();
      _name_.clear();
      _set_defaults_();
      return;
    }

    void use_case_info::set_name(const std::string & name_)
    {
      _name_ = name_;
      return;
    }

    bool use_case_info::has_name() const
    {
      return !_name_.empty();
    }

    const std::string & use_case_info::get_name() const
    {
      return _name_;
    }

    bool use_case_info::has_description() const
    {
      return !_description_.empty();
    }

    void use_case_info::set_description(const std::string & description_)
    {
      _description_ = description_;
      return;
    }

    const std::string & use_case_info::get_description() const
    {
      return _description_;
    }

    bool use_case_info::has_role_definition() const
    {
      return ! _role_definition_.empty();
    }

    void use_case_info::set_role_definition(const std::string & identifier_)
    {
      _role_definition_ = identifier_;
      return;
    }

    const std::string & use_case_info::get_role_definition() const
    {
      return _role_definition_;
    }

    bool use_case_info::has_use_case_type_id() const
    {
      return ! _use_case_type_id_.empty();
    }

    void use_case_info::set_use_case_type_id(const std::string & use_case_type_id_)
    {
      _use_case_type_id_ = use_case_type_id_;
      return;
    }

    const std::string & use_case_info::get_use_case_type_id() const
    {
      return _use_case_type_id_;
    }

    bool use_case_info::has_use_case_config_path() const
    {
      return ! _use_case_config_path_.empty();
    }

    void use_case_info::set_use_case_config_path(const std::string & use_case_config_path_)
    {
      _use_case_config_path_ = use_case_config_path_;
      return;
    }

    const std::string & use_case_info::get_use_case_config_path() const
    {
      return _use_case_config_path_;
    }

    bool use_case_info::has_use_case_config() const
    {
      if (_use_case_config_) return true;
      return false;
    }

    const datatools::properties & use_case_info::get_use_case_config() const
    {
      DT_THROW_IF(!has_use_case_config(), std::logic_error,
                  "No explicit use_case configuration is defined!");
      return *_use_case_config_;
    }

    datatools::properties & use_case_info::grab_use_case_config()
    {
      if (!_use_case_config_) {
        static datatools::properties empty_config;
        _use_case_config_ = empty_config;
      }
      return *_use_case_config_;
    }

    void use_case_info::set_use_case_config(const datatools::properties & cfg_)
    {
      _use_case_config_ = cfg_;
      return;
    }

    void use_case_info::reset_use_case_config()
    {
      _use_case_config_ = boost::none;
      return;
    }

    /*
    bool use_case_info::has_special_functional_cardinalities() const
    {
      return _special_functional_cardinalities_.size() > 0;
    }

    const cardinalities_request_type & use_case_info::get_special_functional_cardinalities() const
    {
      return _special_functional_cardinalities_;
    }

    bool use_case_info::has_special_distributable_cardinalities() const
    {
      return _special_distributable_cardinalities_.size() > 0;
    }

    const cardinalities_request_type & use_case_info::get_special_distributable_cardinalities() const
    {
      return _special_distributable_cardinalities_;
    }
    */

    void use_case_info::tree_dump(std::ostream & out_,
                                  const std::string & title_,
                                  const std::string & indent_,
                                  bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Name         : ";
      if (has_name()) {
        out_ << "'" << _name_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Description : ";
      if (has_description()) {
        out_ << "'" << _description_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Role definition : ";
      if (has_role_definition()) {
        out_ << "'" << _role_definition_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      // out_ << indent_ << i_tree_dumpable::tag
      //      << "Special request on functional resource cardinality    : ";
      // if (has_special_functional_cardinalities()) {
      //   out_ << "[" << _special_functional_cardinalities_.size() << "]";
      // } else {
      //   out_ << "<none>";
      // }
      // out_ << std::endl;

      // out_ << indent_ << i_tree_dumpable::tag
      //      << "Special request on distributable resource cardinality : ";
      // if (has_special_distributable_cardinalities()) {
      //   out_ << "[" << _special_distributable_cardinalities_.size() << "]";
      // } else {
      //   out_ << "<none>";
      // }
      // out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Use case type ID : ";
      if (has_use_case_type_id()) {
        out_ << "'" << _use_case_type_id_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Explicit use case configuration : ";
      if (!has_use_case_config()) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (has_use_case_config()) {
        std::ostringstream indentoss;
        indentoss << indent_ << i_tree_dumpable::skip_tag;
        boost::get(_use_case_config_).tree_dump(out_, "", indentoss.str());
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Use case configuration path : ";
      if (!has_use_case_config_path()) {
        out_ << "<none>";
      } else {
        out_ << "'" << _use_case_config_path_ << "'";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Valid : " << std::boolalpha << is_valid() << std::endl;

      return;
    }

    /*
    void use_case_info::set_special_functional_limited(int32_t resource_id_, std::size_t cardinality_)
    {
      resource_cardinality wrapper(_special_functional_cardinalities_);
      wrapper.set_limited_resource(resource_id_, cardinality_);
      return;
    }

    void use_case_info::set_special_functional_unlimited(int32_t resource_id_)
    {
      resource_cardinality wrapper(_special_functional_cardinalities_);
      wrapper.set_unlimited_resource(resource_id_);
      return;
    }

    void use_case_info::unset_special_functional(int32_t resource_id_)
    {
      resource_cardinality wrapper(_special_functional_cardinalities_);
      wrapper.unset_resource(resource_id_);
      return;
    }

    void use_case_info::set_special_distributable_limited(int32_t resource_id_, std::size_t cardinality_)
    {
      resource_cardinality wrapper(_special_distributable_cardinalities_);
      wrapper.set_limited_resource(resource_id_, cardinality_);
      return;
    }

    void use_case_info::set_special_distributable_unlimited(int32_t resource_id_)
    {
      resource_cardinality wrapper(_special_distributable_cardinalities_);
      wrapper.set_unlimited_resource(resource_id_);
      return;
    }

    void use_case_info::unset_special_distributable(int32_t resource_id_)
    {
      resource_cardinality wrapper(_special_distributable_cardinalities_);
      wrapper.unset_resource(resource_id_);
      return;
    }
    */

  } // namespace cmsserver

} // namespace vire
