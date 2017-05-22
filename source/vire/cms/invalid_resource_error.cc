// vire/cms/invalid_resource_error.cc
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
#include <vire/cms/invalid_resource_error.h>

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
#include "vire/cms/InvalidResourceError.pb.h"

BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::invalid_resource_error",
                               vire::cms::InvalidResourceError)

namespace vire {

  namespace cms {

    DATATOOLS_CLONEABLE_IMPLEMENTATION(invalid_resource_error)

    DATATOOLS_SERIALIZATION_IMPLEMENTATION(invalid_resource_error,
                                           "vire::cms::invalid_resource_error");

    // static
    const int32_t invalid_resource_error::EC_INVALID_PATH;
    const int32_t invalid_resource_error::EC_INVALID_ID;

    invalid_resource_error::invalid_resource_error()
      : ::vire::utility::base_error(base_error::EC_GENERIC_FAILURE, "")
    {
      return;
    }

    invalid_resource_error::invalid_resource_error(int32_t bad_id_)
      : invalid_resource_error()
    {
      set_invalid_resource_id(bad_id_);
      return;
    }

    invalid_resource_error::invalid_resource_error(const std::string & bad_path_)
      : invalid_resource_error()
    {
      set_invalid_resource_path(bad_path_);
      return;
    }

    invalid_resource_error::~invalid_resource_error()
    {
      return;
    }

    bool invalid_resource_error::has_invalid_resource_path() const
    {
      return !_invalid_resource_path_.empty();
    }

    void invalid_resource_error::set_invalid_resource_path(const std::string & bad_path_)
    {
      if (!has_message_format()) {
        set_message_format("Invalid resource path='%p'");
      }
      set_code(EC_INVALID_PATH);
      _invalid_resource_path_ = bad_path_;
      _invalid_resource_id_ = -1;
      return;
    }

    const std::string & invalid_resource_error::get_invalid_resource_path() const
    {
      return _invalid_resource_path_;
    }

    bool invalid_resource_error::has_invalid_resource_id() const
    {
      return _invalid_resource_id_ >= 0;
    }

    void invalid_resource_error::set_invalid_resource_id(int32_t bad_uid_)
    {
      if (!has_message_format()) {
        set_message_format("Invalid resource ID='%i'");
      }
      set_code(EC_INVALID_ID);
      _invalid_resource_id_ = bad_uid_;
      _invalid_resource_path_.clear();
      return;
    }

    int32_t invalid_resource_error::get_invalid_resource_id() const
    {
      return _invalid_resource_id_;
    }

    // virtual
    void invalid_resource_error::_build_message(std::string & message_) const
    {
      // if (!has_message_format()) {
      //   std::string msg_format = "Invalid resource";
      //   if (has_invalid_resource_path()) {
      //     msg_format += " path='%p'";
      //   } else if (has_invalid_resource_id()) {
      //     msg_format += " ID=[%i]";
      //   }
      //   set_message_format(msg_format);
      // }
      if (has_invalid_resource_path()) {
        message_ = boost::replace_all_copy(get_message_format(), "%p", _invalid_resource_path_);
      } else if (has_invalid_resource_id()) {
        message_ = boost::replace_all_copy(get_message_format(), "%i", boost::lexical_cast<std::string>(_invalid_resource_id_));
      }
      return;
    }

    void invalid_resource_error::reset()
    {
      _invalid_resource_path_.clear();
      _invalid_resource_id_ = -1;
      this->base_error::reset();
      return;
    }

    void invalid_resource_error::jsonize(jsontools::node & node_,
                                         const unsigned long int version_)
    {
      this->vire::utility::base_error::jsonize(node_, version_);
      if (get_code() == EC_INVALID_PATH) {
        node_["invalid_resource_path"] % _invalid_resource_path_;
      }
      if (get_code() == EC_INVALID_ID) {
        node_["invalid_resource_id"] % _invalid_resource_id_;
      }
     return;
    }

    void invalid_resource_error::protobufize(protobuftools::message_node & node_,
                                             const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_error, node_);
      if (get_code() == EC_INVALID_PATH) {
        node_["invalid_resource_path"] % _invalid_resource_path_;
      }
      if (get_code() == EC_INVALID_ID) {
        node_["invalid_resource_id"] % _invalid_resource_id_;
      }
      return;
    }

  } // namespace cms

} // namespace vire
