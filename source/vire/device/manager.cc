//! \file vire/device/manager.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/device/manager.h>

// Standard Library:
#include <stdexcept>
#include <sstream>
#include <fstream>

// Third Party:
// - Bayeux/datatools:
#include <datatools/ioutils.h>
#include <datatools/utils.h>
#include <datatools/properties.h>
#include <datatools/multi_properties.h>
#include <datatools/exception.h>
#include <datatools/logger.h>
// - Bayeux/geomtools:
#include <geomtools/mapping_utils.h>

// This project:
#include <vire/device/base_device_model.h>
#include <vire/device/mapping_utils.h>
#include <vire/device/logical_port.h>
#include <vire/device/logical_device.h>
#include <vire/utility/path.h>
#include <vire/device/instance_tree_builder.h>

namespace vire {

  namespace device {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(manager, "vire::device::manager");

    // static
    const std::string & manager::default_top_level_device_model_name()
    {
      static std::string _name("setup");
      return _name;
    }

    // static
    const std::string & manager::default_service_name()
    {
      static std::string _name("devices");
      return _name;
    }

    vire::utility::instance_identifier manager::get_setup_id() const
    {
      vire::utility::instance_identifier iid;
      iid.set(_setup_label_, _setup_version_);
      return iid;
    }

    void manager::set_setup_label(const std::string & label_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Manager is already initialized !");
      static const uint32_t nv_flags = datatools::NV_NO_HYPHEN | datatools::NV_NO_COLON | datatools::NV_NO_DOT;
      DT_THROW_IF(!datatools::name_validation(label_,nv_flags),
                  std::logic_error, "Invalid setup label '" << label_ << "'!");
      _setup_label_ = label_;
      return;
    }

    void manager::set_setup_version(const std::string & version_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Manager is already initialized !");
      _setup_version_ = version_;
      return;
    }

    bool manager::has_setup_label() const
    {
      return !_setup_label_.empty();
    }

    bool manager::has_setup_version() const
    {
      return !_setup_version_.empty();
    }

    bool manager::has_setup_description() const
    {
      return !_setup_description_.empty();
    }

    void manager::set_setup_description(const std::string & description_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Manager is already initialized !");
      _setup_description_ = description_;
      return;
    }

    const std::string& manager::get_setup_label() const
    {
      return _setup_label_;
    }

    const std::string& manager::get_setup_version() const
    {
      return _setup_version_;
    }

    const std::string & manager::get_setup_description() const
    {
      return _setup_description_;
    }

    void manager::set_top_level_device_model_name(const std::string & name_)
    {
      _top_level_device_model_name_ = name_;
      return;
    }

    const std::string & manager::get_top_level_device_model_name() const
    {
      return _top_level_device_model_name_;
    }

    void manager::add_factory_preload_system_only(const std::string & id_)
    {
      DT_THROW_IF(std::find(_factory_preload_system_only_.begin(),
                            _factory_preload_system_only_.end(),
                            id_) != _factory_preload_system_only_.end(),
                  std::logic_error,
                  "Model ID '" << id_ << "' is already marked as preloadable!");
      _factory_preload_system_only_.push_back(id_);
      if (_factory_preload_system_all_) {
        _factory_preload_system_all_ = false;
      }
      return;
    }

    void manager::set_factory_preload_system_all(bool b_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Manager is already initialized !");
      DT_THROW_IF(b_ && _factory_preload_system_only_.size(),
                  std::logic_error,
                  "Preloading of a limited set of specific model factories from system registry is activated!");
      _factory_preload_system_all_ = b_;
      return;
    }

    bool manager::is_factory_preload_system_all() const
    {
      return _factory_preload_system_all_;
    }

    void manager::set_propagate_logging_to_devices(bool p_)
    {
      _propagate_logging_to_devices_ = p_;
      return;
    }

    bool manager::is_propagate_logging_to_devices() const
    {
      return _propagate_logging_to_devices_;
    }

    void manager::set_force_initialization_at_load(bool b_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Manager is already initialized !");
      _force_initialization_at_load_ = b_;
      return;
    }

    bool manager::is_force_initialization_at_load() const
    {
      return _force_initialization_at_load_;
    }

    void manager::set_mapping_requested(bool b_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Manager is already initialized !");
      _mapping_requested_ = b_;
      return;
    }

    bool manager::is_mapping_requested() const
    {
      return _mapping_requested_;
    }

    void manager::add_auxiliary_property_prefix(const std::string & prefix_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error, "Manager is already initialized !");
      DT_THROW_IF(prefix_.empty(),
                  std::logic_error,
                  "Property prefix to be preserved cannot be empty !");
      DT_THROW_IF((std::find(_auxiliary_property_prefixes_.begin (),
                             _auxiliary_property_prefixes_.end (),
                             prefix_) != _auxiliary_property_prefixes_.end()),
                  std::logic_error,
                  "Property prefix to be preserved '" << prefix_<< "' already exists !");
      DT_LOG_DEBUG(get_logging_priority(), "Add prefix '" << prefix_ << "'");
      _auxiliary_property_prefixes_.push_back(prefix_);
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    const std::vector<std::string> &
    manager::get_auxiliary_property_prefixes() const
    {
      return _auxiliary_property_prefixes_;
    }

    bool manager::is_initialized() const
    {
      return _initialized_;
    }

    void manager::load_device_model(const std::string & name_,
                                    const std::string & id_,
                                    const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Device manager is already initialized !");
      this->_load_device_model(name_, id_, config_);
      return;
    }

    void manager::load_port_model(const std::string & name_,
                                  const std::string & id_,
                                  const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Device manager is already initialized !");
      this->_load_port_model(name_, id_, config_);
      return;
    }

    void manager::load_model_definitions(const std::string & filename_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(this->is_initialized(), std::logic_error,
                  "Device manager is already initialized !");
      std::string filename = filename_;
      datatools::fetch_path_with_env(filename);
      DT_LOG_DEBUG(get_logging_priority(), "Loading definitions of models from file '" << filename << "'...");
      datatools::multi_properties mconfig;
      mconfig.read(filename);
      this->load_models(mconfig);
      return;
    }

    void manager::load_model_definitions_list(const std::string & filename_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(this->is_initialized(), std::logic_error, "Device manager is already initialized !");
      std::string filename = filename_;
      DT_LOG_DEBUG(get_logging_priority(), "Loading definitions of models from list file '" << filename << "'...");
      datatools::fetch_path_with_env(filename);
      std::ifstream list_file(filename.c_str());
      DT_THROW_IF(!list_file, std::runtime_error, "Cannot open list of model definitions file '"
                  << filename_ << "'!");
      while (!list_file.eof()) {
        std::string line;
        std::getline(list_file, line);
        DT_THROW_IF(! list_file, std::logic_error,
                    "I/O error while reading file '" << filename_ << "' !");
        std::string word;
        std::istringstream line_iss(line);
        line_iss >> word;
        if (word.length() < 1) {
          // skip blank line
          continue;
        }
        if (word[0] == '#') {
          // skip comment line
          continue;
        }
        std::string model_defs_filename = word;
        load_model_definitions(model_defs_filename);
        list_file >> std::ws;
        if (list_file.eof()) {
          break;
        }
      }
      return;
    }

