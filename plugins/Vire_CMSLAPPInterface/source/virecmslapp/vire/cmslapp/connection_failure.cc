// vire/cmslapp/connection_failure.cc
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
#include <vire/cmslapp/connection_failure.h>

// Third party:
// - BxJsontools:
#include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/std_type_converters.h>
#include <bayeux/jsontools/boost_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_string_converter.h>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cmslapp/ConnectionFailure.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cmslapp::connection_failure",
                               vire::cmslapp::ConnectionFailure)

namespace vire {

  namespace cmslapp {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(connection_failure,
                                        "vire::cmslapp::connection_failure")

    connection_failure::connection_failure()
    {
      return;
    }

    connection_failure::~connection_failure()
    {
      return;
    }

    void connection_failure::set_error(const vire::utility::invalid_context_error & ice_)
    {
      _error_type_id_.set_name("vire::utility::invalid_context_error");
      _error_ = ice_;
      return;
    }

    void connection_failure::set_error(const vire::utility::invalid_setup_id_error & iside_)
    {
      _error_type_id_.set_name("vire::utility::invalid_setup_id_error");
      _error_ = iside_;
      return;
    }

    void connection_failure::set_error(const vire::cms::unknown_resources_error & ure_)
    {
      _error_type_id_.set_name("vire::cms::unknown_resources_error");
      _error_ = ure_;
      return;
    }

    const vire::utility::model_identifier &
    connection_failure::get_error_type_id() const
    {
      return  _error_type_id_;
    }

    connection_failure::error_type
    connection_failure::get_error() const
    {
      return _error_;
    }

    void connection_failure::jsonize(jsontools::node & node_,
                                     const unsigned long int version_)
    {
      this->::vire::utility::base_payload::jsonize(node_, version_);
      node_["error_type_id"] % _error_type_id_;
      if (_error_type_id_.get_name() == "vire::cms::unknown_resources_error") {
        node_["error"] % boost::get<vire::cms::unknown_resources_error>(_error_);
      } else if (_error_type_id_.get_name() == "vire::utility::invalid_setup_id_error") {
        node_["error"] % boost::get<vire::utility::invalid_setup_id_error>(_error_);
      } else if (_error_type_id_.get_name() == "vire::utility::invalid_context_error") {
        node_["error"] % boost::get<vire::utility::invalid_context_error>(_error_);
      }
      return;
    }

    void connection_failure::protobufize(protobuftools::message_node & node_,
                                         const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_payload, node_);
      node_["error_type_id"] % _error_type_id_;
      if (_error_type_id_.get_name() == "vire::cms::unknown_resources_error") {
        node_["error"] % boost::get<vire::cms::unknown_resources_error>(_error_);
      } else if (_error_type_id_.get_name() == "vire::utility::invalid_setup_id_error") {
        node_["error"] % boost::get<vire::utility::invalid_setup_id_error>(_error_);
      } else if (_error_type_id_.get_name() == "vire::utility::invalid_context_error") {
        node_["error"] % boost::get<vire::utility::invalid_context_error>(_error_);
      }
      return;
    }

    void connection_failure::tree_dump(std::ostream & out_,
                                       const std::string & title_,
                                       const std::string & indent_,
                                       bool inherit_) const
    {
      this->vire::utility::base_payload::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Error type ID : '" << _error_type_id_.to_string() << "'" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Error : " << std::endl;
      std::ostringstream indent2;
      indent2 << indent_ << ::datatools::i_tree_dumpable::skip_tag;
      if (_error_type_id_.get_name() == "vire::cms::unknown_resources_error") {
        boost::get<vire::cms::unknown_resources_error>(_error_).tree_dump(out_, "", indent2.str());
      } else if (_error_type_id_.get_name() == "vire::utility::invalid_setup_id_error") {
        boost::get<vire::utility::invalid_setup_id_error>(_error_).tree_dump(out_, "", indent2.str());
      } else if (_error_type_id_.get_name() == "vire::utility::invalid_context_error") {
        boost::get<vire::utility::invalid_context_error>(_error_).tree_dump(out_, "", indent2.str());
      }

      return;
    }

  } // namespace cmslapp

} // namespace vire
