// vire/cms/invalid_credentials_error.cc
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
#include <vire/cms/invalid_credentials_error.h>

// Third party:
// Declare a protobuf registrar instance for the message class:
#include <bayeux/protobuftools/protobuf_factory.h>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cms/InvalidCredentialsError.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::invalid_credentials_error",
                               vire::cms::InvalidCredentialsError)

namespace vire {

  namespace cms {

    invalid_credentials_error::invalid_credentials_error()
      : ::vire::utility::base_error(base_error::EC_GENERIC_FAILURE, "Invalid credentials")
    {
      return;
    }

    invalid_credentials_error::~invalid_credentials_error()
    {
      return;
    }

    void invalid_credentials_error::reset()
    {
      this->::vire::utility::base_error::reset();
      return;
    }

    void invalid_credentials_error::jsonize(jsontools::node & node_,
                                            const unsigned long int /* version_ */)
    {
      this->::vire::utility::base_error::jsonize(node_);
      return;
    }

    void invalid_credentials_error::protobufize(protobuftools::message_node & node_,
                                                const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(::vire::utility::base_error,node_);
      return;
    }

  } // namespace cms

} // namespace vire
