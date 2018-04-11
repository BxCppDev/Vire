//! \file  vire/cmsclient/negotiation.h
//! \brief session manager
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

#ifndef VIRE_CMSCLIENT_NEGOTIATION_H
#define VIRE_CMSCLIENT_NEGOTIATION_H

// This project:
#include <vire/cmsclient/session_connection_parameters.h>

namespace vire {

  namespace cmsclient {

    class session_manager;

    /// \brief Negotiation driver
    class negotiation
    {
    public:

      // Negotiation worker interface
      struct i_worker
      {
        i_worker(negotiation & parent_);
        virtual ~i_worker();
        virtual bool run(session_connection_parameters &) = 0;
      protected:
        negotiation & _nego;
      };

      /// Default constructor
      negotiation(session_manager & parent_);

      /// Destructor
      ~negotiation();

      bool has_user() const;

      void set_user(const std::string &);

      const std::string & get_user() const;

      bool has_password() const;

      void set_password(const std::string &);

      const std::string & get_password() const;

      bool run(session_connection_parameters & nego_result_);

    private:

      session_manager & _parent_; //!< Parent session manager service
      std::string _user_;         //!< User
      std::string _password_;     //!< Password

    };

  } // namespace cmsclient

} // namespace vire

#endif // VIRE_CMSCLIENT_NEGOTIATION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
