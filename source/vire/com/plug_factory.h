//! \file  vire/com/plug_factory.h
//! \brief Vire com plug factory
//
// Copyright (c) 2017-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <mutex>

// This project:
#include <vire/com/base_plug.h>

namespace vire {

  namespace com {

    class actor;
    class domain;

    //! \brief Communication plug factory
    class plug_factory
    {
    public:

      static const std::string & default_private_plug_prefix();
      
      /// Constructor
      plug_factory(actor & parent_);

      /// Destructor
      ~plug_factory();

      const actor & get_parent() const;
 
      /// Create a new service client plug in a domain
      bool make_service_client_plug(const domain & domain_,
                                    std::string & plug_name_);
        

      /// Create a new service client plug in a domain
      bool make_service_server_plug(const domain & domain_,
                                    std::string & plug_name_);

      /// Create a new event listener plug in a domain
      bool make_event_listener_plug(const domain & domain_,
                                    std::string & plug_name_);

      /// Create a new event emitter plug in a domain
      bool make_event_emitter_plug(const domain & domain_,
                                   std::string & plug_name_,
                                   const std::string & event_label_ = "");
      
    private:

      // Configuration:
      actor & _parent_; ///< Parent actor

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
