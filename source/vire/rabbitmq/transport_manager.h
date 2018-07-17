//! \file  vire/rabbitmq/transport_manager.h
//! \brief Vire RabbitMQ transport manager
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

#ifndef VIRE_RABBITMQ_TRANSPORT_MANAGER_H
#define VIRE_RABBITMQ_TRANSPORT_MANAGER_H

// This project:
#include <vire/com/i_transport_manager.h>

namespace vire {

  namespace rabbitmq {

    //! \brief Transport manager interface
    class transport_manager
      : public vire::com::i_transport_manager
    {
    public:

      //! Default constructor
      transport_manager();

      //! Destructor
      virtual ~transport_manager();

      bool has_domain(const std::string & domain_name_) const override;
      
      void add_domain(const std::string & domain_name_,
                      const vire::com::domain_category_type category_) override;
      
      void remove_domain(const std::string & domain_name_) override;
 
      bool has_user(const std::string & login_) const override;
      
      void add_user(const std::string & login_,
                    const std::string & password_,
                    const vire::com::actor_category_type category_) override;
      
      void remove_user(const std::string & login_) override;
      
    private:

      //! Initialization
      void _at_initialize_(const datatools::properties &) override;

      //! Reset
      void _at_reset_() override;

    private:
       
      // Configuration:
      struct pimpl_type;
      std::unique_ptr<pimpl_type> _pimpl_;
       
      VIRE_COM_TRANSPORT_MANAGER_REGISTRATION_INTERFACE(transport_manager)
      
    };

  } // namespace rabbitmq

} // namespace vire

#endif // VIRE_COM_RABBITMQ_TRANSPORT_MANAGER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
