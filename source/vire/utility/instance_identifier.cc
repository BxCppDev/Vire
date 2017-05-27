// vire/utility/instance_identifier.cc
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
#include <vire/utility/instance_identifier.h>

// Standard library:
#include <sstream>

// Third party:
// - Bayeux/datatools:
#include <datatools/utils.h>
// - BxJsontools:
#include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/std_type_converters.h>
// Declare a protobuf registrar instance for the message class:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/utility/InstanceIdentifier.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::instance_identifier",
                               vire::utility::InstanceIdentifier)

namespace vire {

  namespace utility {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(instance_identifier, "vire::utility::instance_identifier")

    instance_identifier::instance_identifier()
    {
      return;
    }

    instance_identifier::instance_identifier(const std::string & name_)
    {
      set_name(name_);
      return;
    }

    instance_identifier::instance_identifier(const std::string & name_,
                                             const datatools::version_id & vid_)
     : instance_identifier(name_)
    {
      set_version(vid_);
      return;
    }

    instance_identifier::~instance_identifier()
    {
      return;
    }

    // virtual
    bool instance_identifier::is_name_valid(const std::string & name_) const
    {
      return datatools::name_validation(name_, datatools::NV_INSTANCE);
    }

    void instance_identifier::jsonize(jsontools::node & node_,
                               const unsigned long int version_)
    {
      base_identifier::jsonize(node_, version_);
      return;
    }

    void instance_identifier::protobufize(protobuftools::message_node & node_,
                                          const unsigned long int version_)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_identifier, node_);
      return;
    }

  } // namespace utility

} // namespace vire
