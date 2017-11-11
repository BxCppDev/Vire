//! \file  vire/rabbitmq/transport_driver.h
//! \brief RabbitMQ based tranport driver for Vire
//
// Copyright (c) 2017 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_RABBITMQ_TRANSPORT_DRIVER_H
#define VIRE_RABBITMQ_TRANSPORT_DRIVER_H

// Standard library:
#include <string>
#include <memory>

// Third party:
// - Vire:
#include <vire/com/i_transport_driver.h>
#include <vire/com/domain.h>

namespace vire {

  namespace rabbitmq {

    //! \brief RabbitMQ transport driver interface
    class transport_driver
      : public vire::com::i_transport_driver
    {
    public:

      //! Default constructor
      transport_driver();

      //! Constructor
      transport_driver(vire::com::domain & domain_);

      //! Destructor
      virtual ~transport_driver();

    private:

      //! Initialization
      virtual void _initialize_impl_(const datatools::properties &);

      //! Reset
      virtual void _reset_impl_();

      //! Return the address of a new private mailbox
      virtual std::string _new_private_mailbox_address_impl_(const vire::com::mailbox::mode_type);

      // Private implementation
      struct pimpl_type;
      std::unique_ptr<pimpl_type> _pimpl_;

      VIRE_COM_TRANSPORT_DRIVER_REGISTRATION_INTERFACE(transport_driver)

    };

  } // namespace rabbitmq

} // namespace vire

#endif // VIRE_RABBITMQ_TRANSPORT_DRIVER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
