//! \file  vire/user/simple_password_generator.h
//! \brief Simple password geenrator
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

#ifndef VIRE_USER_SIMPLE_PASSWORD_GENERATOR_H
#define VIRE_USER_SIMPLE_PASSWORD_GENERATOR_H

// Standard library:
#include <random>
#include <memory>

// Third party:
// - Boost:
#include <boost/noncopyable.hpp>

// This project:
#include <vire/user/i_password_generator.h>

namespace vire {

  namespace user {

    class simple_password_generator
      : public i_password_generator
      , private boost::noncopyable
    {
    public:

      simple_password_generator(uint32_t charset_ = CHARSET_DEFAULT,
                                const unsigned int seed_ = 1);

      virtual void generate_password(std::string & password_,
                                     const std::size_t length_ = DEFAULT_LENGTH);

      const std::string & get_charset() const;

      unsigned int get_seed() const;

    private:

      std::default_random_engine & _grab_generator_();

    private:

      // Configuration:
      std::string _charset_;
      unsigned int _seed_ = 0;

      // Working data:
      std::unique_ptr<std::default_random_engine> _generator_;

    };

  } // namespace user

} // namespace vire

#endif // VIRE_USER_SIMPLE_PASSWORD_GENERATOR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
