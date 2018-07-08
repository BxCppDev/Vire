//! \file  vire/rabbitmq/service_client_plug.h
//! \brief Vire RabbitMQ service client plug
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_RABBITMQ_SERVICE_CLIENT_PLUG_H
#define VIRE_RABBITMQ_SERVICE_CLIENT_PLUG_H

// This project:
#include <vire/com/i_service_client_plug.h>
   
namespace vire {

  namespace rabbitmq {
   
    //! \brief Base communication plug
    class service_client_plug
      : public vire::com::i_service_client_plug
    {
    public:

      //! Constructor
      service_client_plug(const std::string & name_,
                          const vire::com::actor & parent_,
                          const vire::com::domain & domain_,
                          const std::string & mailbox_name_,
                          const datatools::logger::priority logging_ = datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~service_client_plug();

    private:

      void _construct_();

      void _destroy_();

      vire::com::rpc_status _at_send_receive_(const std::string & routing_key_,
                                              const vire::com::raw_message_type & raw_request_,
                                              vire::com::raw_message_type & raw_response_,
                                              const float timeout_sec_) override final;

    private:

      struct pimpl_type;
      std::unique_ptr<pimpl_type> _pimpl_;
      
    };

  } // namespace rabbitmq

} // namespace vire

#endif // VIRE_RABBITMQ_SERVICE_CLIENT_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
