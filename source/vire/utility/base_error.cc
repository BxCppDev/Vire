// vire/utility/base_error.cc
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/utility/base_error.h>

// Third party:
// - BxJsontools:
#include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/std_type_converters.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>
// Declare a protobuf registrar instance for the message class:
#include <bayeux/protobuftools/protobuf_factory.h>

// This project:
#include "vire/utility/BaseError.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::base_error",
                               vire::utility::BaseError)

namespace vire {

  namespace utility {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(base_error, "vire::utility::base_error")

    // static
    const int32_t base_error::EC_SUCCESS;
    const int32_t base_error::EC_GENERIC_FAILURE;
    const int32_t base_error::EC_MAXIMUM_SYSTEM;

    base_error::base_error(const int32_t code_, const std::string & message_format_)
      : base_payload()
      , _code_(code_)
      , _message_format_(message_format_)
    {
      return;
    }

    base_error::base_error(const std::string & message_format_)
      : base_error(EC_GENERIC_FAILURE, message_format_)
    {
      return;
    }

    base_error::base_error(const int32_t code_)
      : base_error(code_, "")
    {
      return;
    }

    base_error::base_error() : base_error(EC_GENERIC_FAILURE)
    {
      return;
    }

    base_error::~base_error()
    {
      return;
    }

    bool base_error::is_success() const
    {
      return _code_ == EC_SUCCESS;
    }

    bool base_error::is_failure() const
    {
      return !is_success();
    }

    void base_error::set_code(const int32_t code_)
    {
      _code_ = code_;
      return;
    }

    int32_t base_error::get_code() const
    {
      return _code_;
    }

    bool base_error::has_message_format() const
    {
      return !_message_format_.empty();
    }

    void base_error::set_message_format(const std::string & message_format_)
    {
      _message_format_ = message_format_;
      _cached_message_.clear();
      return;
    }

    void base_error::set_message(const std::string & message_format_)
    {
      set_message_format(message_format_);
      return;
    }

    void base_error::clear_message_format()
    {
      _message_format_.clear();
      _cached_message_.clear();
      return;
    }

    const std::string & base_error::get_message_format() const
    {
      return _message_format_;
    }

    bool base_error::has_message() const
    {
      if (!_cached_message_.empty()) return true;
      if (!_message_format_.empty()) return true;
      return false;
    }

    const std::string & base_error::get_message() const
    {
      if (_cached_message_.empty()) {
        if (has_message_format()) {
          base_error * mutable_this = const_cast<base_error*>(this);
          _build_message(mutable_this->_cached_message_);
        }
      }
      return _cached_message_;
    }

    void base_error::reset()
    {
      _code_ == EC_SUCCESS;
      _message_format_.clear();
      _cached_message_.clear();
      return;
    }

    void base_error::make_failure(const int32_t code_,
                                  const std::string & message_format_)
    {
      set_code(code_);
      set_message_format(message_format_);
      return;
    }

    void base_error::make_success(const std::string & message_)
    {
      set_code(EC_SUCCESS);
      set_message_format(message_);
      return;
    }

    void base_error::print(std::ostream & out_) const
    {
      out_ << "[code=" << _code_ << "]";
      if (has_message()) {
        out_ << ' ' << get_message();
      }
      return;
    }

    std::string base_error::to_string() const
    {
      std::ostringstream repr;
      print(repr);
      return repr.str();
    }

    // virtual
    void base_error::_build_message(std::string & message_) const
    {
      message_ = _message_format_;
      return;
    }

    void base_error::jsonize(jsontools::node & node_,
                             const unsigned long int version_)
    {
      this->base_payload::jsonize(node_, version_);
      node_["code"] % _code_;
      node_["message"] % _message_format_;
      return;
    }

    void base_error::protobufize(protobuftools::message_node & node_,
                                 const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_payload, node_);
      node_["code"] % _code_;
      node_["message"] % _message_format_;
      return;
    }

    void base_error::tree_dump(std::ostream & out_,
                               const std::string & title_,
                               const std::string & indent_,
                               bool inherit_) const
    {
      if (! title_.empty()) out_ << indent_ << title_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Code : [" << _code_ << "]" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Message format : '" << _message_format_ << "'" << std::endl;

      return;
    }


  } // namespace utility

} // namespace vire
