// vire/cms/connection_success.cc
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
#include <vire/cms/connection_success.h>

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
#include <bayeux/protobuftools/std_vector_converter.h>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ConnectionSuccess.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cmslapp::connection_success",
                               vire::cmslapp::ConnectionSuccess)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(connection_success,
                                        "vire::cmslapp::connection_success")

    connection_success::connection_success()
    {
      return;
    }

    connection_success::~connection_success()
    {
      return;
    }

    void connection_success::reset()
    {
      clear_resource_snapshots();
      return;
    }

    void connection_success::clear_resource_snapshots()
    {
      _resource_snapshots_.clear();
      return;
    }

    void connection_success::add_resource_status_record(const vire::cms::resource_status_record & record_)
    {
      _resource_snapshots_.push_back(record_);
      return;
    }

    const std::vector<vire::cms::resource_status_record> &
    connection_success::get_resource_snapshots() const
    {
      return _resource_snapshots_;
    }

    void connection_success::jsonize(jsontools::node & node_,
                                        const unsigned long int version_)
    {
      this->vire::utility::base_payload::jsonize(node_, version_);
      node_["requested_resources"] % _resource_snapshots_;
      return;
    }

    void connection_success::protobufize(protobuftools::message_node & node_,
                                         const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_payload, node_);
       node_["resource_snapshots"] % _resource_snapshots_;
      return;
    }

    void connection_success::tree_dump(std::ostream & out_,
                                       const std::string & title_,
                                       const std::string & indent_,
                                       bool inherit_) const
    {
      this->vire::utility::base_payload::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Resource snapshots : " << _resource_snapshots_.size() << std::endl;
      {
        std::size_t count = 0;
        for (const auto & resRecord : _resource_snapshots_) {
          out_ << indent_ << ::datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
          if (++count == _resource_snapshots_.size()) {
            out_ << last_tag;
          } else {
            out_ << tag;
          }
          out_ << "Resource : '" << resRecord.get_path() << "' : " << resRecord.get_status_string();
          out_ << std::endl;
        }
      }

      return;
    }

  } // namespace cms

} // namespace vire
