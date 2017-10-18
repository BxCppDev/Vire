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
#include <vector>

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
    class unknown_resources_error
      : public ::vire::utility::base_error
    {
    public:

      static const int32_t EC_UNKNOWN = ::vire::utility::base_error::EC_MAXIMUM_SYSTEM + 1;

      /// Default constructor
      unknown_resources_error();

      /// Constructor with error code and message
      unknown_resources_error(const std::string & unknown_path_);

      /// Destructor
      virtual ~unknown_resources_error();

      /// Check if a path is unknown
      bool has_unknown_path(const std::string & path_) const;

      /// Add a unknown resource path in the list
      void add(const std::string & unknown_path_);

      /// Return the list of unknown paths
      const std::vector<std::string> & get_unknown_paths() const;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

    private:

      std::vector<std::string> _unknown_paths_; ///< List of unknown resource paths

      VIRE_UTILITY_PAYLOAD_INTERFACE(unknown_resources_error)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::unknown_resources_error, "vire::cms::unknown_resources_error")

#endif // VIRE_CMS_UNKNOWN_RESOURCES_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
