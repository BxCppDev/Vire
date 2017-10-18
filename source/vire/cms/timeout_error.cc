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
#include <bayeux/jsontools/boost_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_string_converter.h>
#include <bayeux/protobuftools/boost_datetime_converters.h>

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
      : ::vire::utility::base_error(EC_DEADLINE, "")
    {
      return;
    }

    timeout_error::~timeout_error()
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

    const boost::posix_time::ptime & timeout_error::get_deadline() const
    {
      return _deadline_;
    }

    void timeout_error::reset()
    {
      ::vire::time::invalidate(_deadline_);
      this->base_error::reset();
      return;
    }

    void timeout_error::jsonize(jsontools::node & node_,
                                           const unsigned long int /* version_ */)
    {
      this->::vire::utility::base_error::jsonize(node_);
      node_["deadline"] % _deadline_;
      return;
    }

    void timeout_error::protobufize(protobuftools::message_node & node_,
                                               const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(::vire::utility::base_error, node_);
      node_["deadline"] % _deadline_;
      return;
    }

    // virtual
    void timeout_error::_build_message(std::string & message_) const
    {
      if (is_deadline()) {
        message_ = boost::replace_all_copy(get_message_format(),
                                           "%d",
                                           ::vire::time::to_string(_deadline_));
      }
      return;
    }

  } // namespace cms

} // namespace vire
