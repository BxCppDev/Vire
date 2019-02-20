// vire/utility/unsupported_payload_error.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/utility/unsupported_payload_error.h>

// Third party
// - Boost
#include <boost/algorithm/string/replace.hpp>
// - BxJsontools:
#include <jsontools/base_type_converters.h>
#include <jsontools/std_type_converters.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>
#include <bayeux/protobuftools/protobuf_factory.h>

// Declare a protobuf registrar instance for the message class:
// #include <vire/base_object_protobuf.h>
#include "vire/utility/UnsupportedPayloadError.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::unsupported_payload_error",
                               vire::utility::UnsupportedPayloadError)

namespace vire {

  namespace utility {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(unsupported_payload_error,
                                        "vire::utility::unsupported_payload_error")

    unsupported_payload_error::unsupported_payload_error()
      : ::vire::utility::base_error(base_error::EC_GENERIC_FAILURE, "")
    {
      return;
    }

    unsupported_payload_error::unsupported_payload_error(const int32_t code_,
                                                         const std::string & type_id_,
                                                         const std::string & type_version_)
      : unsupported_payload_error()
    {
      if (code_ == EC_PAYLOAD_TYPE_ID) {
        set_type_id(type_id_);
      } else if (code_ == EC_PAYLOAD_TYPE_VERSION) {
        set_type_version(type_version_);
        set_type_id(type_id_);
      }
      return;
    }

    unsupported_payload_error::~unsupported_payload_error()
    {
      return;
    }

    bool unsupported_payload_error::has_type_id() const
    {
      return !_type_id_.empty();
    }

    void unsupported_payload_error::set_type_id(const std::string & bad_name_)
    {
      if (!has_message_format()) {
        set_message_format("Invalid payload type ID='%n'");
      }
      set_code(EC_PAYLOAD_TYPE_ID);
      _type_version_.clear();
      _type_id_ = bad_name_;
      return;
    }

    const std::string & unsupported_payload_error::get_type_id() const
    {
      return _type_id_;
    }

    bool unsupported_payload_error::has_type_version() const
    {
      return !_type_version_.empty();
    }

    void unsupported_payload_error::set_type_version(const std::string & bad_version_)
    {
      if (!has_message_format()) {
        set_message_format("Invalid payload type version=[%v]");
      }
      set_code(EC_INVALID_VERSION);
      _type_id_.clear();
      _type_version_ = bad_version_;
      return;
    }

    const std::string & unsupported_payload_error::get_type_version() const
    {
      return _type_version_;
    }

    // virtual
    void unsupported_payload_error::_build_message(std::string & message_) const
    {
      if (has_type_id()) {
        message_ = boost::replace_all_copy(get_message_format(), "%n", _type_id_);
      } else if (has_type_version()) {
        message_ = boost::replace_all_copy(get_message_format(), "%v", _type_version_);
      }
      return;
    }

    void unsupported_payload_error::reset()
    {
      _type_id_.clear();
      _type_version_.clear();
      this->base_error::reset();
      return;
    }

    void unsupported_payload_error::jsonize(jsontools::node & node_,
                                         const unsigned long int /* version_ */)
    {
      this->base_error::jsonize(node_);
      node_["type_id"] % _type_id_;
      node_["type_version"] % _type_version_;
      return;
    }

    void unsupported_payload_error::protobufize(protobuftools::message_node & node_,
                                             const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_error,node_);
      node_["type_id"] % _type_id_;
      node_["type_version"] % _type_version_;
      return;
    }

    void unsupported_payload_error::tree_dump(std::ostream & out_,
                                           const std::string & title_,
                                           const std::string & indent_,
                                           bool inherit_) const
    {
      this->base_error::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Type id : '" << _type_id_ << "'" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Type version : '" << _type_version_ << "'" << std::endl;

      return;
    }

  } // namespace utility

} // namespace vire
