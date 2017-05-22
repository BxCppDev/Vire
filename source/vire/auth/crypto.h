//! \file  vire/auth/crypto.h
//! \brief Vire auth encryption tools
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

#ifndef VIRE_AUTH_CRYPTO_H
#define VIRE_AUTH_CRYPTO_H

// Standard library:
#include <string>

namespace vire {

  namespace auth {

    struct sha512 {

      /// Extract the salt from a encrypted token
      static bool extract_salt(const std::string & encrypted_, std::string & salt_);

      enum encryption_flags {
        encrypt_clear_key  = 0x1,
        encrypt_force_salt = 0x2,
        encrypt_debug      = 0x4
      };

      /// Encryption of a string using the SHA-512 method
      static bool encrypt(const std::string & key_,
                          std::string & encrypted_,
                          uint32_t flags_ = 0,
                          const std::string & salt_ = "");

      /// Check if a plain token matches a encrypted string
      static bool match(const std::string & key_, const std::string & encrypted_);

    };

  } // namespace auth

} // namespace vire

#endif // VIRE_AUTH_CRYPTO_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
