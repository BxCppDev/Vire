//! \file  vire/com/i_service_server_plug.h
//! \brief Vire com service server plug interface
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

#ifndef VIRE_COM_I_SERVICE_SERVER_PLUG_H
#define VIRE_COM_I_SERVICE_SERVER_PLUG_H

// This project:
#include <vire/com/base_plug.h>
#include <vire/com/utils.h>
#include <vire/com/addressing.h>
#include <vire/com/subscription_info.h>
#include <vire/utility/base_payload.h>

// Using base RPC worker

namespace vire {

  namespace com {
   
    //! \brief Service server plug interface
    class i_service_server_plug 
      : public base_plug
    {
    public:
      
    protected:

      //! Constructor
      i_service_server_plug(const std::string & name_,
                            const actor & parent_,
                            const domain & domain_,
                            const datatools::logger::priority logging_ = datatools::logger::PRIO_FATAL);
      
    public:
      
      //! Destructor
      virtual ~i_service_server_plug();

      //! Return the mailbox name
      const std::string & get_mailbox_name() const;

      //! Return category
      plug_category_type get_category() const override final;

      void add_subscription(const subscription_info & subinfo_);

      const subscription_info_list & get_subscriptions() const;

      // com_status receive_send(const address & address_,
      //                         const vire::utility::const_payload_ptr_type & request_payload_,
      //                         vire::utility::const_payload_ptr_type & response_payload_);

    private:
      
      // virtual com_status _at_receive_send_(const address & address_,
      //                                      const raw_message_type & raw_request_,
      //                                      raw_message_type & raw_response_) = 0;

    private:
      
      std::string _mailbox_name_; ///< Domain mailbox (exchange)
      subscription_info_list _subscriptions_;

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_I_SERVICE_CLIENT_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
