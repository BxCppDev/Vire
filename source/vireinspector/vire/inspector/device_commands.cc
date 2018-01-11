//  vire/inspector/device_commands.cc
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

// This project:
#include <vire/inspector/device_commands.h>

namespace vire {

  namespace inspector {

    device_print_command::device_print_command(const device_desc & devdesc_)
      : datatools::ui::const_target_command<device_desc>(devdesc_,
                                                         "print",
                                                         "Print informations about a device",
                                                         datatools::version_id(1, 0))
    {
      return;
    }

    device_print_command::~device_print_command()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void device_print_command::_init(const datatools::properties & config_)
    {
      this->base_command::_init(config_);

      // Arguments description:
      _grab_opts().add_options()
        ; // end of options description

      return;
    }

    void device_print_command::_run(datatools::command::returned_info & cri_, uint32_t /* flags_ */)
    {
      cri_.reset();
      cri_.set_error_code(datatools::command::CEC_SUCCESS);

      const device_desc & devdesc = _get_target();

      std::cout << "Path:  '" << devdesc.get_full_path() << "'" << std::endl;
      // std::cout << "ID:     " << minfo.get_mapping_id() << "'" << std::endl;
      // std::cout << "Device: " << (minfo.is_device() ? "yes" : "no") << std::endl;
      // if (minfo.has_parent_device_id()) {
      //   std::cout << "Parent device ID: " << minfo.get_parent_device_id() << std::endl;
      // }
      // if (minfo.has_daughter_device_ids()) {
      //   for (const auto & did : minfo.get_daughter_device_ids()) {
      //     std::cout << "Daughter device ID: " << did << std::endl;
      //   }
      // }
      // if (minfo.has_logical_device()) {
      //   const vire::device::logical_device & log = minfo.get_logical_device();
      //   // std::cout << "Logical device: '" << log.get_name() << "'" << std::endl;
      //   log.tree_dump(std::cout, "Logical device: ");
      // }
      return;
    }

    device_dump_command::device_dump_command(const device_desc & ddesc_)
      : datatools::ui::const_target_command<device_desc>(ddesc_,
                                                         "dump",
                                                         "Dump informations about a device",
                                                         datatools::version_id(1, 0))
    {
      return;
    }

    device_dump_command::~device_dump_command()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void device_dump_command::_init(const datatools::properties & config_)
    {
      this->base_command::_init(config_);

      // Arguments description:
      _grab_opts().add_options()
        ; // end of options description

      return;
    }

    void device_dump_command::_run(datatools::command::returned_info & cri_, uint32_t /* flags_ */)
    {
      cri_.reset();
      cri_.set_error_code(datatools::command::CEC_SUCCESS);

      const device_desc & devdesc = _get_target();

      std::cout << "Path:  '" << devdesc.get_full_path() << "'" << std::endl;

      /*const vire::device::mapping_info & minfo = _get_target();

      std::cout << "Path:  '" << minfo.get_path() << "'" << std::endl;
      std::cout << "ID:     " << minfo.get_mapping_id() << "'" << std::endl;
      std::cout << "Device: " << (minfo.is_device() ? "yes" : "no") << std::endl;
      if (minfo.has_parent_device_id()) {
        std::cout << "Parent device ID: " << minfo.get_parent_device_id() << std::endl;
      }
      if (minfo.has_logical_device()) {
        const vire::device::logical_device & log = minfo.get_logical_device();
        // std::cout << "Logical device: '" << log.get_name() << "'" << std::endl;
        log.tree_dump(std::cout, "Logical device: ");
      }
      */
      return;
    }

  } // end of namespace inspector

} // end of namespace vire
