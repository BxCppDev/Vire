// vire/cms/resource_pubsub_failure.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves
#include <vire/cms/resource_pubsub_failure.h>

// Third Party:
// - datatools:
#include <datatools/exception.h>
#include <datatools/logger.h>
#include <datatools/utils.h>
// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_vector_converter.h>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ResourcePubsubFailure.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::resource_pubsub_failure",
                               vire::cms::ResourcePubsubFailure)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(resource_pubsub_failure,
                                        "vire::cms::resource_pubsub_failure")

    resource_pubsub_failure::resource_pubsub_failure()
    {
      _error_type_id_.set_name("vire::utility::invalid_context_error");
      return;
    }

    resource_pubsub_failure::~resource_pubsub_failure()
    {
      return;
    }

    void resource_pubsub_failure::set_error(const vire::utility::invalid_context_error & e_)
    {
      _error_type_id_.set_name("vire::utility::invalid_context_error");
      _error_ = e_;
      return;
    }

    void resource_pubsub_failure::set_error(const vire::cms::invalid_resource_error & e_)
    {
      _error_type_id_.set_name("vire::cms::invalid_resource_error");
      _error_ = e_;
      return;
    }

    void resource_pubsub_failure::set_error(const vire::cms::invalid_status_error & e_)
    {
      _error_type_id_.set_name("vire::cms::invalid_status_error");
      _error_ = e_;
      return;
    }

    void resource_pubsub_failure::set_error(const vire::cms::no_pubsub_resource_error & e_)
    {
      _error_type_id_.set_name("vire::cms::no_pubsub_resource_error");
      _error_ = e_;
      return;
    }

    const vire::utility::model_identifier &
    resource_pubsub_failure::get_error_type_id() const
    {
      return _error_type_id_;
    }

    resource_pubsub_failure::error_type
    resource_pubsub_failure::get_error() const
    {
      return _error_;
    }

    void resource_pubsub_failure::reset()
    {
      set_error(vire::utility::invalid_context_error());
      return;
    }

    void resource_pubsub_failure::jsonize(jsontools::node & node_,
                                          const unsigned long int version_)
    {
      this->base_payload::jsonize(node_, version_);
      node_["error_type_id"] % _error_type_id_;
      if (_error_type_id_.get_name() == "vire::utility::invalid_context_error") {
        node_["invalid_context_err"] % boost::get<vire::utility::invalid_context_error>(_error_);
      }
      if (_error_type_id_.get_name() == "vire::cms::invalid_resource_error") {
        node_["invalid_resource_err"] % boost::get<vire::cms::invalid_resource_error>(_error_);
      }
      if (_error_type_id_.get_name() == "vire::cms::invalid_status_error") {
        node_["invalid_status_err"] % boost::get<vire::cms::invalid_status_error>(_error_);
      }
      if (_error_type_id_.get_name() == "vire::cms::no_pubsub_resource_error") {
        node_["no_pubsub_resource_err"] % boost::get<vire::cms::no_pubsub_resource_error>(_error_);
      }
      return;
    }

    void resource_pubsub_failure::protobufize(protobuftools::message_node & node_,
                                              const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_payload, node_);
      node_["error_type_id"] % _error_type_id_;
      if (_error_type_id_.get_name() == "vire::utility::invalid_context_error") {
        node_["invalid_context_err"] % boost::get<vire::utility::invalid_context_error>(_error_);
      }
      if (_error_type_id_.get_name() == "vire::cms::invalid_resource_error") {
        node_["invalid_resource_err"] % boost::get<vire::cms::invalid_resource_error>(_error_);
      }
      if (_error_type_id_.get_name() == "vire::cms::invalid_status_error") {
        node_["invalid_status_err"] % boost::get<vire::cms::invalid_status_error>(_error_);
      }
      if (_error_type_id_.get_name() == "vire::cms::no_pubsub_resource_error") {
        node_["no_pubsub_resource_err"] % boost::get<vire::cms::no_pubsub_resource_error>(_error_);
      }
      return;
    }

    void resource_pubsub_failure::tree_dump(std::ostream & out_,
                                            const std::string & title_,
                                            const std::string & indent_,
                                            bool inherit_) const
    {
      this->vire::utility::base_payload::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Error type id : '" << _error_type_id_.to_string() << "'" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Error : ";
      out_ << std::endl;

      const vire::utility::invalid_context_error * ice
        = boost::get<vire::utility::invalid_context_error>(&_error_);
      const vire::cms::invalid_resource_error * ire
        = boost::get<vire::cms::invalid_resource_error>(&_error_);
      const vire::cms::invalid_status_error * ise
        = boost::get<vire::cms::invalid_status_error>(&_error_);
      const vire::cms::no_pubsub_resource_error * npre
        = boost::get<vire::cms::no_pubsub_resource_error>(&_error_);

      std::ostringstream indent2;
      indent2 << indent_ << ::datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
      if (ice != nullptr) {
        ice->tree_dump(out_, "", indent2.str());
      } else if (ire != nullptr) {
        ire->tree_dump(out_, "", indent2.str());
      } else if (ise != nullptr) {
        ise->tree_dump(out_, "", indent2.str());
      } else if ( npre != nullptr) {
        npre->tree_dump(out_, "", indent2.str());
      }

      return;
    }

  } // namespace cms

} // namespace vire
