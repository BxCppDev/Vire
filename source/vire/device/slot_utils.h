//! \file  vire/device/slot_utils.h
//! \brief Various slot utilities
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_DEVICE_SLOT_UTILS_H
#define VIRE_DEVICE_SLOT_UTILS_H

// Standard library:
#include <vector>

// This project:
#include <vire/device/slot.h>

namespace vire {

  namespace device {

    typedef std::vector<slot> slot_path_type;

  } // namespace device

} // namespace vire


#endif // VIRE_DEVICE_SLOT_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
