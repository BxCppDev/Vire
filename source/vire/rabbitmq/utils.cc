//! \file  vire/rabbitmq/utils.h
//! \brief Vire CMS RabbitMQ utilities
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/rabbitmq/utils.h>

// This project:
#include <vire/utility/path.h>

namespace vire {

  namespace rabbitmq {

    bool convert(const vire::com::address & addr_, std::string & key_)
    {
      key_.clear();
      
      if (addr_.is_protocol() || addr_.is_private()) {
        key_ = addr_.get_value();
        return true;
      }
      
      if (addr_.is_device() || addr_.is_resource()) {
        vire::utility::path::to_address(addr_.get_value(), key_);
        return true;
      }
       
      return false;
    }

  
 } // namespace rabbitmq

} // namespace vire
