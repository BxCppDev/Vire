// vire/message/message_header.cc
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
#include <vire/message/message_header.h>

// Third party:
// - BxJsontools:
#include <jsontools/base_type_converters.h>
#include <jsontools/std_type_converters.h>
#include <jsontools/boost_type_converters.h>

// Bayeux/protobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>
#include <bayeux/protobuftools/boost_datetime_converters.h>
#include <bayeux/protobuftools/protobufable_converter.h>
#include <bayeux/protobuftools/enum_converter.h>

// This project:
#include <vire/time/utils.h>

// Declare a protobuf registrar instance for the MessageHeader class:
#include "vire/message/MessageHeader.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::message::message_header", vire::message::MessageHeader)

namespace vire {

  namespace message {

    DATATOOLS_SERIALIZATION_IMPLEMENTATION_ADVANCED(message_header, "vire::message::message_header")

    message_header::message_header()
    {
      _message_id_ = message_identifier::invalid_id();
      _timestamp_ = vire::time::invalid_time();
      _category_ = MESSAGE_INVALID;
      _in_reply_to_ = boost::none;
      _asynchronous_ = false;
      return;
    }

    message_header::~message_header()
    {
      return;
    }

    void message_header::reset()
    {
      _message_id_.reset();
      _timestamp_ = vire::time::invalid_time();
      _category_ = MESSAGE_INVALID;
      _in_reply_to_ = boost::none;
      _asynchronous_ = false;
      _async_address_.clear();
      _body_layout_id_.reset();
      _metadata_.clear();
      return;
    }

    bool message_header::is_valid() const
    {
      if (! _message_id_.is_valid()) return false;
      if (_category_ == MESSAGE_INVALID) return false;
      if (! _body_layout_id_.is_valid()) return false;
      if (! ::vire::time::is_valid(_timestamp_)) return false;
      if (_in_reply_to_) {
        if (!_in_reply_to_.get().is_valid()) return false;
      }
      if (_asynchronous_ && _async_address_.empty()) return false;
      return true;
    }

    void message_header::set_message_id(const message_identifier & mid_)
    {
      _message_id_ = mid_;
      return;
    }

    const message_identifier & message_header::get_message_id() const
    {
      return _message_id_;
    }

    void message_header::set_body_layout_id(const vire::utility::model_identifier & fid_)
    {
      _body_layout_id_ = fid_;
      return;
    }

    const vire::utility::model_identifier & message_header::get_body_layout_id() const
    {
      return _body_layout_id_;
    }

    void message_header::set_timestamp(const boost::posix_time::ptime & ts_)
    {
      _timestamp_ = ts_;
      return;
    }

    const boost::posix_time::ptime & message_header::get_timestamp() const
    {
      return _timestamp_;
    }

    void message_header::set_category(const message_category cat_)
    {
      _category_ = cat_;
      return;
    }

    message_category message_header::get_category() const
    {
      return _category_;
    }

    bool message_header::is_request() const
    {
      return _category_ == MESSAGE_REQUEST;
    }

    bool message_header::is_response() const
    {
      return _category_ == MESSAGE_RESPONSE;
    }

    bool message_header::is_event() const
    {
      return _category_ == MESSAGE_EVENT;
    }

    bool message_header::has_in_reply_to() const
    {
      if (_in_reply_to_) return true;
      return false;
    }

    void message_header::set_in_reply_to(const message_identifier & irt_)
    {
      _in_reply_to_ = irt_;
      return;
    }

    const message_identifier & message_header::get_in_reply_to() const
    {
      return _in_reply_to_.get();
    }

    bool message_header::is_asynchronous() const
    {
      return _asynchronous_;
    }

    void message_header::set_asynchronous(bool async_)
    {
      _asynchronous_ = async_;
      return;
    }

    bool message_header::has_async_address() const
    {
      return !_async_address_.empty();
    }

    void message_header::set_async_address(const std::string & aa_)
    {
      if (! aa_.empty()) {
        set_asynchronous(true);
        _async_address_ = aa_;
      }
      return;
    }

    void message_header::reset_async()
    {
      set_asynchronous(false);
      _async_address_.clear();
      return;
    }

    const std::string & message_header::get_async_address() const
    {
      return _async_address_;
    }

    void message_header::add_metadata(const std::string & key_, const bool value_)
    {
      add_metadata(key_, (value_ ? "true" : "false"));
      return;
    }

    void message_header::add_metadata(const std::string & key_, const int value_)
    {
      add_metadata(key_, boost::lexical_cast<std::string>(value_));
      return;
    }

