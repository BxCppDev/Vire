// vire/message/message.cc
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
#include <vire/message/message.h>

// Third party:
// - BxJsontools:
#include <jsontools/base_type_converters.h>
#include <jsontools/std_type_converters.h>

// Bayeux/protobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>
#include <bayeux/protobuftools/protobufable_converter.h>

// Declare a protobuf registrar instance for the message class:
#include "vire/message/Message.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::message::message", vire::message::Message)

namespace vire {

  namespace message {

    DATATOOLS_SERIALIZATION_IMPLEMENTATION_ADVANCED(message,"vire::message::message")

    message::message()
    {
      return;
    }

    message::~message()
    {
      return;
    }

    void message::reset()
    {
      _header_.reset();
      _body_.reset();
      return;
    }

    bool message::is_valid() const
    {
      if (!_header_.is_valid()) return false;
      if (!_body_.is_valid()) return false;
      return true;
    }

    void message::set_header(const message_header & h_)
    {
      _header_ = h_;
      return;
    }

    const message_header & message::get_header() const
    {
      return _header_;
    }

    message_header & message::grab_header()
    {
      return _header_;
    }

    void message::set_body(const message_body & b_)
    {
      _body_ = b_;
      return;
    }

    const message_body & message::get_body() const
    {
      return _body_;
    }

    message_body & message::grab_body()
    {
      return _body_;
    }

    void message::tree_dump(std::ostream & out_,
                            const std::string & title_,
                            const std::string & indent_,
                            bool inherit_) const
    {
      if (! title_.empty()) out_ << indent_ << title_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Header : " << std::endl;
      {
        std::ostringstream indent2;
        indent2 << indent_ << ::datatools::i_tree_dumpable::skip_tag;
        _header_.tree_dump(out_, "", indent2.str(), false);
      }

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Body : " << std::endl;
      {
        std::ostringstream indent2;
        indent2 << indent_ << ::datatools::i_tree_dumpable::skip_tag;
        _body_.tree_dump(out_, "", indent2.str(), false);
      }

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Valid : " << is_valid() << std::endl;

      return;
    }

    void message::jsonize(jsontools::node & node_,
                          const unsigned long int version_)
    {
      node_["header"] % _header_;
      node_["body"]   % _body_;
      return;
    }

    void message::protobufize(protobuftools::message_node & node_,
                              const unsigned long int /* version_ */)
    {
      node_["header"] % _header_;
      node_["body"]   % _body_;
      return;
    }

  } // namespace message

} // namespace vire
