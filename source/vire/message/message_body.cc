// vire/message/message_body.cc
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
#include <vire/message/message_body.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/ioutils.h>
// - BxJsontools:
#include <jsontools/base_type_converters.h>
#include <jsontools/std_type_converters.h>

// Bayeux/protobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>
#include <bayeux/protobuftools/protobufable_converter.h>

// Declare a protobuf registrar instance for the message class:
#include <bayeux/protobuftools/protobuf_factory.h>
#include "vire/message/MessageBody.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::message::message_body", vire::message::MessageBody)

namespace vire {

  namespace message {

    DATATOOLS_SERIALIZATION_IMPLEMENTATION_ADVANCED(message_body,"vire::message::message_body")

    message_body::message_body()
    {
      _payload_ = nullptr;
      return;
    }

    message_body::message_body(const message_body & mb_)
    {
      _payload_ = nullptr;
      _copy_(mb_);
      return;
    }

    message_body::~message_body()
    {
      reset();
      return;
    }

    message_body & message_body::operator=(const message_body & mb_)
    {
      if (&mb_ != this) {
        _copy_(mb_);
      }
      return *this;
    }

    bool message_body::is_valid() const
    {
      if (! has_payload_type_id()) return false;
      if (! has_payload()) return false;
      return true;
    }

    void message_body::reset()
    {
      remove_payload();
      return;
    }

    bool message_body::has_payload_type_id() const
    {
      return _payload_type_id_.is_valid();
    }

    const vire::utility::model_identifier & message_body::get_payload_type_id() const
    {
      return _payload_type_id_;
    }

    void message_body::set_payload_type_id(const vire::utility::model_identifier & id_)
    {
      _payload_type_id_ = id_;
      return;
    }

    bool message_body::has_payload() const
    {
       return _payload_ != nullptr;
    }

    void message_body::set_payload(const vire::utility::base_payload & payload_)
    {
      vire::utility::base_payload * pl = dynamic_cast<vire::utility::base_payload *>(payload_.clone());
      _payload_ = const_cast<const vire::utility::base_payload *>(pl);
      if (!_payload_type_id_.is_valid()) {
        vire::utility::model_identifier id;
        id.set_name(_payload_->get_serial_tag());
        set_payload_type_id(id);
      }
      return;
    }

    void message_body::set_payload(const vire::utility::base_payload * payload_)
    {
      if (!payload_) {
        remove_payload();
        return;
      }
      _payload_ = payload_;
      if (_payload_ != nullptr) {
        if (!_payload_type_id_.is_valid()) {
          vire::utility::model_identifier id;
          id.set_name(_payload_->get_serial_tag());
          set_payload_type_id(id);
        }
      }
      return;
    }

    void message_body::remove_payload()
    {
      if (_payload_ != nullptr) {
        delete _payload_;
        _payload_ = nullptr;
        _payload_type_id_.reset();
      }
      return;
    }

    const vire::utility::base_payload & message_body::get_payload_ref() const
    {
      DT_THROW_IF(!has_payload(), std::logic_error, "Missing payload object!");
      return *_payload_;
    }

    const vire::utility::base_payload * message_body::get_payload() const
    {
      if (!has_payload()) {
        return nullptr;
      }
      return _payload_;
    }

