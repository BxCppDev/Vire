//! \file vire/cmsserver/uc_model_description.cc
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
#include <vire/cmsserver/uc_model_description.h>

// Standard Library:
#include <sstream>

// This project:
#include <vire/time/utils.h>

namespace vire {

  namespace cmsserver {

    // Serialization implementation
    DATATOOLS_SERIALIZATION_IMPLEMENTATION(uc_model_description, "vire::cmsserver::uc_model_description")

    uc_model_description::uc_model_description()
    {
      return;
    }

    uc_model_description::~uc_model_description()
    {
      reset();
      return;
    }

    void uc_model_description::configure()
    {
      _configure_(datatools::empty_config());
      return;
    }
    
    void uc_model_description::export_to_config(datatools::properties & config_,
                                                const uint32_t flags_,
                                                const std::string & prefix_) const
    {
      if (flags_ & EXPORT_CONFIG_CLEAR) {
        config_.clear();
      }

      if (flags_ & EXPORT_CONFIG_NAME) {
        if (has_name()) {
          config_.store_string(prefix_ + "name", get_name(), "Name");
        }
      }

      if (flags_ & EXPORT_CONFIG_TYPE_ID) {
        if (has_use_case_type_id()) {
          config_.store_string(prefix_ + "type_id", get_use_case_type_id(), "Use case type ID");
        }
      }

      if (flags_ & EXPORT_CONFIG_DESCRIPTION) {
        if (has_description()) {
          config_.store_string(prefix_ + "description", get_description(), "Description");
        }
      }

      if (flags_ & EXPORT_CONFIG_CONFIG_PATH) {
        if (has_use_case_config_path()) {
          config_.store_string(prefix_ + "config_path", get_use_case_config_path(), "Configuration file path");
        }
      }

      if (flags_ & EXPORT_CONFIG_CONFIG) {
        if (has_use_case_config()) {
          _use_case_config_.value().export_all_adding_prefix(config_, prefix_ + "config.");
        }
      }

      if (flags_ & EXPORT_CONFIG_COMPOSITION) {
        if (has_composition_description()) {
          uint32_t export_flags = uc_composition_description::EXPORT_CONFIG_LOCKED
            | uc_composition_description::EXPORT_CONFIG_SCHEDULING
            | uc_composition_description::EXPORT_CONFIG_REPETITIONS
            | uc_composition_description::EXPORT_CONFIG_DAUGHTERS;
          _composition_description_.export_to_config(config_, export_flags, "composition.");
        }
      }

      return;
    }

    void uc_model_description::configure(const datatools::properties & config_)
    {
      _configure_(config_);
      return;
    }

    void uc_model_description::_configure_(const datatools::properties & config_)
    {

      if (!has_name()) {
        if (config_.has_key("name")) {
          std::string name = config_.fetch_string("name");
          set_name(name);
        }
      }
      DT_THROW_IF(!has_name(), std::logic_error, "No name is set!");
      
      if (!has_description()) {
        if (config_.has_key("description")) {
          const std::string & desc = config_.fetch_string("description");
          set_description(desc);
        }
      }

      if (! has_use_case_type_id()) {
        if (config_.has_key("type_id")) {
          const std::string & use_case_type_id = config_.fetch_string("type_id");
          set_use_case_type_id(use_case_type_id);
        }
      }
      DT_THROW_IF(!has_use_case_type_id(), std::logic_error, "No use case type ID is set!");

      if (! has_use_case_config()) {
        datatools::properties use_case_cfg;
        config_.export_and_rename_starting_with(use_case_cfg, "config.", "");
        if (!use_case_cfg.empty()) {
          set_use_case_config(use_case_cfg);
        }
      }
      
      if (! has_use_case_config_path()) {
        if (config_.has_key("config_path")) {
          const std::string & use_case_config_path = config_.fetch_string("config_path");
          set_use_case_config_path(use_case_config_path);
        }
      }
      
      {
        datatools::properties composition_config;
        config_.export_and_rename_starting_with(composition_config, "composition.", "");
        _composition_description_.configure(composition_config);
      }

      return;
    }

    void uc_model_description::reset()
    {
      reset_use_case_config();
      _use_case_config_path_.clear();
      _use_case_type_id_.clear();
      _description_.clear();
      _name_.clear();
      return;
    }

