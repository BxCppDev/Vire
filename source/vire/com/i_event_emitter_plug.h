//! \file  vire/com/i_event_emitter_plug.h
//! \brief Vire com event emitter plug interface
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

#ifndef VIRE_COM_I_EVENT_EMITTER_PLUG_H
#define VIRE_COM_I_EVENT_EMITTER_PLUG_H

// This project:
#include <vire/com/base_plug.h>
#include <vire/com/utils.h>
#include <vire/com/addressing.h>
#include <vire/utility/base_payload.h>

namespace vire {

  namespace com {

    //! \brief Event emitter plug interface
    class i_event_emitter_plug
      : public base_plug
    {
    public:
      
    protected:

      //! Constructor
      i_event_emitter_plug(const std::string & name_,
                           const access_hub & parent_,
                           const domain & domain_,
                           const std::string & default_mailbox_name_ = "",
                           const datatools::logger::priority logging_ = datatools::logger::PRIO_FATAL);
      
     public:
      
      //! Destructor
      virtual ~i_event_emitter_plug();

      //! Return the set of allowed mailboxes
      const std::set<std::string> & get_allowed_mailboxes() const;

      //! Check if the default mailbox name is set
      bool has_default_mailbox_name() const;
      
      //! Return the default mailbox name
      const std::string & get_default_mailbox_name() const;

      //! Return category
      plug_category_type get_category() const override final;
      
      //! Send an event payload to the implicit mailbox of the domain
      com_status send_event(const address & address_,
                            const vire::utility::const_payload_ptr_type & event_payload_);

      //! Send an event payload to an explicit mailbox of the domain
      com_status send_event(const std::string & mailbox_name_,
                            const address & address_,
                            const vire::utility::const_payload_ptr_type & event_payload_);

    private:

      //! Backend action at event sending
      virtual com_status _at_send_event_(const std::string & mailbox_name_,
                                         const address & address_,
                                         const raw_message_type & raw_event_) = 0;

      //! Populate allowed mailboxes
      void _populate_allowed_mailboxes_();
      
    private:
     
      std::set<std::string> _allowed_mailboxes_;    ///< List of allowed mailboxes
      std::string           _default_mailbox_name_; ///< Default domain mailbox (exchange)

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_I_EVENT_EMITTER_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
