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

// This project:
#include <vire/com/utils.h>

namespace vire {

  namespace rabbitmq {

    class manager_service;
    
    ///! Description of a RabbitMQ user for the Vire system
    struct user
    {
    public:
 
      /// Default constructor
      user();

      /// Constructor
      user(const std::string & login_,
           const std::string & password_,
           const vire::com::actor_category_type category_);

      /// Check is a login is valid
      static bool validate_login(const std::string & login_);

      /// Set the users's login
      void set_login(const std::string & login_);

      /// Return the users's login
      const std::string & get_login() const;

      /// Check if the password is set
      bool has_password() const;

      /// Set the password
      void set_password(const std::string & password_);

      /// Return the password
      const std::string & get_password() const;

      /// Check is a word matches the password
      bool match_password(const std::string & word_) const;

      /// Set the user's category
      void set_category(const vire::com::actor_category_type category_);

      /// Return  the user's category
      const vire::com::actor_category_type get_category() const;

      /// Check if the user is complete (all mandatory fields are set)
      bool is_complete() const;

      /// Initialize (lock)
      void initialize();

      /// Initialize from parameters (lock)
      void initialize(const datatools::properties & config_);
 
      /// Reset (invalidate)
     void reset();
      
    private:
      
      std::string   _login_;    //!< Login of the RabbitMQ Vire user
      std::string   _password_; //!< Password
      vire::com::actor_category_type _category_ = vire::com::ACTOR_CATEGORY_INVALID; //!< User category
      
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
