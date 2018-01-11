//  vire/inspector/device_command_interface.cc
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
#include <vire/inspector/device_command_interface.h>

// This project:
#include <vire/inspector/device_commands.h>

namespace vire {

  namespace inspector {

    device_command_interface::device_command_interface(const device_desc & target_,
                                                       const std::string & name_,
                                                       const std::string & description_,
                                                       const datatools::version_id & vid_)
      : datatools::ui::const_target_command_interface<device_desc>(target_, name_, description_, vid_)
    {
      return;
    }

    device_command_interface::~device_command_interface()
    {
      return;
    }

    bool device_command_interface::is_initialized() const
    {
      return size() > 0;
    }

    void device_command_interface::initialize(const datatools::properties & config_,
                                              const datatools::service_manager & /* services_ */)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Command interface is already initialized!");

      _base_initialize(config_);

      // Populate the interface with various commands:

      {
        device_print_command * print = new device_print_command(_get_target());
        print->initialize_simple();
        add_command(print, "print");
      }

      {
        device_dump_command * dump = new device_dump_command(_get_target());
        dump->initialize_simple();
        add_command(dump, "dump");
      }

      return;
    }

    //! Reset
    void device_command_interface::reset()
    {
      DT_THROW_IF(!is_initialized(),
                  std::logic_error,
                  "Command interface is not initialized!");

      remove_command("dump");
      remove_command("print");

      _base_reset();
      return;
    }

  } // end of namespace inspector

} // end of namespace vire
