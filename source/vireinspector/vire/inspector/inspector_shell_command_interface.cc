//  vire/inspector/inspector_shell_command_interface.cc
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/inspector/inspector_shell_command_interface.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/ui/target_command.h>
#include <datatools/detail/command_macros.h>

// This project:
#include <vire/inspector/shell_print_device_command.h>
#include <vire/inspector/shell_find_devices_command.h>

namespace vire {

  namespace inspector {

    DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(datatools::ui::base_command_interface,
                                                              inspector_shell_command_interface,
                                                              "vire::inspector::inspector_shell_command_interface")

    inspector_shell_command_interface::inspector_shell_command_interface()
    : datatools::ui::shell_command_interface()
    {
      set_name("vireinspector");
      return;
    }

    inspector_shell_command_interface::~inspector_shell_command_interface()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void inspector_shell_command_interface::_add_shell_commands()
    {
      datatools::ui::shell_command_interface::_add_shell_commands();

      {
        shell_print_device_command * cmdPrintDev = new shell_print_device_command(dynamic_cast<inspector_shell&>(grab_shell()));
        cmdPrintDev->initialize_simple();
        add_command(cmdPrintDev, "print_device");
      }

      {
        shell_find_devices_command * cmdFindDev = new shell_find_devices_command(dynamic_cast<inspector_shell&>(grab_shell()));
        cmdFindDev->initialize_simple();
        add_command(cmdFindDev, "find_devices");
      }

      return;
    }

    void inspector_shell_command_interface::_remove_shell_commands()
    {

      remove_command("find_devices");
      remove_command("print_device");

      datatools::ui::shell_command_interface::_remove_shell_commands();
      return;
    }

  } // end of namespace inspector

} // end of namespace vire
