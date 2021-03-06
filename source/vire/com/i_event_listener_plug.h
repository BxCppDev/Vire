//! \file  vire/com/i_event_listener_plug.h
//! \brief Vire com event listener plug interface
//
// Copyright (c) 2016-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2016-2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_I_EVENT_LISTENER_PLUG_H
#define VIRE_COM_I_EVENT_LISTENER_PLUG_H

// Standard library:
#include <string>
#include <set>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/logger.h>

// This project:
#include <vire/com/utils.h>
#include <vire/com/addressing.h>
#include <vire/com/base_plug.h>
#include <vire/com/subscription_info.h>
#include <vire/utility/base_payload.h>

namespace vire {

  namespace com {

    class domain;
    class access_hub;
    
    //! \brief Event listener plug interface
    class i_event_listener_plug
      : public base_plug
    {
    protected:

      //! Constructor
      i_event_listener_plug(const std::string & name_,
                            const access_hub & parent_,
                            const domain & domain_,
                            const datatools::logger::priority logging_ = datatools::logger::PRIO_FATAL);
 
    public:
      
      //! Destructor
      virtual ~i_event_listener_plug();

      //! Return the set of allowed mailboxes
      const std::set<std::string> & get_allowed_mailboxes() const;

      //! Return category
      plug_category_type get_category() const override final;

      //! Add asubscription
      void add_subscription(const subscription_info & subinfo_);

      //! Return the list of subscriptions
      const subscription_info_list & get_subscriptions() const;
      
      //! Receive an event payload from the list of subscriptions
      com_status receive_next_event(vire::utility::const_payload_ptr_type & event_payload_);

      //! Check if a private address is set
      bool has_private_address() const;

      //! Return the private address
      const address & get_private_address() const;
      
    protected:

      //! Set the private address
      void _set_private_address(const std::string &);
      
    private:
      
      //! Backend action at next event reception
      virtual com_status _at_receive_next_event_(raw_message_type & raw_event_) = 0;
      
      //! Backend action at subscription adding
      virtual void _at_add_subscription_(const subscription_info & subinfo_) = 0;
     
      //! Populate allowed mailboxes
      void _populate_allowed_mailboxes_();
      
    private:
     
      std::set<std::string>  _allowed_mailboxes_; ///< List of allowed mailboxes
      subscription_info_list _subscriptions_;     ///< List of subscriptions
      address                _private_address_;   ///< Private address
      
    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_I_EVENT_LISTENER_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
