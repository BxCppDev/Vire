//! \file vire/device/base_device_model.cc
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
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/device/base_device_model.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/properties.h>
#include <datatools/exception.h>
#include <datatools/detail/reflection_macros.h>

// This project:
#include <vire/device/device_utils.h>
#include <vire/device/logical_device.h>
#include <vire/device/physical_device.h>
#include <vire/device/base_port_model.h>
#include <vire/device/physical_port.h>
#include <vire/device/logical_port.h>
#include <vire/device/i_instance.h>
#include <vire/device/single_instance.h>
#include <vire/device/physical_link.h>
#include <vire/device/device_link_info.h>

namespace vire {

  namespace device {

    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_device_model,
                                                     "vire::device::base_device_model/__system__");

    VIRE_DEVICE_REGISTRATION_IMPLEMENT(base_device_model, "vire::device::base_device_model");

    // static
    const base_device_model::device_model_key base_device_model::INVALID_DEVICE_MODEL_KEY;

    base_device_model::base_device_model()
    {
      set_logging_priority(datatools::logger::PRIO_FATAL);
      _initialized_ = false;
      _set_defaults();
      return;
    }

    base_device_model::base_device_model(device_type type_)
    {
      set_logging_priority(datatools::logger::PRIO_FATAL);
      _initialized_ = false;
      _set_defaults();
      set_type(type_);
      return;
    }

    base_device_model::~base_device_model()
    {
      _embedded_physical_devices_.clear();
      if (_logical_) {
        _logical_.reset();
      }
      return;
    }

    bool base_device_model::is_allow_embedded_devices() const
    {
      return _allow_embedded_devices_;
    }

    void base_device_model::set_allow_embedded_devices(bool a_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Device model '" << get_name() << "' is already initialized !");
      _allow_embedded_devices_ = a_;
      return;
    }

    bool base_device_model::is_virtual() const
    {
      return _virtual_;
    }

    void base_device_model::set_virtual(bool v_)
    {
      _virtual_ = v_;
      return;
    }

    bool base_device_model::has_type() const
    {
      return _type_ != TYPE_UNDEFINED;
    }

