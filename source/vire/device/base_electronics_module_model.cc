//! \file vire/device/base_electronics_module_model.cc
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
#include <vire/device/base_electronics_module_model.h>

// This project:
#include <vire/device/device_types.h>
#include <vire/device/physical_device.h>
#include <vire/device/logical_device.h>
#include <vire/device/single_instance.h>
#include <vire/device/slot.h>

namespace vire {

  namespace device {

    VIRE_DEVICE_REGISTRATION_IMPLEMENT(base_electronics_module_model,
                                       "vire::device::base_electronics_module_model");

    void base_electronics_module_model::_set_defaults()
    {
      set_type(TYPE_MODULE);
      set_allow_embedded_devices(true);
      _slot_width_ = 0;
      _max_number_of_mezzanine_modules_ = 0;
      return;
    }

    base_electronics_module_model::base_electronics_module_model()
    {
      _set_defaults();
      return;
    }

    base_electronics_module_model::~base_electronics_module_model()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool base_electronics_module_model::has_slot_width() const
    {
      return _slot_width_ > 0;
    }

    uint32_t base_electronics_module_model::get_slot_width() const
    {
      return _slot_width_;
    }

    void base_electronics_module_model::set_slot_width(uint32_t sw_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Module model '" << get_name() << "' is already initialized !");
      _slot_width_ = sw_;
      return;
    }

    bool base_electronics_module_model::has_max_number_of_mezzanine_modules() const
    {
      return _max_number_of_mezzanine_modules_ >= 0;
    }

    uint32_t base_electronics_module_model::get_max_number_of_mezzanine_modules() const
    {
      return _max_number_of_mezzanine_modules_;
    }

    void base_electronics_module_model::set_max_number_of_mezzanine_modules(uint32_t n_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Module model '" << get_name() << "' is already initialized !");
      DT_THROW_IF(_mezzanine_labels_.size() > 0,
                  std::logic_error,
                  "Module model '" << get_name() << "' already has mezzanine modules !");
      _max_number_of_mezzanine_modules_ = n_;
      return;
    }

    bool base_electronics_module_model::has_format() const
    {
      return ! _format_.empty();
    }

    const std::string & base_electronics_module_model::get_format() const
    {
      return _format_;
    }

    void base_electronics_module_model::set_format(const std::string & format_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Module model '" << get_name() << "' is already initialized !");
      _format_ = format_;
      return;
    }

    bool base_electronics_module_model::has_mezzanine_format() const
    {
      return ! _mezzanine_format_.empty();
    }

    const std::string & base_electronics_module_model::get_mezzanine_format() const
    {
      return _mezzanine_format_;
    }

    void base_electronics_module_model::set_mezzanine_format(const std::string & format_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Module model '" << get_name() << "' is already initialized !");
      _mezzanine_format_ = format_;
      return;
    }

    bool base_electronics_module_model::has_mezzanine_module(uint32_t mezzanine_slot_id_) const
    {
      return _mezzanine_labels_.find(mezzanine_slot_id_) != _mezzanine_labels_.end();
    }

    void base_electronics_module_model::remove_mezzanine_module(uint32_t mezzanine_slot_id_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Module model '" << get_name() << "' is initialized !");
      DT_THROW_IF(!has_mezzanine_module(mezzanine_slot_id_),
                  std::logic_error,
                  "Module model '" << get_name() << "' does not have a mezzanine module at slot '"
                  << mezzanine_slot_id_ << "' !");
      {
        std::string mezzanine_label = _mezzanine_labels_[mezzanine_slot_id_];
        // Explicit unregistration of the mezzanine label at its slot:
        _mezzanine_labels_.erase(_mezzanine_labels_.find(mezzanine_slot_id_));
        this->_remove_embedded_physical(mezzanine_label);
      }
      return;
    }

