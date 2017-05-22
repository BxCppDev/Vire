//! \file  vire/message/message.h
//! \brief Message
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

#ifndef VIRE_MESSAGE_MESSAGE_H
#define VIRE_MESSAGE_MESSAGE_H

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/i_serializable.h>
// - BxJsontools:
#include <bayeux/jsontools/i_jsonizable.h>
// - Bxprotobuftools:
#include <bayeux/protobuftools/i_protobufable.h>

// This project:
#include <vire/message/message_header.h>
#include <vire/message/message_body.h>

namespace vire {

  namespace message {

    //! \brief Message
    class message :
      public ::datatools::i_serializable,
      public ::jsontools::i_jsonizable,
      public ::protobuftools::i_protobufable,
      public ::datatools::i_tree_dumpable
    {
    public:

      //! Default constructor
      message();

      //! Destructor
      virtual ~message();

      //! Reset
      void reset();

      // Check validity
      bool is_valid() const;

      //! Set the message header
      void set_header(const message_header &);

      //! Return the message header
      const message_header & get_header() const;

      //! Return the message header
      message_header & grab_header();

      //! Set the message body
      void set_body(const message_body &);

      //! Return the message body
      const message_body & get_body() const;

      //! Return the message body
      message_body & grab_body();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_);

      //! Support for Boost-based serialization
      DATATOOLS_SERIALIZATION_DECLARATION_ADVANCED(message);

    private:

      message_header _header_; //! Message header
      message_body   _body_;   //! Message body

    };

  } // namespace message

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::message::message, "vire::message::message")

// // Explicit class version:
// #include <boost/serialization/version.hpp>
// BOOST_CLASS_VERSION(vire::message::message, 1)

#endif // VIRE_MESSAGE_MESSAGE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