    void base_device_model::reset_type()
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Device model '" << get_name() << "' is already initialized !");
      _type_ = TYPE_UNDEFINED;
      return;
    }

    void base_device_model::set_type(device_type type_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Device model '" << get_name() << "' is already initialized !");
      _type_ = type_;
      return;
    }

    device_type base_device_model::get_type() const
    {
      return (device_type) _type_;
    }

    bool base_device_model::has_model_key() const
    {
      return _model_key_ != INVALID_DEVICE_MODEL_KEY;
    }

    base_device_model::device_model_key base_device_model::get_model_key() const
    {
      return _model_key_;
    }

    void base_device_model::set_model_key(device_model_key dmk_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Device model '" << get_name() << "' is already initialized !");
      _model_key_ = dmk_;
      return;
    }

    void base_device_model::reset_model_key()
    {
      _model_key_ = INVALID_DEVICE_MODEL_KEY;
      return;
    }

    bool base_device_model::has_version() const
    {
      return _version_.is_valid();
    }

    const datatools::version_id & base_device_model::get_version() const
    {
      return _version_;
    }

    void base_device_model::set_version(const datatools::version_id & vid_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Device model '" << get_name() << "' is already initialized !");
      _version_ = vid_;
      return;
    }

    void base_device_model::reset_version()
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Device model '" << get_name() << "' is already initialized !");
      _version_.reset();
      return;
    }

    bool base_device_model::has_logical() const
    {
      return _logical_.has_data();
    }

    const logical_device & base_device_model::get_logical() const
    {
      DT_THROW_IF(! has_logical(), std::logic_error,
                  "Device model '" << get_name() << "' has no logical device !");
      return _logical_.get();
    }

    logical_device & base_device_model::_grab_logical()
    {
      DT_THROW_IF(! has_logical(), std::logic_error,
                  "Device model '" << get_name() << "' has no logical device !");
      return _logical_.grab();
    }

    void base_device_model::_at_initialize(const datatools::properties & config_,
                                           model_repository & models_)
    {
      return;
    }

    void base_device_model::_at_reset()
    {
       return;
    }

    bool base_device_model::is_initialized() const
    {
      return _initialized_;
    }

    void base_device_model::initialize_simple()
    {
      datatools::properties dummy_config;
      this->initialize_standalone(dummy_config);
      return;
    }

    void base_device_model::initialize_standalone(const datatools::properties & config_)
    {
      model_repository dummy_models;
      this->initialize(config_, dummy_models);
      return;
    }

    void base_device_model::initialize(const datatools::properties & config_,
                                       model_repository & models_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Device model '" << get_name() << "' is already initialized !");

      // Basic initialization:
      _device_initialize(config_, models_);

      // Initialization:
      DT_LOG_DEBUG(get_logging_priority(), "Initialization is starting...");
      _at_initialize(config_, models_);
      DT_LOG_DEBUG(get_logging_priority(), "Initialization done.");

      // Post initialization:
      DT_LOG_DEBUG(get_logging_priority(), "Post-initialization is starting...");
      _create_logical_();
      _logical_.grab().set_name(make_logical_name(this->get_name()));

      // Populate the logical:
      DT_LOG_DEBUG(get_logging_priority(), "Populating the logical device...");
      for (physical_device_dict_type::iterator i = _embedded_physical_devices_.begin();
           i != _embedded_physical_devices_.end();
           i++) {
        const std::string & label = i->first;
        const physical_device & physical = i->second;
        DT_LOG_DEBUG(get_logging_priority(), " ==> Adding daughter physical device '" << label << "'...");
        _logical_.grab().add_daughter(label, physical);
      }
      for (physical_port_dict_type::iterator i = _physical_ports_.begin();
           i != _physical_ports_.end();
           i++) {
        const std::string & label = i->first;
        const physical_port & physical = i->second;
        DT_LOG_DEBUG(get_logging_priority(), " ==> Adding physical port '" << label << "'...");
        _logical_.grab().add_port(label, physical);
      }
      for (physical_link_dict_type::iterator i = _physical_links_.begin();
           i != _physical_links_.end();
           i++) {
        const std::string & label = i->first;
        const physical_link & physical = i->second;
        DT_LOG_DEBUG(get_logging_priority(), " ==> Adding physical link '" << label << "'...");
        _logical_.grab().add_link(label, physical);
      }
      _logical_.grab().initialize();
      DT_LOG_DEBUG(get_logging_priority(), "Post-initialization done.");

      _set_initialized_(true);
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void base_device_model::reset()
    {
      DT_THROW_IF(! is_initialized(),
                  std::logic_error,
                  "Device model '" << get_name() << "' is not initialized !");
      _set_initialized_(false);

      // Pre reset:
      _logical_.reset();

      // Reset:
      _at_reset();

      // Basic reset:
      this->_device_reset();
      return;
    }

    void base_device_model::_set_defaults()
    {
      _virtual_ = false;
      _allow_embedded_devices_ = true;
      _type_ = TYPE_UNDEFINED;
      _model_key_ = INVALID_DEVICE_MODEL_KEY;
      return;
    }

    void base_device_model::_device_reset()
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      _embedded_physical_devices_.clear();
      _logical_.reset();
      _set_defaults();
      this->enriched_base::clear();
      DT_LOG_TRACE(get_logging_priority(),"Exiting.");
      return;
    }

    void base_device_model::_device_initialize(const datatools::properties & config_,
                                               model_repository & models_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      this->datatools::enriched_base::initialize(config_, false);

      if (datatools::logger::is_debug(get_logging_priority())) {
        std::cerr << "[debug] " << "\n";
        std::cerr << "[debug] ************************************" << "\n";
        std::cerr << "[debug] *                                   " << "\n";
        std::cerr << "[debug] *  Device model '" << get_name() << "'\n";
        std::cerr << "[debug] *                                   " << "\n";
        std::cerr << "[debug] ************************************" << "\n";
     }

      if (! has_model_key()) {
        if (config_.has_key("model_key")) {
          set_model_key(config_.fetch_integer("model_key"));
        }
      }

      if (! has_type()) {
        if (config_.has_key("type")) {
          std::string type_label = config_.fetch_string("type");
          set_type(get_device_type(type_label));
          DT_LOG_DEBUG(get_logging_priority(),
                       "Type = '" << type_label << "' in device model '" << get_name() << "'...");
        }
      }

      if (! has_type()) {
        set_type(TYPE_GENERIC);
      }

      if (config_.has_key("virtual")) {
        set_virtual(config_.fetch_boolean("virtual"));
      }

      if (config_.has_key("ports.labels")) {
        std::set<std::string> labels;
        config_.fetch("ports.labels", labels);
        for (std::set<std::string>::const_iterator i = labels.begin();
             i != labels.end ();
             i++) {
          const std::string & port_model_label = *i;
          DT_LOG_DEBUG(get_logging_priority(),
                       "Processing port '" << port_model_label << "' in device model '" << get_name() << "'...");
          // Parse model:
          std::ostringstream port_model_oss;
          port_model_oss << "ports.model." << port_model_label;
          DT_THROW_IF (! config_.has_key(port_model_oss.str()),
                       std::logic_error,
                       "Device model '" << get_name() << "' : "
                       << "Missing '" << port_model_oss.str() << "' property !");
          std::string port_model_name = config_.fetch_string(port_model_oss.str());
          DT_THROW_IF(!models_.has_port_model(port_model_name),
                      std::logic_error,
                      "Device model '" << get_name() << "' : "
                      << "Cannot find a port model named '" << port_model_name << "' !");
          // Fetch the initialized port model:
          const base_port_model & port_model = models_.get_port_model(port_model_name);

          // Parse instance:
          const i_instance * instance_ptr = 0;
          bool owned_instance = false;
          {
            std::ostringstream instance_oss;
            instance_oss << "ports."
                         << "instance." << port_model_label;
            if (config_.has_key(instance_oss.str())) {
              std::string instance_rule = config_.fetch_string(instance_oss.str());
              instance_ptr = i_instance::create(instance_rule);
              DT_THROW_IF(instance_ptr == 0, std::logic_error,
                          "Could not create a port instance from rule '" << instance_rule
                          << "' in model '" << get_name() << "'!");
              owned_instance = true;
            }
            if (instance_ptr == nullptr) {
              DT_LOG_DEBUG(get_logging_priority(),
                           "Use default instance for port '"
                           << port_model_label << "' in device model '" << get_name() << "'");
              instance_ptr = &default_instance();
              owned_instance = false;
            }
          }

          DT_LOG_DEBUG(get_logging_priority(), "Adding port '" << port_model_label << "' in device model '" << get_name() << "' :");
          DT_LOG_DEBUG(get_logging_priority(), "  with instance [@" << instance_ptr << "] of type '" << typeid(*instance_ptr).name() << "'");
          if (datatools::logger::is_debug(get_logging_priority())) {
            if (instance_ptr != nullptr) {
              instance_ptr->tree_dump(std::cerr, "", "[debug] ");
            }
          }
          // Add the port in the device model:
          physical_port & phys_port = this->add_port(port_model_label,
                                                     port_model,
                                                     instance_ptr,
                                                     owned_instance);
          {
            // Fetch auxiliaries properties and stored them in the physical:
            std::ostringstream aux_oss;
            aux_oss << "ports."
                    << "aux." << port_model_label << '.';
            datatools::properties aux_config;
            config_.export_and_rename_starting_with(aux_config, aux_oss.str(), "");
            aux_config.export_all(phys_port.grab_auxiliaries());
          }

        }
      }

      if (config_.has_key("allow_embedded_devices")) {
        set_allow_embedded_devices(config_.fetch_boolean("allow_embedded_devices"));
      }

      if (config_.has_key("embedded_devices.labels")) {
        DT_THROW_IF(! is_allow_embedded_devices(),
                    std::logic_error,
                    "Device model '" << get_name() << "' does not allow embedded devices !");

        std::set<std::string> labels;
        config_.fetch("embedded_devices.labels", labels);
        for (std::set<std::string>::const_iterator i = labels.begin();
             i != labels.end();
             i++) {
          const std::string & device_model_label = *i;

          // Parse model:
          std::ostringstream device_model_oss;
          device_model_oss << "embedded_devices."
                           << "model." << device_model_label;
          DT_THROW_IF (! config_.has_key(device_model_oss.str()),
                       std::logic_error,
                       "Device model '" << get_name() << "' : "
                       << "Missing '" << device_model_oss.str () << "' property !");
          std::string device_model_name = config_.fetch_string(device_model_oss.str());
          DT_THROW_IF(!models_.has_device_model(device_model_name),
                      std::logic_error,
                      "Device model '" << get_name() << "' : "
                      << "Cannot find a device model named '" << device_model_name << "' !");
          // Fetch the initialized device model:
          const base_device_model & device_model = models_.get_device_model(device_model_name);

          // Parse instance:
          const i_instance * instance_ptr = 0;
          bool owned_instance = false;
          {
            std::ostringstream instance_oss;
            instance_oss << "embedded_devices."
                         << "instance." << device_model_label;
            if (config_.has_key(instance_oss.str())) {
              std::string instance_rule = config_.fetch_string(instance_oss.str());
              instance_ptr = i_instance::create(instance_rule);
              DT_THROW_IF(instance_ptr == 0, std::logic_error,
                          "Could not create a device instance from rule '" << instance_rule
                          << "' in model '" << get_name() << "'!");
              owned_instance = true;
            }
            if (instance_ptr == 0) {
              instance_ptr = &default_instance();
              owned_instance = false;
            }
          }

          // Add the embedded device in the device model:
          physical_device & phys_device = this->add_embedded_device(device_model_label,
                                                                    device_model,
                                                                    instance_ptr,
                                                                    owned_instance);
          {
            // Fetch auxiliaries properties and stored them in the physical:
            std::ostringstream aux_oss;
            aux_oss << "embedded_devices."
                    << "aux." << device_model_label << '.';
            datatools::properties aux_config;
            config_.export_and_rename_starting_with(aux_config, aux_oss.str(), "");
            aux_config.export_all(phys_device.grab_auxiliaries());
          }
        }
      }

      if (datatools::logger::is_debug(get_logging_priority())) {
        DT_LOG_DEBUG(get_logging_priority(), "List of ports in device model '" << get_name() << "' : ");
        for (const auto & prt : _physical_ports_) {
          DT_LOG_DEBUG(get_logging_priority(), " ==> Port '" << prt.first << "'");
        }
      }

      if (config_.has_key("links.labels")) {
        std::set<std::string> labels;
        config_.fetch("links.labels", labels);
        for (std::set<std::string>::const_iterator i = labels.begin();
             i != labels.end ();
             i++) {
          const std::string & link_label = *i;
          DT_LOG_DEBUG(get_logging_priority(), "Processing link '" << link_label << "' in device model '" << get_name() <<"'...");
          std::ostringstream link_from_key;
          link_from_key << "links." << "from." << link_label;
          std::ostringstream link_to_key;
          link_to_key << "links." << "to." << link_label;
          std::string link_from_info;
          DT_THROW_IF(!config_.has_key(link_from_key.str()), std::logic_error,
                      "Missing link from property '" << link_from_key.str() << "' in device model '" << get_name() <<"'");
          link_from_info = config_.fetch_string(link_from_key.str());
          std::string link_to_info;
          DT_THROW_IF(!config_.has_key(link_to_key.str()), std::logic_error,
                      "Missing link to property '" << link_to_key.str() << "' in device model '" << get_name() <<"'");
          link_to_info = config_.fetch_string(link_to_key.str());
          DT_LOG_DEBUG(get_logging_priority(), "  Link from port info '" << link_from_info << "'");
          DT_LOG_DEBUG(get_logging_priority(), "  Link to   port info '" << link_to_info << "'");
          device_link_info dev_link_info;
          DT_THROW_IF(!dev_link_info.parse(link_from_info, link_to_info, *this),
                      std::logic_error,
                      "Cannot parse a device link info for link '" << link_label << "' in device model '" << get_name() <<"'");
          DT_LOG_DEBUG(get_logging_priority(), "  Device link info '" << link_label << "' has been successfully parsed");
          DT_THROW_IF(!dev_link_info.is_valid(),
                      std::logic_error,
                      "Invalid device link info for link '" << link_label << "' in device model '" << get_name() <<"'");
          if (datatools::logger::is_debug(get_logging_priority())) {
            dev_link_info.tree_dump(std::cerr, "Device link info: ", "[debug] ");
          }
          // Add the link in the device model:
          physical_link & phys_link = this->add_link(link_label, dev_link_info);
        }
      }

      DT_LOG_TRACE(get_logging_priority(),"Exiting.");
      return;
    }

    // virtual
    bool base_device_model::_check_embedded_type(device_type type_) const
    {
      return (get_type() == TYPE_GENERIC
              || type_ == TYPE_GENERIC
              || (int) type_ > (int) get_type());
    }

    physical_device & base_device_model::_add_embedded_physical(const std::string & device_label_,
                                                                const logical_device & logical_,
                                                                const i_instance * instance_,
                                                                bool owned_instance_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      _pre_add_embedded_physical(device_label_);
      {
        physical_device_dict_type::const_iterator found_embedded
          = _embedded_physical_devices_.find(device_label_);
        DT_THROW_IF(found_embedded != _embedded_physical_devices_.end(),
                    std::logic_error,
                    "Device model '" << get_name()
                    << "' already has an embedded physical device with label '"
                    << device_label_ << "' !");
      }
      DT_THROW_IF(!_check_embedded_type(logical_.get_model().get_type()),
                  std::logic_error,
                  "Device model '" << get_name()
                  << "' of type '" << get_device_type_label(get_type())
                  << "' cannot accept an embedded device with label '"
                  << device_label_ << "' of type '"
                  << get_device_type_label(logical_.get_model().get_type())
                  << "' !");
      {
        physical_device dummy_physical;
        _embedded_physical_devices_[device_label_] = dummy_physical;
      }
      physical_device & physical = _embedded_physical_devices_[device_label_];
      physical.set_name(base_physical::make_physical_name(get_name() + "." + device_label_));
      physical.set_logical(logical_);
      if (instance_ != 0) {
        physical.set_instance(instance_, owned_instance_);
      } else {
        physical.set_instance(default_instance());
        // physical.set_instance(new single_instance(i_instance::NA_SLOT), true);
      }

      DT_LOG_TRACE(get_logging_priority(), "New physical device '" << device_label_ << "' is added.");
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return physical;
    }

    void base_device_model::_remove_embedded_physical(const std::string & device_label_)
    {
      _pre_remove_embedded_physical(device_label_);
      {
        physical_device_dict_type::iterator found_embedded
          = _embedded_physical_devices_.find(device_label_);
        DT_THROW_IF (found_embedded == _embedded_physical_devices_.end(),
                     std::logic_error,
                     "Device model '" << get_name()
                     << "' has no embedded physical device with label '"
                     << device_label_ << "' !");
        _embedded_physical_devices_.erase(found_embedded);
      }
      _post_remove_embedded_physical(device_label_);
      return;
    }

    physical_port & base_device_model::_add_physical_port(const std::string & port_label_,
                                                          const logical_port & port_logical_,
                                                          const i_instance * instance_,
                                                          bool owned_instance_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      _pre_add_physical_port(port_label_);
      {
        physical_port_dict_type::const_iterator found_port
          = _physical_ports_.find(port_label_);
        DT_THROW_IF(found_port != _physical_ports_.end(),
                    std::logic_error,
                    "Device model '" << get_name()
                    << "' already has a physical port with label '"
                    << port_label_ << "' !");
      }
      {
        physical_port dummy_physical;
        _physical_ports_[port_label_] = dummy_physical;
      }
      physical_port & physical = _physical_ports_[port_label_];
      physical.set_name(base_physical::make_physical_name(get_name()+"."+port_label_));
      // physical.set_mother_logical(this->get_logical());
      physical.set_logical(port_logical_);
      if (instance_ != 0) {
        physical.set_instance(instance_, owned_instance_);
      } else {
        physical.set_instance(default_instance());
      }
      DT_LOG_TRACE(get_logging_priority(), "New physical port '" << port_label_ << "' is added.");
      _post_add_physical_port(port_label_);
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return physical;
    }

    physical_link &
    base_device_model::_add_physical_link(const std::string & link_label_,
                                          const device_link_info & dli_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      //_pre_add_physical_link(link_label_);
      {
        physical_link_dict_type::const_iterator found_link
          = _physical_links_.find(link_label_);
        DT_THROW_IF(found_link != _physical_links_.end(),
                    std::logic_error,
                    "Device model '" << get_name()
                    << "' already has a physical link with label '"
                    << link_label_ << "' !");
      }
      {
        physical_link dummy_physical;
        _physical_links_[link_label_] = dummy_physical;
      }
      physical_link & physical = _physical_links_[link_label_];
      physical.set_name(base_physical::make_physical_name(get_name()+"."+link_label_));
      // physical.set_mother_logical(this->get_logical());
      physical.set_link_info(dli_);
      DT_LOG_TRACE(get_logging_priority(), "New physical link '" << link_label_ << "' is added.");
      // _post_add_physical_link(link_label_);
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return physical;
    }

    void base_device_model::_remove_physical_link(const std::string & link_label_)
    {
      //_pre_remove_physical_link(link_label_);
      {
        physical_link_dict_type::iterator found_link
          = _physical_links_.find(link_label_);
        DT_THROW_IF(found_link == _physical_links_.end(),
                    std::logic_error,
                    "Device model '" << get_name()
                    << "' has no physical link with label '"
                    << link_label_ << "' !");
        _physical_links_.erase(found_link);
      }
      //_post_remove_physical_link(link_label_);
      return;
    }

    void base_device_model::_remove_physical_port(const std::string & port_label_)
    {
      _pre_remove_physical_port(port_label_);
      {
        physical_port_dict_type::iterator found_port
          = _physical_ports_.find(port_label_);
        DT_THROW_IF (found_port == _physical_ports_.end(),
                     std::logic_error,
                     "Device model '" << get_name()
                     << "' has no physical port with label '"
                     << port_label_ << "' !");
        _physical_ports_.erase(found_port);
      }
      _post_remove_physical_port(port_label_);
      return;
    }

    void base_device_model::_set_initialized_(bool init_)
    {
      _initialized_ = init_;
      return;
    }

    void base_device_model::_create_logical_()
    {
      if (! _logical_.has_data()) {
        _logical_.reset(new logical_device);
        _logical_.grab().set_model(*this);
      }
      return;
    }

    bool base_device_model::has_link(const std::string & link_label_) const
    {
      physical_link_dict_type::const_iterator found_link
        = _physical_links_.find(link_label_);
      return found_link != _physical_links_.end();
    }

    physical_link & base_device_model::add_link(const std::string & link_label_,
                                                const device_link_info & dli_)
    {
      return _add_physical_link(link_label_,  dli_);
    }

    void base_device_model::remove_link(const std::string & link_label_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Device model '" << get_name() << "' is already initialized !");
      _remove_physical_link(link_label_);
      return;
    }

    const physical_link &
    base_device_model::get_link(const std::string & link_label_) const
    {
      physical_link_dict_type::const_iterator found_link
        = _physical_links_.find(link_label_);
      DT_THROW_IF (found_link == _physical_links_.end(),
                   std::logic_error,
                   "Device model '" << get_name()
                   << "' has no link with label '"
                   << link_label_ << "' !");
      return found_link->second;
    }

    void base_device_model::fetch_link_labels(std::set<std::string> & link_labels_) const
    {
      link_labels_.clear();
      for (const auto & p : _physical_links_) {
        link_labels_.insert(p.first);
      }
      return;
    }

    bool base_device_model::has_port(const std::string & port_label_) const
    {
      physical_port_dict_type::const_iterator found_port
        = _physical_ports_.find(port_label_);
      return found_port != _physical_ports_.end();
    }

    void base_device_model::fetch_port_labels(std::set<std::string> & port_labels_) const
    {
      port_labels_.clear();
      for (const auto & p : _physical_ports_) {
        port_labels_.insert(p.first);
      }
      return;
    }

    physical_port & base_device_model::add_port(const std::string & port_label_,
                                                const base_port_model & port_model_,
                                                const i_instance & instance_)
    {
      return add_port(port_label_,  port_model_, &instance_, false);
    }

    physical_port & base_device_model::add_port(const std::string & port_label_,
                                                const base_port_model & port_model_,
                                                const i_instance * instance_,
                                                bool owned_instance_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Cannot add port '" << port_label_ << "' for device model '" << get_name() << "' is already initialized !");
      DT_THROW_IF(! port_model_.is_initialized(),
                  std::logic_error,
                  "Device model '" << port_model_.get_name() << "' is not initialized !");
      physical_port & phys = _add_physical_port(port_label_,
                                                port_model_.get_logical(),
                                                instance_,
                                                owned_instance_);
      return phys;
    }

    void base_device_model::remove_port(const std::string & port_label_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Device model '" << get_name() << "' is already initialized !");
      _remove_physical_port(port_label_);
      return;
    }

    const physical_port &
    base_device_model::get_port(const std::string & port_label_) const
    {
      physical_port_dict_type::const_iterator found_port
        = _physical_ports_.find(port_label_);
      DT_THROW_IF (found_port == _physical_ports_.end(),
                   std::logic_error,
                   "Device model '" << get_name()
                   << "' has no port with label '"
                   << port_label_ << "' !");
      return found_port->second;
    }

    bool base_device_model::has_embedded_device(const std::string & device_label_) const
    {
      physical_device_dict_type::const_iterator found_embedded
        = _embedded_physical_devices_.find(device_label_);
      return found_embedded != _embedded_physical_devices_.end();
    }

    void base_device_model::fetch_embedded_device_labels(std::set<std::string> & edev_labels_) const
    {
      edev_labels_.clear();
      for (const auto & p : _embedded_physical_devices_) {
        edev_labels_.insert(p.first);
      }
      return;
    }

    physical_device &
    base_device_model::add_embedded_device(const std::string & device_label_,
                                           const base_device_model & device_model_,
                                           const i_instance & instance_)
    {
      return add_embedded_device(device_label_, device_model_, &instance_, false);
    }

    physical_device &
    base_device_model::add_embedded_device(const std::string & device_label_,
                                           const base_device_model & device_model_,
                                           const i_instance * instance_,
                                           bool owned_instance_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Cannot add embedded device '" << device_label_ << "' for device model '" << get_name() << "' is already initialized !");
      DT_THROW_IF(! device_model_.is_initialized(),
                  std::logic_error,
                  "Embedded device model '" << device_model_.get_name() << "' is not initialized !");
      physical_device & phys = _add_embedded_physical(device_label_,
                                                      device_model_.get_logical(),
                                                      instance_,
                                                      owned_instance_);
      return phys;
    }

    void base_device_model::remove_embedded_device(const std::string & device_label_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Device model '" << get_name() << "' is already initialized !");
      _remove_embedded_physical(device_label_);
      return;
    }

    const physical_device &
    base_device_model::get_embedded_device(const std::string & device_label_) const
    {
      physical_device_dict_type::const_iterator found_embedded
        = _embedded_physical_devices_.find(device_label_);
      DT_THROW_IF (found_embedded == _embedded_physical_devices_.end(),
                   std::logic_error,
                   "Device model '" << get_name()
                   << "' has no embedded physical device with label '"
                   << device_label_ << "' !");
      return found_embedded->second;
    }

    // virtual
    void base_device_model::_pre_add_embedded_physical(const std::string & embedded_label_)
    {
      return;
    }

    // virtual
    void base_device_model::_post_add_embedded_physical(const std::string & embedded_label_)
    {
      return;
    }

    // virtual
    void base_device_model::_pre_remove_embedded_physical(const std::string & embedded_label_)
    {
      return;
    }

    // virtual
    void base_device_model::_post_remove_embedded_physical(const std::string & embedded_label_)
    {
      return;
    }

    // virtual
    void base_device_model::_pre_add_physical_port(const std::string & port_label_)
    {
      return;
    }

    // virtual
    void base_device_model::_post_add_physical_port(const std::string & port_label_)
    {
      return;
    }

    // virtual
    void base_device_model::_pre_remove_physical_port(const std::string & port_label_)
    {
      return;
    }

    // virtual
    void base_device_model::_post_remove_physical_port(const std::string & port_label_)
    {
      return;
    }

    void base_device_model::tree_dump(std::ostream& out_,
                                      const std::string& title_,
                                      const std::string& indent_,
                                      bool inherit_) const
    {
      this->enriched_base::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Type : '" << get_device_type_label(_type_)
           << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Model key : ";
      if (has_model_key()) {
        out_ << '[' << _model_key_ << ']';
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Model version : ";
      if (_version_.is_valid()) {
        out_ << "'" << _version_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Ports : ";
      if (_physical_ports_.size() == 0) {
        out_ << "<none>";
      } else {
        out_ << '[' << _physical_ports_.size() << ']';
      }
      out_ << std::endl;
      for (physical_port_dict_type::const_iterator i = _physical_ports_.begin();
           i != _physical_ports_.end();
           i++) {
        const std::string & port_label = i->first;
        const physical_port & physical = i->second;
        physical_port_dict_type::const_iterator j = i;
        j++;
        out_ << indent_ << i_tree_dumpable::skip_tag;
        if (j == _physical_ports_.end()) {
          out_ << i_tree_dumpable::last_tag;
        } else {
          out_ << i_tree_dumpable::tag;
        }
        out_ << "'" << port_label << "' : as logical '";
        out_ << physical.get_logical().get_name() << "'";
        out_ << " (";
        size_t noi = physical.get_instance().get_number_of_items();
        out_ << noi << ' ';
        out_ << "instance";
        if (noi > 1) out_ << 's';
        out_ << ')';
        // out_ << " of type '" << get_port_type_label(physical.get_model().get_type()) << "'";
        out_ << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Allow embedded devices : " << _allow_embedded_devices_
           << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Embedded devices : ";
      if (_embedded_physical_devices_.size() == 0) {
        out_ << "<none>";
      } else {
        out_ << '[' << _embedded_physical_devices_.size() << ']';
      }
      out_ << std::endl;
      for (physical_device_dict_type::const_iterator i = _embedded_physical_devices_.begin();
           i != _embedded_physical_devices_.end();
           i++) {
        const std::string & device_label = i->first;
        const physical_device & physical = i->second;
        physical_device_dict_type::const_iterator j = i;
        j++;
        out_ << indent_ << i_tree_dumpable::skip_tag;
        if (j == _embedded_physical_devices_.end()) {
          out_ << i_tree_dumpable::last_tag;
        } else {
          out_ << i_tree_dumpable::tag;
        }
        out_ << "'" << device_label << "' : as logical '";
        out_ << physical.get_logical().get_name() << "'";
        out_ << " (";
        size_t noi = physical.get_instance().get_number_of_items();
        out_ << noi << ' ';
        out_ << "instance";
        if (noi > 1) out_ << 's';
        out_ << ')';
        // out_ << " of type '" << get_device_type_label(physical.get_logical().get_model().get_type()) << "'";
        out_ << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Links : ";
      if (_physical_links_.size() == 0) {
        out_ << "<none>";
      } else {
        out_ << '[' << _physical_links_.size() << ']';
      }
      out_ << std::endl;
      for (physical_link_dict_type::const_iterator i = _physical_links_.begin();
           i != _physical_links_.end();
           i++) {
        const std::string & link_label = i->first;
        const physical_link & physical = i->second;
        physical_link_dict_type::const_iterator j = i;
        j++;
        out_ << indent_ << i_tree_dumpable::skip_tag;
        std::ostringstream indent2;
        indent2 << indent_ << i_tree_dumpable::skip_tag;
        if (j == _physical_links_.end()) {
          out_ << i_tree_dumpable::last_tag;
          indent2 << i_tree_dumpable::last_skip_tag;
        } else {
          out_ << i_tree_dumpable::tag;
          indent2 << i_tree_dumpable::skip_tag;
        }
        out_ << "'" << link_label << "' ";
        out_ << std::endl;
        const device_link_info & dli = physical.get_link_info();
        dli.tree_dump(out_, "", indent2.str());
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Initialized : " << _initialized_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Logical : ";
      if (! has_logical()) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (has_logical()) {
        std::ostringstream indent_oss;
        indent_oss << indent_ << i_tree_dumpable::inherit_skip_tag(inherit_);
        _logical_.get().tree_dump(out_, "", indent_oss.str());
      }

      return;
    }

  } // end of namespace device

} // end of namespace vire
