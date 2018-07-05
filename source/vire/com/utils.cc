//! \file vire/com/utils.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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

// Ourselves:
#include <vire/com/utils.h>

namespace vire {

  namespace com {

    void raw_message_type::reset()
    {
      buffer.clear();
      metadata.clear();
      return;
    }

    std::string to_string(const plug_category_type cat_)
    {
      switch(cat_) {
      case PLUG_EVENT_EMITTER: return std::string("event_emitter");
      case PLUG_EVENT_LISTENER: return std::string("event_listener");
      case PLUG_SERVICE_CLIENT: return std::string("service_client");
      case PLUG_SERVICE_SERVER: return std::string("service_server");
      default: return std::string();
      }
    }

    const std::string & message_id_key()
    {
      static const std::string _k("message_id");
      return _k;
    }
    
    const std::string & correlation_id_key()
    {
      static const std::string _k("correlation_id");
      return _k;
    }
    
    const std::string & address_key()
    {
      static const std::string _k("address");
      return _k;
    }

    const std::string & system_connection_key()
    {
      static const std::string _k("connection");
      return _k;
    }

  } // namespace com

} // namespace vire
