//! \file vire/device/base_rackable_model.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/device/base_rackable_model.h>

// This project:
#include <vire/device/device_types.h>
#include <vire/device/physical_device.h>
#include <vire/device/logical_device.h>
#include <vire/device/single_instance.h>

namespace vire {

  namespace device {

    VIRE_DEVICE_REGISTRATION_IMPLEMENT(base_rackable_model,
                                       "vire::device::base_rackable_model");

    void base_rackable_model::_set_defaults()
    {
      set_type(TYPE_RACKABLE);
      set_allow_embedded_devices(true);
      _height_ = 0;
      _width_ = 0;
      return;
    }

    base_rackable_model::base_rackable_model()
    {
      _set_defaults();
      return;
    }

    base_rackable_model::~base_rackable_model()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool base_rackable_model::has_height() const
    {
      return _height_ > 0;
    }

    void base_rackable_model::set_height(uint32_t height_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Rackable model '" << get_name() << "' is already initialized !");
      _height_ = height_;
      return;
    }

    uint32_t base_rackable_model::get_height() const
    {
      return _height_;
    }

    bool base_rackable_model::has_width() const
    {
      return _width_ > 0;
    }

    void base_rackable_model::set_width(uint32_t width_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Rackable model '" << get_name() << "' is already initialized !");
      _width_ = width_;
      return;
    }

    uint32_t base_rackable_model::get_width() const
    {
      return _width_;
    }

    bool base_rackable_model::has_format() const
    {
      return ! _format_.empty();
    }

    const std::string & base_rackable_model::get_format() const
    {
      return _format_;
    }

    void base_rackable_model::set_format(const std::string & format_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Rackable model '" << get_name() << "' is already initialized !");
      _format_ = format_;
      return;
    }

    void base_rackable_model::_at_initialize(const datatools::properties& config_,
                                             model_repository & models_)
    {
      this->_rackable_initialize(config_, models_);
      return;
    }

    void base_rackable_model::_at_reset()
    {
      this->_rackable_reset();
      return;
    }

    void base_rackable_model::_rackable_reset()
    {
      _height_ = 0;
      _width_ = 0;
      _format_.clear();
      return;
    }

    void base_rackable_model::_rackable_initialize(const datatools::properties & config_,
                                                         model_repository & models_)
    {
      if (! has_format()) {
        if (config_.has_key("format")) {
          std::string cf = config_.fetch_string("format");
          set_format(cf);
        }
      }

      if (!has_height()) {
        if (config_.has_key("height")) {
          int32_t n = config_.fetch_positive_integer("height");
          DT_THROW_IF ((n < 0), std::domain_error,
                       "Invalid height '" << n << "' !");
          uint32_t height = n;
          set_height(height);
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

      return;
    }

    void base_rackable_model::tree_dump(std::ostream& out_,
                                        const std::string& title_,
                                        const std::string& indent_,
                                        bool inherit_) const
    {
      this->base_device_model::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Format : '" << _format_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Height : " << _height_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Width : " << _width_ << std::endl;

      return;
    }

  } // end of namespace device

} // end of namespace vire
