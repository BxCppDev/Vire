// vire/cms/resource_pubsub_success.cc
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
#include <vire/cms/resource_pubsub_success.h>

// Third Party:
// - datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/utils.h>
// - BxJsontools:
#include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/boost_type_converters.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/boost_optional_converter.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ResourcePubsubSuccess.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::resource_pubsub_success",
                               vire::cms::ResourcePubsubSuccess)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(resource_pubsub_success,
                                        "vire::cms::resource_pubsub_success")


    resource_pubsub_success::resource_pubsub_success()
    {
      return;
    }

    resource_pubsub_success::~resource_pubsub_success()
    {
      return;
    }

    void resource_pubsub_success::set_subscribed(const bool s_)
    {
      _subscribed_ = s_;
      return;
    }

    bool resource_pubsub_success::is_subscribed() const
    {
      return _subscribed_;
    }

    bool resource_pubsub_success::has_audit() const
    {
      return _audit_ != boost::none;
    }

    const resource_exec_success & resource_pubsub_success::get_audit() const
    {
      DT_THROW_IF(!has_audit(), std::logic_error, "No audit is set!");
      return *_audit_;
    }

    void resource_pubsub_success::set_audit(const resource_exec_success & audit_)
    {
      _audit_ = audit_;
      return;
    }

    void resource_pubsub_success::reset_audit()
    {
      _audit_ = boost::none;
      return;
    }

    void resource_pubsub_success::jsonize(jsontools::node & node_,
                                          const unsigned long int version_)
    {
      this->vire::utility::base_payload::jsonize(node_, version_);
      node_["subscribed"] % _subscribed_;
      node_["audit"] % _audit_;
      return;
    }

    void resource_pubsub_success::protobufize(protobuftools::message_node & node_,
                                              const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_payload, node_);
      node_["subscribed"] % _subscribed_;
      node_["audit"] % _audit_;
      return;
    }

    void resource_pubsub_success::tree_dump(std::ostream & out_,
                                            const std::string & title_,
                                            const std::string & indent_,
                                            bool inherit_) const
    {
      this->vire::utility::base_payload::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Subscribed : " << std::boolalpha << _subscribed_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Audit : ";
      if (!has_audit()) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (has_audit()) {
        std::ostringstream indentss;
        indentss << indent_ << ::datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
        _audit_->tree_dump(out_, "", indentss.str());
      }

      return;
    }

  } // namespace cms

} // namespace vire
