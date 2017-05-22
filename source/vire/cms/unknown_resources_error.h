//! \file  vire/cms/unknown_resources_error.h
//! \brief Error occuring when unknown resources are met
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMS_UNKNOWN_RESOURCES_ERROR_H
#define VIRE_CMS_UNKNOWN_RESOURCES_ERROR_H

// Standard library:
#include <string>
#include <set>

// This project:
#include <vire/utility/base_error.h>

namespace vire {

  namespace cms {

    /// \brief Error occuring in case of unrecognized resource(s)
    ///
    /// Type id: "unknown_resources_error", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "code" : "100",
    ///   "message" : "Some requested resources are not known by the CMS server",
    ///   "unknown_paths" : [
    ///     "SuperNEMO://Demonstrator/CMS/NotExistingDevice/start",
    ///     "SuperNEMO://Demonstrator/CMS/NotExistingDevice/stop"
    ///   ]
    /// }
    /// @encode
    class unknown_resources_error : public vire::utility::base_error
    {
    public:

      /// Default constructor
      unknown_resources_error();

      /// Constructor with error code
      unknown_resources_error(int32_t code_);

      /// Constructor with error code and message
      unknown_resources_error(int32_t code_, const std::string & message_);

      /// Destructor
      virtual ~unknown_resources_error();

      /// Add a unknown resource path in the list
      void add(const std::string & unknown_path_);

      /// Return the list of unknown paths
      const std::set<std::string> & get_unknown_paths() const;

    private:

      std::set<std::string> _unknown_paths_; ///< List of unknown resource paths

    };

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_UNKNOWN_RESOURCES_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
