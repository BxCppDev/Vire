//! \file  vire/inspector/inspector_shell.h
//! \brief Vire inspector shell
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

#ifndef VIRE_INSPECTOR_INSPECTOR_SHELL_H
#define VIRE_INSPECTOR_INSPECTOR_SHELL_H

// Standard library:
#include <memory>

// Third party:
// - Bayeux:
#include <datatools/ui/basic_shell.h>

namespace vire {

  namespace inspector {

    class inspector_shell_command_interface;
    class ihs_handler;

    //! \brief Vire inspector shell
    class inspector_shell
      : public datatools::ui::basic_shell
    {
    public:

      //! Default constructor
      inspector_shell(ihs_handler & h_);

      //! Destructor
      virtual ~inspector_shell();

      ihs_handler & grab_ihsh();

      const ihs_handler & get_ihsh() const;

    protected:

      //! At initialization
      virtual void _at_init(const datatools::properties & config_);

      //! At reset
      virtual void _at_reset();

    private:

      ihs_handler * _ihsh_ = nullptr; //!< IHS handler
      std::unique_ptr<inspector_shell_command_interface> _commands_; //!< Command interfaces

    };

  } // end of namespace inspector

} // end of namespace vire

#endif // VIRE_INSPECTOR_INSPECTOR_SHELL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
