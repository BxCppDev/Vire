// vire/cms/no_pubsub_resource_error.cc
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
#include <vire/cms/no_pubsub_resource_error.h>

// Third party
// - Boost
#include <boost/algorithm/string/replace.hpp>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cms/NoPubsubResourceError.pb.h"

BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::no_pubsub_resource_error",
                               vire::cms::NoPubsubResourceError)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(no_pubsub_resource_errord,
                                        "vire::cms::no_pubsub_resource_errord");

    no_pubsub_resource_error::no_pubsub_resource_error()
      : ::vire::utility::base_error(base_error::EC_GENERIC_FAILURE, "")
    {
      return;
    }

    no_pubsub_resource_error::no_pubsub_resource_error(const std::string & path_)
      : ::vire::utility::base_error(base_error::EC_GENERIC_FAILURE, "")
    {
      set_path(path_);
      return;
    }

    no_pubsub_resource_error::~no_pubsub_resource_error()
    {
      return;
    }

    bool no_pubsub_resource_error::has_path() const
    {
      return !_path_.empty();
    }

    void no_pubsub_resource_error::set_path(const std::string & path_)
    {
      if (!has_message_format()) {
        set_message_format("Resource path='%p' has no pub/sub support!");
      }
      _path_ = path_;
      return;
    }

    const std::string & no_pubsub_resource_error::get_path() const
    {
      return _path_;
    }

    void no_pubsub_resource_error::reset()
    {
      _path_.clear();
      this->base_error::reset();
      return;
    }

    // virtual
    void no_pubsub_resource_error::_build_message(std::string & message_) const
    {
      if (has_path()) {
        message_ = boost::replace_all_copy(get_message_format(), "%p", _path_);
      }
      return;
    }

    void no_pubsub_resource_error::jsonize(jsontools::node & node_,
                                     const unsigned long int /* version_ */)
    {
      this->::vire::utility::base_error::jsonize(node_);
      node_["path"] % _path_;
      return;
    }

    void no_pubsub_resource_error::protobufize(protobuftools::message_node & node_,
                                         const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(::vire::utility::base_error,node_);
      node_["path"] % _path_;
      return;
    }

  } // namespace cms

} // namespace vire
