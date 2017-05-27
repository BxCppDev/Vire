// vire/utility/invalid_context_error.cc
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
#include <vire/utility/invalid_context_error.h>

// - BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>

// Declare a protobuf registrar instance for the message class:
// #include <vire/base_object_protobuf.h>
#include "vire/utility/InvalidContextError.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::invalid_context_error_payload",
                               vire::utility::InvalidContextError)

namespace vire {

  namespace utility {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(invalid_context_error,
                                        "vire::utility::invalid_context_error")

    invalid_context_error::invalid_context_error()
      : invalid_context_error(base_error::EC_GENERIC_FAILURE, "")
    {
      return;
    }

    invalid_context_error::invalid_context_error(const int32_t code_)
      : invalid_context_error(code_, "")
    {
      return;
    }

    invalid_context_error::invalid_context_error(const int32_t code_, const std::string & message_)
      : base_error(code_, message_)
    {
      return;
    }

    invalid_context_error::~invalid_context_error()
    {
      return;
    }

    void invalid_context_error::jsonize(jsontools::node & node_,
                                        const unsigned long int version_)
    {
      this->base_error::jsonize(node_);
      return;
    }

    void invalid_context_error::protobufize(protobuftools::message_node & node_,
                                            const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_error,node_);
      return;
    }

  } // namespace utility

} // namespace vire
