//  vire/inspector/inspector_shell.cc
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
#include <vire/inspector/inspector_shell.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/logger.h>

// This project:
#include <vire/inspector/inspector_shell_command_interface.h>
#include <vire/inspector/ihs_handler.h>

namespace vire {

  namespace inspector {

    inspector_shell::inspector_shell(ihs_handler & h_)
      : datatools::ui::basic_shell("vireinspector")
      , _ihsh_(&h_)
    {
      set_ihs(_ihsh_->grab_ihs());
      return;
    }

    inspector_shell::~inspector_shell()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    ihs_handler & inspector_shell::grab_ihsh()
    {
      return *_ihsh_;
    }

    const ihs_handler & inspector_shell::get_ihsh() const
    {
      return *_ihsh_;
    }

    void inspector_shell::_at_init(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging());
      if (!has_system_interface()) {
        _commands_.reset(new inspector_shell_command_interface);
        set_system_interface(*_commands_.get());
      }
      datatools::ui::basic_shell::_at_init(config_);
      DT_LOG_TRACE_EXITING(get_logging());
      return;
    }

    void inspector_shell::_at_reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging());
      datatools::ui::basic_shell::_at_reset();
      _commands_.reset();
      DT_LOG_TRACE_EXITING(get_logging());
      return;
    }

  } // end of namespace inspector

} // end of namespace vire
