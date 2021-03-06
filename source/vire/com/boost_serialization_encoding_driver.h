//! \file  vire/utility/boost_serialization_encoding_driver.h
//! \brief Vire utility boost serialization encoding driver
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

#ifndef VIRE_COM_BOOST_SERIALIZATION_ENCODING_DRIVER_H
#define VIRE_COM_BOOST_SERIALIZATION_ENCODING_DRIVER_H

// Standard Library:
#include <locale>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

// This project:
#include <vire/com/i_encoding_driver.h>

namespace vire {

  namespace com {

    //! \brief Boost serialization encoding driver
    class boost_serialization_encoding_driver
      : public i_encoding_driver
    {
    public:

      /// \brief Type of Boost archive
      enum archive_format_type {
        AR_XML  = 0, ///< XML archive format
        AR_TEXT = 1, ///< Text archive format
        AR_PBA  = 2  ///< Portable Binary archive format
      };

      //! Default constructor
      boost_serialization_encoding_driver();

      //! Destructor
      virtual ~boost_serialization_encoding_driver();

      void set_archive_format(const archive_format_type);

      archive_format_type get_archive_format() const;

    private:

      //! Initialization
      std::string _class_guid_() const override;

      //! Initialization
      void _initialize_impl_(const datatools::properties &) override;

      //! Reset
      void _reset_impl_() override;

      //! Encode a message object to a raw message
      int _encode_impl_(const vire::message::message & msg_,
                        raw_message_type & raw_msg_) const override;

      //! Decode a message object from a raw message
      int _decode_impl_(const raw_message_type & raw_msg__,
                        vire::message::message & msg_) const override;

    private:

      archive_format_type _archive_format_ = AR_XML;  //!< Archive type tag
      std::locale *       _default_locale_ = nullptr; //!< Handle to the default locale
      std::locale *       _out_locale_     = nullptr; //!< Handle to the effective output locale
      std::locale *       _in_locale_      = nullptr; //!< Handle to the effective input locale

      VIRE_COM_ENCODING_DRIVER_REGISTRATION_INTERFACE(boost_serialization_encoding_driver);

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_BOOST_SERIALIZATION_ENCODING_DRIVER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
