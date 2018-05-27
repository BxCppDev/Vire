//! \file  vire/cmsserver/error.h
//! \brief Vire CMS error handling
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

#ifndef VIRE_CMSSERVER_ERROR_H
#define VIRE_CMSSERVER_ERROR_H

// Standard Library:
#include <exception>

// - Boost:
#include <boost/property_tree/ptree.hpp>

namespace vire {

  namespace cmsserver {

      struct base_exception
        : public std::exception
      {
        virtual void export_error_data(boost::property_tree::ptree & error_data_) const = 0;
      };

      /*
      struct resource_exception : public base_exception
      {
        virtual void export(boost::property_tree::ptree & error_data_) const
        {
          error_data_.put("resource_path", resource_path);
          error_data_.put("invalid_value", invalid_value);
        }

        std::string resource_path;
        int invalid_value;
      };
      */

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
