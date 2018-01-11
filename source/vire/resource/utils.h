//! \file    vire/resource/utils.h
//! \brief   Vire resource utilities
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

#ifndef VIRE_RESOURCE_UTILS_H
#define VIRE_RESOURCE_UTILS_H

// Standard Library
#include <string>

// #ifndef Q_MOC_RUN
// #include <datatools/reflection_interface.h>
// #endif // Q_MOC_RUN

namespace vire {

  //! Nested namespace for the Vire library's resource module
  namespace resource {

    //! \brief Access mode for a given resource
    enum access_type {
      ACCESS_INVALID    = -1, //!< Invalid access
      ACCESS_NONE       =  0, //!< No access
      ACCESS_READ_ONLY  =  1, //!< Read-only access
      ACCESS_WRITE_ONLY =  2, //!< Write-only access
      ACCESS_READ_WRITE =  3  //!< Read-write access
    };

    //! Return the label associated to a given access mode
    std::string access_to_label(access_type);

    //! Return the access mode associated to a given label
    access_type label_to_access(const std::string &);

    //! \brief Resource type
    enum resource_category_type {
      RESOURCE_CATEGORY_INVALID   = 0, //!< Invalid resource type
      RESOURCE_CATEGORY_DATAPOINT = 1, //!< Datapoint
      RESOURCE_CATEGORY_METHOD    = 2, //!< Method
      RESOURCE_CATEGORY_USER      = 3  //!< User
    };

    //! Return the label associated to a given resource category
    std::string resource_category_to_label(resource_category_type);

    //! Return the resource category associated to a given label
    resource_category_type label_to_resource_category(const std::string &);

  } // namespace resource

} // namespace vire

#endif // VIRE_RESOURCE_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
