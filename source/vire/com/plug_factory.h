//! \file  vire/com/plug_factory.h
//! \brief Vire com plug factory
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2017 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

namespace vire {

  namespace com {

    class manager;

    //! \brief Communication plug factory
    class plug_factory
    {
    public:

      /// Constructor
      plug_factory(manager &);

      /// Destructor
      ~plug_factory();

      /// Create a new event listener plug in a domain
      void make_event_listener_plug(const std::string & domain_name_,
                                    const std::string & name_);

      /// Create a new event emitter plug in a domain
      void make_event_emitter_plug(const std::string & domain_name_,
                                   const std::string & name_);

    private:

      /// Create a plug of given category and attached to a domain
      plug_ptr_type _create_plug_(domain & dom_,
                                  const std::string & name_,
                                  const plug_category_type plug_cat_) const;

    private:

      manager & _mgr_; ///< Top communication manager

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_PLUG_FACTORY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
