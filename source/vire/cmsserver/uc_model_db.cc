//! \file vire/cmsserver/uc_model_db.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/uc_model_db.h>

// Third party:
#include <bayeux/datatools/exception.h>

// This project:
#include <vire/cmsserver/base_use_case.h>

namespace vire {

  namespace cmsserver {

    // Serialization implementation
    DATATOOLS_SERIALIZATION_IMPLEMENTATION(uc_model_db, "vire::cmsserver::uc_model_db")

    uc_model_db::uc_model_db()
    {
      return;
    }

    // virtual
    uc_model_db::~uc_model_db()
    {
      return;
    }

    void uc_model_db::set_check_uc_factory(const bool check_)
    {
      _check_uc_factory_ = check_;
      return;
    }

    bool uc_model_db::is_check_uc_factory() const
    {
      return _check_uc_factory_;
    }

    bool uc_model_db::match_type_id(const std::string & type_id_) const
    {
      if (_supported_types_.size()) {
        return _supported_types_.count(type_id_);
      }
      return true;
    }

    void uc_model_db::add_supported_type_id(const std::string & type_id_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Database is locked!");
      if (is_check_uc_factory()) {
        if (!DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(vire::cmsserver::base_use_case).has(type_id_)) {
          DT_THROW(std::logic_error,
                   "Use cas type ID '" << type_id_ << "' is not registered in the system factory!");
        }
      }
      _supported_types_.insert(type_id_);
      return;
    }

    bool uc_model_db::has_supported_type_id(const std::string & type_id_) const
    {
      return _supported_types_.count(type_id_);
    }

    bool uc_model_db::has(const std::string & name_) const
    {
      return _models_.count(name_);
    }

    bool uc_model_db::has_cyclic_dependencies() const
    {
      return _cyclic_dependencies_.size();
    }

    const std::set<std::string> & uc_model_db::get_cyclic_dependencies() const
    {
      return _cyclic_dependencies_;     
    }
 
    bool uc_model_db::has_unsatisfied_dependencies() const
    {
      return _unsatisfied_dependencies_.size();
    }

    const std::set<std::string> & uc_model_db::get_unsatisfied_dependencies() const
    {
      return _unsatisfied_dependencies_;     
    }
    
    void uc_model_db::_add_check_(const uc_model_description & umd_)
    {
      if (!_dependencies_.has_vertex(umd_.get_name())) {
        _dependencies_.add_vertex(umd_.get_name(), "use_case_model");
      }
      if (_unsatisfied_dependencies_.count(umd_.get_name())) {
        _unsatisfied_dependencies_.erase(umd_.get_name());
      }
      if (umd_.has_composition_description()) {
        const uc_composition_description & ucd = umd_.get_composition_description();
        std::set<std::string> daughter_names;
        ucd.build_daughter_names(daughter_names);
        for (const auto & name : daughter_names) {
          const uc_composition_description::daughter_entry & de = ucd.get_daughter(name);
          const std::string & model_id = de.model_id;
          if (!has(model_id)) {
            _unsatisfied_dependencies_.insert(model_id);
          }
          if (!_dependencies_.has_vertex(de.model_id)) {
            _dependencies_.add_vertex(de.model_id, "use_case_model");
          }
          _dependencies_.add_out_edge(umd_.get_name(), de.model_id, "dependency");
        }
        if (_dependencies_.has_cycle()) {
          _cyclic_dependencies_.insert(umd_.get_name());
        }
      }
      return;
    }
    
    void uc_model_db::add(const uc_model_description & umd_)
    {
      DT_THROW_IF(is_locked(),
                  std::logic_error,
                  "Use case model database is locked!");
      DT_THROW_IF(!umd_.has_name(),
                  std::logic_error,
                  "Use case model has no name!");
      DT_THROW_IF(has(umd_.get_name()),
                  std::logic_error,
                  "Database already has a model named '" << umd_.get_name() << "'!");
      DT_THROW_IF(!umd_.has_use_case_type_id(),
                  std::logic_error,
                  "Use case model '" << umd_.get_name() << "' has no type ID!");
      DT_THROW_IF(!match_type_id(umd_.get_use_case_type_id()),
                  std::logic_error,
                  "Database does not accept use case type ID '" << umd_.get_use_case_type_id() << "'!");
      _add_check_(umd_);
      _models_[umd_.get_name()] = umd_;
      return;
    }

