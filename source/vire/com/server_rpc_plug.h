//! \file  vire/com/server_rpc_plug.h
//! \brief Vire com server rpc plug
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_SERVER_RPC_PLUG_H
#define VIRE_COM_SERVER_RPC_PLUG_H

// Standard library:
#include <cstdint>

// This project:
#include <vire/com/base_plug.h>

namespace vire {

  namespace com {

   //! \brief User credentials data
    class server_rpc_plug : public vire::com::base_plug
    {
    public:

      //! Default constructor
      server_rpc_plug(const manager & mgr_);

      //! Destructor
      virtual ~server_rpc_plug();

      // //! Receive a request message and send back a response
      // virtual int receive_send(vire::utility::base_request & request_,
      //                          vire::utility::base_response & response_) = 0;

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_SERVER_RPC_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
