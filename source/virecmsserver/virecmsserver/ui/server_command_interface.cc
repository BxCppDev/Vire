//  virecmsserver/ui/server_command_interface.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <virecmsserver/ui/server_command_interface.h>

// This project:
#include <virecmsserver/ui/server_print_command.h>
#include <virecmsserver/ui/server_stop_command.h>
#include <virecmsserver/ui/server_uptime_command.h>

namespace virecmsserver {

  namespace ui {

    server_command_interface::server_command_interface(vire::cmsserver::server & target_,
                                                       const std::string & name_,
                                                       const std::string & description_,
                                                       const datatools::version_id & vid_)
      : datatools::ui::target_command_interface<vire::cmsserver::server>(target_, name_, description_, vid_)
    {
      return;
    }

    server_command_interface::~server_command_interface()
    {
      return;
    }

    bool server_command_interface::is_initialized() const
    {
      return size() > 0;
    }

    void server_command_interface::initialize(const datatools::properties & config_,
                                              const datatools::service_manager & /* services_ */)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Command interface is already initialized!");

      _base_initialize(config_);

      // Populate the interface with various commands:

      {
        server_print_command * print = new server_print_command(get_target());
        print->initialize_simple();
        add_command(print, "print");
      }

      {
        server_uptime_command * uptime = new server_uptime_command(get_target());
        uptime->initialize_simple();
        add_command(uptime, "uptime");
      }

      {
        server_stop_command * stop = new server_stop_command(grab_target());
        stop->initialize_simple();
        add_command(stop, "stop");
      }

      return;
    }

    //! Reset
    void server_command_interface::reset()
    {
      DT_THROW_IF(!is_initialized(),
                  std::logic_error,
                  "Command interface is not initialized!");

      remove_command("stop");
      remove_command("uptime");
      remove_command("print");

      _base_reset();
      return;
    }

    // void server_command_interface::sync()
    // {
    //   DT_THROW_IF(!is_initialized(),
    //               std::logic_error,
    //               "Command interface is not initialized!");
    //   vire::cmsserver::server svr = this->get_target();
    //   if (svr.is_running() && ! svr.is_stop_requested()) {
    //   }
    //   return;
    // }

  } // end of namespace ui

} // end of namespace virecmsserver
