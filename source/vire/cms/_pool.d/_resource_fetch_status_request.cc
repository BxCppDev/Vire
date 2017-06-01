// vire/cms/resource_fetch_status_request.cc
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
#include <vire/cms/resource_fetch_status_request.h>

// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_string_converter.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ResourceFetchStatusRequest.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::resource_fetch_status_request",
                               vire::cms::ResourceFetchStatusRequest)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(resource_fetch_status_request,
                                        "vire::cms::resource_fetch_status_request")

    resource_fetch_status_request::resource_fetch_status_request()
    {
      return;
    }

    resource_fetch_status_request::resource_fetch_status_request(const std::string & path_)
      : resource_base_request(path_)
    {
      return;
    }

    resource_fetch_status_request::~resource_fetch_status_request()
    {
      return;
    }

    void resource_fetch_status_request::jsonize(jsontools::node & node_,
                                                const unsigned long int /* version_ */)
    {
      this->vire::cms::resource_base_request::jsonize(node_);
      return;
    }

    void resource_fetch_status_request::protobufize(protobuftools::message_node & node_,
                                                    const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::cms::resource_base_request,node_);
      return;
    }

    // void resource_fetch_status_request::tree_dump(std::ostream & out_,
    //                                               const std::string & title_,
    //                                               const std::string & indent_,
    //                                               bool inherit_) const
    // {
    //   this->base_request::tree_dump(out_, title_, indent_, true);
    //   out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
    //        << "Resource path : ";
    //   if (has_path()) {
    //     out_ << "'" << _path_ << "'";
    //   } else {
    //     out_ << "<none>";
    //   }
    //   out_ << std::endl;
    //   return;
    // }

  } // namespace cms

} // namespace vire
