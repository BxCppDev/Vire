//! \file  vire/inspector/shell_print_device_command.h
//! \brief Vire inspector shell find devices command
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

#ifndef VIRE_INSPECTOR_SHELL_PRINT_DEVICE_COMMAND_H
#define VIRE_INSPECTOR_SHELL_PRINT_DEVICE_COMMAND_H

// Third party:
// - Bayeux/datatools:
#include <datatools/ui/target_command.h>

// This project:
#include <vire/inspector/inspector_shell.h>

namespace vire {

  namespace inspector {

    //! \brief Shell find devices command
    struct shell_print_device_command
      : public datatools::ui::const_target_command<inspector_shell>
    {
      shell_print_device_command(const inspector_shell &);

      virtual ~shell_print_device_command();

    protected:

      virtual void _init(const datatools::properties & config_);

      virtual void _run(datatools::command::returned_info & cri_, uint32_t flags_ = 0);

    };


  } // end of namespace inspector

} // end of namespace vire

#endif // VIRE_INSPECTOR_SHELL_PRINT_DEVICE_COMMAND_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