    physical_device &
    base_electronics_module_model::add_mezzanine_module(uint32_t mezzanine_slot_id_,
                                                        const base_device_model & mezzanine_model_,
                                                        const std::string & mezzanine_label_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Module model '" << get_name() << "' is already initialized !");
      indexed_labels_dict_type::iterator found = _mezzanine_labels_.find(mezzanine_slot_id_);
      DT_THROW_IF(_max_number_of_mezzanine_modules_ == 0,
                  std::logic_error,
                  "Module model '" << get_name() << "' does not accept mezzanine modules !");
      DT_THROW_IF(mezzanine_slot_id_ >= _max_number_of_mezzanine_modules_,
                  std::logic_error,
                  "Module model '" << get_name() << "' : Invalid mezzanine slot '" << mezzanine_slot_id_ << "' !");
      DT_THROW_IF(has_mezzanine_module(mezzanine_slot_id_),
                  std::logic_error,
                  "Module model '" << get_name() << "' already has a mezzanine module at slot '" << mezzanine_slot_id_ << "' !");
      const base_electronics_module_model * mez_module = dynamic_cast<const base_electronics_module_model *>(&mezzanine_model_);
      DT_THROW_IF(mez_module == 0 || mezzanine_model_.get_type() != TYPE_MODULE_MEZZANINE_BOARD,
                  std::logic_error,
                  "Module model '" << get_name() << "' : Attempt to embed a non mezzanine device '"
                  << mezzanine_model_.get_name()
                  << "' at slot '" << mezzanine_slot_id_ << "' !");

      if (has_mezzanine_format() && mez_module->has_format()) {
        DT_THROW_IF(_mezzanine_format_ != mez_module->get_format(),
                    std::logic_error,
                    "Module model '" << get_name() << "' :  Attempt to embed a mezzanine module with incompatible format ('"
                    << mez_module->get_format()
                    << "') at slot '" << mezzanine_slot_id_ << "' !");
      }
      // Finally, insert the mezzanine as a embedded device:
      // Compute the mezzanine label:
      std::string mezzanine_label = mezzanine_label_;
      if (mezzanine_label.empty()) {
        if (_mezzanine_labels_.find(mezzanine_slot_id_) != _mezzanine_labels_.end()) {
          mezzanine_label = _mezzanine_labels_[mezzanine_slot_id_];
        } else {
          // Automated mezzanine label:
          std::ostringstream mod_label;
          mod_label << "mezzanine_" << mezzanine_slot_id_;
          mezzanine_label = mod_label.str();
        }
      }
      // Explicit registration of the mezzanine label at its slot:
      _mezzanine_labels_[mezzanine_slot_id_] = mezzanine_label;
      slot mez_slot;
      mez_slot.make_dimension_0(true);
      // mez_slot.make_dimension_1(mezzanine_slot_id_);
      return this->_add_embedded_physical(mezzanine_label,
                                          mezzanine_model_.get_logical(),
                                          new single_instance(mez_slot),
                                          true);
    }

    const physical_device &
    base_electronics_module_model::get_mezzanine_module(uint32_t mezzanine_slot_id_)
    {
      indexed_labels_dict_type::iterator found = _mezzanine_labels_.find(mezzanine_slot_id_);
      DT_THROW_IF(found == _mezzanine_labels_.end(),
                  std::logic_error,
                  "Module model '" << get_name() << "' has no embedded mezzanine module at slot '" << mezzanine_slot_id_ << "' !");
      return get_embedded_device(found->second);
    }

    void base_electronics_module_model::_at_initialize(const datatools::properties& config_,
                                                       model_repository & models_)
    {
      this->_module_initialize(config_, models_);
      return;
    }

    void base_electronics_module_model::_at_reset()
    {
      this->_module_reset();
      return;
    }

    void base_electronics_module_model::_module_reset()
    {
      _mezzanine_labels_.clear();
      _slot_width_ = 0;
      _max_number_of_mezzanine_modules_ = 0;
      _mezzanine_format_.clear();
      _format_.clear();
      return;
    }