    void manager::load_models(const datatools::multi_properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(this->is_initialized(), std::logic_error, "Device manager is already initialized !");
      for (datatools::multi_properties::entries_ordered_col_type::const_iterator i
             = config_.ordered_entries().begin();
           i != config_.ordered_entries().end();
           ++i) {
        datatools::multi_properties::entry * mpe = *i;
        const std::string & model_name_info = mpe->get_key();
        // std::cerr << "DEVEL: manager::load_models: model_name_info='" << model_name_info << "'" << std::endl;
        DT_LOG_DEBUG(get_logging_priority(), "Loading model '" << model_name_info << "'...");
        model_type mtype = MODEL_UNDEFINED;
        std::string model_name;
        DT_THROW_IF(! decode_model_key(model_name_info, mtype, model_name),
                    std::logic_error,
                    "Invalid model name format '" << model_name_info << "'!");
        const std::string & model_id = mpe->get_meta();
        DT_LOG_DEBUG (get_logging_priority(), "Configuration for device named '"
                      << model_name << "'"
                      << " with ID '" << model_id << "'...");
        if (mtype == MODEL_DEVICE) {
          this->_load_device_model(model_name, model_id, mpe->get_properties());
        }
        if (mtype == MODEL_PORT) {
          this->_load_port_model(model_name, model_id, mpe->get_properties());
        }
      }
     DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    manager::device_factory_register_type & manager::_grab_device_factory_register()
    {
      if (! _device_factory_register_) {
        uint32_t factory_flags = 0;
        _device_factory_register_.reset(new device_factory_register_type("vire::device::base_device_model/device_factory",
                                                                         factory_flags));
        if (_factory_preload_system_only_.size()) {
          DT_LOG_TRACE(get_logging_priority(), "Preloading only a limited set of device model factories...");
          _device_factory_register_->import_some(DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(base_device_model),
                                                 _factory_preload_system_only_);
          DT_LOG_TRACE(get_logging_priority(), "Done.");
        } else if (_factory_preload_system_all_) {
          DT_LOG_TRACE(get_logging_priority(), "Preloading the full set of device model factories...");
          _device_factory_register_->import(DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(base_device_model));
          DT_LOG_TRACE(get_logging_priority(), "Done.");
        }
      }
      return *_device_factory_register_;
    }

    manager::port_factory_register_type & manager::_grab_port_factory_register()
    {
      if (! _port_factory_register_) {
        uint32_t factory_flags = 0;
        _port_factory_register_.reset(new port_factory_register_type("vire::device::base_port_model/port_factory",
                                                                     factory_flags));
        if (_factory_preload_system_only_.size() > 1) {
          DT_LOG_TRACE(get_logging_priority(), "Preloading only a limited set of port model factories...");
          _port_factory_register_->import_some(DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(base_port_model),
                                               _factory_preload_system_only_);
          DT_LOG_TRACE(get_logging_priority(), "Done.");
        } else if (_factory_preload_system_all_) {
          DT_LOG_TRACE(get_logging_priority(), "Preloading the full set of port model factories...");
          _port_factory_register_->import(DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(base_port_model));
          DT_LOG_TRACE(get_logging_priority(), "Done.");
        }
      }
      return *_port_factory_register_;
    }

    base_device_model & manager::grab_device_model(const std::string & name_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering with device model name = '" << name_ << "'");
      std::string model_key;
      encode_model_key(model_key, MODEL_DEVICE, name_);
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      model_pool_type::iterator found = _models_.find(model_key);
      DT_THROW_IF (found == _models_.end(),
                   std::logic_error,
                   "No device named '" << name_ << "' !");
      model_entry & entry = found->second;
      if (! entry.is_initialized()) {
        DT_LOG_TRACE(get_logging_priority(), "Entry '" << name_ << "' is not initialized yet! Do it!");
        this->_initialize_device_model(entry);
        DT_LOG_TRACE(get_logging_priority(), "Entry '" << name_ << "' is not initialized yet! Done.");
      }
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return entry.grab_device_model_handle().grab();
    }

    base_port_model & manager::grab_port_model(const std::string & name_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering with port model name = '" << name_ << "'");
      std::string model_key;
      encode_model_key(model_key, MODEL_PORT, name_);
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      model_pool_type::iterator found = _models_.find(model_key);
      DT_THROW_IF (found == _models_.end(),
                   std::logic_error,
                   "No port named '" << name_ << "' !");
      model_entry & entry = found->second;
      if (! entry.is_initialized()) {
        DT_LOG_TRACE(get_logging_priority(), "Entry '" << name_ << "' is not initialized yet! Do it!");
        this->_initialize_port_model(entry);
        DT_LOG_TRACE(get_logging_priority(), "Entry '" << name_ << "' is not initialized yet! Done.");
      }
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return entry.grab_port_model_handle().grab();
    }

    const base_device_model & manager::get_device_model(const std::string & name_) const
    {
      manager * sm = const_cast<manager*>(this);
      return const_cast<base_device_model&>(sm->grab_device_model(name_));
    }

    const base_port_model & manager::get_port_model(const std::string & name_) const
    {
      manager * sm = const_cast<manager*>(this);
      return const_cast<base_port_model &>(sm->grab_port_model(name_));
    }

    int manager::initialize(const datatools::properties & config_,
                             datatools::service_dict_type & /* service_dict_ */)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(this->is_initialized(), std::logic_error, "Device manager is already initialized !");

      // Parse logging and other parameters:
      // DT_LOG_NOTICE(datatools::logger::PRIO_ALWAYS,
      //               "Logging priority : '" << datatools::logger::get_priority_label(get_logging_priority()) << "'.");
      // config_.tree_dump(std::cerr, "Manager config: ", "[devel] ");
      // datatools::logger::priority p = get_logging_priority();
      // set_logging_priority(datatools::logger::PRIO_UNDEFINED);
      this->::datatools::base_service::common_initialize(config_);
      // if (p != datatools::logger::PRIO_UNDEFINED) {
      //   set_logging_priority(p);
      // }
      DT_LOG_DEBUG(get_logging_priority(),
                   "Logging priority : '" << datatools::logger::get_priority_label(get_logging_priority()) << "'.");

      // Documentation for this setup
      if (_setup_label_.empty()) {
        if (config_.has_key("setup_label")) {
          _setup_label_ = config_.fetch_string("setup_label");
        }
      }

      if (_setup_version_.empty()) {
        if (config_.has_key("setup_version")) {
          _setup_version_ = config_.fetch_string("setup_version");
        }
      }

      if (_setup_description_.empty()) {
        if (config_.has_key("setup_description")) {
          _setup_description_ = config_.fetch_string("setup_description");
        }
      }

      if (config_.has_key("factory_preload_system_only")) {
        std::vector<std::string> only_ids;
        config_.fetch("factory_preload_system_only", only_ids);
        for (int i = 0; i < (int) only_ids.size(); i++) {
          add_factory_preload_system_only(only_ids[i]);
        }
      }

      if (_factory_preload_system_only_.size() == 0) {
        if (config_.has_key("factory_preload_system_all")) {
          bool fpsa = config_.fetch_boolean("factory_preload_system_all");
          set_factory_preload_system_all(fpsa);
        }
      }

      if (config_.has_key("force_initialization_at_load")) {
        set_force_initialization_at_load(config_.fetch_boolean("force_initialization_at_load"));
      }

      if (config_.has_key("propagate_logging_to_devices")) {
        set_propagate_logging_to_devices(config_.fetch_boolean("propagate_logging_to_devices"));
      }

      if (config_.has_key("top_level_device_model_name")) {
        set_top_level_device_model_name(config_.fetch_string("top_level_device_model_name"));
      }

      if (_top_level_device_model_name_.empty()) {
        _top_level_device_model_name_ = default_top_level_device_model_name();
      }

      if (config_.has_key("mapping_requested")) {
        set_mapping_requested(config_.fetch_boolean("mapping_requested"));
      }

      DT_LOG_DEBUG(get_logging_priority(),
                   "setup_label = " << _setup_label_);
      DT_LOG_DEBUG(get_logging_priority(),
                   "setup_version = " << _setup_version_);
      DT_LOG_DEBUG(get_logging_priority(),
                   "setup_description = " << _setup_description_);
      DT_LOG_DEBUG(get_logging_priority(),
                   "factory_preload_system_only = " << _factory_preload_system_only_.size());
      DT_LOG_DEBUG(get_logging_priority(),
                   "factory_preload_system_all = " << _factory_preload_system_all_);
      DT_LOG_DEBUG(get_logging_priority(),
                   "force_initialization_at_load = " << _force_initialization_at_load_);
      DT_LOG_DEBUG(get_logging_priority(),
                   "propagate_logging_to_devices = " << _propagate_logging_to_devices_);
      DT_LOG_DEBUG(get_logging_priority(),
                   "mapping_requested = " << _mapping_requested_);

      if (is_mapping_requested()) {

        DT_LOG_DEBUG(get_logging_priority(), "Searching for external mapping rules...");
        if (config_.has_key("mapping_external_rules")) {
          // List of files where to find external mapping rules associated to device models:
          std::vector<std::string> mapping_rules_files;
          config_.fetch("mapping_external_rules", mapping_rules_files);
          _external_mapping_rules_.set_key_label("model");
          _external_mapping_rules_.set_meta_label("policy");
          for (size_t i = 0; i < mapping_rules_files.size(); i++) {
            std::string mrfile = mapping_rules_files[i];
            datatools::fetch_path_with_env(mrfile);
            _external_mapping_rules_.read(mrfile);
          }
        }
        if (_external_mapping_rules_.size()) {
          DT_LOG_DEBUG(get_logging_priority(), "Found some external mapping rules.");
        }

        // Mapping manager:
        {
          datatools::properties mapping_mgr_config;
          config_.export_and_rename_starting_with(mapping_mgr_config, "mapping_manager.", "");
          _mapping_manager_.set_world_category("setup");
          _mapping_manager_.set_plain_category("plain");
          DT_LOG_DEBUG(get_logging_priority(), "Initializing mapping ID manager...");
          _mapping_manager_.initialize(mapping_mgr_config);
          DT_LOG_DEBUG(get_logging_priority(), "Initialization of the mapping ID manager is done.");
          DT_LOG_DEBUG(get_logging_priority(), "Mapping ID manager:");
          if (datatools::logger::is_debug(get_logging_priority())) {
            _mapping_manager_.tree_dump(std::cerr, "", "[debug] ");
          }
          /*
            std::vector<std::string> mapping_mgr_defs;
            {
            if (config_.has_key("mapping_manager.categories")) {
            config_.fetch("mapping_manager.categories", mapping_mgr_defs);
            }
            }
            for (int i = 0; i < mapping_mgr_defs.size(); i++) {
            load_mapping_categories_from_file(mapping_mgr_defs[i]);
            }
          */
        }

      } // end of mapping requested

      // Property prefixes to be preserved in device models:
      std::vector<std::string> preserved_property_prefixes;
      if (config_.has_key("preserved_property_prefixes")) {
        config_.fetch("preserved_property_prefixes", preserved_property_prefixes);
        //std::cerr << "DEVEL: manager::initialize: # prefixes = " << preserved_property_prefixes.size() << std::endl;
      }

      // Default list of property prefixes to be preserved in device models:
      std::vector<std::string> default_preserved_property_prefixes;
      default_preserved_property_prefixes.push_back(geomtools::mapping_utils::mapping_prefix());
      default_preserved_property_prefixes.push_back("tree.");
      // default_preserved_property_prefixes.push_back("visibility.");
      // default_preserved_property_prefixes.push_back("foo.");
      // default_preserved_property_prefixes.push_back("bar.");

      std::vector<std::string> effective_preserved_property_prefixes = default_preserved_property_prefixes;

      for (int i = 0; i < preserved_property_prefixes.size (); i++) {
        const std::string & prefix = preserved_property_prefixes[i];
        if (prefix.empty()) {
          continue;
        }
        if (std::find(effective_preserved_property_prefixes.begin(),
                      effective_preserved_property_prefixes.end(),
                      prefix) == effective_preserved_property_prefixes.end()) {
          effective_preserved_property_prefixes.push_back(prefix);
        }
      }
      //std::cerr << "DEVEL: manager::initialize: # effective prefixes = "
      //          << effective_preserved_property_prefixes.size() << std::endl;

      for (int i = 0; i < effective_preserved_property_prefixes.size(); i++) {
        const std::string & prefix = effective_preserved_property_prefixes[i];
        DT_THROW_IF(prefix.empty(),
                    std::logic_error,
                    "Property prefix to be preserved in device models is empty !");
        DT_THROW_IF(prefix[prefix.length() - 1] != '.',
                    std::logic_error,
                    "Property prefix to be preserved in device models must end with a dot '.' ("
                    << prefix << " ) !");
        DT_LOG_NOTICE(get_logging_priority(),
                      "Properties with prefix '" << prefix << "' will be preserved in device models");
        add_auxiliary_property_prefix(prefix);
      }

      // Build model entries :
      DT_LOG_DEBUG(get_logging_priority(), "Building models...");
      {
       DT_LOG_DEBUG(get_logging_priority(), "Using model definition list files...");
       // By definition list files:
        std::string model_defs_listfiles_key("models.definition_listfiles");
        if (config_.has_key(model_defs_listfiles_key)) {
          std::vector<std::string> model_defs_listfiles;
          config_.fetch(model_defs_listfiles_key, model_defs_listfiles);
          for (std::vector<std::string>::const_iterator i = model_defs_listfiles.begin();
               i < model_defs_listfiles.end();
               ++i) {
            load_model_definitions_list(*i);
          }
        }
      }

      {
        // By definition files:
        DT_LOG_DEBUG(get_logging_priority(), "Using model definition files...");
        std::string model_defs_files_key("models.definition_files");
        if (config_.has_key(model_defs_files_key)) {
          std::vector<std::string> model_defs_files;
          config_.fetch(model_defs_files_key, model_defs_files);
          for (std::vector<std::string>::const_iterator i = model_defs_files.begin();
               i < model_defs_files.end();
               ++i) {
            load_model_definitions(*i);
          }
        }
      }

      if (is_mapping_requested()) {
        DT_LOG_DEBUG(get_logging_priority(), "Mapping is requested...");
        // Push external mapping rules in device models configuration section:
        if (! _external_mapping_rules_.empty()) {
          DT_LOG_DEBUG(get_logging_priority(), "Loading external mapping rules...");
          std::vector<std::string> model_names;
          _external_mapping_rules_.keys(model_names);
          for (int i = 0; i < (int) model_names.size(); i++) {
            const std::string & model_name = model_names[i];
            const datatools::multi_properties::entry & model_mapping_entry = _external_mapping_rules_.get(model_name);
            std::string model_mapping_policy = model_mapping_entry.get_meta();
            if (model_mapping_policy == "merge") {
              DT_THROW(std::logic_error, "Policy ('" << model_mapping_policy << "') for external mapping rules is not supported yet!");
            } else if (model_mapping_policy == "ignore") {
              DT_THROW(std::logic_error, "Policy ('" << model_mapping_policy << "') for external mapping rules is not supported yet!");
            } else if (model_mapping_policy == "supersede") {
            } else if (model_mapping_policy.empty()) {
              // Default value:
              model_mapping_policy = "supersede";
            } else {
              DT_THROW(std::logic_error, "Unknown policy ('" << model_mapping_policy << "') for external mapping rules!");
            }
            const datatools::properties & model_mapping_rules = _external_mapping_rules_.get_section(model_name);
            /*
            model_mapping_rules.tree_dump(std::cerr,
                                          std::string("External mapping rules for device model '") + model_name + "': ",
                                          "DEVEL: ");
            */
            std::string device_model_key;
            encode_model_key(device_model_key, MODEL_DEVICE, model_name);
            model_pool_type::iterator found = _models_.find(device_model_key);
            // std::cerr << "DEVEL: device_model_key='" << device_model_key << "'" << std::endl;
            if (found != _models_.end()) {
              model_entry & dme = found->second;
              DT_THROW_IF(!dme.is_device_model(), std::logic_error,
                          "Model '" <<  model_name << "' is not a device model!");
              datatools::properties * target_mapping_config = nullptr;
              if (dme.is_initialized()) {
                // If device model is already initialized, push the mapping rules inside the associated logical:
                DT_THROW_IF(!dme.grab_device_model_handle_plain().get().has_logical(), std::logic_error,
                            "Missing logical for initialized device model '" << model_name << "'!");
                const datatools::properties & log_aux = dme.grab_device_model_handle_plain().get().get_logical().get_auxiliaries();
                target_mapping_config = const_cast<datatools::properties *>(&log_aux);
              } else if (dme.is_created()) {
                // If device model has not been initialized yet, push the mapping rules in the
                // configuration properties associated to the device model:
                const datatools::properties & dme_config = dme.get_config();
                target_mapping_config = const_cast<datatools::properties *>(&dme_config);
              }
              if (model_mapping_policy == "supersede") {
                // First delete former 'mapping' properties:
                target_mapping_config->erase_all_starting_with(geomtools::mapping_utils::mapping_prefix());
              }
              // Then install the ones for external mapping rules:
              model_mapping_rules.export_starting_with(*target_mapping_config, geomtools::mapping_utils::mapping_prefix());
              /*
              // What to do with "ignore" and "merge" policy:
              if (model_mapping_policy == "ignore") {
                model_mapping_rules.export_starting_with(factory_model_section, geomtools::mapping_utils::mapping_prefix());
              }
              if (model_mapping_policy == "merge") {
              }
              */
              DT_LOG_DEBUG(get_logging_priority(), "Device model '" << model_name << "' is now enriched with external mapping rules...");
              if (datatools::logger::is_debug(get_logging_priority())) {
                target_mapping_config->tree_dump(std::cerr, "Mapping config for model '" + model_name + "'", "[debug] ");
              }
            } else {
              DT_LOG_WARNING(get_logging_priority(), "Ignoring external mapping rules for non-existing model '" << model_name << "'!");
            }
          }
        }
      }

      // Set the initialization flag:
      _initialized_ = true;

      // Post-initialization:
      _post_init_(config_);

      // // Mapping :
      // if (is_mapping_requested()) {
      //   _mapping_.set_logging_priority(get_logging_priority());
      //   _mapping_.set_manager(*this);
      //   _mapping_.set_mapping_manager(_mapping_manager_);
      //   _mapping_.set_top_level_device_model_name(get_top_level_device_model_name());
      //   _mapping_.set_top_level_device_instance_name(get_setup_label() + vire::utility::path::setup_separator());
      //   datatools::properties mapping_config;
      //   config_.export_and_rename_starting_with(mapping_config,
      //                                           geomtools::mapping_utils::mapping_prefix(),
      //                                           "");
      //   if (datatools::logger::is_debug(get_logging_priority())) {
      //     DT_LOG_DEBUG(get_logging_priority(), "Mapping configuration:");
      //     mapping_config.tree_dump(std::cerr, "", "[debug] ");
      //   }
      //   // _mapping_.configure(mapping_config);
      //   if (datatools::logger::is_debug(get_logging_priority())) {
      //     DT_LOG_DEBUG(get_logging_priority(), "Mapping before initialization:");
      //     _mapping_.tree_dump(std::cerr, "", "[debug] ");
      //   }
      //   DT_LOG_DEBUG(get_logging_priority(), "Initializing the mapping...");
      //   _mapping_.initialize(mapping_config);
      //   DT_LOG_DEBUG(get_logging_priority(), "Mapping is built");
      // }

      // Plugins:
      // ... nothing yet...

      if (datatools::logger::is_debug(get_logging_priority())) {
        DT_LOG_DEBUG(get_logging_priority(), "List of known models : " << _models_.size() << " : ");
        for (const auto & p : _models_) {
          DT_LOG_DEBUG(get_logging_priority(), "Model '" << p.first << "' : ");
          p.second.tree_dump(std::cerr, "", "[debug] ");
        }
        DT_LOG_DEBUG(get_logging_priority(), "List of known device logicals : " << _logical_devices_.size() << " : ");
        for (const auto & p : _logical_devices_) {
          DT_LOG_DEBUG(get_logging_priority(), "Logical '" << p.first << "' : ");
          p.second->tree_dump(std::cerr, "", "[debug] ");
        }
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    void manager::load_mapping_categories_from_file(const std::string & file_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Manager is already initialized !");
      std::string ftmp = file_;
      datatools::fetch_path_with_env(ftmp);
      _mapping_manager_.load(ftmp);
      return;
    }

    void manager::_set_defaults()
    {
      _factory_preload_system_all_ = true;
      _force_initialization_at_load_ = false;
      _mapping_requested_ = false; // true;
      return;
    }

    int manager::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!_initialized_, std::logic_error, "Manager is not initialized !");
      _pre_reset_();
      _initialized_ = false;
      size_t count = _models_.size();
      size_t initial_size = _models_.size();
      while (_models_.size() > 0) {
        for (model_pool_type::iterator it = _models_.begin();
             it != _models_.end();
             ++it) {
          model_entry & entry = it->second;
          DT_LOG_DEBUG(get_logging_priority(), "Removing model '" << entry.get_name ()  << "'...");
          this->_reset_model(entry);
          _models_.erase(it);
          --count;
          break;
        }
        if (count == initial_size) {
          break;
        }
      }
      if (_models_.size() > 0) {
        DT_LOG_WARNING(get_logging_priority(), "There are some remaining models !");
      }
      _mapping_manager_.reset();
      _logical_ports_.clear();
      _logical_devices_.clear();
      _models_.clear();
      if (_device_factory_register_) {
        _device_factory_register_->reset();
        _device_factory_register_.reset();
      }
      if (_port_factory_register_) {
        _port_factory_register_->reset();
        _port_factory_register_.reset();
      }

      _factory_preload_system_only_.clear();
      _setup_label_.clear();
      _setup_version_.clear();
      _setup_description_.clear();
      _auxiliary_property_prefixes_.clear();
      _set_defaults();

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    manager::manager(uint32_t flags_)
    {
      _initialized_ = false;
      set_logging_priority(datatools::logger::PRIO_FATAL);

      _set_defaults();

      // Process constructor flags:
      if (flags_ & FORCE_INITIALIZATION_AT_LOAD) {
        set_force_initialization_at_load(true);
      }

      if (flags_ & NO_BUILD_MAPPING) {
        set_mapping_requested(false);
      }

      if (flags_ & NO_FACTORY_PRELOAD) {
        set_factory_preload_system_all(false);
      }

      return;
    }

    manager::~manager()
    {
      if (_initialized_) this->reset();
      return;
    }

    const geomtools::id_mgr & manager::get_mapping_manager() const
    {
      return _mapping_manager_;
    }

    // **************
    // *   MODELS   *
    // **************

    bool manager::has_device_model(const std::string & name_) const
    {
      DT_LOG_TRACE(get_logging_priority(), "name_='" << name_ << "'");
      std::string model_key;
      encode_model_key(model_key, MODEL_DEVICE, name_);
      DT_LOG_TRACE(get_logging_priority(), "model_key='" << model_key << "'");
      return _models_.find(model_key) != _models_.end();
    }

    bool manager::has_port_model(const std::string & name_) const
    {
      std::string model_key;
      encode_model_key(model_key, MODEL_PORT, name_);
      DT_LOG_TRACE(get_logging_priority(), "model_key='" << model_key << "'");
      return _models_.find(model_key) != _models_.end();
    }

    bool manager::is_device_model_initialized(const std::string & name_) const
    {
      std::string model_key;
      encode_model_key(model_key, MODEL_DEVICE, name_);
      model_pool_type::const_iterator found = _models_.find(model_key);
      return found != _models_.end() && found->second.is_initialized();
    }

    bool manager::is_port_model_initialized(const std::string & name_) const
    {
      std::string model_key;
      encode_model_key(model_key, MODEL_PORT, name_);
      model_pool_type::const_iterator found = _models_.find(model_key);
      return found != _models_.end() && found->second.is_initialized();
    }

    const manager::logical_device_dict_type &
    manager::get_logical_devices() const
    {
      return _logical_devices_;
    }

    const model_pool_type & manager::get_models() const
    {
      return _models_;
    }

    const mapping & manager::get_mapping() const
    {
      return _mapping_;
    }

    const instance_tree & manager::get_tree() const
    {
      return _tree_;
    }

    void manager::_post_init_(const datatools::properties & config_)
    {
      _init_tree_(config_);
      _init_mapping_(config_);
      return;
    }

    void manager::_pre_reset_()
    {
      _reset_mapping_();
      _reset_tree_();
      return;
    }

    void manager::_init_tree_(const datatools::properties & config_)
    {
      instance_tree_builder builder;
      datatools::properties builder_config;
      config_.export_and_rename_starting_with(builder_config, "tree.", "");
      builder.configure(builder_config);
      builder.build(*this, _tree_);
      return;
    }

    void manager::_reset_tree_()
    {
      _tree_.reset();
      return;
    }

    void manager::_init_mapping_(const datatools::properties & config_)
    {
      // Mapping :
      if (is_mapping_requested()) {
        _mapping_.set_logging_priority(get_logging_priority());
        _mapping_.set_manager(*this);
        _mapping_.set_mapping_manager(_mapping_manager_);
        _mapping_.set_top_level_device_model_name(get_top_level_device_model_name());
        _mapping_.set_top_level_device_instance_name(get_setup_label() + vire::utility::path::setup_separator());
        datatools::properties mapping_config;
        config_.export_and_rename_starting_with(mapping_config,
                                                geomtools::mapping_utils::mapping_prefix(),
                                                "");
        if (datatools::logger::is_debug(get_logging_priority())) {
          DT_LOG_DEBUG(get_logging_priority(), "Mapping configuration:");
          mapping_config.tree_dump(std::cerr, "", "[debug] ");
        }
        if (datatools::logger::is_debug(get_logging_priority())) {
          DT_LOG_DEBUG(get_logging_priority(), "Mapping before initialization:");
          _mapping_.tree_dump(std::cerr, "", "[debug] ");
        }
        DT_LOG_DEBUG(get_logging_priority(), "Initializing the mapping...");
        _mapping_.initialize(mapping_config);
        DT_LOG_DEBUG(get_logging_priority(), "Mapping is built");
      }
      return;
    }

    void manager::_reset_mapping_()
    {
      if (_mapping_.is_initialized()) {
        _mapping_.reset();
      }
      return;
    }

    void manager::dump_device_models(std::ostream & out_,
                                     const std::string & title_,
                                     const std::string & indent_) const
    {
      if (!title_.empty()) out_ << indent_ << title_ << ":" << std::endl;

      // Devices:
      {
        size_t sz = _models_.size();
        size_t count = 0;
        for (model_pool_type::const_iterator it = _models_.begin();
             it != _models_.end();
             ++it) {
          count++;
          out_ << indent_;
          if (count == sz) {
            out_ << "`-- ";
          } else {
            out_ << "|-- ";
          }
          const std::string & model_name = it->first;
          const model_entry & model_record = it->second;

          out_ << "Name : '" << model_name << "' "
               << "Type : '" << model_record.get_model_id () << "' ";
          out_ << '(';
          int count = 0;
          if (model_record.is_initialized()) {
            out_ << "initialized";
            count++;
          }
          out_ << std::endl;
        }
      }
      return;
    }

    bool manager::has_device_type(const std::string & id_) const
    {
      return const_cast<manager *>(this)->_grab_device_factory_register().has(id_);
    }

    void manager::unregister_device_type(const std::string & id_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Device manager is not initialized!");
      _grab_device_factory_register().unregistration(id_);
      return;
    }

    // void manager::tree_dump(std::ostream & out_,
    //                       const std::string & title_,
    //                       const std::string & indent_,
    //                       bool inherit_) const
    // {
    //   datatools::i_tree_dumpable::print_options popts;
    //   popts.configure(options_)
    //   return;
    // }

    void manager::print_tree(std::ostream & out_,
                             const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);
      bool tree_list     = options_.get<bool>("tree.list_instances", false);
      // bool tree_full     = options_.get<bool>("tree.full_instances", false);
      bool mapping_list  = options_.get<bool>("mapping.list_categories", false);
      bool mapping_full  = options_.get<bool>("mapping.full_categories", false);
      bool mapping_list_ids = options_.get<bool>("mapping.list_ids", false);
      bool models_list   = options_.get<bool>("models.list", false);
      bool models_full   = options_.get<bool>("models.full", false);
      bool logicals_list = options_.get<bool>("logicals.list", false);
      bool ports_list    = options_.get<bool>("ports.list", false);

      this->datatools::base_service::tree_dump(out_, popts.title, popts.indent, true);

      out_ << popts.indent << i_tree_dumpable::tag
           << "Setup label       : '"
           << _setup_label_
           << "'" << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Setup version     : '"
           << _setup_version_
           << "'" << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Setup description : '"
           << _setup_description_
           << "'" << std::endl;

      // out_ << popts.indent << i_tree_dumpable::tag
      //      << "Logging priority : '"
      //      << datatools::logger::get_priority_label(get_logging_priority())
      //      << "'" << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Factory preload system all : "
           << _factory_preload_system_all_
           << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Force initialization : "
           << _force_initialization_at_load_
           << std::endl;

      out_ << popts.indent << i_tree_dumpable::tag
           << "Auxiliary property prefixes : " << '[' << _auxiliary_property_prefixes_.size() << ']'
           << std::endl;
      for (int i = 0; i < _auxiliary_property_prefixes_.size(); i++) {
        const std::string & prefix = _auxiliary_property_prefixes_[i];
        out_ << popts.indent << i_tree_dumpable::skip_tag;
        if (i < _auxiliary_property_prefixes_.size() - 1) {
          out_ << i_tree_dumpable::tag;
        } else {
          out_ << i_tree_dumpable::last_tag;
        }
        out_ << "'" << prefix << "'";
        out_ << std::endl;
      }

      if (_device_factory_register_) {
        out_ << popts.indent << i_tree_dumpable::tag
             << "List of registered device model factories : " << std::endl;
        {
          std::ostringstream indent_oss;
          indent_oss << popts.indent << i_tree_dumpable::skip_tag;
          _device_factory_register_->tree_dump(out_, "", indent_oss.str());
        }
      }

      if (_port_factory_register_) {
        out_ << popts.indent << i_tree_dumpable::tag
             << "List of registered port model factories : " << std::endl;
        {
          std::ostringstream indent_oss;
          indent_oss << popts.indent << i_tree_dumpable::skip_tag;
          _port_factory_register_->tree_dump(out_, "", indent_oss.str());
        }
      }

      {
        out_ << popts.indent << i_tree_dumpable::tag
             << "Models : ";
        size_t sz = _models_.size();
        if (sz == 0) {
          out_ << "<none>";
        } else {
          out_ << '[' << _models_.size() << ']';
        }
        out_ << std::endl;
        if (models_list) {
          for (model_pool_type::const_iterator i = _models_.begin();
               i != _models_.end();
               ++i) {
            const std::string & model_name = i->first;
            const model_entry & comp_entry = i->second;
            out_ << popts.indent << i_tree_dumpable::skip_tag;

            std::ostringstream indent_oss;
            indent_oss << popts.indent << i_tree_dumpable::skip_tag;
            model_pool_type::const_iterator j = i;
            j++;
            if (j == _models_.end()) {
              out_ << i_tree_dumpable::last_tag;
              indent_oss << i_tree_dumpable::last_skip_tag;
            } else {
              out_ << i_tree_dumpable::tag;
              indent_oss << i_tree_dumpable::skip_tag;
            }
            out_ << "Model : '" << model_name << "'" << std::endl;
            if (models_full) {
              comp_entry.tree_dump(out_, "", indent_oss.str());
            }
          }
        }
      }

      if (logicals_list) {
        out_ << popts.indent << i_tree_dumpable::tag
             << "Logical devices : ";
        size_t sz = _logical_devices_.size();
        if (sz == 0) {
          out_ << "<none>";
        } else {
          out_ << "[" << _logical_devices_.size() << "]";
        }
        out_ << std::endl;
      }

      if (ports_list) {
        out_ << popts.indent << i_tree_dumpable::tag
             << "Logical ports : ";
        size_t sz = _logical_ports_.size();
        if (sz == 0) {
          out_ << "<none>";
        } else {
          out_ << "[" << _logical_ports_.size() << "]";
        }
        out_ << std::endl;
      }

      {
        out_ << popts.indent << i_tree_dumpable::tag
             << "Instance tree : "
          // << _tree_.size() << " instance entries"
             << std::endl;
        boost::property_tree::ptree tree_options;
        if (tree_list) {
          tree_options.put("list_instances", true);
          tree_options.put(datatools::i_tree_dumpable::base_print_options::indent_key(),
                           popts.indent + tags::skip_item());

          _tree_.print_tree(out_, tree_options);
          // std::size_t count = 0;
          // for (const auto & p : _tree_.get_instances()) {
          //   count++;
          //   out_ << popts.indent <<  i_tree_dumpable::tag;
          //   if (count == _tree_.get_instances().size()) {
          //     out_ << i_tree_dumpable::last_tag;
          //   } else {
          //     out_ << i_tree_dumpable::tag;
          //   }
          //   out_ << "Instance path : '" << p.first << "'";
          //   out_ << std::endl;
          // }
        }
      }

      out_ << popts.indent << i_tree_dumpable::tag
           << "Mapping requested : "
           << _mapping_requested_
           << std::endl;

      if (_mapping_requested_) {
        out_ << popts.indent << i_tree_dumpable::tag
             << "Mapping ID manager    : "
             << std::endl;
        std::ostringstream indent_oss;
        indent_oss << popts.indent << i_tree_dumpable::skip_tag;

        boost::property_tree::ptree mapmgr_options;
        mapmgr_options.put(datatools::i_tree_dumpable::base_print_options::indent_key(),
                           popts.indent + datatools::i_tree_dumpable::tags::item(false, true));
        if (mapping_list) {
          mapmgr_options.put("list_categories", true);
          if (mapping_full) {
            mapmgr_options.put("full_categories", true);
          }
        }
        _mapping_manager_.print_tree(out_, mapmgr_options);
      }

      if (_mapping_requested_) {
        out_ << popts.indent << i_tree_dumpable::tag
             << "Mapping : " << _mapping_.get_number_of_entries() << " mapping entries"
             << std::endl;
        boost::property_tree::ptree map_options;
        std::ostringstream indent_oss;
        indent_oss << popts.indent << i_tree_dumpable::skip_tag;
        map_options.put(datatools::i_tree_dumpable::base_print_options::indent_key(),
                        popts.indent + datatools::i_tree_dumpable::tags::item(false, true));
        if (mapping_list_ids) {
          map_options.put("list_ids", true);
        }
        _mapping_.print_tree(std::clog, map_options);
      }

      {
        out_ << popts.indent << i_tree_dumpable::inherit_tag(popts.inherit)
             << "Initialized    : "
             << this->is_initialized()
             << std::endl;
      }

      return;
    }

