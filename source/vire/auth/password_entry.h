//! \file  vire/auth/password_entry.h
//! \brief Password input
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_AUTH_PASSWORD_ENTRY_H
#define VIRE_AUTH_PASSWORD_ENTRY_H

// Standard libraries:
#include <string>

namespace vire {

  namespace auth {

    class password_entry
    {
    public:
      static const size_t BUFFER_SIZE = 256;
      password_entry();
      ~password_entry();
      std::string operator()(const std::string & message_);
    private:
      char _password_[BUFFER_SIZE];
    };

  } // namespace auth

} // namespace vire

#endif // VIRE_AUTH_PASSWORD_ENTRY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
