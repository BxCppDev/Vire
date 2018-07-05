//! \file  vire/com/i_encoding_driver.h
//! \brief Vire com encoding driver interface
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

#ifndef VIRE_COM_I_ENCODING_DRIVER_H
#define VIRE_COM_I_ENCODING_DRIVER_H

// Standard library:
#include <vector>
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/factory_macros.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/logger.h>

// This project:
#include <vire/com/utils.h>
#include <vire/message/message.h>

namespace datatools {
  class properties;
}

namespace vire {

  namespace com {

    //! \brief Encoding driver abstract interface
    //!
    //! An encoding driver is responsible to encode a Vire message (header + payload)
    //! in a raw message data structure (buffer + metadata). It also performs decoding
    //! in the other direction.
    //!
    //! Concrete encoding driver classes must implement 4 protected methods to fulfill
    //! the interface.
    //!
    class i_encoding_driver
    {
    public:

      //! Default constructor
      i_encoding_driver();

      //! Destructor
      virtual ~i_encoding_driver();

      //! Return the logging priority threshold
      datatools::logger::priority get_logging() const;

      //! Set the logging priority threshold
      void set_logging(const datatools::logger::priority p_);

      //! Check initialization flag
      bool is_initialized() const;

      //! Initialize the driver
      void initialize_simple();

      //! Initialize the driver from a set of configuration parameters
      void initialize(const datatools::properties &);

      //! Reset the driver
      void reset();

      //! Encode a message object to a target raw message
      int encode(const vire::message::message & msg_, raw_message_type & msg_data_) const;

      //! Decode a message object from a raw message
      int decode(const raw_message_type & msg_data_, vire::message::message & msg_) const;

      //! Return the class unique identifier
      std::string class_guid() const;

    private:

      //! Initialization
      virtual std::string _class_guid_() const = 0;

      //! Initialization
      virtual void _initialize_impl_(const datatools::properties &) = 0;

      //! Reset
      virtual void _reset_impl_() = 0;

      //! Encode a message object to a raw message
      virtual int _encode_impl_(const vire::message::message & msg_,
                                raw_message_type & raw_msg_) const = 0;

      //! Decode a message object from a raw message
      virtual int _decode_impl_(const raw_message_type & raw_msg_,
                                vire::message::message & msg_) const = 0;

    private:

      bool _initialized_ = false;            //!< Initialization flag
      datatools::logger::priority _logging_; //!< Logging priority threshold

      // Factory stuff :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(i_encoding_driver)

    };

    typedef std::shared_ptr<i_encoding_driver> encoding_driver_ptr; 

  } // namespace com

} // namespace vire

#define VIRE_COM_ENCODING_DRIVER_REGISTRATION_INTERFACE(EncodingClass)  \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::vire::com::i_encoding_driver, EncodingClass) \
  /**/

#define VIRE_COM_ENCODING_DRIVER_REGISTRATION_IMPLEMENTATION(EncodingClass,EncodingId) \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(::vire::com::i_encoding_driver,EncodingClass,EncodingId) \
  /**/

#endif // VIRE_COM_I_ENCODING_DRIVER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
