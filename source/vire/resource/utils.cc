// vire/resource/utils.cc - Implementation of Vire resource utilities
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
#include <vire/resource/utils.h>

namespace vire {

  namespace resource {

    std::string access_to_label(access_type a_)
    {
      switch (a_) {
      case ACCESS_INVALID    : return "invalid";
      case ACCESS_READ_ONLY  : return "ro";
      case ACCESS_WRITE_ONLY : return "wo";
      case ACCESS_READ_WRITE : return "rw";
      }
    }

    access_type label_to_access(const std::string & l_)
    {
      if (l_ == access_to_label(ACCESS_READ_ONLY)) return ACCESS_READ_ONLY;
      if (l_ == access_to_label(ACCESS_WRITE_ONLY)) return ACCESS_WRITE_ONLY;
      if (l_ == access_to_label(ACCESS_READ_WRITE)) return ACCESS_READ_WRITE;
      return ACCESS_INVALID;
    }

    std::string resource_category_to_label(resource_category_type rc_)
    {
      switch (rc_) {
      case RESOURCE_CATEGORY_INVALID   : return "invalid";
      case RESOURCE_CATEGORY_DATAPOINT : return "datapoint";
      case RESOURCE_CATEGORY_METHOD    : return "method";
      case RESOURCE_CATEGORY_USER      : return "user";
      }
    }

    resource_category_type label_to_resource_category(const std::string & label_)
    {
      if (label_ == resource_category_to_label(RESOURCE_CATEGORY_INVALID)) return RESOURCE_CATEGORY_INVALID;
      if (label_ == resource_category_to_label(RESOURCE_CATEGORY_DATAPOINT)) return RESOURCE_CATEGORY_DATAPOINT;
      if (label_ == resource_category_to_label(RESOURCE_CATEGORY_METHOD)) return RESOURCE_CATEGORY_METHOD;
      if (label_ == resource_category_to_label(RESOURCE_CATEGORY_USER)) return RESOURCE_CATEGORY_USER;
      return RESOURCE_CATEGORY_INVALID;
    }

  } // namespace resource

} // namespace vire
