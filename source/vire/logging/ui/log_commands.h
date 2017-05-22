//! \file  vire/logging/ui/log_command_interface.h
//! \brief Command interface for simple logging service
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_LOGGING_UI_LOG_COMMANDS_H
#define VIRE_LOGGING_UI_LOG_COMMANDS_H

// Third Party:
// - Bayeux/datatools:
#include <datatools/ui/target_command.h>
#include <datatools/detail/command_macros.h>

// This project:
#include <vire/logging/simple_logging_service.h>

namespace vire {

  namespace logging {

    namespace ui {

      //! Command
      class log_print : public datatools::ui::const_target_command<simple_logging_service>
      {
      public:

        /// Constructor
        log_print(const simple_logging_service & log_);

        /// Destructor
        virtual ~log_print();

      protected:

        /// Run
        virtual void _run(datatools::command::returned_info & cri_, uint32_t /*flags_ = 0*/);

      };

    } // end of namespace ui

  } // end of namespace logging

} // end of namespace vire

#endif // VIRE_LOGGING_UI_LOG_COMMANDS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
