//! \file vire/com/transport_utils.cc
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

// Ourselves:
#include <vire/com/transport_utils.h>

// Standard library:
#include <string>
#include <memory>

namespace vire {

  namespace com {

    // static
    const std::set<vire::utility::model_identifier> & transport::supported_transport_systems()
    {
      static std::set<vire::utility::model_identifier> _s;
      if (_s.size() == 0) {
        {
          vire::utility::model_identifier rabbitmq_id;
          rabbitmq_id.set_name("rabbitmq");
          _s.insert(rabbitmq_id);
        }
      }
      return _s;
    }

  } // namespace com

} // namespace vire
