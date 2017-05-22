// vire/utility/base_event.cc
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
#include <vire/utility/base_event.h>

// Third party:
// - BxJsontools:
#include <bayeux/jsontools/boost_type_converters.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/boost_datetime_converters.h>
#include <bayeux/protobuftools/protobuf_factory.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/utility/BaseEvent.pb.h"

BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::base_event",
                               vire::utility::BaseEvent)

namespace vire {

  namespace utility {

    base_event::base_event()
    {
      return;
    }

    base_event::base_event(const boost::posix_time::ptime & t_)
    {
      set_timestamp(t_);
      return;
    }

    base_event::~base_event()
    {
      return;
    }

    // virtual
    base_payload::payload_category base_event::get_category() const
    {
      return CATEGORY_EVENT;
    }

    bool base_event::has_timestamp() const
    {
      return vire::time::is_valid(_timestamp_);
    }

    void base_event::set_timestamp(const boost::posix_time::ptime & t_)
    {
      _timestamp_ = t_;
      return;
    }

    void base_event::reset_timestamp()
    {
      vire::time::invalidate_time(_timestamp_);
      return;
    }

    const boost::posix_time::ptime & base_event::get_timestamp() const
    {
      return _timestamp_;
    }

    void base_event::jsonize(jsontools::node & node_,
                             const unsigned long int version_)
    {
      this->base_payload::jsonize(node_, version_);
      node_["timestamp"] % _timestamp_;
      return;
    }

    void base_event::protobufize(protobuftools::message_node & node_,
                                 const unsigned long int version_)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_payload,node_);
      node_["timestamp"] % _timestamp_;
      return;
    }

    void base_event::tree_dump(std::ostream & out_,
                                 const std::string & title_,
                                 const std::string & indent_,
                                 bool inherit_) const
    {
      this->base_payload::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Timestamp : ";
      if (has_timestamp()) {
        out_ << '[' << vire::time::to_string(_timestamp_) << ']';
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // namespace utility

} // namespace vire
