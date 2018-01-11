// virecmsserver/ui/server_stop_command.h - Vire CMS server UI commands
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

#ifndef VIRECMSSERVER_UI_SERVER_STOP_COMMAND_H
#define VIRECMSSERVER_UI_SERVER_STOP_COMMAND_H

// Third party:
// - Bayeux/datatools:
#include <datatools/ui/target_command.h>

// This project:
#include <vire/cmsserver/server.h>

namespace virecmsserver {

  namespace ui {

    //! Stop device command
    class server_stop_command
      : public datatools::ui::target_command<vire::cmsserver::server>
    {
    public:

      //! Default constructor
      server_stop_command(vire::cmsserver::server & server_);

      //! Destructor
      virtual ~server_stop_command();

    protected:

      virtual void _init(const datatools::properties & config_);

      virtual void _run(datatools::command::returned_info & cri_, uint32_t flags_ = 0);

    };

  } // namespace ui

} // namespace virecmsserver

#endif // VIRECMSSERVER_UI_SERVER_STOP_COMMAND_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
