// vire/cms/timeout_error.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cms/timeout_error.h>

// Third party
// - Boost
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
// BxJsontools:
#include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_string_converter.h>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cms/TimeoutError.pb.h"

BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::timeout_error",
                               vire::cms::TimeoutError)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(timeout_error,
                                        "vire::cms::timeout_error");

    // static
    const int32_t timeout_error::EC_DEADLINE;

    timeout_error::timeout_error()
      : ::vire::utility::base_error(base_error::EC_GENERIC_FAILURE, "")
    {
      return;
    }

    timeout_resource_error::~timeout_error()
    {
      return;
    }

    bool timeout_error::is_deadline() const
    {
      return get_code() == EC_DEADLINE;
    }

    void timeout_error::set_deadline(const boost::posix_time::ptime & deadline_)
    {
      _deadline_ = deadline_;
      return;
    }

    const boost::posix_time::ptime & get_deadline() const
    {
      return _deadline_;
    }

  } // namespace cms

} // namespace vire
