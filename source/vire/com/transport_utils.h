//! \file  vire/com/transport_utils.h
//! \brief Vire com transport utilities
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_TRANSPORT_UTILS_H
#define VIRE_COM_TRANSPORT_UTILS_H

// Standard library:
#include <memory>
#include <set>

// This project:
#include <vire/utility/model_identifier.h>

namespace vire {

  namespace com {

    struct transport
    {
      static const std::set<vire::utility::model_identifier> & supported_transport_systems();
    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_TRANSPORT_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
