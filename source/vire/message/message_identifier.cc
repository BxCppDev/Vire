// vire/message/message_identifier.cc
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
#include <vire/message/message_identifier.h>

// Standard library:
#include <sstream>

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>
// - BxJsontools:
#include <jsontools/base_type_converters.h>
#include <jsontools/std_type_converters.h>

// Bayeux/protobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>

// Declare a protobuf registrar instance for the MessageHeader class:
#include "vire/message/MessageIdentifier.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::message::message_identifier", vire::message::MessageIdentifier)

namespace vire {

  namespace message {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(message_identifier,"vire::message::message_identifier")

    // static
    const int32_t message_identifier::INVALID_NUMBER;

    // static
    const message_identifier & message_identifier::invalid_id()
    {
      static boost::scoped_ptr<message_identifier> _imid;
      if (_imid.get() == nullptr) {
        _imid.reset(new message_identifier);
      }
      return *_imid.get();
    }

    message_identifier::message_identifier()
    {
      _number_ = INVALID_NUMBER;
      return;
    }

    message_identifier::message_identifier(const std::string & emitter_,
                                           int32_t number_)
    {
      set_emitter(emitter_);
      set_number(number_);
      return;
    }

    message_identifier::~message_identifier()
    {
      return;
    }

    void message_identifier::set_emitter(const std::string & name_)
    {
      _emitter_ = name_;
      return;
    }

    const std::string & message_identifier::get_emitter() const
    {
      return _emitter_;
    }

    void message_identifier::set_number(int32_t number_)
    {
      _number_ = number_;
      return;
    }

    int32_t message_identifier::get_number() const
    {
      return _number_;
    }

    void message_identifier::reset()
    {
      _emitter_.clear();
      _number_ = INVALID_NUMBER;
      return;
    }

    bool message_identifier::is_valid() const
    {
      if (_emitter_.empty()) return false;
      if (_number_ == INVALID_NUMBER) return false;
      return true;
    }

    void message_identifier::to_string(std::string & format_) const
    {
      std::ostringstream repr_oss;
      repr_oss << _emitter_ << '[' << _number_ << ']';
      format_ = repr_oss.str();
      return;
    }

    std::string message_identifier::to_string() const
    {
      std::string repr;
      to_string(repr);
      return repr;
    }

    void message_identifier::tree_dump(std::ostream & out_,
                                       const std::string & title_,
                                       const std::string & indent_,
                                       bool inherit_) const
    {
      base_payload::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Emitter : '" << _emitter_ << "'" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Number : [" << _number_ << "]" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Valid : " << is_valid() << std::endl;

      return;
    }

    void message_identifier::jsonize(jsontools::node & node_,
                                       unsigned long int version_)
    {
      this->base_payload::jsonize(node_, version_);
      node_["emitter"] % _emitter_;
      node_["number"] % _number_;
      return;
    }

    void message_identifier::protobufize(protobuftools::message_node & node_,
                                         const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_payload, node_);
      node_["emitter"] % _emitter_;
      node_["number"] % _number_;
      return;
    }

  } // namespace message

} // namespace vire
