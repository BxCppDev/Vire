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
// - Boost
#include <boost/algorithm/string/replace.hpp>
// - BxJsontools:
#include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/std_type_converters.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cms/InvalidCredentialsError.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::invalid_credentials_error",
                               vire::cms::InvalidCredentialsError)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(invalid_credentials_error,
                                        "vire::cms::invalid_credentials_error");

    // static
    const int32_t invalid_credentials_error::EC_INVALID_LOGIN;
    const int32_t invalid_credentials_error::EC_INVALID_PASSWORD;

    invalid_credentials_error::invalid_credentials_error()
      : ::vire::utility::base_error(base_error::EC_GENERIC_FAILURE, "")
    {
      return;
    }

    invalid_credentials_error::~invalid_credentials_error()
    {
      return;
    }

    void invalid_credentials_error::reset()
    {
      _invalid_login_.clear();
      _invalid_password_.clear();
      this->::vire::utility::base_error::reset();
      return;
    }

    bool invalid_credentials_error::is_invalid_login() const
    {
      return get_code() == EC_INVALID_LOGIN;
    }

    bool invalid_credentials_error::has_invalid_login() const
    {
      return !_invalid_login_.empty();
    }

    void invalid_credentials_error::set_invalid_login(const std::string & bad_login_)
    {
      if (!has_message_format()) {
        set_message_format("Invalid login='%l'");
      }
      set_code(EC_INVALID_LOGIN);
      _invalid_login_ = bad_login_;
      _invalid_password_.clear();
      return;
    }

    const std::string & invalid_credentials_error::get_invalid_login() const
    {
      return _invalid_login_;
    }

    bool invalid_credentials_error::is_invalid_password() const
    {
      return get_code() == EC_INVALID_PASSWORD;
    }

    bool invalid_credentials_error::has_invalid_password() const
    {
      return !_invalid_password_.empty();
    }

    void invalid_credentials_error::set_invalid_password(const std::string & bad_password_)
    {
      if (!has_message_format()) {
        set_message_format("Invalid password='%p' for user '%l'");
      }
      set_code(EC_INVALID_PASSWORD);
      _invalid_password_ = bad_password_;
      _invalid_password_.clear();
      return;
    }

    const std::string & invalid_credentials_error::get_invalid_password() const
    {
      return _invalid_password_;
    }

    // virtual
    void invalid_credentials_error::_build_message(std::string & message_) const
    {
      if (has_invalid_login()) {
        message_ = boost::replace_all_copy(get_message_format(), "%l", _invalid_login_);
      } else if (has_invalid_password()) {
        message_ = boost::replace_all_copy(get_message_format(), "%l", _invalid_login_);
        message_ = boost::replace_all_copy(get_message_format(), "%p", _invalid_password_);
      }
      return;
    }

    void invalid_credentials_error::jsonize(jsontools::node & node_,
                                            const unsigned long int /* version_ */)
    {
      this->::vire::utility::base_error::jsonize(node_);
      if (is_invalid_login()) {
        node_["invalid_login"] % _invalid_login_;
      }
      if (is_invalid_password()) {
        node_["invalid_password"] % _invalid_password_;
      }
      return;
    }

    void invalid_credentials_error::protobufize(protobuftools::message_node & node_,
                                                const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_error, node_);
      if (is_invalid_login()) {
        node_["invalid_login"] % _invalid_login_;
      }
      if (is_invalid_password()) {
        node_["invalid_password"] % _invalid_password_;
      }
      return;
    }

  } // namespace cms

} // namespace vire
