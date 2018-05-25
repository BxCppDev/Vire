// vire/utility/base_alarm.cc
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/utility/base_alarm.h>

// Third party:
// - BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
#include <bayeux/jsontools/boost_type_converters.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/std_type_converters.h>
#include <bayeux/protobuftools/boost_datetime_converters.h>
#include <bayeux/protobuftools/protobuf_factory.h>

// Declare a protobuf registrar instance for the message class:
#include "vire/utility/BaseAlarm.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::base_alarm",
                               vire::utility::BaseAlarm)

namespace vire {

  namespace utility {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(base_alarm, "vire::utility::base_alarm")

    base_alarm::base_alarm()
    {
      return;
    }

    base_alarm::base_alarm(const boost::posix_time::ptime & t_,
                           const std::string & severity_,
                           const std::string & message_)
    {
      set_timestamp(t_);
      set_severity(severity_);
      set_message(message_);
      return;
    }

    base_alarm::base_alarm(const std::string & severity_,
                           const std::string & message_)
      : base_alarm(vire::time::now_utc(), severity_, message_)
    {
      return;
    }

    base_alarm::~base_alarm()
    {
      return;
    }

    bool base_alarm::has_timestamp() const
    {
      return vire::time::is_valid(_timestamp_);
    }

    void base_alarm::set_timestamp(const boost::posix_time::ptime & t_)
    {
      _timestamp_ = t_;
      return;
    }

    void base_alarm::reset_timestamp()
    {
      vire::time::invalidate_time(_timestamp_);
      return;
    }

    const boost::posix_time::ptime & base_alarm::get_timestamp() const
    {
      return _timestamp_;
    }

    bool base_alarm::has_severity() const
    {
      return !_severity_.empty();
    }

    void base_alarm::set_severity(const std::string & s_)
    {
      _severity_ = s_;
      return;
    }

    const std::string & base_alarm::get_severity() const
    {
      return _severity_;
    }

    bool base_alarm::has_message() const
    {
      return !_message_.empty();
    }

    void base_alarm::set_message(const std::string & m_)
    {
      _message_ = m_;
      return;
    }

    const std::string & base_alarm::get_message() const
    {
      return _message_;
    }

    void base_alarm::jsonize(jsontools::node & node_,
                             const unsigned long int version_)
    {
      this->base_payload::jsonize(node_, version_);
      node_["timestamp"] % _timestamp_;
      node_["severity"] % _severity_;
      node_["message"] % _message_;
      return;
    }

    void base_alarm::protobufize(protobuftools::message_node & node_,
                                 const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_payload, node_);
      node_["timestamp"] % _timestamp_;
      node_["severity"] % _severity_;
      node_["message"] % _message_;
      return;
    }

    void base_alarm::tree_dump(std::ostream & out_,
                                 const std::string & title_,
                                 const std::string & indent_,
                                 bool inherit_) const
    {
      this->base_payload::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Timestamp : ";
      if (has_timestamp()) {
        out_ << '[' << vire::time::to_string(_timestamp_) << ']';
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Severity : ";
      if (has_severity()) {
        out_ << "'" << _severity_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Message : ";
      if (has_message()) {
        out_ << "'" << _message_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // namespace utility

} // namespace vire
