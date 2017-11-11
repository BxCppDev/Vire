//! \file vire/user/i_password_generator.cc
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

// Ourselves:
#include <vire/user/i_password_generator.h>

// Standard library:
#include <chrono>

namespace vire {

  namespace user {

    // static
    const std::string & i_password_generator::alpha_charset()
    {
      static const std::string _cs("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
      return _cs;
    }

    // static
    const std::string & i_password_generator::numeric_charset()
    {
      static const std::string _cs("012334567890");
      return _cs;
    }

    // static
    const std::string & i_password_generator::punct_charset()
    {
      static const std::string _cs("&~-_^@=+$*,?;./!");
      return _cs;
    }

    // static
    std::string i_password_generator::charset(uint32_t flags_)
    {
      std::string s;
      if (flags_ & CHARSET_ALPHA) s += alpha_charset();
      if (flags_ & CHARSET_NUMERIC) s += numeric_charset();
      if (flags_ & CHARSET_PUNCT) s += punct_charset();
      if (s.empty()) s = alpha_charset();
      return s;
    }

  } // namespace user

} // namespace vire
