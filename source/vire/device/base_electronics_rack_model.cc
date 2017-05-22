//! \file vire/device/base_electronics_rack_model.cc
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
#include <vire/device/base_electronics_rack_model.h>

// This project:
#include <vire/device/device_types.h>
#include <vire/device/physical_device.h>
#include <vire/device/logical_device.h>
#include <vire/device/base_electronics_crate_model.h>
#include <vire/device/single_instance.h>

namespace vire {

  namespace device {

    VIRE_DEVICE_REGISTRATION_IMPLEMENT(base_electronics_rack_model,
                                       "vire::device::base_electronics_rack_model");

    void base_electronics_rack_model::_set_defaults()
    {
      set_type(TYPE_RACK);
      set_allow_embedded_devices(true);
      _max_number_of_crates_ = 0;
      return;
    }

    base_electronics_rack_model::base_electronics_rack_model()
    {
      _set_defaults();
      return;
    }

    base_electronics_rack_model::~base_electronics_rack_model()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void base_electronics_rack_model::set_max_number_of_crates(uint32_t nos_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Rack model '" << get_name() << "' is already initialized !");
      DT_THROW_IF(_crate_labels_.size() > 0,
                  std::logic_error,
                  "Rack model '" << get_name() << "' already has plugged crates !");
      _max_number_of_crates_ = nos_;
      return;
    }

    bool base_electronics_rack_model::has_max_number_of_crates() const
    {
      return _max_number_of_crates_ > 0;
    }

    uint32_t base_electronics_rack_model::get_max_number_of_crates()
    {
      return _max_number_of_crates_;
    }

    bool base_electronics_rack_model::has_format() const
    {
      return ! _format_.empty();
    }

    const std::string & base_electronics_rack_model::get_format() const
    {
      return _format_;
    }

    void base_electronics_rack_model::set_format(const std::string & format_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Rack model '" << get_name() << "' is already initialized !");
      _format_ = format_;
      return;
    }

    bool base_electronics_rack_model::has_crate(uint32_t crate_slot_id_) const
    {
      return _crate_labels_.find(crate_slot_id_) != _crate_labels_.end();
    }

    void base_electronics_rack_model::remove_crate(uint32_t crate_slot_id_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Rack model '" << get_name() << "' is initialized !");
      DT_THROW_IF(!has_crate(crate_slot_id_),
                  std::logic_error,
                  "Rack model '" << get_name() << "' does not have a crate at slot '" << crate_slot_id_ << "' !");
      {
        std::string crate_label = _crate_labels_[crate_slot_id_];
        // Explicit unregistration of the crate label at its slot:
        _crate_labels_.erase(_crate_labels_.find(crate_slot_id_));
        this->_remove_embedded_physical(crate_label);
      }
      return;
    }

    physical_device &
    base_electronics_rack_model::add_crate(uint32_t crate_slot_id_,
                                           const base_device_model & crate_model_,
                                           const std::string & crate_label_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Rack model '" << get_name() << "' is already initialized !");
      indexed_labels_dict_type::iterator found = _crate_labels_.find(crate_slot_id_);
      DT_THROW_IF(has_max_number_of_crates() && crate_slot_id_ >= _max_number_of_crates_,
                  std::logic_error,
                  "Rack model '" << get_name() << "' : Invalid crate slot '" << crate_slot_id_ << "' !");
      const base_electronics_crate_model * crate = dynamic_cast<const base_electronics_crate_model *>(&crate_model_);
      DT_THROW_IF(crate == 0,
                  std::logic_error,
                  "Rack model '" << get_name() << "' : Attempt to embed a non crate device '"
                  << crate_model_.get_name()
                  << "' at slot '" << crate_slot_id_ << "' !");
      uint32_t crate_height = 1;
      crate_height = 1; // crate->get_slot_height();
      for (int islot = crate_slot_id_;
           islot < crate_slot_id_ + crate_height;
           islot++) {
        DT_THROW_IF(has_crate(islot),
                    std::logic_error,
                    "Rack model '" << get_name() << "' already has a crate at slot '" << islot << "' !");
      }
      if (has_format() && crate->has_format()) {
        DT_THROW_IF(_format_ != crate->get_format(),
                    std::logic_error,
                    "Rack model '" << get_name() << "' :  Attempt to embed a crate with incompatible format ('"
                    << crate->get_format()
                    << "') at slot '" << crate_slot_id_ << "' !");
      }
      // Finally, insert the crate as a embedded device:
      // Compute the crate label:
      std::string crate_label = crate_label_;
      if (crate_label.empty()) {
        if (_crate_labels_.find(crate_slot_id_) != _crate_labels_.end()) {
          crate_label = _crate_labels_[crate_slot_id_];
        } else {
          // Automated crate label:
          std::ostringstream mod_label;
          mod_label << "crate_" << crate_slot_id_;
          crate_label = mod_label.str();
        }
      }
      // Explicit registration of the crate label at its slot:
      _crate_labels_[crate_slot_id_] = crate_label;
      slot cr_slot;
      // cr_slot.make_dimension_1(crate_slot_id_);
      cr_slot.make_dimension_0(true);
      return this->_add_embedded_physical(crate_label,
                                          crate_model_.get_logical(),
                                          new single_instance(cr_slot),
                                          true);
    }