    void uc_model_db::add(const std::string & name_,
                          const std::string & type_id_,
                          const std::string & description_,
                          const uc_composition_description & composition_,
                          const datatools::properties & config_)
    {
      DT_THROW_IF(is_locked(),
                  std::logic_error,
                  "Use case model database is locked!");
      DT_THROW_IF(has(name_),
                  std::logic_error,
                  "Database already has a model named '" << name_ << "'!");
      DT_THROW_IF(!match_type_id(type_id_),
                  std::logic_error,
                  "Database does not accept use case type ID '" << type_id_ << "'!");
      {
        uc_model_description umd;
        _models_[name_] = umd;
      }
      uc_model_description & umd = _models_.find(name_)->second;
      umd.set_name(name_);
      umd.set_description(description_);
      umd.set_use_case_type_id(type_id_);
      umd.set_use_case_config(config_);
      return;
    }

    void uc_model_db::add(const std::string & name_,
                          const std::string & type_id_,
                          const std::string & description_,
                          const uc_composition_description & composition_description_,
                          const std::string & config_path_)
    {
      DT_THROW_IF(is_locked(),
                  std::logic_error,
                  "Database is locked!");
      DT_THROW_IF(has(name_),
                  std::logic_error,
                  "Database already has a model named '" << name_ << "'!");
      DT_THROW_IF(!match_type_id(type_id_),
                  std::logic_error,
                  "Database does not accept use case type ID '" << type_id_ << "'!");
      {
        uc_model_description umd;
        _models_[name_] = umd;
      }
      uc_model_description & umd = _models_.find(name_)->second;
      umd.set_name(name_);
      umd.set_description(description_);
      umd.set_use_case_type_id(type_id_);
      umd.set_use_case_config_path(config_path_);
      umd.set_composition_description(composition_description_);
      return;
    }

    void uc_model_db::remove(const std::string & name_)
    {
      DT_THROW_IF(is_locked(),
                  std::logic_error,
                  "Database is locked!");
      DT_THROW_IF(!has(name_),
                  std::logic_error,
                  "Database has no model named '" << name_ << "'!");
      _models_.erase(name_);
      return;
    }

    void uc_model_db::remove_all()
    {
      DT_THROW_IF(is_locked(),
                  std::logic_error,
                  "Database is locked!");
      _models_.clear();
      return;
    }

    void uc_model_db::build_list_of_model_names(std::vector<std::string> & names_) const
    {
      names_.clear();
      for (const auto & p : _models_) {
        names_.push_back(p.first);
      }
      return;
    }

    const uc_model_db::dict_type & uc_model_db::get_models() const
    {
      return _models_;
    }

    const uc_model_description & uc_model_db::get(const std::string & name_) const
    {
      dict_type::const_iterator found = _models_.find(name_);
      DT_THROW_IF(found == _models_.end(),
                  std::logic_error,
                  "Database has no model named '" << name_ << "'!");
      return found->second;
    }

    // void uc_model_db::store(datatools::multi_properties & model_defs_)
    // {
    //   return;
    // }
      
