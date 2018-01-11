//  vire/inspector/shell_find_devices_command.cc
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
#include <vire/inspector/shell_find_devices_command.h>

// Standard library:
#include <regex>

// Third party:
// - Bayeux/datatools:
#include <datatools/ui/target_command.h>
#include <datatools/detail/command_macros.h>

// This project:
#include <vire/inspector/ihs_handler.h>
// #include <vire/device/instance_tree.h>
// #include <vire/device/instance_info.h>
// #include <vire/device/logical_device.h>

namespace vire {

  namespace inspector {

    shell_find_devices_command::shell_find_devices_command(const inspector_shell & ishell_)
      : datatools::ui::const_target_command<inspector_shell>(ishell_, "find_devices", "Find devices")
    {
      return;
    }

    shell_find_devices_command::~shell_find_devices_command()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void shell_find_devices_command::_init(const datatools::properties & config_)
    {
      this->base_command::_init(config_);

      // Arguments description:
      _grab_opts().add_options()

        ("pattern",
         boost::program_options::value<std::string>()
         ->value_name("pattern"),
         "Pattern of the device instance to be searched for\n"
         "Examples: \n"
         "  find_devices --pattern /path/to/the/device/* \n"
         "  find_devices /path/to/the/device/*"
         )

        ; // end of options description

      // Positional options:
      _grab_args().add("pattern", 1);


      return;
    }

    void shell_find_devices_command::_run(datatools::command::returned_info & cri_, uint32_t /*flags_*/)
    {
      cri_.reset();
      cri_.set_error_code(datatools::command::CEC_SUCCESS);
      DT_LOG_TRACE(get_logging_priority(), "Running command '" << get_name() << "'...");
      try {
       // Parse argument:
        std::string arg_dev_path_pattern;

        // Set the path:
        if (cri_.is_success()) {
          if (_grab_vmap().count("pattern")) {
            DT_LOG_TRACE(get_logging_priority(), "Decoding the device path pattern...");
            arg_dev_path_pattern = _grab_vmap()["pattern"].as<std::string>();
            DT_LOG_TRACE(get_logging_priority(), "Setting the device path pattern...");
          }
        }

        const inspector_shell & ishell = _get_target();
        const ihs_handler & ihsh = ishell.get_ihsh();
        std::string ui_dev_path_pattern = arg_dev_path_pattern;
        if (ui_dev_path_pattern.empty()) {
          // Default to current path:
          ui_dev_path_pattern = "$(.*)^";
        }
        std::cerr << "UI device path pattern : '" << ui_dev_path_pattern << "'" << std::endl;

        std::regex regexp(ui_dev_path_pattern);
        std::set<std::string> ui_paths;

        ihsh.get_ihs().build_path(ui_paths);
        for (const auto & path : ui_paths) {
          if (std::regex_match(path, regexp)) {
            std::cout << path << std::endl;
          }
        }
       } catch (std::exception & error) {
        DT_COMMAND_RETURNED_ERROR(cri_,
                                  datatools::command::CEC_FAILURE,
                                  get_name() + ": " + error.what());
      }

      DT_LOG_TRACE(get_logging_priority(), "End of command '" << get_name() << "'.");
      return;
    }

  } // end of namespace inspector

} // end of namespace vire