    const physical_device &
    base_electronics_rack_model::get_crate(uint32_t crate_slot_id_)
    {
      indexed_labels_dict_type::iterator found = _crate_labels_.find(crate_slot_id_);
      DT_THROW_IF(found == _crate_labels_.end(),
                  std::logic_error,
                  "Rack model '" << get_name() << "' has no embedded crate at slot '" << crate_slot_id_ << "' !");
      return get_embedded_device(found->second);
    }

    void base_electronics_rack_model::_at_initialize(const datatools::properties& config_,
                                                     model_repository & models_)
    {
      this->_rack_initialize(config_, models_);
      return;
    }

    void base_electronics_rack_model::_at_reset()
    {
      this->_rack_reset();
      return;
    }

    void base_electronics_rack_model::_rack_reset()
    {
      _crate_labels_.clear();
      _max_number_of_crates_ = 0;
      _format_.clear();
      return;
    }

    void base_electronics_rack_model::_rack_initialize(const datatools::properties & config_,
                                                       model_repository & models_)
    {
      if (! has_format()) {
        if (config_.has_key("format")) {
          std::string cf = config_.fetch_string("format");
          set_format(cf);
        }
      }

      if (!has_max_number_of_crates()) {
        if (config_.has_key("max_number_of_crates")) {
          int32_t n = config_.fetch_integer("max_number_of_crates");
          DT_THROW_IF ((n < 1 || n > 64), std::domain_error,
                       "Invalid number of crates '" << n << "' !");
          uint32_t mnom = n;
          set_max_number_of_crates(mnom);
        }
      }

      if (has_max_number_of_crates()) {
        for (int islot = 0; islot < _max_number_of_crates_; islot++) {
          // Attempt to find a crate model at the visited slot:
          std::ostringstream crate_slot_model_key;
          crate_slot_model_key << "crates.slot_" << islot << ".model" ;
          if (config_.has_key(crate_slot_model_key.str())) {
            std::string crate_model_name = config_.fetch_string(crate_slot_model_key.str());
            DT_THROW_IF(!models_.has_device_model(crate_model_name),
                        std::logic_error,
                        "Rack model '" << get_name() << "' : "
                        << "Cannot find a crate model named '" << crate_model_name << "' for slot [" << islot << "]!");
            // Fetch the initialized crate model:
            const base_device_model & crate_model = models_.get_device_model(crate_model_name);
            std::ostringstream crate_slot_label_key;
            crate_slot_label_key << "crates.slot_" << islot << ".label";
            std::string crate_label;
            // Attempt to find a crate label:
            if (config_.has_key(crate_slot_label_key.str())) {
              crate_label = config_.fetch_string(crate_slot_label_key.str());
            }
            add_crate(islot, crate_model, crate_label);
          }
        }
      }

      return;
    }

    /// Post remove
    void base_electronics_rack_model::_post_remove_embedded_physical(const std::string & embedded_label_)
    {
      // Explicitely unregistration of a crate:
      indexed_labels_dict_type::iterator found
        = std::find_if(_crate_labels_.begin(),
                       _crate_labels_.end(),
                       std::bind2nd(map_data_compare<indexed_labels_dict_type>(),
                                    embedded_label_));
      if (found != _crate_labels_.end()) {
        // If the removed embedded device is one of the registered crates, it is unregistered:
        DT_LOG_TRACE(get_logging_priority(),
                     "Unregistering crate at slot='" << found->first << "'");
        _crate_labels_.erase(found);
      }
      return;
    }

    void base_electronics_rack_model::tree_dump(std::ostream& out_,
                                                 const std::string& title_,
                                                 const std::string& indent_,
                                                 bool inherit_) const
    {
      this->base_device_model::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Format : '" << _format_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Max. number of crates : '" << _max_number_of_crates_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Crates : ";
      if (_max_number_of_crates_ == 0) {
        out_ << "<none>";
      }
      out_ << std::endl;
      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherit_)
           << i_tree_dumpable::tag
           << "Number of slots : "
           << _max_number_of_crates_ << std::endl;

      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherit_)
           << i_tree_dumpable::tag
           << "Format : '" << _format_ << "'" << std::endl;

      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherit_)
           << i_tree_dumpable::last_tag
           << "Crates : ";
      if (_crate_labels_.size() > 0) {
        out_ << _crate_labels_.size();
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      int count_width = 0;
      for (int islot = 0; islot < _max_number_of_crates_; islot++) {
        out_ << indent_ << i_tree_dumpable::inherit_skip_tag(inherit_);
        out_ << i_tree_dumpable::last_skip_tag;
        if (islot == _max_number_of_crates_ - 1) {
          out_ << i_tree_dumpable::last_tag;
        } else {
          out_ << i_tree_dumpable::tag;
        }
        out_ << "Slot #" << islot << " : ";
        indexed_labels_dict_type::const_iterator found_at_slot = _crate_labels_.find(islot);
        if (found_at_slot != _crate_labels_.end()) {
          const base_electronics_crate_model & crate = dynamic_cast<const base_electronics_crate_model &>(get_embedded_device(found_at_slot->second).get_logical().get_model());
          out_ << "Model '" << crate.get_name() << "'";
          count_width = 1; // crate.get_slot_width();
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