    void message_body::tree_dump(std::ostream & out_,
                                 const std::string & title_,
                                 const std::string & indent_,
                                 bool inherit_) const
    {
      if (! title_.empty()) out_ << indent_ << title_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Payload type ID : ";
      if (has_payload_type_id()) {
        out_ << "'" << _payload_type_id_.to_string() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Payload : ";
      if (has_payload()) {
        out_ << "[@" << _payload_ << "]";
        out_ << " (as '" << typeid(*_payload_).name() << "')";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (has_payload()) {
        std::ostringstream indent2;
        indent2 << indent_ << ::datatools::i_tree_dumpable::skip_tag;
        _payload_->tree_dump(out_, "", indent2.str());
      }

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Valid : " << is_valid() << std::endl;

      return;
    }

    void message_body::_copy_(const message_body & source_)
    {
      this->remove_payload();
      if (source_._payload_ != nullptr) {
        if (has_payload_type_id()) {
          _payload_type_id_ = source_._payload_type_id_;
        }
        this->set_payload(dynamic_cast<vire::utility::base_payload*>(source_._payload_->clone()));
      }
      return;
    }

    void message_body::protobufize(protobuftools::message_node & node_,
                                   const unsigned long int /* version_ */)
    {
      static const std::string payload_type_id_field_name = "payload_type_id";
      static const std::string payload_field_name = "payload";
      if (node_.is_serializing()) {
        // Serialization:
        if (has_payload_type_id()) {
          node_[payload_type_id_field_name] % _payload_type_id_;
          if (has_payload()) {
            vire::utility::base_payload * mutable_payload
              = const_cast<vire::utility::base_payload *>(_payload_);
            const google::protobuf::FieldDescriptor * payload_field_desc
              = node_.get_message().GetDescriptor()->FindFieldByName(payload_field_name);
            std::string payload_type_id = get_payload_type_id().get_name();
            std::shared_ptr<google::protobuf::Message> payloadMsg
              = protobuftools::protobuf_factory::system_factory()
              .create_message_instance(payload_type_id);
            if (payloadMsg) {
              const google::protobuf::Message & msg = *payloadMsg.get();
              protobuftools::message_node_value payload_item_node(*payloadMsg.get(),
                                                                  payload_field_desc,
                                                                  node_.is_serializing(),
                                                                  false,
                                                                  node_.get_logging());
              mutable_payload->protobufize(payload_item_node);
              // msg.PrintDebugString();
              const google::protobuf::Reflection * reflection = node_.get_message().GetReflection();
              google::protobuf::Message * anyMsg
                = reflection->MutableMessage(&node_.grab_message(), payload_field_desc);
              google::protobuf::Any * any = dynamic_cast<google::protobuf::Any*>(anyMsg);
              any->PackFrom(*payloadMsg.get());
            }
          } // has_payload()
        } // has_payload_type_id()
      } // is_serializing()
      else {
        // Deserialization:
        reset();
        if (node_.has_field(payload_type_id_field_name)) {
          node_[payload_type_id_field_name] % _payload_type_id_;
          if (_payload_type_id_.is_valid()) {
            static const std::string payload_desc_name("google.protobuf.Any");
            DT_THROW_IF(!node_.has_field(payload_field_name),
                        std::runtime_error,
                        "Missing payload field!");
            const google::protobuf::FieldDescriptor * payload_field_desc
              = node_.get_message().GetDescriptor()->FindFieldByName(payload_field_name);
            const google::protobuf::Reflection * reflection = node_.get_message().GetReflection();
            std::string payload_type_id = get_payload_type_id().get_name();
            std::shared_ptr<google::protobuf::Message> payloadMsg
              = protobuftools::protobuf_factory::system_factory()
              .create_message_instance(payload_type_id);
            const google::protobuf::Message & anyMsg
              = reflection->GetMessage(node_.get_message(), payload_field_desc);
            const google::protobuf::Any & any = dynamic_cast<const google::protobuf::Any&>(anyMsg);
            DT_THROW_IF(!any.UnpackTo(payloadMsg.get()),
                        std::runtime_error,
                        "Unpacking field '" << payload_field_name << "' failed!");
            protobuftools::message_node_value payload_item_node(*payloadMsg.get(),
                                                                payload_field_desc,
                                                                node_.is_serializing(),
                                                                false,
                                                                node_.get_logging());
            vire::utility::base_payload::factory_register_type & the_factory_register =
              DATATOOLS_FACTORY_GRAB_SYSTEM_REGISTER(vire::utility::base_payload);
            // the_factory_register.print(std::cerr, "DEVEL:");
            DT_THROW_IF(!the_factory_register.has(payload_type_id),
                        std::runtime_error,
                        "Unknown payload type identifier '" << payload_type_id << "'!");
            const vire::utility::base_payload::factory_register_type::factory_type & the_factory
              = the_factory_register.get(payload_type_id);
            vire::utility::base_payload * payload_ptr = the_factory();
            DT_THROW_IF(payload_ptr == nullptr, std::logic_error,
                        "Cannot instantiate a payload object of type '" << payload_type_id << "'!");
            payload_ptr->protobufize(payload_item_node);
            set_payload(payload_ptr);
          }
        }
      }
      return;
    }

  } // namespace message

} // namespace vire

namespace jsontools {

  // static
  void converter<vire::message::message_body>::jsonize(node & node_,
                                                       vire::message::message_body & mb_)
  {
    node_.grab_value() = Json::objectValue;
    if (mb_.has_payload_type_id()) {
      {
        Json::Value & idValue = node_.grab_value()["payload_type_id"];
        node_value newNode(idValue, node_.is_serializing(), false);
        newNode % const_cast<vire::utility::model_identifier &>(mb_.get_payload_type_id());
      }
      if (mb_.has_payload()) {
        Json::Value & payloadValue = node_.grab_value()["payload"];
        node_value newNode(payloadValue, node_.is_serializing(), false);
        vire::utility::base_payload * bp
          = const_cast<vire::utility::base_payload *>(mb_.get_payload());
        bp->jsonize(newNode);
        // newNode % const_cast<vire::utility::base_payload &>(*mb_.get_payload());
      }
    }

    return;
  }

  // static
  void converter<vire::message::message_body>::dejsonize(node & node_,
                                                           vire::message::message_body & mb_)
  {
    if (not node_.get_value().isObject()) {
      throw wrong_type(node_.get_value(), "Expected object!");
    }

    std::string payload_type_id_name;
    // Not used: int payload_type_id_version = 0;
    {
      Json::Value & idValue = node_.grab_value()["payload_type_id"];
      node_value newNode(idValue, node_.is_serializing(), false);
      newNode % const_cast<vire::utility::model_identifier &>(mb_.get_payload_type_id());
      payload_type_id_name = mb_.get_payload_type_id().get_name();
    }

    {
      Json::Value & payloadValue = node_.grab_value()["payload"];
      node_value newNode(payloadValue, node_.is_serializing(), false);
      vire::utility::base_payload::factory_register_type & the_factory_register =
        DATATOOLS_FACTORY_GRAB_SYSTEM_REGISTER(vire::utility::base_payload);
      // the_factory_register.print(std::cerr, "DEVEL:");
      DT_THROW_IF(!the_factory_register.has(payload_type_id_name),
                  std::runtime_error,
                  "Unknown payload type identifier '" << payload_type_id_name << "'!");
      const vire::utility::base_payload::factory_register_type::factory_type & the_factory
                = the_factory_register.get(payload_type_id_name);
      vire::utility::base_payload * bp = the_factory();
      bp->jsonize(newNode);
      mb_.set_payload(bp);
    }

    return;
  }

} // namespace jsontools