    void manager::_load_device_model(const std::string & name_,
                                     const std::string & id_,
                                     const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(this->has_device_model(name_),
                  std::logic_error,
                  "Device model '" << name_ << "' already exists !");
      std::string model_key;
      encode_model_key(model_key, MODEL_DEVICE, name_);
      DT_LOG_DEBUG(get_logging_priority(), "Loading device model '" << model_key << "' of type '" << id_ << "'...");
      {
        // Add a new entry :
        model_entry tmp_entry;
        tmp_entry.set_name(name_);
        DT_LOG_DEBUG(get_logging_priority(),
                     "Add entry for device model '"<< name_ << "'...");
        _models_[model_key] = tmp_entry;
      }
      // Fetch a reference on it and update :
      model_entry & new_entry = _models_.find(model_key)->second;
      new_entry.set_device_manager(*this);
      new_entry.set_model_id(id_);
      new_entry.set_model_type(MODEL_DEVICE);
      new_entry.set_config(config_);
      DT_LOG_DEBUG(get_logging_priority(),
                   "Creating the device model '" << name_ << "' of type '" << id_ << "'...");
      this->_create_device_model(new_entry);
      DT_LOG_DEBUG(get_logging_priority(), "Creating the device model '" << name_ << "'... done.");
      if (_force_initialization_at_load_) {
        DT_LOG_DEBUG(get_logging_priority(), "Initializing the device model '" << name_ << "'...");
        this->_initialize_device_model(new_entry);
        DT_LOG_DEBUG(get_logging_priority(), "Initialization of the device model '" << name_ << "' is done.");
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::_load_port_model(const std::string & name_,
                                   const std::string & id_,
                                   const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(this->has_port_model(name_),
                  std::logic_error,
                  "Port model '" << name_ << "' already exists !");
      std::string model_key;
      encode_model_key(model_key, MODEL_PORT, name_);
      DT_LOG_DEBUG(get_logging_priority(), "Loading port model '" << model_key << "' of type '" << id_ << "'...");
     {
        // Add a new entry :
        model_entry tmp_entry;
        tmp_entry.set_name(name_);
        DT_LOG_DEBUG(get_logging_priority(),
                     "Add entry for port model '"<< name_ << "'...");
        _models_[model_key] = tmp_entry;
      }
      // Fetch a reference on it and update :
      model_entry & new_entry = _models_.find(model_key)->second;
      new_entry.set_device_manager(*this);
      new_entry.set_model_id(id_);
      new_entry.set_model_type(MODEL_PORT);
      new_entry.set_config(config_);
      DT_LOG_DEBUG(get_logging_priority(),
                   "Creating the port model '" << name_ << "' of type '" << id_ << "'...");
      this->_create_port_model(new_entry);
      DT_LOG_DEBUG(get_logging_priority(), "Creating the port model '" << name_ << "'... done.");
      if (_force_initialization_at_load_) {
        DT_LOG_DEBUG(get_logging_priority(), "Initializing the port model '" << name_ << "'...");
        this->_initialize_port_model(new_entry);
        DT_LOG_DEBUG(get_logging_priority(), "Initialization of the port model '" << name_ << "' is done.");
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::_create_device_model(model_entry & entry_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (get_logging_priority() == datatools::logger::PRIO_TRACE) {
        entry_.tree_dump(std::cerr, "Entry: ", "[trace] ");
      }
      DT_THROW_IF(!entry_.is_device_model(), std::logic_error,
                  "Model named '" << entry_.get_name() << "' "
                  << "of type id '" << entry_.get_model_id() << "' is not a device model!");
      DT_THROW_IF(entry_.is_created(), std::logic_error,
                  "Device model named '" << entry_.get_name() << "' "
                  << "of type id '" << entry_.get_model_id() << "' is already created!");
      DT_LOG_DEBUG(get_logging_priority(),
                   "Creating device model named '" << entry_.get_name()
                   << "'...");
      // Search for the device model's label in the factory dictionary:
      DT_THROW_IF (!_grab_device_factory_register().has(entry_.get_model_id()),
                   std::logic_error,
                   "Cannot find device model factory with ID '"
                   << entry_.get_model_id()
                   << "' for device model named '"
                   << entry_.get_name() << "' !");
      DT_LOG_DEBUG(get_logging_priority(), "Invoke factory for type id '" << entry_.get_model_id() << "'...");
      typedef device_factory_register_type::factory_type FactoryType;
      const FactoryType & the_factory = _grab_device_factory_register().get(entry_.get_model_id());
      base_device_model * ptr = the_factory();
      ptr->set_name(entry_.get_name());
      entry_.grab_device_model_handle_plain().reset(ptr);
      DT_LOG_DEBUG(get_logging_priority(),
                   "Device model named '" <<  entry_.get_name() << "' has been created !");
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::_create_port_model(model_entry & entry_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (get_logging_priority() == datatools::logger::PRIO_TRACE) {
        entry_.tree_dump(std::cerr, "Entry: ", "[trace] ");
      }
      DT_THROW_IF(!entry_.is_port_model(), std::logic_error,
                  "Model named '" << entry_.get_name() << "' "
                  << "of type id '" << entry_.get_model_id() << "' is not a port model!");
      DT_THROW_IF(entry_.is_created(), std::logic_error,
                  "Port model named '" << entry_.get_name() << "' "
                  << "of type id '" << entry_.get_model_id() << "' is already created!");
      DT_LOG_DEBUG(get_logging_priority(),
                   "Creating port model named '" << entry_.get_name()
                   << "'...");
      // Search for the port model's label in the factory dictionary:
      DT_THROW_IF (!_grab_port_factory_register().has(entry_.get_model_id()),
                   std::logic_error,
                   "Cannot find port model factory with ID '"
                   << entry_.get_model_id()
                   << "' for port model named '"
                   << entry_.get_name() << "' !");
      DT_LOG_TRACE(get_logging_priority(), "Invoke factory for type id '" << entry_.get_model_id() << "'...");
      typedef port_factory_register_type::factory_type FactoryType;
      const FactoryType & the_factory = _grab_port_factory_register().get(entry_.get_model_id());
      base_port_model * ptr = the_factory();
      ptr->set_name(entry_.get_name());
      entry_.grab_port_model_handle_plain().reset(ptr);
      DT_LOG_DEBUG(get_logging_priority(),
                   "Port model named '" <<  entry_.get_name() << "' has been created !");
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::_initialize_device_model(model_entry & entry_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      // If not created, first do it :
      if (! entry_.is_created()) {
        this->_create_device_model(entry_);
      }

      // If not initialized, do it :
      if (! entry_.is_initialized()) {
        DT_LOG_DEBUG(get_logging_priority(),
                     "Initializing device model named '"
                     << entry_.get_name()
                     << "'...");
        base_device_model & the_device = entry_.grab_device_model_handle_plain().grab();
        model_repository model_rep;
        model_rep.set_models(_models_);
        DT_LOG_DEBUG(get_logging_priority(), "Running initialization...");

        if (is_propagate_logging_to_devices()) {
          if (datatools::logger::is_debug(get_logging_priority())) {
            the_device.set_logging_priority(get_logging_priority());
          }
        }
        the_device.initialize(entry_.get_config(), model_rep);
        DT_LOG_DEBUG(get_logging_priority(), "Initialization done.");
        for (int i = 0; i < _auxiliary_property_prefixes_.size(); i++) {
          const std::string & prefix = _auxiliary_property_prefixes_[i];
          DT_LOG_DEBUG(get_logging_priority(), "Export auxiliary properties starting with '"
                       << prefix << "' to the device model named '" << entry_.get_name() << "'...");
          logical_device & log_dev = const_cast<logical_device &>(the_device.get_logical());
          entry_.get_config().export_starting_with(log_dev.grab_auxiliaries(), prefix);
          _logical_devices_[log_dev.get_name()] = &log_dev;
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::_initialize_port_model(model_entry & entry_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      // If not created, first do it :
      if (! entry_.is_created()) {
        this->_create_port_model(entry_);
      }

      // If not initialized, do it :
      if (! entry_.is_initialized()) {
        DT_LOG_DEBUG(get_logging_priority(),
                     "Initializing port model named '"
                     << entry_.get_name()
                     << "'...");
        base_port_model & the_port = entry_.grab_port_model_handle_plain().grab();
        model_repository model_rep;
        model_rep.set_models(_models_);
        the_port.initialize(entry_.get_config());
        DT_LOG_DEBUG(get_logging_priority(), "Initialization done.");
        for (int i = 0; i < _auxiliary_property_prefixes_.size(); i++) {
          const std::string & prefix = _auxiliary_property_prefixes_[i];
          DT_LOG_DEBUG(get_logging_priority(), "Export auxiliary properties starting with '"
                       << prefix << "' to the port model named '" << entry_.get_name() << "'...");
          logical_port & log_prt = const_cast<logical_port &>(the_port.get_logical());
          entry_.get_config().export_starting_with(log_prt.grab_auxiliaries(), prefix);
          _logical_ports_[log_prt.get_name()] = &log_prt;
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::_reset_model(model_entry & entry_)
    {
      if (entry_.is_device_model()) {
        _reset_device_model(entry_);
      } else if (entry_.is_port_model()) {
        _reset_port_model(entry_);
      }
      return;
    }

    void manager::_reset_device_model(model_entry & entry_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (entry_.is_initialized()) {
        base_device_model & the_device = entry_.grab_device_model_handle_plain().grab();
        if (the_device.has_logical()) {
          const std::string & log_dev_name = the_device.get_logical().get_name();
          _logical_devices_.erase(log_dev_name);
        }
        the_device.reset();
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::_reset_port_model(model_entry & entry_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (entry_.is_initialized()) {
        base_port_model & the_port = entry_.grab_port_model_handle_plain().grab();
        if (the_port.has_logical()) {
          const std::string & log_port_name = the_port.get_logical().get_name();
          _logical_ports_.erase(log_port_name);
        }
        the_port.reset();
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

  } // end of namespace device

} // end of namespace vire

// OCD support:
#include <datatools/ocd_macros.h>

// OCD support for class '::vire::device::manager' :
DOCD_CLASS_IMPLEMENT_LOAD_BEGIN(::vire::device::manager, ocd_)
{
  ocd_.set_class_name ("::ire::device::manager");
  ocd_.set_class_description ("A generic manager for device models");
  ocd_.set_class_library ("Vire");
  ocd_.set_class_documentation ("A *device manager* is responsible of the instantiation,           \n"
                                "initialization, management and destruction of device models.      \n"
                                "A device model is a special object that fulfils the generic       \n"
                                "``vire::device::base_device_model`` interface. A device model is  \n"
                                "generally dedicated to some special task : description of a crate,\n"
                                "a board, a chip... Such device models are hosted by the           \n"
                                "*device manager* which provides an unique counter for             \n"
                                "end-users and client applications to configure, initialize and    \n"
                                "access to the functionalities offered by the device models.       \n"
                                "A device manager can be configured *manually* from plain C++ code.\n"
                                "Is is also possible to initialize it through a set of ASCII       \n"
                                "configuration files, thus dynamically instantiating a list of     \n"
                                "device models with embedded functionalities.                      \n"
                                )
    ;

  datatools::base_service::common_ocd(ocd_);

  {
    configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("setup_label")
      .set_terse_description("The label of the virtual setup")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(true)
      .set_long_description(
                            "Superseded by the previous method call :          \n"
                            "  ``vire::device::manager::set_setup_label(...)`` \n"
                            "                                                  \n")
      .add_example("::                                 \n"
                   "                                   \n"
                   "  setup_label : string = \"ECM\"   \n"
                   "                                   \n"
                   )
      ;
  }

  {
    configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("setup_version")
      .set_terse_description("The version of the virtual device setup")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(true)
      .set_long_description(
                            "Superseded by the previous method call :            \n"
                            "  ``vire::device::manager::set_setup_version(...)`` \n"
                            "                                                    \n")
      .add_example("::                                 \n"
                   "                                   \n"
                   "  setup_version : string = \"1.0\" \n"
                   "                                   \n"
                   )
      ;
  }

  {
    configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("setup_description")
      .set_terse_description("The description of the virtual device setup")
      .set_traits(datatools::TYPE_STRING)
      .set_mandatory(false)
      .set_long_description("Superseded by the previous method call :                \n"
                            "  `vire::device::manager::set_setup_description(...)``  \n"
                            "                                                        \n")
      .add_example("::                                                               \n"
                   "                                                                 \n"
                   "  setup_description : string = \"The virtual setup\"             \n"
                   "                                                                 \n"
                   )
      ;
  }

  {
    configuration_property_description & cpd = ocd_.add_configuration_property_info();
    cpd.set_name_pattern("devices.configuration_files")
      .set_terse_description("A list of configuration filenames for embedded devices")
      .set_traits(datatools::TYPE_STRING,
                  configuration_property_description::ARRAY)
      .set_mandatory(false)
      .set_path(true)
      .set_long_description(
                            "A list of filenames from where the manager loads the     \n"
                            "directives to dynamically instantiate new embedded       \n"
                            "device model objects. The filenames may contains         \n"
                            "some environment variables.                              \n"
                            "The target files must use the format of the              \n"
                            "``datatools::multi_properties`` class.                   \n"
                            "The loading order of the files is critical               \n"
                            "because some device models may depend on other ones      \n"
                            "which should thus be defined **before** their            \n"
                            "dependers.                                               \n"
                            "                                                         \n"
                            "Extends the instantiation of device models triggered by  \n"
                            "previous method calls :                                  \n"
                            "  ``vire::device::manager::load(...)``                   \n"
                            "                                                         \n")
      .add_example("A setup with two files::                                   \n"
                   "                                                           \n"
                   "   devices.configuration_files : string[2] as path = \\    \n"
                   "     \"${CONFIG_REPOSITORY_DIR}/devices.conf\" \\          \n"
                   "     \"${CONFIG_REPOSITORY_DIR}/other_devices.conf\"       \n"
                   "                                                           \n"
                   )
      ;
  }


  ocd_.set_configuration_hints ("Configuration example for a ``vire::device::manager`` object::  \n"
                                "                                                                \n"
                                "   setup_label : string = \"MyExperiment\"                      \n"
                                "   setup_version : string = \"1.0\"                             \n"
                                "   setup_description  : string = \\                             \n"
                                "     \"Core device manager for MyExperiment\"                   \n"
                                "   devices.configuration_files : string[2] as path = \\      \n"
                                "     \"${CONFIG_DIR}/frontend_devices.conf\" \\                 \n"
                                "     \"${CONFIG_DIR}/trigger_devices.conf\"                     \n"
                                "                                                                \n"
                                "The ``${CONFIG_DIR}/frontend_device.conf``,                     \n"
                                "and ``${CONFIG_DIR}/trigger_devices.conf`` files use            \n"
                                "the ``datatools::multi_properties`` ASCII format.               \n"
                                "                                                                \n"
                                "Example::                                                       \n"
                                "                                                                \n"
                                "  #@description A list of devices                               \n"
                                "  #@key_label   \"name\"                                        \n"
                                "  #@meta_label  \"type\"                                        \n"
                                "                                                                \n"
                                "  [name=\"device_0@device\" type=\"device_model_0\"]            \n"
                                "                                                                \n"
                                "  #@config A device object                                      \n"
                                "                                                                \n"
                                "  #@description A configuration parameter                       \n"
                                "  logging.priority : string = \"error\"                         \n"
                                "                                                                \n"
                                "  #@description Another configuration parameter                 \n"
                                "  display_name : string = \"Device0\"                           \n"
                                "                                                                \n"
                                "  ...                                                           \n"
                                "                                                                \n"
                                "  [name=\"device_1@device\" type=\"device_model_1\"]            \n"
                                "                                                                \n"
                                "  #@config Another device object                                \n"
                                "                                                                \n"
                                "  #@description A configuration parameter                       \n"
                                "  logging.priority : string = \"error\"                         \n"
                                "                                                                \n"
                                "  #@description Another configuration parameter                 \n"
                                "  display_name : string = \"Device1\"                           \n"
                                "                                                                \n"
                                "  ...                                                           \n"
                                "                                                                \n"
                                );

  ocd_.set_validation_support(true);
  ocd_.lock();
  return;
}
DOCD_CLASS_IMPLEMENT_LOAD_END()

DOCD_CLASS_SYSTEM_REGISTRATION(vire::device::manager, "vire::device::manager")