    void message_header::add_metadata(const std::string & key_, const double value_)
    {
      add_metadata(key_, boost::lexical_cast<std::string>(value_));
      return;
    }

    void message_header::add_metadata(const std::string & key_, const std::string & value_)
    {
      if (has_metadata(key_)) {
        vire::utility::metadata_record::has_key_predicate pred(key_);
        std::remove_if(_metadata_.begin(), _metadata_.end(), pred);
      }
      _metadata_.push_back(vire::utility::metadata_record(key_, value_));
      return;
    }

    void message_header::add_metadata(const std::string & key_, const char * value_)
    {
      add_metadata(key_, std::string(value_));
      return;
    }

    bool message_header::has_metadata(const std::string & key_) const
    {
      for (const auto & mdr : _metadata_) {
        if (vire::utility::metadata_record::has_key(mdr, key_)) return true;
      }
      return false;
    }

    const std::string & message_header::get_metadata_value_repr(const std::string & key_) const
    {
      vire::utility::metadata_record::has_key_predicate pred(key_);
      std::vector<vire::utility::metadata_record>::const_iterator found
        = std::find_if(_metadata_.begin(), _metadata_.end(), pred);
      DT_THROW_IF(found == _metadata_.end(), std::logic_error,
                  "No metadata with key '" << key_ << "'!");
      return found->get_value();
    }

    void message_header::tree_dump(std::ostream & out_,
                                   const std::string & title_,
                                   const std::string & indent_,
                                   bool inherit_) const
    {
      if (! title_.empty()) out_ << indent_ << title_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Message ID  : '" << _message_id_.to_string() << "'" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Timestamp   : [" << boost::posix_time::to_iso_string(_timestamp_) << ']'
           << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Category    : '" << message_category_label(_category_) << "'" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "In reply to : ";
      if (!has_in_reply_to()) {
        out_ << "<none>";
      } else {
        out_ << "'" << get_in_reply_to().to_string() << "'";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Asynchronous : " << std::boolalpha << _asynchronous_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Async address : '" << _async_address_ << "'" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Body layout ID : '" << _body_layout_id_.to_string() << "'" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Metadata : [" << _metadata_.size() << "]" << std::endl;
      {
        std::size_t counter = 0;
        for (std::vector<vire::utility::metadata_record>::const_iterator imd
               = _metadata_.begin();
             imd != _metadata_.end();
             imd++) {
          out_ << indent_ << ::datatools::i_tree_dumpable::skip_tag;
          if (++counter == _metadata_.size()) {
            out_ << ::datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << ::datatools::i_tree_dumpable::tag;
          }
          out_ << "Key: '" << imd->get_key() << "' : Value='" << imd->get_value() << "'";
          out_ << std::endl;
        }
      }

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Valid : " << std::boolalpha << is_valid() << std::endl;

      return;
    }

    void message_header::jsonize(jsontools::node & node_,
                                   unsigned long int version_)
    {
      node_["message_id"] % _message_id_;
      node_["timestamp"] % _timestamp_;
      node_["category"] % _category_;
      node_["in_reply_to"] % _in_reply_to_;
      node_["asynchronous"] % _asynchronous_;
      if (_asynchronous_) {
        node_["async_address"] % _async_address_;
      }
      node_["body_layout_id"] % _body_layout_id_;
      node_["metadata"] % _metadata_;
      return;
    }

    void message_header::protobufize(protobuftools::message_node & node_,
                                     const unsigned long int /* version_ */)
    {
      node_["message_id"] % _message_id_;
      node_["timestamp"] % _timestamp_;
      node_["category"] % _category_;
      if (node_.is_serializing()) {
        if (has_in_reply_to()) {
          node_["in_reply_to"] % _in_reply_to_.get();
        }
      } else {
        message_identifier in_reply_to;
        if (node_.has_field("in_reply_to")) {
          node_["in_reply_to"] % in_reply_to;
          if (in_reply_to.is_valid()) {
            _in_reply_to_ = in_reply_to;
          }
        }
      }
      node_["asynchronous"] % _asynchronous_;
      if (_asynchronous_) {
        node_["async_address"] % _async_address_;
      }
      node_["body_layout_id"] % _body_layout_id_;
      if (node_.is_serializing()) {
        if (_metadata_.size()) {
          node_["metadata"] % _metadata_;
        }
      } else {
        if (node_.has_field("metadata")) {
          node_["metadata"] % _metadata_;
        }
      }
      return;
    }

  } // namespace message

} // namespace vire
