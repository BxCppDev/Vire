// vire/cmslapp/pubsub_support.cc
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

// Ourselves
#include <vire/cmslapp/pubsub_support.h>

namespace vire {

  namespace cmslapp {

    // static
    const std::string & dp_pub_suffix()
    {
      static const std::string _s("__dp_subscribe__");
      return _s;
    }

    int convert(const vire::cms::resource_pubsub & pubsub_,
                vire::cms::resource_exec & exec_)
    {
      std::string dp_sub_method_path = pubsub_.get_path() + "/" +  dp_pub_suffix();
      exec_.reset();
      exec_.set_path(dp_sub_method_path);
      bool active = true;
      if (pubsub_.get_action() == vire::cms::resource_pubsub::ACTION_SUBSCRIBE) {
        active = true;
      } else if (pubsub_.get_action() == vire::cms::resource_pubsub::ACTION_UNSUBSCRIBE) {
        active = false;
      } {
        return 0;
      }
      std::string active_repr;
      if (active) active_repr="true";
      else active_repr="false";
      exec_.add_input_argument("subscription", active_repr);
      return 0;
    }

  } // namespace cmslapp

} // namespace vire
