//! \file  vire/inspector/inspector_shell_command_interface.h
//! \brief Vire inspector shell command interface
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

#ifndef VIRE_INSPECTOR_INSPECTOR_SHELL_COMMAND_INTERFACE_H
#define VIRE_INSPECTOR_INSPECTOR_SHELL_COMMAND_INTERFACE_H

// Third party:
// - Bayeux/datatools:
#include <datatools/ui/target_command.h>
#include <datatools/ui/target_command_interface.h>
#include <datatools/ui/shell_command_interface.h>

// This project:
#include <vire/inspector/ihs_handler.h>
#include <vire/inspector/inspector_shell.h>

namespace vire {

  namespace inspector {

    //! \brief Vire inspector shell command interface
    class inspector_shell_command_interface
      : public datatools::ui::shell_command_interface
    {
    public:

      //! Default constructor
      inspector_shell_command_interface();

      //! Destructor
      virtual ~inspector_shell_command_interface();

    protected:

      //! Add base shell commands
      virtual void _add_shell_commands();

      //! Remove base shell commands
      virtual void _remove_shell_commands();

    private:

      DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE(datatools::ui::base_command_interface,
                                                           inspector_shell_command_interface)

    };

  } // end of namespace inspector

} // end of namespace vire

#endif // VIRE_INSPECTOR_INSPECTOR_SHELL_COMMAND_INTERFACE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
