//! \file vire/device/base_electronics_crate_model.cc
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
#include <vire/device/base_electronics_crate_model.h>

// This project:
#include <vire/device/device_types.h>
#include <vire/device/physical_device.h>
#include <vire/device/logical_device.h>
#include <vire/device/base_electronics_module_model.h>
#include <vire/device/single_instance.h>

namespace vire {

  namespace device {

    VIRE_DEVICE_REGISTRATION_IMPLEMENT(base_electronics_crate_model,
                                       "vire::device::base_electronics_crate_model");

    void base_electronics_crate_model::_set_defaults()
    {
      set_type(TYPE_CRATE);
      set_allow_embedded_devices(true);
      _max_number_of_modules_ = 0;
      return;
    }

    base_electronics_crate_model::base_electronics_crate_model()
    {
      _set_defaults();
      return;
    }

    base_electronics_crate_model::~base_electronics_crate_model()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool base_electronics_crate_model::has_max_number_of_modules() const
    {
      return _max_number_of_modules_ > 0;
    }

    void base_electronics_crate_model::set_max_number_of_modules(uint32_t nos_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Crate model '" << get_name() << "' is already initialized !");
      DT_THROW_IF(_module_labels_.size() > 0,
                  std::logic_error,
                  "Crate model '" << get_name() << "' already has plugged modules !");
      _max_number_of_modules_ = nos_;
      return;
    }

    uint32_t base_electronics_crate_model::get_max_number_of_modules()
    {
      return _max_number_of_modules_;
    }

    bool base_electronics_crate_model::has_format() const
    {
      return ! _format_.empty();
    }

    const std::string & base_electronics_crate_model::get_format() const
    {
      return _format_;
    }

    void base_electronics_crate_model::set_format(const std::string & format_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Crate model '" << get_name() << "' is already initialized !");
      _format_ = format_;
      return;
    }

    bool base_electronics_crate_model::has_module(uint32_t module_slot_id_) const
    {
      return _module_labels_.find(module_slot_id_) != _module_labels_.end();
    }

    void base_electronics_crate_model::remove_module(uint32_t module_slot_id_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Crate model '" << get_name() << "' is initialized !");
      DT_THROW_IF(!has_module(module_slot_id_),
                  std::logic_error,
                  "Crate model '" << get_name() << "' does not have a module at slot '" << module_slot_id_ << "' !");
      {
        std::string module_label = _module_labels_[module_slot_id_];
        // Explicit unregistration of the module label at its slot:
        _module_labels_.erase(_module_labels_.find(module_slot_id_));
        this->_remove_embedded_physical(module_label);
      }
      return;
    }

    physical_device &
    base_electronics_crate_model::add_module(uint32_t module_slot_id_,
                                             const base_device_model & module_model_,
                                             const std::string & module_label_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Crate model '" << get_name() << "' is already initialized !");
      indexed_labels_dict_type::iterator found = _module_labels_.find(module_slot_id_);
      DT_THROW_IF(_max_number_of_modules_ == 0,
                  std::logic_error,
                  "Crate model '" << get_name() << "' does not accept modules !");
      DT_THROW_IF(module_slot_id_ >= _max_number_of_modules_,
                  std::logic_error,
                  "Crate model '" << get_name() << "' : Invalid module slot '" << module_slot_id_ << "' !");
      const base_electronics_module_model * module = dynamic_cast<const base_electronics_module_model *>(&module_model_);
      DT_THROW_IF(module == 0,
                  std::logic_error,
                  "Crate model '" << get_name() << "' : Attempt to embed a non module device '"
                  << module_model_.get_name()
                  << "' at slot '" << module_slot_id_ << "' !");
      uint32_t module_width = 1;
      module_width = module->get_slot_width();
      for (int islot = module_slot_id_;
           islot < module_slot_id_ + module_width;
           islot++) {
        DT_THROW_IF(has_module(islot),
                    std::logic_error,
                    "Crate model '" << get_name() << "' already has a module at slot '" << islot << "' !");
      }
      if (has_format() && module->has_format()) {
        DT_THROW_IF(_format_ != module->get_format(),
                    std::logic_error,
                    "Crate model '" << get_name() << "' :  Attempt to embed a module module with incompatible format ('"
                    << module->get_format()
                    << "') at slot '" << module_slot_id_ << "' !");
      }
      // Finally, insert the module as a embedded device:
      // Compute the module label:
      std::string module_label = module_label_;
      if (module_label.empty()) {
        if (_module_labels_.find(module_slot_id_) != _module_labels_.end()) {
          module_label = _module_labels_[module_slot_id_];
        } else {
          // Automated module label:
          std::ostringstream mod_label;
          mod_label << "module_" << module_slot_id_;
          module_label = mod_label.str();
        }
      }
      // Explicit registration of the module label at its slot:
      _module_labels_[module_slot_id_] = module_label;
      slot mod_slot;
      mod_slot.make_dimension_0(true);
      // mod_slot.make_dimension_1(module_slot_id_);
      return this->_add_embedded_physical(module_label,
                                          module_model_.get_logical(),
                                          new single_instance(mod_slot),
                                          true);
    }

