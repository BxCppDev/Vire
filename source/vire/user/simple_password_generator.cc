//! \file vire/user/simple_password_generator.cc
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
#include <vire/user/simple_password_generator.h>

// Standard library:
#include <chrono>

namespace vire {

  namespace user {

    simple_password_generator::simple_password_generator(uint32_t charset_,
                                                         const unsigned int seed_)
    {
      _charset_ = charset(charset_);
      _seed_ = seed_;
      return;
    }

    const std::string & simple_password_generator::get_charset() const
    {
      return _charset_;
    }

    unsigned int simple_password_generator::get_seed() const
    {
      return _seed_;
    }

    void simple_password_generator::generate_password(std::string & password_,
                                                      const std::size_t length_)
    {
      password_.clear();
      password_.assign(length_, 0);
      std::uniform_int_distribution<int> distribution(1, _charset_.size());
      for (std::size_t i = 0; i < length_; i++) {
        int ran_index = distribution(_grab_generator_());
        password_[i] = _charset_[ran_index - 1];
      }
      return;
    }

    std::default_random_engine & simple_password_generator::_grab_generator_()
    {
      if (_generator_.get() == nullptr) {
        if (_seed_ == 0) {
          _seed_ = std::chrono::system_clock::now().time_since_epoch().count();
        }
        _generator_.reset(new std::default_random_engine(_seed_));
      }
      return *_generator_.get();
    }

  } // namespace user

} // namespace vire
