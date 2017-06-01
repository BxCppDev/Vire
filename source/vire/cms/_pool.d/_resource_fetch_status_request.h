//! \file  vire/cms/resource_fetch_status_request.h
//! \brief Resource fetch status request
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

#ifndef VIRE_CMS_RESOURCE_FETCH_STATUS_REQUEST_H
#define VIRE_CMS_RESOURCE_FETCH_STATUS_REQUEST_H

// Standard Library:
#include <string>

// Third party:
#include <boost/variant.hpp>

// This project:
#include <vire/cms/resource_base_request.h>

namespace vire {

  namespace cms {

    /// \brief Resource fetch status request
    class resource_fetch_status_request
      : public resource_base_request
    {
    public:

      // Default constructor
      resource_fetch_status_request();

      /// Constructor
      resource_fetch_status_request(const std::string & path_);

      /// Destructor
      virtual ~resource_fetch_status_request();

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      // //! Smart print
      // virtual void tree_dump(std::ostream & out_ = std::clog,
      //                        const std::string & title_  = "",
      //                        const std::string & indent_ = "",
      //                        bool inherit_ = false) const;

    private:

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_fetch_status_request)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_fetch_status_request,
                                                  "vire::cms::resource_fetch_status_request")

#endif // VIRE_CMS_RESOURCE_FETCH_STATUS_REQUEST_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