    void uc_model_db::load(const datatools::multi_properties & model_defs_)
    {
      DT_THROW_IF(is_locked(),
                  std::logic_error,
                  "Database is locked!");
      DT_THROW_IF(model_defs_.get_key_label() != "name",
                  std::logic_error,
                  "Cannot find the 'name' key label!");
      DT_THROW_IF(model_defs_.get_meta_label() != "type",
                  std::logic_error,
                  "Cannot find the 'type' meta label!");       
      const datatools::multi_properties::entries_ordered_col_type & oentries
        = model_defs_.ordered_entries();
      for (datatools::multi_properties::entries_ordered_col_type::const_iterator i
             = oentries.begin();
           i != oentries.end();
           i++) {
        const datatools::multi_properties::entry & e = **i;
        const std::string &           model_name    = e.get_key();
        const std::string &           model_type_id = e.get_meta();
        const datatools::properties & model_config  = e.get_properties();
        std::string                   model_description = model_config.get_description();
        uc_model_description umd;
        umd.set_name(model_name);
        umd.set_use_case_type_id(model_type_id);
        umd.set_description(model_description);
        datatools::properties comp_config;
        model_config.export_and_rename_starting_with(comp_config, "composition.", "");
        if (! comp_config.empty()) {
          uc_composition_description model_composition_desc;
          model_composition_desc.configure(comp_config);
          umd.set_composition_description(model_composition_desc);
        }
        if (model_config.has_key("config_path")) {
          std::string config_path = model_config.fetch_path("config_path");
          umd.set_use_case_config_path(config_path);
        } else {
          datatools::properties uc_config;
          model_config.export_not_starting_with(uc_config, "composition.");
          if (! uc_config.empty()) {
            umd.set_use_case_config(uc_config);
          }
        }
        add(umd);
       }
      return;
    }
 
    bool uc_model_db::is_locked() const
    {
      return _locked_;
    }

    void uc_model_db::lock()
    {
      _locked_ = true;
      return;
    }

    void uc_model_db::unlock()
    {
      _locked_ = false;
      return;
    }

    void uc_model_db::configure(const datatools::properties & config_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Use case model DB is locked!");

      std::set<std::string> supported_types;
      if (config_.has_key("model_supported_types")) {
        config_.fetch("model_supported_types", supported_types);
        for (auto sut : supported_types) {
          this->add_supported_type_id(sut);
        }
      }

      std::vector<std::string> model_defs_paths;
      if (config_.has_key("model_definition_paths")) {
        config_.fetch("model_definition_paths", model_defs_paths);
        for (auto model_defs_path : model_defs_paths) {
          datatools::fetch_path_with_env(model_defs_path);
          datatools::multi_properties model_defs;
          model_defs.read(model_defs_path);
          this->load(model_defs);
        }
      }

      if (config_.has_key("locked")) {
        bool locked = config_.fetch_boolean("locked");
        if (locked) {
          this->lock();
        }
      }
    
      return;
    }
    
    void uc_model_db::reset()
    {
      _locked_ = false;
      remove_all();
      _supported_types_.clear();
      return;
    }
    
    void uc_model_db::print_tree(std::ostream & out_,
                                 const boost::property_tree::ptree & options_) const
    {
      /* datatools::i_tree_dumpable::*/
      base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }

      out_ << popts.indent << tag << "Check use case type IDs : "
           << std::boolalpha << _check_uc_factory_ << std::endl;
 
      out_ << popts.indent << tag << "Supported types : ";
      if (_supported_types_.size()) {
        out_ << _supported_types_.size();
      } else {
        out_ << "<all>";
      }
      out_ << std::endl;
      if (_supported_types_.size()) {
        std::size_t count = 0;
        for (const std::string & type_id : _supported_types_) {
          out_ << popts.indent << skip_tag;
          if (++count == _supported_types_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Use case type ID = '" << type_id << "'" << std::endl;
        }
      }

      out_ << popts.indent << tag << "Models : ";
      if (_models_.size()) {
        out_ << _models_.size();
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (_models_.size()) {
        std::size_t count = 0;
        for (const auto & p : _models_) {
          out_ << popts.indent << skip_tag;
          std::ostringstream indent_ss2;
          indent_ss2 << popts.indent << skip_tag;
          if (++count == _models_.size()) {
            out_ << last_tag;
            indent_ss2  << last_skip_tag;
          } else {
            out_ << tag;
            indent_ss2  << skip_tag;
           }
          out_ << "Use case model : " << std::endl;
          {
            boost::property_tree::ptree popts2;
            popts2.put(datatools::i_tree_dumpable::base_print_options::indent_key(),
                       indent_ss2.str());
            p.second.print_tree(out_, popts2);
          }
          // out_ << "Use case model = '" << p.first << "' : "
          //      << p.second.get_description() << std::endl;
        }
      }

      out_ << popts.indent << inherit_tag(popts.inherit) << "Locked : "
           << std::boolalpha << is_locked() << std::endl;

      return;
    }

  } // namespace cmsserver

} // namespace vire
