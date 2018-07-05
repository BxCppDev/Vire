//! \file  vire/com/rabbitmq_service_client_plug.h
//! \brief Vire com rabbitmq service client plug
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                      J
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

#ifndef VIRE_COM_RABBITMQ_SERVICE_CLIENT_PLUG_H
#define VIRE_COM_RABBITMQ_SERVICE_CLIENT_PLUG_H

// This project:
#include <vire/com/i_service_client_plug.h>

namespace vire {

  namespace com {
   
    //! \brief Base communication plug
    class rabbitmq_service_client_plug
      : public i_service_client_plug
    {
    public:

      //! Constructor
      rabbitmq_service_client_plug(const std::string & name_,
                                   const actor & parent_,
                                   const domain & domain_,
                                   const std::string & mailbox_name_,
                                   const datatools::logger::priority logging_ = datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~rabbitmq_service_client_plug();

    private:

      void _construct_();

      void _destroy_();

      rpc_status _at_send_receive_(const std::string & routing_key_,
                                   const raw_message_type & raw_request_,
                                   raw_message_type & raw_response_,
                                   const float timeout_sec_) override final;

    private:

      struct pimpl_type;
      std::unique_ptr<pimpl_type> _pimpl_;
      
    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_RABBITMQ_SERVICE_CLIENT_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
