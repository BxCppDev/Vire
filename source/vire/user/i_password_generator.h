//! \file  vire/user/i_password_generator.h
//! \brief Interface for password generators
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_USER_I_PASSWORD_GENERATOR_H
#define VIRE_USER_I_PASSWORD_GENERATOR_H

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <datatools/bit_mask.h>

namespace vire {

  namespace user {

    struct i_password_generator
    {

      enum charset_type {
	CHARSET_ALPHA    = datatools::bit_mask::bit00,
	CHARSET_NUMERIC  = datatools::bit_mask::bit01,
	CHARSET_PUNCT    = datatools::bit_mask::bit02,
	CHARSET_ALPHANUM = CHARSET_ALPHA | CHARSET_NUMERIC,
 	CHARSET_ALL      = CHARSET_ALPHANUM | CHARSET_PUNCT,
 	CHARSET_DEFAULT  = CHARSET_ALL
      };

      static const std::string & alpha_charset();
      static const std::string & numeric_charset();
      static const std::string & punct_charset();
      static std::string charset(uint32_t flags_);

      static const std::size_t DEFAULT_LENGTH = 8;

      virtual void generate_password(std::string & password_,
				     const std::size_t length_ = DEFAULT_LENGTH) = 0;

    };

  } // namespace user

} // namespace vire

#endif // VIRE_USER_I_PASSWORD_GENERATOR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
