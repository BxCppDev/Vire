//! \file  vire/message/message_body-inl.h
//! \brief Message body implementation
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

#ifndef VIRE_MESSAGE_MESSAGE_BODY_INL_H
#define VIRE_MESSAGE_MESSAGE_BODY_INL_H

namespace vire {

  namespace message {

    // Template methods of the message_body class:

    // Check if the stored payload is of given type
    template<class T>
    bool message_body::payload_is_a() const
    {
      const std::type_info& ti = typeid(T);
      const vire::utility::base_payload & cur = *_payload_;
      const std::type_info& tf = typeid(cur);
      return (ti == tf);
    }

    // Return a reference to a non mutable payload of given type
    template<class T>
    const T & message_body::get_payload_as() const
    {
      const std::type_info & ti = typeid(T);
      const vire::utility::base_payload & cur = *_payload_;
      const std::type_info & tf = typeid(cur);
      static const T tmp;
      DT_THROW_IF(ti != tf, std::logic_error,
                  "Request type '" << ti.name() << "' ('" << tmp.get_serial_tag()
                  << "') does not match the type '"
                  << tf.name() << "' of the stored payload object ('"
                  << _payload_->get_serial_tag() << "') !");
      return *(dynamic_cast<const T*>(_payload_));
    }

  } // namespace message

} // namespace vire

#endif // VIRE_MESSAGE_MESSAGE_BODY_INL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
