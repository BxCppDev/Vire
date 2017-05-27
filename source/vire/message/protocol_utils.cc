// vire/message/protocol_utils.cc
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

// Ourselves:
#include <vire/message/protocol_utils.h>

namespace vire {

  namespace message {

    std::string message_category_label(const message_category cat_)
    {
      switch (cat_) {
      case MESSAGE_REQUEST  : return std::string("request");
      case MESSAGE_RESPONSE : return std::string("response");
      case MESSAGE_EVENT    : return std::string("event");
      default:
        return std::string("");
      }
    }

  } // namespace message

} // namespace vire
