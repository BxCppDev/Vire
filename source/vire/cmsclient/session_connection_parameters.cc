// /vire/cmsclient/session_connection_parameters.cc
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

// Ourselves:
#include <vire/cmsclient/session_connection_parameters.h>

namespace vire {

  namespace cmsclient {

    session_connection_parameters::session_connection_parameters()
    {
      return;
    }

    session_connection_parameters::~session_connection_parameters()
    {
      return;
    }

    void session_connection_parameters::reset()
    {
      return;
    }

    bool session_connection_parameters::is_valid() const
    {
      if (_id_.empty()) return false;
      if (_role_.empty()) return false;
      if (!vire::time::is_valid(_deadline_)) return false;
      if (_login_.empty()) return false;
      if (_password_.empty()) return false;
      return true;
    }

    bool session_connection_parameters::has_id() const
    {
      return !_id_.empty();
    }

    void session_connection_parameters::set_id(const std::string & id_)
    {
      _id_ = id_;
      return;
    }

    const std::string & session_connection_parameters::get_id() const
    {
      return _id_;
    }

    void session_connection_parameters::set_role(const std::string & role_)
    {
      _role_ = role_;
      return;
    }

    const std::string & session_connection_parameters::get_role() const
    {
      return _role_;
    }

    void session_connection_parameters::set_deadline(const boost::posix_time::ptime & deadline_)
    {
      _deadline_ = deadline_;
      return;
    }

    const boost::posix_time::ptime & session_connection_parameters::get_deadline() const
    {
      return _deadline_;
    }

    void session_connection_parameters::set_login(const std::string & login_)
    {
      _login_ = login_;
      return;
    }

    const std::string & session_connection_parameters::get_login() const
    {
      return _login_;
    }

    void session_connection_parameters::set_password(const std::string & password_)
     {
       _password_ = password_;
      return;
    }

    const std::string & session_connection_parameters::get_password() const
    {
      return _password_;
    }

  } // namespace cmsclient

} // namespace vire
