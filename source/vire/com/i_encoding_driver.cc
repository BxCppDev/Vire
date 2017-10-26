//! \file vire/com/i_encoding_driver.cc
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
#include <vire/com/i_encoding_driver.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace com {

    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(i_encoding_driver,
                                                     "vire::com::i_encoding_driver/__system__")

    i_encoding_driver::i_encoding_driver()
    : _initialized_(false)
      ,_logging_(datatools::logger::PRIO_FATAL)
    {
      return;
    }

    i_encoding_driver::~i_encoding_driver()
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Encoding driver was not properly reset!");
      return;
    }

    datatools::logger::priority i_encoding_driver::get_logging() const
    {
      return _logging_;
    }

    void i_encoding_driver::set_logging(const datatools::logger::priority p_)
    {
      _logging_ = p_;
      return;
    }

    bool i_encoding_driver::is_initialized() const
    {
      return _initialized_;
    }

    void i_encoding_driver::initialize_simple()
    {
      datatools::properties dummy_config;
      initialize(dummy_config);
      return;
    }

    std::string i_encoding_driver::class_guid() const
    {
      return _class_guid_();
    }

    void i_encoding_driver::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Encoding driver is already initialized!");
      _initialize_impl_(config_);
      _initialized_ = true;
      return;
    }

    void i_encoding_driver::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Encoding driver is not initialized!");
      _initialized_ = false;
      _reset_impl_();
      return;
    }

    int i_encoding_driver::encode(const vire::message::message & msg_,
                                  raw_message_type & raw_msg_)
    {
      int error_code = 0;
      error_code = _encode_impl_(msg_, raw_msg_);
      return error_code;
    }

    int i_encoding_driver::decode(const raw_message_type & raw_msg_,
                                  vire::message::message & msg_)
    {
      int error_code = 0;
      msg_.reset();
      error_code = _decode_impl_(raw_msg_, msg_);
      return error_code;
    }

  } // namespace com

} // namespace vire
