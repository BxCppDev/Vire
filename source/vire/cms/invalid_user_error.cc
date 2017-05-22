// vire/cms/invalid_user_error.cc
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
#include <vire/cms/invalid_user_error.h>

// Third party
// - Boost
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
// - BxJsontools:
#include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cms/InvalidUserError.pb.h"

BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::invalid_user_error",
                               vire::cms::InvalidUserError)

namespace vire {

  namespace cms {

    // static
    const int32_t invalid_user_error::EC_INVALID_NAME;
    const int32_t invalid_user_error::EC_INVALID_UID;

    invalid_user_error::invalid_user_error()
      : ::vire::utility::base_error(base_error::EC_GENERIC_FAILURE, "")
    {
      return;
    }

    invalid_user_error::invalid_user_error(int32_t bad_uid_)
      : invalid_user_error()
    {
      set_invalid_user_id(bad_uid_);
      return;
    }

    invalid_user_error::invalid_user_error(const std::string & bad_name_)
      : invalid_user_error()
    {
      set_invalid_user_name(bad_name_);
      return;
    }

    invalid_user_error::~invalid_user_error()
    {
      return;
    }

    bool invalid_user_error::has_invalid_user_name() const
    {
      return !_invalid_user_name_.empty();
    }

    void invalid_user_error::set_invalid_user_name(const std::string & bad_name_)
    {
      if (!has_message_format()) {
        set_message_format("Invalid name='%n'");
      }
      set_code(EC_INVALID_NAME);
      _invalid_user_name_ = bad_name_;
      _invalid_user_id_ = -1;
      return;
    }

    const std::string & invalid_user_error::get_invalid_user_name() const
    {
      return _invalid_user_name_;
    }

    bool invalid_user_error::has_invalid_user_id() const
    {
      return _invalid_user_id_ >= 0;
    }

    void invalid_user_error::set_invalid_user_id(int32_t bad_uid_)
    {
      if (!has_message_format()) {
        set_message_format("Invalid UID=[%u]");
      }
      set_code(EC_INVALID_UID);
      _invalid_user_name_.clear();
      _invalid_user_id_ = bad_uid_;
      return;
    }

    int32_t invalid_user_error::get_invalid_user_id() const
    {
      return _invalid_user_id_;
    }

    // virtual
    void invalid_user_error::_build_message(std::string & message_) const
    {
      if (has_invalid_user_name()) {
        message_ = boost::replace_all_copy(get_message_format(), "%n", _invalid_user_name_);
      } else if (has_invalid_user_id()) {
        message_ = boost::replace_all_copy(get_message_format(), "%u", boost::lexical_cast<std::string>(_invalid_user_id_));
      }
      return;
    }

    void invalid_user_error::reset()
    {
      _invalid_user_name_.clear();
      _invalid_user_id_ = -1;
      this->base_error::reset();
      return;
    }

    void invalid_user_error::jsonize(jsontools::node & node_,
                                     const unsigned long int /* version_ */)
    {
      this->::vire::utility::base_error::jsonize(node_);
      node_["invalid_user_name"] % _invalid_user_name_;
      node_["invalid_user_id"]   % _invalid_user_id_;
      return;
    }

    void invalid_user_error::protobufize(protobuftools::message_node & node_,
                                         const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(::vire::utility::base_error,node_);
      node_["invalid_user_name"] % _invalid_user_name_;
      node_["invalid_user_id"]   % _invalid_user_id_;
      return;
    }

  } // namespace cms

} // namespace vire
