//! \file  vire/rabbitmq/user.h
//! \brief Vire CMS RabbitMQ user
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_RABBITMQ_USER_H
#define VIRE_RABBITMQ_USER_H

// Standard Library:
#include <string>

// Third party:
// - Vire:
#include <vire/com/actor.h>
// - Bayeux:
#include <bayeux/datatools/properties.h>

namespace vire {

  namespace rabbitmq {

    ///! RabbitMQ user for the Vire system
    struct user
    {
    private:
      std::string _login_;  //!< Login of the RabbitMQ Vire user
    public:
      std::string password; //!< Password
      vire::com::actor::category_type category = vire::com::actor::CATEGORY_INVALID;
      std::string system_domain;   //!< Special name of the system domain
      bool use_monitoring = true;  //!< Access to the monitoring domain (client)
      bool use_control    = false; //!< Access to the control domain (client)

      /// Default constructor
      user();

      /// Constructor
      user(const std::string & login_,
           const std::string & password_,
           const vire::com::actor::category_type category_,
           const std::string & system_domain_ = "");

      static bool validate_login(const std::string & login_);
      void set_login(const std::string & login_);
      const std::string & get_login() const;
      void set_password(const std::string & password_);
      const std::string & get_password() const;
      void set_category(const vire::com::actor::category_type & category_);
      const vire::com::actor::category_type & get_category() const;
      bool is_valid() const;
      bool is_server() const;
      bool is_client() const;
      bool is_system() const;
      bool is_subcontractor() const;
      std::string get_system_domain() const;

      void initialize();
      void initialize(const datatools::properties &);
      void reset();

    };

  } // namespace rabbitmq

} // namespace vire

#endif // VIRE_RABBITMQ_USER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
