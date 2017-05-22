//! \file  vire/utility/base_alarm.h
//! \brief Simple base alarm object
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

#ifndef VIRE_UTILITY_BASE_ALARM_H
#define VIRE_UTILITY_BASE_ALARM_H

// Standard library
#include <string>
#include <iostream>

// Third party
// - Boost
#include <boost/cstdint.hpp>

// This project
#include <vire/utility/base_event.h>

namespace vire {

  namespace utility {

    /// \brief Simple base alarm object
    class base_alarm : public base_event
    {
    public:

      /// Default constructor
      base_alarm();

      /// Constructor
      base_alarm(const boost::posix_time::ptime & t_,
                 const std::string & severity_, const std::string & message_);

      /// Constructor
      base_alarm(const std::string & severity_, const std::string & message_);

      /// Destructor
      virtual ~base_alarm();

      /// Check if severity is set
      bool has_severity() const;

      /// Set the severity
      void set_severity(const std::string &);

      /// Return the severity
      const std::string & get_severity() const;

      /// Check if message is set
      bool has_message() const;

      /// Set the message
      void set_message(const std::string &);

      /// Return the message
      const std::string & get_message() const;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      std::string _severity_; //!< Alarm severity
      std::string _message_;  //!< Alarm message

      VIRE_UTILITY_PAYLOAD_INTERFACE(base_alarm)

    };

  } // namespace utility

} // namespace vire

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif
#include <boost/serialization/export.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif
BOOST_CLASS_EXPORT_KEY2(vire::utility::base_alarm,"vire::utility::base_alarm")

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::utility::base_alarm, "vire::utility::base_alarm")

#endif // VIRE_UTILITY_BASE_ALARM_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
