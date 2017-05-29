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

// Third party:
// - BxJsontools:
#include <json/json.h>
#include <jsontools/exception.h>
#include <jsontools/core.h>
#include <jsontools/node.h>

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

namespace jsontools {

  /// \brief Converter class
  template<>
  class converter<vire::message::message_category>
  {
  public:

    // Generic serialization method for enumeration value
    static void jsonize(node & node_,
                        vire::message::message_category & enum_value_)
    {
      int32_t val = static_cast<int32_t>(enum_value_);
      node_.grab_value() = val;
      return;
    }

    // Generic deserialization method for enumeration value
    static void dejsonize(node & node_,
                          vire::message::message_category & enum_value_)
    {
      if (not node_.get_value().isInt()) {
        throw wrong_type(node_.get_value(), "expected int32_t");
      }
      int32_t val = node_.get_value().asInt();
      enum_value_ = static_cast<vire::message::message_category>(val);
      return;
    }

  };

} // namespace jsontools

#endif // VIRE_MESSAGE_PROTOCOL_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
