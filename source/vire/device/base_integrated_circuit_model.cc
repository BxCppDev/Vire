//! \file vire/device/base_integrated_circuit_model.cc
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
#include <vire/device/base_integrated_circuit_model.h>

// This project:
#include <vire/device/device_types.h>
#include <vire/device/physical_device.h>
#include <vire/device/logical_device.h>

namespace vire {

  namespace device {

    VIRE_DEVICE_REGISTRATION_IMPLEMENT(base_integrated_circuit_model,
                                       "vire::device::base_integrated_circuit_model");

    void base_integrated_circuit_model::_set_defaults()
    {
      set_type(TYPE_CHIP);
      set_allow_embedded_devices(true);
      return;
    }

    base_integrated_circuit_model::base_integrated_circuit_model()
    {
      _set_defaults();
      return;
    }

    base_integrated_circuit_model::~base_integrated_circuit_model()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void base_integrated_circuit_model::_at_initialize(const datatools::properties& config_,
                                                       model_repository & models_)
    {
      this->_chip_initialize(config_, models_);
      return;
    }

    void base_integrated_circuit_model::_at_reset()
    {
      this->_chip_reset();
      return;
    }

    void base_integrated_circuit_model::_chip_initialize(const datatools::properties & config_,
                                                         model_repository & models_)
    {
      return;
    }

    void base_integrated_circuit_model::_chip_reset()
    {
      return;
    }

    void base_integrated_circuit_model::tree_dump(std::ostream& out_,
                                                 const std::string& title_,
                                                 const std::string& indent_,
                                                 bool inherits_) const
    {
      this->base_device_model::tree_dump(out_, title_, indent_, false);

      return;
    }

  } // end of namespace device

} // end of namespace vire
