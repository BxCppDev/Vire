//! \file  vire/message/message_identifier.h
//! \brief Message identifier
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

#ifndef VIRE_MESSAGE_MESSAGE_IDENTIFIER_H
#define VIRE_MESSAGE_MESSAGE_IDENTIFIER_H

// Standard library:
#include <string>

// Third party:
// - Boost:
#include <boost/stdint.hpp>

// This project:
#include <vire/utility/base_payload.h>

namespace vire {

  namespace message {

    //! \brief Message identifier
    class message_identifier
      : public ::vire::utility::base_payload
    {
    public:

      static const int32_t INVALID_NUMBER = -1;

      //! Return a static invalid message id
      static const message_identifier & invalid_id();

      //! Default constructor
      message_identifier();

      //! Constructor
      message_identifier(const std::string & emitter_, int32_t number_);

      //! Destructor
      ~message_identifier();

      //! Set the emitter
      void set_emitter(const std::string & name_);

      //! Return the emitter
      const std::string & get_emitter() const;

      //! Set the message number
      void set_number(int32_t);

      //! Return the message number
      int32_t get_number() const;

      //! Reset
      void reset();

      //! Check message validity
      bool is_valid() const;

      /// Convert to string representation
      void to_string(std::string & format_) const;

      /// Convert to string representation
      std::string to_string() const;

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

      std::string _emitter_; //!< Name of the emitter
      int32_t     _number_;  //!< Message unique number with respect from the emitter

      //! Payload interface
      VIRE_UTILITY_PAYLOAD_INTERFACE(message_header)

    };

  } // namespace message

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::message::message_identifier, "vire::message::message_identifier")

#endif // VIRE_MESSAGE_MESSAGE_IDENTIFIER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
