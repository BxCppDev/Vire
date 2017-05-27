//! \file  vire/message/protocol_utils.h
//! \brief Protocol utilities
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_MESSAGE_PROTOCOL_UTILS_H
#define VIRE_MESSAGE_PROTOCOL_UTILS_H

// Standard library:
#include <string>

namespace vire {

  namespace message {

    /// \brief Category of protocol message
    enum message_category {
      MESSAGE_INVALID  = 0, ///< Invalid message category
      MESSAGE_REQUEST  = 1, ///< Request RPC/Service message
      MESSAGE_RESPONSE = 2, ///< Response RPC/Service message
      MESSAGE_EVENT    = 3  ///< Event message
    };

    std::string message_category_label(const message_category cat_);

  } // namespace message

} // namespace vire

#endif // VIRE_MESSAGE_PROTOCOL_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
