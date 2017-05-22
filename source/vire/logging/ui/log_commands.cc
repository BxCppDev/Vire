//! \file vire/logging/ui/log_commands.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/logging/ui/log_commands.h>

namespace vire {

  namespace logging {

    namespace ui {

      log_print::log_print(const simple_logging_service & log_)
        : datatools::ui::const_target_command<simple_logging_service>(log_,
                                                                      "print",
                                                                      "Print the logging service status",
                                                                      datatools::version_id(1, 0))
      {
        return;
      }

      log_print::~log_print()
      {
        if (is_initialized()) {
          reset();
        }
        return;
      }

      void log_print::_run(datatools::command::returned_info & cri_, uint32_t /*flags_ = 0*/)
      {
        cri_.reset();
        cri_.set_error_code(datatools::command::CEC_SUCCESS);
        try {
          DT_LOG_TRACE(get_logging_priority(), "Test...");
          const simple_logging_service & log = get_target();
          log.tree_dump(std::cout, log.get_name() + " : ");
        } catch (std::exception & error) {
          DT_COMMAND_RETURNED_ERROR(cri_,
                                    datatools::command::CEC_FAILURE,
                                    get_name() + ": " + error.what());
        }
        return;
      }

    } // end of namespace ui

  } // end of namespace logging

} // end of namespace vire
