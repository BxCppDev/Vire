//! \file  vire/device/utils.h
//! \brief Vire device utilities
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

#ifndef VIRE_DEVICE_UTILS_H
#define VIRE_DEVICE_UTILS_H

// Standard Library:
#include <string>

namespace vire {

  //! Nested namespace for the Vire library's device module
  namespace device {

    //! Return the key of the property for device class identifier
    const std::string & class_id_key();

    //! Return the key of the property for device role name
    const std::string & local_role_name_key();

    //! Return the key of the property for device role tags
    const std::string & local_role_tags_key();

    //! Return the name of the expert device role
    const std::string & local_role_expert();

    // //! Return the name of the control device role
    // const std::string & local_role_control();

    // //! Return the name of the monitor device role
    // const std::string & local_role_monitor();

  } // namespace device

} // namespace vire

#endif // VIRE_DEVICE_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
