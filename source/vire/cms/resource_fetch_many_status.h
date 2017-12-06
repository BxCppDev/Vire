//! \file  vire/cms/resource_fetch_many_status.h
//! \brief Many resources fetch status
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

#ifndef VIRE_CMS_RESOURCE_FETCH_MANY_STATUS_H
#define VIRE_CMS_RESOURCE_FETCH_MANY_STATUS_H

// Standard Library:
#include <string>
#include <vector>

// Third party:
#include <boost/variant.hpp>

// This project:
#include <vire/utility/base_payload.h>

namespace vire {

  namespace cms {

    /// \brief Many resources fetch status command
    class resource_fetch_many_status
      : public ::vire::utility::base_payload
    {
    public:

      // Default constructor
      resource_fetch_many_status();

      /// Constructor
      resource_fetch_many_status(const std::string & path_);

      /// Destructor
      virtual ~resource_fetch_many_status();

      /// Check if path is set
      bool has_path(const std::string & path_) const;

      /// Set the path
      void add_path(const std::string & path_);

      /// Return the set of paths
      const std::vector<std::string> & get_paths() const;

      /// Reset the path
      void reset_paths();

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      std::vector<std::string> _paths_; ///< Set of resource paths

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_fetch_many_status)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_fetch_many_status,
                                                  "vire::cms::resource_fetch_many_status")

#endif // VIRE_CMS_RESOURCE_FETCH_MANY_STATUS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