    const physical_device &
    base_electronics_crate_model::get_module(uint32_t module_slot_id_)
    {
      indexed_labels_dict_type::iterator found = _module_labels_.find(module_slot_id_);
      DT_THROW_IF(found == _module_labels_.end(),
                  std::logic_error,
                  "Crate model '" << get_name() << "' has no embedded module at slot '" << module_slot_id_ << "' !");
      return get_embedded_device(found->second);
    }

    void base_electronics_crate_model::_at_initialize(const datatools::properties& config_,
                                                      model_repository & models_)
    {
      this->_crate_initialize(config_, models_);
      return;
    }

    void base_electronics_crate_model::_at_reset()
    {
      this->_crate_reset();
      return;
    }

    void base_electronics_crate_model::_crate_reset()
    {
      _module_labels_.clear();
      _max_number_of_modules_ = 0;
      _format_.clear();
      return;
    }

    void base_electronics_crate_model::_crate_initialize(const datatools::properties & config_,
                                                         model_repository & models_)
    {
      if (! has_format()) {
        if (config_.has_key("format")) {
          std::string cf = config_.fetch_string("format");
          set_format(cf);
        }
      }

      if (!has_max_number_of_modules()) {
        if (config_.has_key("max_number_of_modules")) {
          int32_t n = config_.fetch_integer("max_number_of_modules");
          DT_THROW_IF ((n < 1 || n > 64), std::domain_error,
                       "Invalid number of modules '" << n << "' !");
          uint32_t mnom = n;
          set_max_number_of_modules(mnom);
        }
      }

      if (has_max_number_of_modules()) {
        for (int islot = 0; islot < _max_number_of_modules_; islot++) {
          // Attempt to find a module model at the visited slot:
          std::ostringstream module_slot_model_key;
          module_slot_model_key << "modules.slot_" << islot << ".model" ;
          if (config_.has_key(module_slot_model_key.str())) {
            std::string module_model_name = config_.fetch_string(module_slot_model_key.str());
            DT_THROW_IF(!models_.has_device_model(module_model_name),
                        std::logic_error,
                        "Cannot find a device model named '" << module_model_name << "' !");
            const base_device_model & module_model = models_.get_device_model(module_model_name);
            std::ostringstream module_slot_label_key;
            module_slot_label_key << "modules.slot_" << islot << ".label";
            std::string module_label;
            // Attempt to find a module label:
            if (config_.has_key(module_slot_label_key.str())) {
              module_label = config_.fetch_string(module_slot_label_key.str());
            }
            add_module(islot, module_model, module_label);
          }
        }
      }

      return;
    }

    /// Post remove
    void base_electronics_crate_model::_post_remove_embedded_physical(const std::string & embedded_label_)
    {
      // Explicitely unregistration of a module:
      indexed_labels_dict_type::iterator found
        = std::find_if(_module_labels_.begin(),
                       _module_labels_.end(),
                       std::bind2nd(map_data_compare<indexed_labels_dict_type>(),
                                    embedded_label_));
      if (found != _module_labels_.end()) {
        // If the removed embedded device is one of the registered modules, it is unregistered:
        DT_LOG_TRACE(get_logging_priority(),
                     "Unregistering module at slot='" << found->first << "'");
        _module_labels_.erase(found);
      }
      return;
    }

    void base_electronics_crate_model::tree_dump(std::ostream& out_,
                                                 const std::string& title_,
                                                 const std::string& indent_,
                                                 bool inherit_) const
    {
      this->base_device_model::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Format : '" << _format_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Max. number of modules : '" << _max_number_of_modules_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Modules : ";
      if (_max_number_of_modules_ == 0) {
        out_ << "<none>";
      }
      out_ << std::endl;
      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherit_)
           << i_tree_dumpable::tag
           << "Number of slots : "
           << _max_number_of_modules_ << std::endl;

      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherit_)
           << i_tree_dumpable::tag
           << "Format : '" << _format_ << "'" << std::endl;

      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherit_)
           << i_tree_dumpable::last_tag
           << "Modules : ";
      if (_module_labels_.size() > 0) {
        out_ << _module_labels_.size();
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      int count_width = 0;
      for (int islot = 0; islot < _max_number_of_modules_; islot++) {
        out_ << indent_ << i_tree_dumpable::inherit_skip_tag(inherit_);
        out_ << i_tree_dumpable::last_skip_tag;
        if (islot == _max_number_of_modules_ - 1) {
          out_ << i_tree_dumpable::last_tag;
        } else {
          out_ << i_tree_dumpable::tag;
        }
        out_ << "Slot #" << islot << " : ";
        indexed_labels_dict_type::const_iterator found_at_slot = _module_labels_.find(islot);
        if (found_at_slot != _module_labels_.end()) {
          const base_electronics_module_model & module = dynamic_cast<const base_electronics_module_model &>(get_embedded_device(found_at_slot->second).get_logical().get_model());
          out_ << "Model '" << module.get_name() << "'";
          count_width = module.get_slot_width();
          count_width--;
        } else {
          if (count_width > 0) {
            out_ << "<>";
            count_width--;
          } else {
            out_ << "<empty>";
            count_width = 0;
          }
        }
        out_ << std::endl;
      }

      return;
    }

  } // end of namespace device

} // end of namespace vire
