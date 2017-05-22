//! \file vire/com/base_port.cc
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

// Ourselves:
#include <vire/com/base_port.h>

namespace vire {

  namespace com {

    base_port::base_port()
    {
      return;
    }

    base_port::~base_port()
    {
      reset();
      return;
    }

    bool base_port::has_io_type() const
    {
      return _io_ != io_none;
    }

    void base_port::set_io_type(io_type io_)
    {
      _io_ = io_;
      return;
    }

    bool base_port::is_io_in() const
    {
      return _io_ == io_in;
    }

    bool base_port::is_io_out() const
    {
      return _io_ == io_out;
    }

    bool base_port::has_encoding_driver() const
    {
      return _encoding_driver_ != nullptr;
    }

    void base_port::set_encoding_driver(i_encoding_driver & driver_)
    {
      if (has_encoding_driver()) {
        if (&driver_ != _encoding_driver_) {
          _reset_encoding_driver();
        } else {
          return;
        }
      }
      _owned_encoding_driver_ = false;
      _encoding_driver_ = &driver_;
      return;
    }

    void base_port::set_encoding_driver(i_encoding_driver * driver_ptr_)
    {
      if (has_encoding_driver()) {
        if (driver_ptr_ != _encoding_driver_) {
          _reset_encoding_driver();
        } else {
          return;
        }
      }
      _owned_encoding_driver_ = true;
      _encoding_driver_ = driver_ptr_;
      return;
    }

    const i_encoding_driver & base_port::get_encoding_driver() const
    {
      DT_THROW_IF(_encoding_driver_ == nullptr,
                  std::logic_error,
                  "No encoding driver is defined!");
      return *_encoding_driver_;
    }

    i_encoding_driver & base_port::grab_encoding_driver()
    {
      DT_THROW_IF(_encoding_driver_ == nullptr,
                  std::logic_error,
                  "No encoding driver is defined!");
      return *_encoding_driver_;
    }

    void base_port::reset()
    {
      if (has_encoding_driver()) {
        _reset_encoding_driver();
      }
      return;
    }

    void base_port::_reset_encoding_driver()
    {
      if (_owned_encoding_driver_) {
        delete _encoding_driver_;
      }
      _owned_encoding_driver_ = false;
      _encoding_driver_ = nullptr;
      return;
    }

  } // namespace com

} // namespace vire
