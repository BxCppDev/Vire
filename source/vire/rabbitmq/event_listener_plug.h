//! \file  vire/rabbitmq/event_listener_plug.h
//! \brief Vire RabbitMQ event listener plug
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

#ifndef VIRE_RABBITMQ_EVENT_LISTENER_PLUG_H
#define VIRE_RABBITMQ_EVENT_LISTENER_PLUG_H

// This project:
#include <vire/com/i_event_listener_plug.h>
   
namespace vire {

  namespace rabbitmq {
   
    //! \brief Base communication plug
    class event_listener_plug
      : public vire::com::i_event_listener_plug
    {
    public:

      //! Constructor
      event_listener_plug(const std::string & name_,
                          const vire::com::actor & parent_,
                          const vire::com::domain & domain_,
                          const datatools::logger::priority logging_ = datatools::logger::PRIO_FATAL);

      //! Destructor
      virtual ~event_listener_plug();

    private:

      void _construct_();

      void _destroy_();
     
      vire::com::com_status _at_receive_next_event_(vire::com::raw_message_type & raw_event_) override final;

      void _at_add_subscription_(const subscription_info & subinfo_) override final;

    private:

      struct pimpl_type;
      std::unique_ptr<pimpl_type> _pimpl_;
      
    };

  } // namespace rabbitmq

} // namespace vire

#endif // VIRE_RABBITMQ_EVENT_LISTENER_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
