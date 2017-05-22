//! \file  vire/com/client_rpc_plug.h
//! \brief Vire com client rpc plug
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

#ifndef VIRE_COM_CLIENT_RPC_PLUG_H
#define VIRE_COM_CLIENT_RPC_PLUG_H

// Standard library:
#include <cstdint>
#include <mutex>
#include <memory>

// This project:
#include <vire/com/base_plug.h>
#include <vire/utility/base_request.h>
#include <vire/utility/base_response.h>

namespace vire {

  namespace com {

   //! \brief Client RPC plug
    class client_rpc_plug : public vire::com::base_plug
    {
    public:

      //! Default constructor
      client_rpc_plug(const manager & mgr_);

      //! Destructor
      virtual ~client_rpc_plug();

      //! Send a request message and receive a response (synchronous version)
      int send_receive(const vire::utility::base_request & request_payload_,
                       const std::string & relay_,
                       const std::string & address_,
                       vire::utility::base_response & response_payload_);

      //! Send a request message and receive a response (asynchronous version)
      int send_receive_async(const vire::utility::base_request & request_payload_,
                             vire::utility::base_response & response_payload_,
                             const std::string & listener_key_,
                             const std::string & user_correlation_id_ = "");

    private:

      std::mutex _send_mutex_;

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_CLIENT_RPC_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
