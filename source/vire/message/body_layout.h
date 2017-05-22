//! \file  vire/message/body_layout.h
//! \brief Message header
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

#ifndef VIRE_MESSAGE_BODY_LAYOUT_H
#define VIRE_MESSAGE_BODY_LAYOUT_H

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <datatools/version_id.h>

namespace vire {

  namespace message {

    /// \brief Basic Vire message body layout
    class body_layout
    {
    public:

      /// Return the name of the message format
      static const std::string & name();

      /// Return the current version identifier of the message format
      static const datatools::version_id & current_version();

    };

  } // namespace message

} // namespace vire

#endif // VIRE_MESSAGE_BODY_LAYOUT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
