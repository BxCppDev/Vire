// vire/cms/disconnection_request.cc
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
#include <vire/cms/disconnection_request.h>

// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_string_converter.h>
//#include <bayeux/protobuftools/std_vector_converter.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/cms/DisconnectionRequest.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::disconnection_request",
                               vire::cms::DisconnectionRequest)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(disconnection_request,
                                        "vire::cms::disconnection_request")

    disconnection_request::disconnection_request()
    {
      return;
    }

    disconnection_request::~disconnection_request()
    {
      return;
    }

    void disconnection_request::reset()
    {
      reset_setup_id();
      return;
    }

    bool disconnection_request::has_setup_id() const
    {
      return _setup_id_.is_valid();
    }

    void disconnection_request::reset_setup_id()
    {
      _setup_id_.reset();
      return;
    }

    void disconnection_request::set_setup_id(const vire::utility::instance_identifier & id_)
    {
      _setup_id_ = id_;
      return;
    }

    const vire::utility::instance_identifier &
    disconnection_request::get_setup_id() const
    {
      return _setup_id_;
    }

    void disconnection_request::jsonize(jsontools::node & node_,
                                        const unsigned long int version_)
    {
      this->vire::utility::base_payload::jsonize(node_, version_);
      node_["setup_id"] % _setup_id_;
      return;
    }

    void disconnection_request::protobufize(protobuftools::message_node & node_,
                                         const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_payload, node_);
      node_["setup_id"] % _setup_id_;
      return;
    }

    void disconnection_request::tree_dump(std::ostream & out_,
                                       const std::string & title_,
                                       const std::string & indent_,
                                       bool inherit_) const
    {
      this->vire::utility::base_payload::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Setup ID : '" << _setup_id_.to_string() << "'" << std::endl;

      return;
    }

  } // namespace cms

} // namespace vire
