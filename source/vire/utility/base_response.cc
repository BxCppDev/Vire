// vire/utility/base_response.cc
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

// Ourselves:
#include <vire/utility/base_response.h>

// - BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/utility/BaseResponse.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::base_response",
                               vire::utility::BaseResponse)

namespace vire {

  namespace utility {

    base_response::base_response()
    {
      return;
    }

    base_response::~base_response()
    {
      return;
    }

    // virtual
    base_payload::payload_category base_response::get_category() const
    {
      return CATEGORY_RESPONSE;
    }

    void base_response::jsonize(jsontools::node & node_,
                                const unsigned long int version_)
    {
      base_payload::jsonize(node_, version_);
      return;
    }

    void base_response::protobufize(protobuftools::message_node & node_,
                                    const unsigned long int version_)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_payload,node_);
      return;
    }

    void base_response::tree_dump(std::ostream & out_,
                                  const std::string & title_,
                                  const std::string & indent_,
                                  bool inherit_) const
    {
      this->base_payload::tree_dump(out_, title_, indent_, inherit_);

      return;
    }

  } // namespace utility

} // namespace vire