    void base_electronics_module_model::_module_initialize(const datatools::properties & config_,
                                                           model_repository & models_)
    {
      if (! has_format()) {
        if (config_.has_key("format")) {
          std::string bf = config_.fetch_string("format");
          set_format(bf);
        }
      }

      if (! has_mezzanine_format()) {
        if (config_.has_key("mezzanine_format")) {
          std::string mf = config_.fetch_string("mezzanine_format");
          set_mezzanine_format(mf);
        }
      }

      if (! _slot_width_) {
        if (config_.has_key("slot_width")) {
          int32_t sw = config_.fetch_integer("slot_width");
          DT_THROW_IF ((sw < 1 || sw > 64), std::domain_error,
                       "Invalid slot width '" << sw << "' !");
          uint32_t usw = sw;
          set_slot_width(usw);
        }
      }

      if (_max_number_of_mezzanine_modules_ == 0) {
        if (config_.has_key("max_number_of_mezzanine_modules")) {
          int32_t mnomm = config_.fetch_integer("max_number_of_mezzanine_modules");
          DT_THROW_IF(mnomm < 0, std::domain_error,
                      "Invalid max number of mezzanine modules '" << mnomm << "' !");
          uint32_t umnomm = (uint32_t) mnomm;
          set_max_number_of_mezzanine_modules(umnomm);
        }
      }

      if (_max_number_of_mezzanine_modules_ > 0) {
        for (int islot = 0; islot < _max_number_of_mezzanine_modules_; islot++) {
          // Attempt to find a mezzanine model at the visited slot:
          std::ostringstream mezzanine_slot_model_key;
          mezzanine_slot_model_key << "mezzanine_modules.slot_" << islot << ".model" ;
          if (config_.has_key(mezzanine_slot_model_key.str())) {
            std::string mezzanine_model_name = config_.fetch_string(mezzanine_slot_model_key.str());
            DT_THROW_IF(!models_.has_device_model(mezzanine_model_name),
                        std::logic_error,
                        "Module model '" << get_name() << "' : "
                        << "Cannot find a mezzanine module model named '" << mezzanine_model_name << "' for slot [" << islot << "]!");
            // Fetch the initialized crate model:
            const base_device_model & mezzanine_model = models_.get_device_model(mezzanine_model_name);
            std::ostringstream mezzanine_slot_label_key;
            mezzanine_slot_label_key << "mezzanine_modules.slot_" << islot << ".label";
            std::string mezzanine_label;
            // Attempt to find a mezzanine label:
            if (config_.has_key(mezzanine_slot_label_key.str())) {
              mezzanine_label = config_.fetch_string(mezzanine_slot_label_key.str());
            }
            add_mezzanine_module(islot, mezzanine_model, mezzanine_label);
          }
        }
      }

      return;
    }

    /// Post remove
    void base_electronics_module_model::_post_remove_embedded_physical(const std::string & embedded_label_)
    {
      // Explicitely unregistration of a mezzanine module:
      indexed_labels_dict_type::iterator found
        = std::find_if(_mezzanine_labels_.begin(),
                       _mezzanine_labels_.end(),
                       std::bind2nd(map_data_compare<indexed_labels_dict_type>(),
                                    embedded_label_));
      if (found != _mezzanine_labels_.end()) {
        // If the removed embedded device is one of the registered mezzanine modules, it is unregistered:
        DT_LOG_TRACE(get_logging_priority(),
                     "Unregistering mezzanine module at slot='" << found->first << "'");
        _mezzanine_labels_.erase(found);
      }
      return;
    }

    void base_electronics_module_model::tree_dump(std::ostream& out_,
                                                  const std::string& title_,
                                                  const std::string& indent_,
                                                  bool inherits_) const
    {
      this->base_device_model::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Format : '" << _format_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Slot width : " << _slot_width_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherits_)
           << "Mezzanine modules : ";
      if (_max_number_of_mezzanine_modules_ == 0) {
        out_ << "<none>";
      }
      out_ << std::endl;
      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherits_)
           << i_tree_dumpable::tag
           << "Number of mezzanine slots : "
           << _max_number_of_mezzanine_modules_ << std::endl;

      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherits_)
           << i_tree_dumpable::tag
           << "Format : '" << _mezzanine_format_ << "'" << std::endl;

      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherits_)
           << i_tree_dumpable::last_tag
           << "Mezzanine modules : ";
      if (_mezzanine_labels_.size() > 0) {
        out_ << _mezzanine_labels_.size();
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      for (int islot = 0; islot < _max_number_of_mezzanine_modules_; islot++) {
        out_ << indent_ << i_tree_dumpable::inherit_skip_tag(inherits_);
        out_ << i_tree_dumpable::last_skip_tag;
        if (islot == _max_number_of_mezzanine_modules_ - 1) {
          out_ << i_tree_dumpable::last_tag;
        } else {
          out_ << i_tree_dumpable::tag;
        }
        out_ << "Slot #" << islot << " : ";
        indexed_labels_dict_type::const_iterator found_at_slot = _mezzanine_labels_.find(islot);
        if (found_at_slot != _mezzanine_labels_.end()) {
          out_ << "Model '" << get_embedded_device(found_at_slot->second).get_logical().get_model().get_name() << "'";
        } else {
          out_ << "<empty>";
        }
        out_ << std::endl;
      }

      return;
    }

  } // end of namespace device

} // end of namespace vire
