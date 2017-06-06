//! \file  vire/cmslapp/disconnection_success_response.h
//! \brief Disconnection success response from the CMS server to the Vire server
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

#ifndef VIRE_CMSLAPP_DISCONNECTION_SUCCESS_RESPONSE_H
#define VIRE_CMSLAPP_DISCONNECTION_SUCCESS_RESPONSE_H

// Standard library:
#include <string>
#include <vector>

// Ourselves:
#include <vire/utility/base_request.h>
#include <vire/utility/base_response.h>

namespace vire {

  namespace cmslapp {

    /// \brief Disconnection request success response from the CMS server to the Vire server
    ///
    /// Type id: "vire::cmslapp::disconnection_success_response", version "1.0"
    ///
    /// @code JSON
    /// {
    /// }
    class disconnection_success_response : public vire::utility::base_response
    {
    public:

      /// Default constructor
      disconnection_success_response();

      /// Destructor
      virtual ~disconnection_success_response();

    };

  } // namespace cmslapp

} // namespace vire

#endif // VIRE_CMSLAPP_DISCONNECTION_SUCCESS_RESPONSE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
