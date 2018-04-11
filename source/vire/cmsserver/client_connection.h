//! \file  vire/cmsserver/client_connection.h
//! \brief The client connection
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

#ifndef VIRE_CMSSERVER_CLIENT_CONNECTION_H
#define VIRE_CMSSERVER_CLIENT_CONNECTION_H

// Standard Library:
#include <string>

// This project:
#include <vire/time/utils.h>

namespace vire {

  namespace cmsserver {

    class session;

    /// \brief The client connection
    class client_connection
    {

    public:

      // XXX TO BE REVIEWED
      //! \brief Connection access
      enum connection_access_type {
        CONN_ACCESS_INVALID = 0,
        CONN_ACCESS_REMOTE  = 1,
        CONN_ACCESS_LOCAL   = 2
      };

      /// Default constructor
      client_connection();

      /// Destructor
      ~client_connection();

      //! Set the mother session
      void set_mother_session(const session &);

      //! Check if the mother session is set
      bool has_mother_session() const;

      //! Return the mother session
      const session & get_mother_session() const;

      bool has_id() const;

      void set_id(const int32_t);

      int32_t get_id() const;

      bool has_user_login() const;

      void set_user_login(const std::string & user_login_);

      const std::string & get_user_login() const;

      bool has_user_password() const;

      void set_user_password(const std::string & user_password_);

      const std::string & get_user_password() const;

      bool has_time_interval() const;

      void set_time_interval(const boost::posix_time::time_period & time_interval_);

      const boost::posix_time::time_period & get_time_interval() const;

      //! Check the validity of the client connection
      bool is_valid() const;

      bool is_initialized() const;

      void initialize();

      void reset();

    private:

      /*  Action executed at connection
       * - create a new user
       * - create the "system" domain for the client
       *
       *
       */
      void _at_connect_();

      /** Action executed at disconnection
       *
       * - destroy the "system" domain for the client
       * - destroy the user
       */
      void _at_disconnect_();

    private:

      bool        _initialized_ = false;
      int32_t     _id_ = -1;
      std::string _user_login_;    //! Unique generated key used as the user login identifier
      std::string _user_password_; //! User's credentials
      boost::posix_time::time_period _time_interval_; //! Connection max time interval
      const session * _mother_session_ = nullptr; //!< Handle to the mother session

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_CLIENT_CONNECTION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
