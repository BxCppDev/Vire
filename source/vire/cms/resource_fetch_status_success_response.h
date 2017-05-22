//! \file  vire/cms/resource_fetch_status_success_response.h
//! \brief Resource fetch status success response
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

#ifndef VIRE_CMS_RESOURCE_FETCH_STATUS_SUCCESS_RESPONSE_H
#define VIRE_CMS_RESOURCE_FETCH_STATUS_SUCCESS_RESPONSE_H

// Standard Library:
#include <string>

// Third party:
#include <boost/variant.hpp>

// This project:
#include <vire/cms/resource_base_response.h>
#include <vire/cms/resource_status_record.h>

namespace vire {

  namespace cms {

    /// \brief Resource fetch status success response
    class resource_fetch_status_success_response
      : public resource_base_response
    {
    public:

      // Default constructor
      resource_fetch_status_success_response();

      /// Constructor
      resource_fetch_status_success_response(const resource_status_record & status_);

      /// Destructor
      virtual ~resource_fetch_status_success_response();

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_fetch_status_success_response)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_fetch_status_success_response,
                                                  "vire::cms::resource_fetch_status_success_response")

#endif // VIRE_CMS_RESOURCE_FETCH_STATUS_SUCCESS_RESPONSE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
