//! \file  vire/message/message_body.h
//! \brief Message body
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

#ifndef VIRE_MESSAGE_MESSAGE_BODY_H
#define VIRE_MESSAGE_MESSAGE_BODY_H

// Third party:
// - Boost:
#include <boost/utility.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/i_serializable.h>
// - BxJsontools:
#include <jsontools/core.h>
// - Bxprotobuftools:
#include <protobuftools/core.h>


// This project:
#include <vire/utility/model_identifier.h>
#include <vire/utility/base_payload.h>

namespace vire {

  namespace message {

    //! \brief Message body
    //!
    //! The message body contains the user data to be transmitted to some destination.
    //! It handles a 'payload' object with a conventional type signature represented
    //! by a 'vire::utility::model_identifier' instance.
    //! The payload object is handled through a plain pointer on a
    //! 'const vire::utility::base_payload' object. In that way, it is not possible to
    //! modify a payload object published through a message.
    class message_body
      : public ::datatools::i_serializable
      , public ::protobuftools::i_protobufable
      , public ::datatools::i_tree_dumpable
    {
    public:

      //! Default constructor
      message_body();

      //! Copy constructor
      message_body(const message_body & msg_body_);

      //! Destructor
      virtual ~message_body();

      //! Assignment operator
      message_body & operator=(const message_body & msg_body_);

      // Check validity
      bool is_valid() const;

      //! Reset
      void reset();

      //! Check if payload object handle is set
      bool has_payload() const;

      //! Set the payload object handle
      //! This method automatically computes the payload type ID if not set
      void set_payload(const vire::utility::base_payload *);

      //! Set the payload object by copy
      //! This method automatically computes the payload type ID if not set
      void set_payload(const vire::utility::base_payload &);

      //! Return the payload object handle
      const vire::utility::base_payload & get_payload_ref() const;

      //! Return the payload object shared pointer
      const vire::utility::base_payload * get_payload() const;

      //! Remove the payload object
      void remove_payload();

      //! Check if payload object type ID is set
      bool has_payload_type_id() const;

      //! Set the payload object type ID
      void set_payload_type_id(const vire::utility::model_identifier & id_);

      //! Return the payload object type ID
      const vire::utility::model_identifier & get_payload_type_id() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      /// Check if the stored payload is of given type
      template<class T>
      bool payload_is_a() const;

      /// Return a reference to a non mutable payload of given type
      template<class T>
      const T & get_payload_as() const;

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_);

    private:

      /// Private copy using the cloneable interface of the payload objects
      void _copy_(const message_body & msg_body_);

    private:

      vire::utility::model_identifier     _payload_type_id_;   //!< Payload type identifier
      const vire::utility::base_payload * _payload_ = nullptr; //!< Handle to the payload object

      //! Support for Boost-based serialization
      DATATOOLS_SERIALIZATION_DECLARATION_ADVANCED(message_body)

    };

  } // namespace message

} // namespace vire

namespace jsontools {

  template<>
  class converter<vire::message::message_body>
  {
  public:

    static void jsonize(node & node_, vire::message::message_body & mb_);

    static void dejsonize(node & node_, vire::message::message_body & mb_);

  };

} // namespace jsontools

// Template methods
#include <vire/message/message_body-inl.h>

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::message::message_body, "vire::message::message_body")

#endif // VIRE_MESSAGE_MESSAGE_BODY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
