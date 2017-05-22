//! \file  vire/device/manager_command_interface.h
//! \brief Vire device manager command interface
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

#ifndef VIRE_DEVICE_MANAGER_COMMAND_INTERFACE_H
#define VIRE_DEVICE_MANAGER_COMMAND_INTERFACE_H

// Third party:
// - Bayeux/datatools:
#include <datatools/command_utils.h>
#include <datatools/service_manager.h>
#include <datatools/properties.h>
#include <datatools/version_id.h>
#include <datatools/ui/target_command_interface.h>

namespace vire {

  namespace device {

    class manager;

    //! \brief Command interface for a device manager
    class manager_command_interface : public datatools::ui::target_command_interface<manager>
    {
    public:

      //! Default constructor
      manager_command_interface(const std::string & name_ = "",
                                const std::string & description_ = "",
                                const datatools::version_id & vid_ = datatools::version_id::invalid());

      //! Constructor
      manager_command_interface(manager & mgr_,
                                const std::string & name_ = "",
                                const std::string & description_ = "",
                                const datatools::version_id & vid_ = datatools::version_id::invalid());

      //! Destructor
      virtual ~manager_command_interface();

      //! Check initialization status
      virtual bool is_initialized() const;

      //! Initialization
      virtual void initialize(const datatools::properties & config_,
                              const datatools::service_manager & services_);

      //! Reset
      virtual void reset();

      /* //! Check if the load command is inhibited */
      /* bool is_inhibit_load() const; */

      /* //! Set the load command inhibition flag */
      /* void set_inhibit_load(bool); */

      /* //! Check if the cd command is inhibited */
      /* bool is_inhibit_cd() const; */

      /* //! Set the cd command inhibition flag */
      /* void set_inhibit_cd(bool); */

      //! Check if the shell is set
      bool has_manager() const;

      //! Return a mutable reference to the manager
      manager & grab_manager();

      //! Return a non mutable reference to the manager
      const manager & get_manager() const;

    protected:

      //! Add base manager commands
      void _add_manager_commands();

      //! Remove base manager commands
      void _remove_manager_commands();

    private:

      // Management:
      bool _initialized_; //!< Initialization flag

      // Configuration:
      /* bool _inhibit_cd_;   //!< Inhibition flag for the cd command */
      /* bool _inhibit_load_; //!< Inhibition flag for the load command */

    };

  } // namespace device

} // namespace vire

#endif // VIRE_DEVICE_MANAGER_COMMAND_INTERFACE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
