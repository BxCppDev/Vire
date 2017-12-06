//! \file vire/device/base_electronics_rack_model.cc
//
// Copyright (c) 2015-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
// #include <vire/device/base_electronics_crate_model.h>
#include <vire/device/base_rackable_model.h>
#include <vire/device/single_instance.h>

namespace vire {

  namespace device {

    VIRE_DEVICE_REGISTRATION_IMPLEMENT(base_electronics_rack_model,
                                       "vire::device::base_electronics_rack_model");

    void base_electronics_rack_model::_set_defaults()
    {
      set_type(TYPE_RACK);
      set_allow_embedded_devices(true);
      _max_number_of_rackable_ = 0;
      _width_ = 0;
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

    bool base_electronics_rack_model::has_width() const
    {
      return _width_ > 0;
    }

    void base_electronics_rack_model::set_width(uint32_t width_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Rack model '" << get_name() << "' is already initialized !");
      _width_ = width_;
      return;
    }

    uint32_t base_electronics_rack_model::get_width() const
    {
      return _width_;
    }

    void base_electronics_rack_model::set_max_number_of_rackable(uint32_t nos_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Rack model '" << get_name() << "' is already initialized !");
      DT_THROW_IF(_rackable_labels_.size() > 0,
                  std::logic_error,
                  "Rack model '" << get_name() << "' already has plugged rackable !");
      _max_number_of_rackable_ = nos_;
      return;
    }

    bool base_electronics_rack_model::has_max_number_of_rackable() const
    {
      return _max_number_of_rackable_ > 0;
    }

    uint32_t base_electronics_rack_model::get_max_number_of_rackable()
    {
      return _max_number_of_rackable_;
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

    bool base_electronics_rack_model::has_rackable(uint32_t rackable_slot_id_) const
    {
      return _rackable_labels_.find(rackable_slot_id_) != _rackable_labels_.end();
    }

    void base_electronics_rack_model::remove_rackable(uint32_t rackable_slot_id_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Rack model '" << get_name() << "' is initialized !");
      DT_THROW_IF(!has_rackable(rackable_slot_id_),
                  std::logic_error,
                  "Rack model '" << get_name() << "' does not have a rackable at slot '" << rackable_slot_id_ << "' !");
      {
        std::string rackable_label = _rackable_labels_[rackable_slot_id_];
        // Explicit unregistration of the rackable label at its slot:
        _rackable_labels_.erase(_rackable_labels_.find(rackable_slot_id_));
        this->_remove_embedded_physical(rackable_label);
      }
      return;
    }

    physical_device &
    base_electronics_rack_model::add_rackable(uint32_t rackable_slot_id_,
                                           const base_device_model & rackable_model_,
                                           const std::string & rackable_label_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Rack model '" << get_name() << "' is already initialized !");
      indexed_labels_dict_type::iterator found = _rackable_labels_.find(rackable_slot_id_);
      DT_THROW_IF(has_max_number_of_rackable() && rackable_slot_id_ >= _max_number_of_rackable_,
                  std::logic_error,
                  "Rack model '" << get_name() << "' : Invalid rackable slot '" << rackable_slot_id_ << "' !");
      const base_rackable_model * rackable = dynamic_cast<const base_rackable_model *>(&rackable_model_);
      DT_THROW_IF(rackable == 0,
                  std::logic_error,
                  "Rack model '" << get_name() << "' : Attempt to embed a non rackable device '"
                  << rackable_model_.get_name()
                  << "' at slot '" << rackable_slot_id_ << "' !");
      uint32_t rackable_height = 1;
      rackable_height = 1; // rackable->get_slot_height();
      for (int islot = rackable_slot_id_;
           islot < rackable_slot_id_ + rackable_height;
           islot++) {
        DT_THROW_IF(has_rackable(islot),
                    std::logic_error,
                    "Rack model '" << get_name() << "' already has a rackable at slot '" << islot << "' !");
      }
      if (has_format() && rackable->has_format()) {
        DT_THROW_IF(_format_ != rackable->get_format(),
                    std::logic_error,
                    "Rack model '" << get_name() << "' :  Attempt to embed a rackable with incompatible format ('"
                    << rackable->get_format()
                    << "') at slot '" << rackable_slot_id_ << "' !");
      }
      // Finally, insert the rackable as a embedded device:
      // Compute the rackable label:
      std::string rackable_label = rackable_label_;
      if (rackable_label.empty()) {
        if (_rackable_labels_.find(rackable_slot_id_) != _rackable_labels_.end()) {
          rackable_label = _rackable_labels_[rackable_slot_id_];
        } else {
          // Automated rackable label:
          std::ostringstream mod_label;
          mod_label << "rackable_" << rackable_slot_id_;
          rackable_label = mod_label.str();
        }
      }
      // Explicit registration of the rackable label at its slot:
      _rackable_labels_[rackable_slot_id_] = rackable_label;
      slot cr_slot;
      // cr_slot.make_dimension_1(rackable_slot_id_);
      cr_slot.make_dimension_0(true);
      return this->_add_embedded_physical(rackable_label,
                                          rackable_model_.get_logical(),
                                          new single_instance(cr_slot),
                                          true);
    }

    const physical_device &
    base_electronics_rack_model::get_rackable(uint32_t rackable_slot_id_)
    {
      indexed_labels_dict_type::iterator found = _rackable_labels_.find(rackable_slot_id_);
      DT_THROW_IF(found == _rackable_labels_.end(),
                  std::logic_error,
                  "Rack model '" << get_name() << "' has no embedded rackable at slot '" << rackable_slot_id_ << "' !");
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
      _rackable_labels_.clear();
      _max_number_of_rackable_ = 0;
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

      if (!has_width()) {
        if (config_.has_key("width")) {
          int32_t n = config_.fetch_positive_integer("width");
          DT_THROW_IF ((n < 0), std::domain_error,
                       "Invalid width '" << n << "' !");
          uint32_t width = n;
          set_width(width);
        }
      }

      if (!has_max_number_of_rackable()) {
        if (config_.has_key("max_number_of_rackable")) {
          int32_t n = config_.fetch_integer("max_number_of_rackable");
          DT_THROW_IF ((n < 1 || n > 64), std::domain_error,
                       "Invalid number of rackable '" << n << "' !");
          uint32_t mnom = n;
          set_max_number_of_rackable(mnom);
        }
      }

      if (has_max_number_of_rackable()) {
        for (int islot = 0; islot < _max_number_of_rackable_; islot++) {
          // Attempt to find a rackable model at the visited slot:
          std::ostringstream rackable_slot_model_key;
          rackable_slot_model_key << "rackable.slot_" << islot << ".model" ;
          if (config_.has_key(rackable_slot_model_key.str())) {
            std::string rackable_model_name = config_.fetch_string(rackable_slot_model_key.str());
            DT_THROW_IF(!models_.has_device_model(rackable_model_name),
                        std::logic_error,
                        "Rack model '" << get_name() << "' : "
                        << "Cannot find a rackable model named '" << rackable_model_name << "' for slot [" << islot << "]!");
            // Fetch the initialized rackable model:
            const base_device_model & rackable_model = models_.get_device_model(rackable_model_name);
            std::ostringstream rackable_slot_label_key;
            rackable_slot_label_key << "rackable.slot_" << islot << ".label";
            std::string rackable_label;
            // Attempt to find a rackable label:
            if (config_.has_key(rackable_slot_label_key.str())) {
              rackable_label = config_.fetch_string(rackable_slot_label_key.str());
            }
            add_rackable(islot, rackable_model, rackable_label);
          }
        }
      }
      return;
    }

    /// Post remove
    void base_electronics_rack_model::_post_remove_embedded_physical(const std::string & embedded_label_)
    {
      // Explicitely unregistration of a rackable:
      indexed_labels_dict_type::iterator found
        = std::find_if(_rackable_labels_.begin(),
                       _rackable_labels_.end(),
                       std::bind2nd(map_data_compare<indexed_labels_dict_type>(),
                                    embedded_label_));
      if (found != _rackable_labels_.end()) {
        // If the removed embedded device is one of the registered rackable, it is unregistered:
        DT_LOG_TRACE(get_logging_priority(),
                     "Unregistering rackable at slot='" << found->first << "'");
        _rackable_labels_.erase(found);
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
           << "Max. number of rackable : '" << _max_number_of_rackable_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Rackable : ";
      if (_max_number_of_rackable_ == 0) {
        out_ << "<none>";
      }
      out_ << std::endl;
      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherit_)
           << i_tree_dumpable::tag
           << "Number of slots : "
           << _max_number_of_rackable_ << std::endl;

      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherit_)
           << i_tree_dumpable::tag
           << "Format : '" << _format_ << "'" << std::endl;

      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherit_)
           << i_tree_dumpable::last_tag
           << "Rackable : ";
      if (_rackable_labels_.size() > 0) {
        out_ << _rackable_labels_.size();
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      int count_width = 0;
      for (int islot = 0; islot < _max_number_of_rackable_; islot++) {
        out_ << indent_ << i_tree_dumpable::inherit_skip_tag(inherit_);
        out_ << i_tree_dumpable::last_skip_tag;
        if (islot == _max_number_of_rackable_ - 1) {
          out_ << i_tree_dumpable::last_tag;
        } else {
          out_ << i_tree_dumpable::tag;
        }
        out_ << "Slot #" << islot << " : ";
        indexed_labels_dict_type::const_iterator found_at_slot = _rackable_labels_.find(islot);
        if (found_at_slot != _rackable_labels_.end()) {
          const base_rackable_model & rackable = dynamic_cast<const base_rackable_model &>(get_embedded_device(found_at_slot->second).get_logical().get_model());
          out_ << "Model '" << rackable.get_name() << "'";
          count_width = 1; // rackable.get_slot_width();
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
