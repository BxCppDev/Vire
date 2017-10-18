//! \file  vire/cms/resource_base_request.h
//! \brief Resource base request
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

#ifndef VIRE_CMS_RESOURCE_BASE_REQUEST_H
#define VIRE_CMS_RESOURCE_BASE_REQUEST_H

// Standard Library:
#include <string>

// This project:
#include <vire/utility/base_request.h>

namespace vire {

  namespace cms {

    /// \brief Request for a resource baseution
    class resource_base_request
      : public ::vire::utility::payload
    {
    public:

      /// Constructor
      resource_base_request();

      /// Constructor
      resource_base_request(const std::string & path_);

      /// Destructor
      virtual ~resource_base_request();

      /// Check if path is set
      bool has_path() const;

      /// Set the path
      void set_path(const std::string & path_);

      /// Return the path
      const std::string & get_path() const;

      /// Reset the path
      void reset_path();

      /// Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_base_request)

    private:

      std::string _path_; ///< Resource path

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_base_request,
                                                  "vire::cms::resource_base_request")

#endif // VIRE_CMS_RESOURCE_BASE_REQUEST_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --