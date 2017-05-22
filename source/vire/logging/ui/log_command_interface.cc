//! \file vire/logging/ui/log_command_interface.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/logging/ui/log_command_interface.h>

// This project:
#include <vire/logging/ui/log_commands.h>

namespace vire {

  namespace logging {

    namespace ui {

      DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION(datatools::ui::base_command_interface,
                                                                log_command_interface,
                                                                "vire::logging::ui::log_command_interface")

      log_command_interface::log_command_interface()
      : datatools::ui::target_command_interface<simple_logging_service>()
      {
        _initialized_ = false;
        return;
      }

      log_command_interface::log_command_interface(simple_logging_service & target_,
                                                   const std::string & name_,
                                                   const std::string & description_,
                                                   const datatools::version_id & vid_)
        : datatools::ui::target_command_interface<simple_logging_service>(target_, name_, description_, vid_)
      {
        _initialized_ = false;
        return;
      }

      log_command_interface::~log_command_interface()
      {
        return;
      }

      bool log_command_interface::is_initialized() const
      {
        return _initialized_;
      }

      void log_command_interface::initialize(const datatools::properties & config_,
                                             const datatools::service_manager & /* services_ */)
      {
        DT_THROW_IF(is_initialized(),
                    std::logic_error,
                    "Simple Logging Service command interface is already initialized!");

        _base_initialize(config_);

        // Populate the interface with various commands:
        {
          log_print * print = new log_print(get_target());
          print->initialize_simple();
          add_command(print, "print");
        }

        _initialized_ = true;
        return;
      }

      void log_command_interface::reset()
      {
        DT_THROW_IF(!is_initialized(),
                    std::logic_error,
                    "Simple Logging Service command interface is not initialized!");
        _initialized_ = false;

        // Remove commands:
        if (has_command("print")) {
          remove_command("print");
        }

        _base_reset();
        return;
      }

    } // end of namespace ui

  } // end of namespace logging

} // end of namespace vire
