//  vire/inspector/shell_print_device_command.cc
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
#include <vire/inspector/shell_print_device_command.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/ui/target_command.h>
#include <datatools/detail/command_macros.h>

// This project:
#include <vire/inspector/ihs_handler.h>
#include <vire/device/instance_tree.h>
#include <vire/device/instance_info.h>
#include <vire/device/logical_device.h>

namespace vire {

  namespace inspector {

    shell_print_device_command::shell_print_device_command(const inspector_shell & ishell_)
      : datatools::ui::const_target_command<inspector_shell>(ishell_, "print_device", "Print information about the current working device interface")
    {
      return;
    }

    shell_print_device_command::~shell_print_device_command()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void shell_print_device_command::_init(const datatools::properties & config_)
    {
      this->base_command::_init(config_);

      // Arguments description:
      _grab_opts().add_options()

        ("path",
         boost::program_options::value<std::string>()
         ->value_name("path"),
         "Path of the device instance to be printed\n"
         "Examples: \n"
         "  print_device --path /path/to/the/device/instance \n"
         "  print_device /path/to/the/device/instance"
         )

        ; // end of options description

      // Positional options:
      _grab_args().add("path", 1);

      return;
    }

    void shell_print_device_command::_run(datatools::command::returned_info & cri_, uint32_t /*flags_*/)
    {
      cri_.reset();
      cri_.set_error_code(datatools::command::CEC_SUCCESS);
      DT_LOG_TRACE(get_logging_priority(), "Running command '" << get_name() << "'...");
      try {
        // Parse argument:
        std::string arg_dev_path;
        // if (!_grab_vmap().count("path")) {
        //   DT_COMMAND_RETURNED_ERROR(cri_,
        //                          datatools::command::CEC_COMMAND_INVALID_NUMBER_OF_ARGUMENTS,
        //                          get_name() + ": " + "Missing device path argument!");
        // }

        // Set the path:
        if (cri_.is_success()) {
          if (_grab_vmap().count("path")) {
            DT_LOG_TRACE(get_logging_priority(), "Decoding the device path...");
            arg_dev_path = _grab_vmap()["path"].as<std::string>();
            DT_LOG_TRACE(get_logging_priority(), "Setting the device path...");
          }
        }

        const inspector_shell & ishell = _get_target();
        const ihs_handler & ihsh = ishell.get_ihsh();
        std::string ui_dev_path = arg_dev_path;
        if (ui_dev_path.empty()) {
          // Default to current path:
          ui_dev_path = ishell.get_current_path();
        }
        std::cerr << "UI device path : '" << ui_dev_path << "'" << std::endl;
        std::string can_ui_dev_path = ishell.canonical_path(ui_dev_path);

        const vire::device::instance_tree & tree = ihsh.get_device_manager().get_tree();
        std::string instance_dev_path = ihsh.ui_to_instance_path(can_ui_dev_path);
        if (tree.has_instance(instance_dev_path)) {
          std::cout << "Device path : '" << instance_dev_path << "'" << std::endl;
          const vire::device::instance_info & iinfo = tree.get_instance(instance_dev_path);
          const vire::device::logical_device & logdev = iinfo.get_logical_device();
          std::cout << "|-- Serial number : ";
          if (iinfo.has_serial_number()) {
            std::cout << iinfo.get_serial_number();
          } else {
            std::cout << "<none>";
          }
          std::cout << std::endl;
          std::cout << "`-- Logical device : " << std::endl;
          boost::property_tree::ptree options;
          options.put("list_daughters",true);
          options.put("list_ports",true);
          options.put("list_links",true);
          options.put("indent","    ");
          logdev.print_tree(std::cout,options);
        } else {
          DT_COMMAND_RETURNED_ERROR(cri_,
                                    datatools::command::CEC_PARAMETER_INVALID,
                                    get_name() + ": " + "Invalid device path argument '" + arg_dev_path + "'!");
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
