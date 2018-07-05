//! \file  vire/com/i_transport_driver.h
//! \brief Vire com transport driver interface
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

#ifndef VIRE_COM_I_TRANSPORT_DRIVER_H
#define VIRE_COM_I_TRANSPORT_DRIVER_H

// Standard library:
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <datatools/properties.h>

// This project:
#include <vire/message/message.h>
#include <vire/com/mailbox.h>

namespace vire {

  namespace com {

    class domain;

    //! \brief Transport driver interface
    class i_transport_driver
    {
    public:

      //! Default constructor
      i_transport_driver();

      //! Constructor
      i_transport_driver(domain & domain_);

      //! Destructor
      virtual ~i_transport_driver();

      //! Check if domain is set
      bool has_domain() const;

      //! Set the domain
      void set_domain(domain & domain_);

      //! Return a non mutable handle to the domain
      const domain & get_domain() const;

      //! Return a mutable handle to the domain
      domain & grab_domain();

      //! Check initialization flag
      bool is_initialized() const;

      //! Initialize the driver from the domain it is associated to
      void initialize();

      //! Initialize the driver from a set of configuration parameters
      void initialize(const datatools::properties &);

      //! Reset the driver
      void reset();

      //! Create a new private mailbox address
      std::string new_private_mailbox_address(const mailbox::mode_type);

      // //! Send a message
      // int send(const mailbox & mailbox_,
      //          const vire::message::message & msg_,
      //          const datatools::properties & msg_metadata_);

      // //! Receive a message
      // int receive(vire::message::message & msg_,
      //             datatools::properties & msg_metadata_);

    private:

      //! Initialization
      virtual void _initialize_impl_(const datatools::properties &) = 0;

      //! Reset
      virtual void _reset_impl_() = 0;

      //! Return the address of a new private mailbox
      virtual std::string _new_private_mailbox_address_impl_(const mailbox::mode_type) = 0;

      //!
      // virtual std::string _create_private_service_mailbox_(const std::string & name_) = 0;

      //! Send a Vire message to from a source string buffer
      // virtual int _send_impl_(const mailbox & mailbox_,
      // const vire::message::message & msg_,
      //  const datatools::properties & msg_metadata_) = 0;

      // //! Receive a target object from a source string buffer
      // virtual int _receive_impl_(vire::message::message & msg_,
      //                           datatools::properties & msg_metadata_) = 0;

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration:
      domain * _domain_ = nullptr;

      // Factory stuff :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(i_transport_driver)

    };

    typedef std::shared_ptr<i_transport_driver> transport_driver_ptr; 

  } // namespace com

} // namespace vire

#define VIRE_COM_TRANSPORT_DRIVER_REGISTRATION_INTERFACE(TransportClassName)    \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(::vire::com::i_transport_driver, TransportClassName) \
  /**/

#define VIRE_COM_TRANSPORT_DRIVER_REGISTRATION_IMPLEMENT(TransportClassName,TransportId) \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(::vire::com::i_transport_driver,TransportClassName,TransportId) \
  /**/

#endif // VIRE_COM_I_TRANSPORT_DRIVER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
