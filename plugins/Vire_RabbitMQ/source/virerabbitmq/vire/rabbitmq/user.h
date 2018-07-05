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
// - Bayeux:
#include <bayeux/datatools/properties.h>

namespace vire {

  namespace rabbitmq {

    class manager_service;
    
    ///! RabbitMQ user for the Vire system
    struct user
    {
      enum user_category {
        CATEGORY_INVALID       = 0,
        CATEGORY_SERVER        = 1,
        CATEGORY_SUBCONTRACTOR = 2,
        CATEGORY_CLIENT        = 3,
        CATEGORY_SYSTEM        = 4
      };
    private:
      std::string   _login_;    //!< Login of the RabbitMQ Vire user
      std::string   _password_; //!< Password
      user_category _category_ = CATEGORY_INVALID; //!< User category
    public:
      bool          use_monitoring = true;  //!< Access to the monitoring domain (client)
      bool          use_control    = false; //!< Access to the control domain (client)

      /// Default constructor
      user();

      /// Constructor
      user(const std::string & login_,
           const std::string & password_,
           const user_category category_);

      static bool validate_login(const std::string & login_);
      void set_login(const std::string & login_);
      const std::string & get_login() const;
      bool has_password() const;
      void set_password(const std::string & password_);
      bool match_password(const std::string & word_) const;
      bool is_server() const;
      bool is_client() const;
      bool is_subcontractor() const;
      bool is_system() const;
      void set_category(const user_category category_);
      const user_category get_category() const;
      bool is_complete() const;
      void initialize();
      void initialize(const datatools::properties & config_);
      void reset();
      
      friend manager_service;
      
    };

  } // namespace rabbitmq

} // namespace vire

#endif // VIRE_RABBITMQ_USER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
