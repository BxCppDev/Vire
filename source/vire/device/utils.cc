// vire/device/utils.cc - Implementation of Vire device utilities
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/device/utils.h>

namespace vire {

  namespace device {

    const std::string & class_id_key()
    {
      static std::string _k("vire.device.class_id");
      return _k;
    }

    const std::string & local_role_name_key()
    {
      static std::string _k("vire.device.local_role.name");
      return _k;
    }

    const std::string & local_role_tags_key()
    {
      static std::string _k("vire.device.local_role.tags");
      return _k;
    }

    const std::string & local_role_expert()
    {
      static std::string _k("expert");
      return _k;
    }

  } // namespace resource

} // namespace vire
