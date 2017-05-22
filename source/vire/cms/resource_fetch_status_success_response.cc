// vire/cms/resource_fetch_status_success_response.cc
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

// Ourselves:
#include <vire/cms/resource_fetch_status_success_response.h>

// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_string_converter.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ResourceFetchStatusSuccessResponse.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::resource_fetch_status_success_response",
                               vire::cms::ResourceFetchStatusSuccessResponse)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(resource_fetch_status_success_response,
                                        "vire::cms::resource_fetch_status_success_response")

    resource_fetch_status_success_response::resource_fetch_status_success_response()
    {
      return;
    }

    resource_fetch_status_success_response::resource_fetch_status_success_response(const resource_status_record & status_)
      : resource_base_response(status_)
    {
      return;
    }

    resource_fetch_status_success_response::~resource_fetch_status_success_response()
    {
      return;
    }

    void resource_fetch_status_success_response::jsonize(jsontools::node & node_,
                                                         const unsigned long int /* version_ */)
    {
      this->resource_base_response::jsonize(node_);
      return;
    }

    void resource_fetch_status_success_response::protobufize(protobuftools::message_node & node_,
                                                             const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(resource_base_response, node_);
      return;
    }

  } // namespace cms

} // namespace vire
