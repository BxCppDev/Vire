//! \file  vire/device/base_integrated_circuit_model.h
//! \brief Base class for an integrated circuit model
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRE_DEVICE_BASE_INTEGRATED_CIRCUIT_MODEL_H
#define VIRE_DEVICE_BASE_INTEGRATED_CIRCUIT_MODEL_H

// Standard library:
#include <iostream>
#include <string>

// This project:
#include <vire/device/base_device_model.h>

namespace vire {

  namespace device {

    //! \brief Base class for integrated circuit  model
    class base_integrated_circuit_model : public base_device_model
    {
    public:

      //! Default constructor
      base_integrated_circuit_model();

      //! Destructor
      virtual ~base_integrated_circuit_model();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    protected:

      //! Overloaded initialization method
      virtual void _at_initialize(const datatools::properties & config_,
                                  model_repository & models_);

      //! Overloaded reset method
      virtual void _at_reset();

      //! Basic initialization of the chip model
      void _chip_initialize(const datatools::properties & config_,
                            model_repository & models_);

      //! Common termination of the chip model
      void _chip_reset();

      //! Set default attribute values
      void _set_defaults();

    private:

      //! Registration of a system factory
      VIRE_DEVICE_REGISTRATION_INTERFACE(base_integrated_circuit_model);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace device

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::base_integrated_circuit_model' class:
DR_CLASS_INIT(vire::device::base_integrated_circuit_model);
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_BASE_INTEGRATED_CIRCUIT_MODEL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
