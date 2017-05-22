//! \file  vire/cmslapp/disconnection_request.h
//! \brief Connection request to the CMS server by the Vire server
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

#ifndef VIRE_CMSLAPP_DISCONNECTION_REQUEST_H
#define VIRE_CMSLAPP_DISCONNECTION_REQUEST_H

// Standard library:
#include <string>
#include <vector>

// Ourselves:
#include <vire/utility/base_request.h>
#include <vire/utility/base_response.h>

namespace vire {

  namespace cmslapp {

    /// \brief Disconnection request from the Vire server to the CMS server
    ///
    /// Type id: "vire::cmslapp::disconnection_request", version "1.0"
    ///
    /// @code JSON
    /// {
    /// }
    /// @encode
    class disconnection_request : public vire::utility::base_request
    {
    public:
      /// Default constructor
      disconnection_request();

      /// Destructor
      virtual ~disconnection_request();

    };

  } // namespace cmslapp

} // namespace vire

#endif // VIRE_CMSLAPP_DISCONNECTION_REQUEST_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
