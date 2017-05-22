//! \file  vire/utility/json_encoding_driver.h
//! \brief Vire utility JSON encoding driver
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

#ifndef VIRE_COM_JSON_ENCODING_DRIVER_H
#define VIRE_COM_JSON_ENCODING_DRIVER_H

// Standard Library:
#include <locale>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

// This project:
#include <vire/com/i_encoding_driver.h>

namespace vire {

  namespace com {

    //! \brief JSON encoding driver
    class json_encoding_driver
      : public i_encoding_driver
    {
    public:

      //! Default constructor
      json_encoding_driver();

      //! Destructor
      virtual ~json_encoding_driver();

    private:

      //! Initialization
      virtual void _initialize_impl_(const datatools::properties &);

      //! Reset
      virtual void _reset_impl_();

      //! Encode a source object to a target string buffer
      virtual int _encode_impl_(const vire::message::message & msg_,
                               std::vector<char> & buffer_);

      //! Decode a target object from a source string buffer
      virtual int _decode_impl_(const std::vector<char> & buffer_,
                                vire::message::message & msg_);

    private:

      std::locale * _default_locale_ = nullptr; //!< Handle to the default locale
      std::locale * _out_locale_     = nullptr; //!< Handle to the effective output locale
      std::locale * _in_locale_      = nullptr; //!< Handle to the effective input locale

      VIRE_COM_ENCODING_DRIVER_REGISTRATION_INTERFACE(json_encoding_driver);

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_JSON_ENCODING_DRIVER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
