// vire/cms/unknown_resources_error.cc
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
#include <vire/cms/unknown_resources_error.h>

// Third party
// - BxJsontools:
#include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cms/UnknownResourcesError.pb.h"

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(unknown_resources_error,
                                        "vire::cms::unknown_resources_error");

    unknown_resources_error::unknown_resources_error()
      : ::vire::utility::base_error(::vire::utility::base_error::EC_GENERIC_FAILURE, "")
    {
      return;
    }

    unknown_resources_error::unknown_resources_error(int32_t code_)
      : ::vire::utility::base_error(code_, "")
    {
      return;
    }

    unknown_resources_error::unknown_resources_error(int32_t code_,
                                                     const std::string & message_)
      : ::vire::utility::base_error(code_, message_)
    {
      return;
    }

    unknown_resources_error::~unknown_resources_error()
    {
      return;
    }

    void
    unknown_resources_error::add(const std::string & unknown_path_)
    {
      _unknown_paths_.insert(unknown_path_);
      return;
    }

    const std::set<std::string> &
    unknown_resources_error::get_unknown_paths() const
    {
      return _unknown_paths_;
    }

    void unknown_resources_error::jsonize(jsontools::node & node_,
                                     const unsigned long int /* version_ */)
    {
      this->::vire::utility::base_error::jsonize(node_);
      node_["unknown_paths"] % _unknown_paths_;
      return;
    }

    void invalid_user_error::protobufize(protobuftools::message_node & node_,
                                         const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(::vire::utility::base_error,node_);
      node_["unknown_paths"] % _unknown_paths_;
      return;
    }

  } // namespace cms

} // namespace vire
