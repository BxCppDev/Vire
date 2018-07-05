//! \file  vire/com/i_service_client_plug.h
//! \brief Vire com service client plug interface
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

#ifndef VIRE_COM_I_SERVICE_CLIENT_PLUG_H
#define VIRE_COM_I_SERVICE_CLIENT_PLUG_H

// This project:
#include <vire/com/base_plug.h>
#include <vire/com/utils.h>
#include <vire/utility/base_payload.h>

namespace vire {

  namespace com {
   
    //! \brief Base communication plug
    class i_service_client_plug 
      : public base_plug
    {
    public:
      
    protected:

      //! Constructor
      i_service_client_plug(const std::string & name_,
                            const actor & parent_,
                            const domain & domain_,
                            const std::string & mailbox_name_,
                            const datatools::logger::priority logging_ = datatools::logger::PRIO_FATAL);
      
    public:
      
      //! Destructor
      virtual ~i_service_client_plug();

      //! Return the mailbox name
      const std::string & get_mailbox_name() const;

      //! Return category
      plug_category_type get_category() const override final;

      // timeout is expressed in time unit (example: double timeout = 3.14 * CLHEP::second)
      rpc_status send_receive(const std::string & address_,
                              const vire::utility::const_payload_ptr_type & request_payload_,
                              vire::utility::const_payload_ptr_type & response_payload_,
                              const double timeout_ = -1.0);

    private:
      
      virtual rpc_status _at_send_receive_(const std::string & address_,
                                           const raw_message_type & raw_request_,
                                           raw_message_type & raw_response_,
                                           const float timeout_) = 0;

    private:
      
      std::string _mailbox_name_; ///< Domain mailbox (exchange)
      
    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_I_SERVICE_CLIENT_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