    void uc_model_description::set_name(const std::string & name_)
    {
      _name_ = name_;
      return;
    }

    bool uc_model_description::has_name() const
    {
      return !_name_.empty();
    }

    const std::string & uc_model_description::get_name() const
    {
      return _name_;
    }

    bool uc_model_description::has_description() const
    {
      return !_description_.empty();
    }

    void uc_model_description::set_description(const std::string & description_)
    {
      _description_ = description_;
      return;
    }

    const std::string & uc_model_description::get_description() const
    {
      return _description_;
    }

    bool uc_model_description::has_use_case_type_id() const
    {
      return ! _use_case_type_id_.empty();
    }

    void uc_model_description::set_use_case_type_id(const std::string & use_case_type_id_)
    {
      _use_case_type_id_ = use_case_type_id_;
      return;
    }

    const std::string & uc_model_description::get_use_case_type_id() const
    {
      return _use_case_type_id_;
    }

    bool uc_model_description::has_use_case_config_path() const
    {
      return ! _use_case_config_path_.empty();
    }

    void uc_model_description::set_use_case_config_path(const std::string & use_case_config_path_)
    {
      DT_THROW_IF(has_use_case_config(), std::logic_error,
                  "Use case configuration is already set!");
      _use_case_config_path_ = use_case_config_path_;
      return;
    }

    const std::string & uc_model_description::get_use_case_config_path() const
    {
      return _use_case_config_path_;
    }

    bool uc_model_description::has_use_case_config() const
    {
      if (_use_case_config_) return true;
      return false;
    }

    const datatools::properties & uc_model_description::get_use_case_config() const
    {
      DT_THROW_IF(!has_use_case_config(), std::logic_error,
                  "No explicit use case configuration is defined!");
      return *_use_case_config_;
    }

    void uc_model_description::set_use_case_config(const datatools::properties & cfg_)
    {
      DT_THROW_IF(has_use_case_config_path(), std::logic_error,
                  "Use case configuration path is already set!");
      _use_case_config_ = cfg_;
      return;
    }

    void uc_model_description::reset_use_case_config()
    {
      _use_case_config_ = boost::none;
      return;
    }

    bool uc_model_description::is_leaf() const
    {
      return !has_composition_description();
    }

    bool uc_model_description::has_composition_description() const
    {
      return !_composition_description_.is_empty();
    }

    void uc_model_description::set_composition_description(const uc_composition_description & comp_)
    {
      _composition_description_ = comp_;
      return;
    }
 
    const uc_composition_description & uc_model_description::get_composition_description() const
    {
      return _composition_description_;
    }

    uc_composition_description & uc_model_description::grab_composition_description()
    {
      return _composition_description_;
    }

    void uc_model_description::print_tree(std::ostream & out_,
                                          const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }

      out_ << popts.indent << tag
           << "Name        : ";
      if (has_name()) {
        out_ << "'" << _name_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << tag
           << "Description : ";
      if (has_description()) {
        out_ << "'" << _description_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << tag
           << "Use case type ID : ";
      if (has_use_case_type_id()) {
        out_ << "'" << _use_case_type_id_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << tag
           << "Explicit use case configuration : ";
      if (!has_use_case_config()) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (has_use_case_config()) {
        std::ostringstream indentoss;
        indentoss << popts.indent << skip_tag;
        boost::get(_use_case_config_).tree_dump(out_, "", indentoss.str());
      }

      out_ << popts.indent << tag
           << "Use case configuration path : ";
      if (!has_use_case_config_path()) {
        out_ << "<none>";
      } else {
        out_ << "'" << _use_case_config_path_ << "'";
      }
      out_ << std::endl;

      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Composition description : ";
      if (is_leaf()) {
        out_ << "<none>";
      } 
      out_ << std::endl;
      if (!is_leaf()) {
        boost::property_tree::ptree popts2;
        std::ostringstream indentss2;
        indentss2 << popts.indent << inherit_skip_tag(popts.inherit);
        popts2.put(datatools::i_tree_dumpable::base_print_options::indent_key(),
                   indentss2.str());
        _composition_description_.print_tree(out_, popts2);
      }
      return;
    }

  } // namespace cmsserver

} // namespace vire
