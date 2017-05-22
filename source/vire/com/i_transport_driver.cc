//! \file vire/com/i_transport_driver.cc
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
#include <vire/com/i_transport_driver.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

// This project:
#include <vire/com/utils.h>
#include <vire/com/mailbox.h>

namespace vire {

  namespace com {

    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(i_transport_driver,
                                                     "vire::com::i_transport_driver/__system__")

    i_transport_driver::i_transport_driver()
    {
      _initialized_ = false;
      return;
    }

    i_transport_driver::i_transport_driver(domain & domain_)
    {
      set_domain(domain_);
      return;
    }

    i_transport_driver::~i_transport_driver()
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Transport driver was not properly reset!");
      return;
    }

    void i_transport_driver::set_domain(domain & domain_)
    {
      _domain_ = &domain_;
      return;
    }

    const domain & i_transport_driver::get_domain() const
    {
      return *_domain_;
    }

    domain & i_transport_driver::grab_domain()
    {
      return *_domain_;
    }

    bool i_transport_driver::is_initialized() const
    {
      return _initialized_;
    }

    void i_transport_driver::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Transport driver is already initialized!");
      DT_THROW_IF(_domain_ == nullptr, std::logic_error,
                  "Transport driver has no domain set!");
      _initialize_impl_(config_);
      _initialized_ = true;
      return;
    }

    void i_transport_driver::reset()
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Transport driver is not initialized!");
      _initialized_ = false;
      _reset_impl_();
      _domain_ = nullptr;
      return;
    }

    std::string i_transport_driver::new_private_mailbox_address(const mailbox::mode_type mode_)
    {
      std::string addr;
      try {
        addr = _new_private_mailbox_address_impl_(mode_);
      } catch (std::exception & error) {
        addr.clear();
      }
      return addr;
    }

    int i_transport_driver::send(const mailbox & mailbox_,
                                 const vire::message::message & msg_,
                                 const datatools::properties & msg_metadata_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Transport driver is not initialized!");
      int error_code = COM_OK;
      error_code = _send_impl_(mailbox_, msg_, msg_metadata_);
      return error_code;
    }

  } // namespace com

} // namespace vire
