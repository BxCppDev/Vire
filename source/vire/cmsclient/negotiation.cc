// /vire/cmsclient/negotiation.cc
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
#include <vire/cmsclient/negotiation.h>

// This project:
#include <vire/cmsclient/session_manager.h>

namespace vire {

  namespace cmsclient {

    negotiation::i_worker::i_worker(negotiation & nego_)
      : _nego(parent_)
    {
      return;
    }

    negotiation::i_worker::~i_worker()
    {
      return;
    }

    negotiation::negotiation(session_manager & parent_)
      : _parent_(parent_)
    {
      return;
    }

    negotiation::~negotiation()
    {
      return;
    }

    bool negotiation::has_user() const
    {
      return !_user_.empty();
    }

    void negotiation::set_user(const std::string & user_)
    {
      _user_ = user_;
      return;
    }

    const std::string & negotiation::get_user() const
    {
      return _user_;
    }

    bool negotiation::has_password() const
    {
      return !_password_.empty();
    }

    void negotiation::set_password(const std::string & password_)
    {
      _password_ = password_;
      return;
    }

    const std::string & negotiation::get_password() const
    {
      return _password_;
    }

    bool negotiation::run(session_connection_parameters & nego_result_)
    {
      bool success = false;
      nego_result_.reset();



      return nego_result_.is_valid();
    }

  } // namespace cmsclient

} // namespace vire
