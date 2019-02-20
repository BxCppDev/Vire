//! \file  vire/com/i_service_server_plug.h
//! \brief Vire com service server plug interface
//
// Copyright (c) 2019 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2019 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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
                            const access_hub & parent_,
                            const domain & domain_,
                            const datatools::logger::priority logging_ = datatools::logger::PRIO_FATAL);
      
    public:
      
      //! Destructor
      virtual ~i_service_server_plug();

      //! Return the mailbox name
      const std::string & get_mailbox_name() const;

      //! Return category
      plug_category_type get_category() const override final;

      //! Add subscription
      void add_subscription(const subscription_info & subinfo_);

      //! Return the list of subscriptions
      const subscription_info_list & get_subscriptions() const;

      //! Receive the next request
      com_status receive_next_request(vire::utility::const_payload_ptr_type & request_payload_,
                                      address & response_address_,
                                      vire::message::message_identifier & origine_msg_id_,
                                      address & async_response_address_);

      //! Send back response
      com_status send_back_response(const address & response_address_,
                                    const vire::message::message_identifier & in_reply_to_,
                                    const vire::utility::const_payload_ptr_type & response_payload_);

    private:

      //! Backend action at subscription adding
      virtual void _at_add_subscription_(const subscription_info & subinfo_) = 0;
  
      //! Backend action at next request reception
      virtual com_status _at_receive_next_request_(raw_message_type & raw_request_,
                                                   address & response_address_) = 0;

      //! Backend action at response sending back 
      virtual com_status _at_send_back_response_(const address & response_address_,
                                                 const raw_message_type & raw_response_) = 0;
      
    private:
      
      std::string            _mailbox_name_;  ///< Domain mailbox (exchange)
      subscription_info_list _subscriptions_; ///< List of subscriptions

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_I_SERVICE_CLIENT_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
