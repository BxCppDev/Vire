//! \file vire/com/i_transport_manager.cc
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

// Ourselves:
#include <vire/com/i_transport_manager.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

// This project:
// #include <vire/com/utils.h>
// #include <vire/com/mailbox.h>
// #include <vire/com/domain.h>

namespace vire {

  namespace com {

    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(i_transport_manager,
                                                     "vire::com::i_transport_manager/__system__")

    i_transport_manager::i_transport_manager()
    {
      _initialized_ = false;
      return;
    }

    i_transport_manager::~i_transport_manager()
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Transport manager was not properly reset!");
      return;
    }

    bool i_transport_manager::is_initialized() const
    {
      return _initialized_;
    }

    void i_transport_manager::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Transport manager is already initialized!");
      _at_initialize_(config_);
      _initialized_ = true;
      return;
    }

    void i_transport_manager::reset()
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Transport manager is not initialized!");
      _initialized_ = false;
      _at_reset_();
      return;
    }

    // std::string i_transport_manager::new_private_mailbox_address(const mailbox::mode_type mode_)
    // {
    //   std::string addr;
    //   try {
    //     addr = _new_private_mailbox_address_impl_(mode_);
    //   } catch (std::exception & error) {
    //     addr.clear();
    //   }
    //   return addr;
    // }

  } // namespace com

} // namespace vire
