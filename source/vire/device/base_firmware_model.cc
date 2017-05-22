//! \file vire/device/base_firmware_model.cc
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
#include <vire/device/base_firmware_model.h>

// This project:
#include <vire/device/device_types.h>
#include <vire/device/physical_device.h>
#include <vire/device/logical_device.h>

namespace vire {

  namespace device {

    VIRE_DEVICE_REGISTRATION_IMPLEMENT(base_firmware_model,
                                       "vire::device::base_firmware_model");

    void base_firmware_model::_set_defaults()
    {
      set_type(TYPE_FIRMWARE);
      set_allow_embedded_devices(true);
      return;
    }

    bool base_firmware_model::has_version() const
    {
      return ! _version_.empty();
    }

    const std::string & base_firmware_model::get_version() const
    {
      return _version_;
    }

    void base_firmware_model::set_version(const std::string & version_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Firmware model '" << get_name() << "' is already initialized !");
      _version_ = version_;
      return;
    }

    base_firmware_model::base_firmware_model()
    {
      _set_defaults();
      return;
    }

    base_firmware_model::~base_firmware_model()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void base_firmware_model::_at_initialize(const datatools::properties& config_,
                                             model_repository & models_)
    {
      this->_firmware_initialize(config_, models_);
      return;
    }

    void base_firmware_model::_at_reset()
    {
      this->_firmware_reset();
      return;
    }

    void base_firmware_model::_firmware_initialize(const datatools::properties & config_,
                                                   model_repository & models_)
    {
      return;
    }

    void base_firmware_model::_firmware_reset()
    {
      _version_.clear();
      return;
    }

    void base_firmware_model::tree_dump(std::ostream& out_,
                                        const std::string& title_,
                                        const std::string& indent_,
                                        bool inherits_) const
    {
      this->base_device_model::tree_dump(out_, title_, indent_, false);

      return;
    }

  } // end of namespace device

} // end of namespace vire
