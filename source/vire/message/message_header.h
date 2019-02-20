//! \file  vire/message/message_header.h
//! \brief Message header
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

#ifndef VIRE_MESSAGE_MESSAGE_HEADER_H
#define VIRE_MESSAGE_MESSAGE_HEADER_H

// Standard library:
#include <string>
#include <iostream>
#include <vector>

// Third party:
// - Boost:
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/i_serializable.h>
// - BxJsontools:
#include <bayeux/jsontools/i_jsonizable.h>
// - Bxprotobuftools:
#include <bayeux/protobuftools/i_protobufable.h>

// This project:
#include <vire/message/message_identifier.h>
#include <vire/message/protocol_utils.h>
#include <vire/utility/model_identifier.h>
#include <vire/utility/metadata_record.h>

namespace vire {

  namespace message {

    //! \brief Message header
    class message_header
      : public ::datatools::i_serializable
      , public ::jsontools::i_jsonizable
      , public ::protobuftools::i_protobufable
      , public ::datatools::i_tree_dumpable
    {
    public:

      //! Default constructor
      message_header();

      //! Destructor
      ~message_header();

      //! Reset
      void reset();

      // Check validity
      bool is_valid() const;

      // Check if the message identifier is set
      bool has_message_id() const;

      //! Set the message identifier
      void set_message_id(const message_identifier &);

      //! Return the message identifier
      const message_identifier & get_message_id() const;

      //! Set the body format identifier
      void set_body_layout_id(const vire::utility::model_identifier &);

      //! Return the body format identifier
      const vire::utility::model_identifier & get_body_layout_id() const;

      //! Set the UTC timestamp
      void set_timestamp(const boost::posix_time::ptime &);

      //! Return the UTC timestamp
      const boost::posix_time::ptime & get_timestamp() const;

      /// Check if the category of the message is set
      bool has_category() const;
      
      /// Set the category of the message
      void set_category(const message_category);

      /// Return the category of the message
      message_category get_category() const;

      /// Check if the message is a request
      bool is_request() const;

      /// Check if the message is a response
      bool is_response() const;

      /// Check if the message is an event
      bool is_event() const;

      //! Check if 'in-reply-to' message ID is set
      bool has_in_reply_to() const;

      //! Set the 'in-reply-to' message identifier
      void set_in_reply_to(const message_identifier &);

      //! Return the 'in-reply-to' message identifier
      const message_identifier & get_in_reply_to() const;

      //! Check if the message is asynchronous
      bool is_asynchronous() const;

      //! Set the asynchronous flag
      void set_asynchronous(bool);

      //! Check if 'async' address is set
      bool has_async_address() const;

      //! Set the 'async' address
      void set_async_address(const std::string &);

      //! Return the 'async' address
      const std::string & get_async_address() const;

      //! Reset the asynchronous flag and address
      void reset_async();

      //! Add an boolean metadata
      void add_metadata(const std::string & key_, const bool value_);

      //! Add an integer metadata
      void add_metadata(const std::string & key_, const int value_);

      //! Add a real metadata
      void add_metadata(const std::string & key_, const double value_);

      //! Add a string metadata
      void add_metadata(const std::string & key_, const std::string & value_);

      //! Add a string metadata
      void add_metadata(const std::string & key_, const char * value_);

      //! Check if a metadata exists
      bool has_metadata(const std::string & key_) const;

      //! Return the representation of a metadata value
      const std::string & get_metadata_value_repr(const std::string & key_) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_);

    private:

      message_identifier                  _message_id_;       //!< Message identifier
      boost::posix_time::ptime            _timestamp_;        //!< Message timestamp (UTC)
      message_category                    _category_;         //!< Message category
      boost::optional<message_identifier> _in_reply_to_;      //!< Reference to an original request message (for a message which wraps a 'response' or 'event' payload)
      bool                                _asynchronous_;     //!< Asynchronous flag
      std::string                         _async_address_;    //!< Asynchronous address
      vire::utility::model_identifier     _body_layout_id_;   //!< Body layout identifier
      std::vector<vire::utility::metadata_record> _metadata_; //!< List of key/value metadata pairs

      //! Support for Boost-based serialization
      DATATOOLS_SERIALIZATION_DECLARATION_ADVANCED(message_header)

    };

  } // namespace message

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::message::message_header, "vire::message::message_header")

#endif // VIRE_MESSAGE_MESSAGE_HEADER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
