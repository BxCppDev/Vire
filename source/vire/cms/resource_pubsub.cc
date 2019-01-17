// vire/cms/resource_pubsub.cc
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
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves
#include <vire/cms/resource_pubsub.h>

// Third Party:
// - datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/utils.h>
// - BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
#include <bayeux/jsontools/enum_converter.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>
#include <bayeux/protobuftools/std_vector_converter.h>
#include <bayeux/protobuftools/enum_converter.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ResourcePubsub.pb.h"

BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::resource_pubsub",
                               vire::cms::ResourcePubsub)

  namespace vire {

    namespace cms {

      VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(resource_pubsub,
                                          "vire::cms::resource_pubsub")

      // static
      std::string resource_pubsub::action_to_label(const pubsub_action action_)
      {
        switch (action_) {
        case ACTION_CHECK: return std::string("check");
        case ACTION_SUBSCRIBE: return std::string("subscribe");
        case ACTION_UNSUBSCRIBE: return std::string("unsubscribe");
        default: return std::string("");
        }
      }

      // static
      resource_pubsub::pubsub_action
      resource_pubsub::action_from_label(const std::string & label_,
                                         pubsub_action & action_)
      {
        if (label_ == "check") return ACTION_CHECK;
        if (label_ == "subscribe") return ACTION_SUBSCRIBE;
        if (label_ == "unsubscribe") return ACTION_UNSUBSCRIBE;
        return ACTION_INVALID;
      }


      resource_pubsub::resource_pubsub()
      {
        return;
      }

      resource_pubsub::resource_pubsub(const std::string & path_,
                                       const pubsub_action action_)
      {
        set_path(path_);
        set_action(action_);
        return;
      }

      void resource_pubsub::reset()
      {
        reset_path();
        reset_action();
        return;
      }

      resource_pubsub::~resource_pubsub()
      {
        return;
      }

      bool resource_pubsub::has_path() const
      {
        return !_path_.empty();
      }

      void resource_pubsub::set_path(const std::string & path_)
      {
        _path_ = path_;
        return;
      }

      void resource_pubsub::reset_path()
      {
        _path_.clear();
        return;
      }

      const std::string & resource_pubsub::get_path() const
      {
        return _path_;
      }

      bool resource_pubsub::has_action() const
      {
        return _action_ != ACTION_INVALID;
      }

      void resource_pubsub::set_action(const pubsub_action action_)
      {
        _action_ = action_;
        return;
      }

      resource_pubsub::pubsub_action
      resource_pubsub::get_action() const
      {
        return _action_;
      }

      void resource_pubsub::reset_action()
      {
        _action_ = ACTION_INVALID;
        return;
      }

      void resource_pubsub::jsonize(jsontools::node & node_,
                                    const unsigned long int version_)
      {
        this->vire::utility::base_payload::jsonize(node_, version_);
        node_["path"] % _path_;
        node_["action"] % _action_;
        return;
      }

      void resource_pubsub::protobufize(protobuftools::message_node & node_,
                                        const unsigned long int /* version_ */)
      {
        VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_payload, node_);
        node_["path"] % _path_;
        node_["action"] % _action_;
        return;
      }

      void resource_pubsub::tree_dump(std::ostream & out_,
                                      const std::string & title_,
                                      const std::string & indent_,
                                      bool inherit_) const
      {
        this->vire::utility::base_payload::tree_dump(out_, title_, indent_, true);

        out_ << indent_ << ::datatools::i_tree_dumpable::tag
             << "Path : '" << get_path() << "'" << std::endl;

        out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
             << "Action : '" << action_to_label(get_action()) << "'" << std::endl;

        return;
      }

      /*

        void resource_pubsub_failure_response::set_error(const vire::utility::invalid_context_error & e_)
        {
        _error_type_id_.set_name("vire::utility::invalid_context_error");
        _error_ = e_;
        return;
        }

        void resource_pubsub_failure_response::set_error(const vire::cms::invalid_resource_error & e_)
        {
        _error_type_id_.set_name("vire::cms::invalid_resource_error");
        _error_ = e_;
        return;
        }

        void resource_pubsub_failure_response::set_error(const vire::cms::no_pubsub_resource_error & e_)
        {
        _error_type_id_.set_name("vire::cms::no_pubsub_resource_error");
        _error_ = e_;
        return;
        }

        void resource_pubsub_failure_response::set_error(const vire::cms::invalid_status_error & e_)
        {
        _error_type_id_.set_name("vire::cms::invalid_status_error");
        _error_ = e_;
        return;
        }

        const vire::utility::model_identifier &
        resource_pubsub_failure_response::get_error_type_id() const
        {
        return _error_type_id_;
        }

        resource_pubsub_failure_response::error_type
        resource_pubsub_failure_response::get_error() const
        {
        return _error_;
        }
      */

    } // namespace cms

  } // namespace vire
