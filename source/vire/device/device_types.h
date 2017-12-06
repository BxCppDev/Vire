//! \file  vire/device/device_types.h
//! \brief Vire device types
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

#ifndef VIRE_DEVICE_DEVICE_TYPES_H
#define VIRE_DEVICE_DEVICE_TYPES_H

// Standard library
#include <string>

// Third party
// - Bayeux/datatools
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

namespace vire {

  namespace device {

    //! \brief Device type identifier
    enum device_type {
      TYPE_UNDEFINED              =   -1, //!< Undefined device type
      TYPE_GENERIC                =    0, //!< Generic device
      TYPE_SETUP                  =    1, //!< Generic setup
      TYPE_RACK                   = 1100, //!< Generic rack
      TYPE_RACKABLE               = 1150, //!< Generic rackable
      TYPE_CRATE                  = 1200, //!< Generic crate
      TYPE_MODULE                 = 1300, //!< Generic module
      TYPE_MODULE_BOARD           = 1310, //!< Generic board
      TYPE_MODULE_MEZZANINE_BOARD = 1320, //!< Generic mezzanine board
      TYPE_CHIP                   = 1400, //!< Generic chip
      TYPE_CHIP_PLD               = 1410, //!< Generic Programmable Logic Device (FPGA)
      TYPE_CHIP_ASIC              = 1420, //!< Generic Application-Specific Integrated Circuit (ASIC)
      TYPE_CHIP_EPROM             = 1430, //!< Generic e-PROM device
      TYPE_WIRE                   = 4000, //!< Generic wire
      TYPE_CABLE                  = 4100, //!< Generic cable
      TYPE_FLAT_RIBBON_CABLE      = 4200, //!< Generic flat ribbon cable
      TYPE_CABLE_STRAND           = 4300, //!< Generic cable strand
      TYPE_FIRMWARE               = 10000, //!< Generic firmware
      TYPE_SOFTWARE               = 20000, //!< Generic software
      TYPE_DATAPOINT              = 30000, //!< Generic datapoint
      TYPE_REGISTER               = 50000, //!< Generic register
    };

    //! Return the device type associated to a device label
    device_type get_device_type(const std::string & label_);

    //! Return the device label associated to a device type
    std::string get_device_type_label(device_type);

  } // end of namespace device

} // end of namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::device::device_type' enum:
DR_TYPE_INIT(::vire::device::device_type)
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_DEVICE_TYPES_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
