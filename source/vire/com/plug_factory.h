//! \file  vire/com/plug_factory.h
//! \brief Vire com plug factory
//
// Copyright (c) 2017-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
//                            Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_PLUG_FACTORY_H
#define VIRE_COM_PLUG_FACTORY_H

// Standard library:
#include <string>
#include <memory>

// This project:
#include <vire/com/base_plug.h>
#include <vire/com/i_event_listener_plug.h>
#include <vire/com/i_event_emitter_plug.h>
#include <vire/com/i_service_client_plug.h>
#include <vire/com/i_service_server_plug.h>

namespace vire {

  namespace com {

    class access_hub;
    class domain;

    //! \brief Communication plug factory associated to an actor
    class plug_factory
    {
    public:

      // static const std::string & default_private_plug_prefix();
      
      /// Constructor
      plug_factory(const access_hub & parent_);

      /// Destructor
      ~plug_factory();

      /// Return the set of supported transport driver type IDs
      static const std::set<std::string> & supported_transport_driver_type_ids();

      /// Return handle to the parent access hub
      const access_hub & get_parent() const;

      /// Create a new event listener plug in a domain
      std::shared_ptr<i_event_listener_plug> make_event_listener_plug(const std::string & domain_name_,
                                                                      const std::string & plug_name_,
                                                                      const std::string & event_label_ = "");

      /// Create a new event emitter plug in a domain
      std::shared_ptr<i_event_emitter_plug> make_event_emitter_plug(const std::string & domain_name_,
                                                                    const std::string & plug_name_,
                                                                    const std::string & event_label_ = "");

      /// Create a new service client plug in a domain
      std::shared_ptr<i_service_client_plug> make_service_client_plug(const std::string & domain_name_,
                                                                      const std::string & plug_name_);

      /// Create a new service server plug in a domain
      std::shared_ptr<i_service_server_plug> make_service_server_plug(const std::string & domain_name_,
                                                                      const std::string & plug_name_);
      
    private:

      // Configuration:
      const access_hub & _parent_; ///< Parent access hub

      struct pimpl_type;
      std::unique_ptr<pimpl_type> _pimpl_;
      
    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_PLUG_FACTORY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
