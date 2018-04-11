//! \file  vire/cmsclient/session_connection_parameters.h
//! \brief Session connection parameters
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

#ifndef VIRE_CMSCLIENT_SESSION_CONNECTION_PARAMETERS_H
#define VIRE_CMSCLIENT_SESSION_CONNECTION_PARAMETERS_H

// Standard library:
#include <string>

// This project:
#include <vire/time/utils.h>

namespace vire {

  namespace cmsclient {

    //! \brief Session connection parameters used by the client to gain access to the CMS
    class session_connection_parameters
    {
    public:

      session_connection_parameters();

      ~session_connection_parameters();

      void reset();

      bool is_valid() const;

      bool has_id() const;

      void set_id(const std::string &);

      const std::string & get_id() const;

      void set_role(const std::string &);

      const std::string & get_role() const;

      void set_deadline(const boost::posix_time::ptime &);

      const boost::posix_time::ptime & get_deadline() const;

      void set_login(const std::string &);

      const std::string & get_login() const;

      void set_password(const std::string &);

      const std::string & get_password() const;

    private:

      std::string _id_;   //!< Session identifier
      std::string _role_; //!< Role
      boost::posix_time::ptime _deadline_; //!< Session deadline
      std::string _login_;    //!< Login
      std::string _password_; //!< Password

    };

  } // namespace cmsclient

} // namespace vire

#endif // VIRE_CMSCLIENT_SESSION_CONNECTION_PARAMETERS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
