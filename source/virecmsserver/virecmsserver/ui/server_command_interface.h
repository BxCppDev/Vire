// virecmsserver/ui/server_command_interface.h - Vire CMS server UI command interface
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRECMSSERVER_UI_SERVER_COMMAND_INTERFACE_H
#define VIRECMSSERVER_UI_SERVER_COMMAND_INTERFACE_H

// Third party:
// - Bayeux/datatools:
#include <datatools/ui/target_command_interface.h>

// This project:
#include <vire/cmsserver/server.h>

namespace virecmsserver {

  namespace ui {


    //! \brief Server command interface
    class server_command_interface
      : public datatools::ui::target_command_interface<vire::cmsserver::server>
    {
    public:

      //! Default constructor
      server_command_interface(vire::cmsserver::server & target_,
                               const std::string & name_ = "",
                               const std::string & description_ = "",
                               const datatools::version_id & vid_ = datatools::version_id::invalid());

      //! Destructor
      virtual ~server_command_interface();

      //! Check initialization status
      virtual bool is_initialized() const;

      //! Initialization
      virtual void initialize(const datatools::properties & config_,
                              const datatools::service_manager & /* services_ */);

      //! Reset
      virtual void reset();

      void sync();

    };

  } // namespace ui

} // namespace virecmsserver

#endif // VIRECMSSERVER_UI_SERVER_COMMAND_INTERFACE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
