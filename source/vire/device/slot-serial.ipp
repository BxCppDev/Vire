//! \file    vire/device/slot-serial.ipp
//! \brief   Vire device slot serialization support
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

#ifndef VIRE_DEVICE_SLOT_SERIAL_IPP
#define VIRE_DEVICE_SLOT_SERIAL_IPP

// Ourselves:
#include <vire/device/slot.h>

// Third party:
// - Boost:
// - Bayeux/datatools:

namespace vire {

  namespace device {

    template<class Archive>
    void slot::serialize(Archive & archive_,
                         const unsigned int /* version_*/)
    {
      archive_ & DATATOOLS_SERIALIZATION_I_SERIALIZABLE_BASE_OBJECT_NVP;

      archive_ & boost::serialization::make_nvp("dimension", _dimension_);
      archive_ & boost::serialization::make_nvp("coordinates", _coordinates_);
      return;
    }

  } // end of namespace device

} // end of namespace vire

// #include <boost/serialization/version.hpp>
// BOOST_CLASS_VERSION(vire::device::slot, 0)

#endif // VIRE_DEVICE_SLOT_SERIAL_IPP

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
