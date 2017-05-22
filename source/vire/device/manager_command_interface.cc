// vire/device/manager_command_interface.cc
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
#include <vire/device/manager_command_interface.h>

// This project:
#include <vire/device/manager.h>

namespace vire {

  namespace device {

    manager_command_interface::manager_command_interface(const std::string & name_,
                                                         const std::string & description_,
                                                         const datatools::version_id & vid_)
      : datatools::ui::target_command_interface<manager>(name_, description_, vid_)
    {
      _initialized_  = false;
      // _inhibit_cd_   = false;
      // _inhibit_load_ = false;
      return;
    }

    manager_command_interface::manager_command_interface(manager & sh_,
                                                         const std::string & name_,
                                                         const std::string & description_,
                                                         const datatools::version_id & vid_)
      : datatools::ui::target_command_interface<manager>(sh_, name_, description_, vid_)
    {
      _initialized_  = false;
      // _inhibit_cd_   = false;
      // _inhibit_load_ = false;
      return;
    }

    manager_command_interface::~manager_command_interface()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool manager_command_interface::has_manager() const
    {
      return has_target();
    }

    manager & manager_command_interface::grab_manager()
    {
      return grab_target();
    }

    const manager & manager_command_interface::get_manager() const
    {
      return get_target();
    }

    bool manager_command_interface::is_initialized() const
    {
      return _initialized_;
    }

    void manager_command_interface::initialize(const datatools::properties & config_,
                                               const datatools::service_manager & /* services_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Command interface is already initialized!");
      DT_THROW_IF(!has_manager(), std::logic_error,
                  "Command interface has no manager!");
      base_command_interface::_base_initialize(config_);

      // if (config_.has_key("inhibit_cd")) {
      //   bool icd = config_.fetch_boolean("inhibit_cd");
      //   set_inhibit_cd(icd);
      // }

      // if (config_.has_key("inhibit_load")) {
      //   bool iload = config_.fetch_boolean("inhibit_load");
      //   set_inhibit_load(iload);
      // }

      _add_manager_commands();
      _initialized_ = true;
      return;
    }

    void manager_command_interface::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Command interface is not initialized!");
      _initialized_ = false;
      _remove_manager_commands();
      // _inhibit_cd_ = false;
      // _inhibit_load_ = false;
      base_command_interface::_base_reset();
      return;
    }

    /*
      bool manager_command_interface::is_inhibit_load() const
      {
      return _inhibit_load_;
      }

      void manager_command_interface::set_inhibit_load(bool iload_)
      {
      DT_THROW_IF(is_initialized(), std::logic_error,
      "Manager command interface is already initialized!");
      _inhibit_load_ = iload_;
      return;
      }

      bool manager_command_interface::is_inhibit_cd() const
      {
      return _inhibit_cd_;
      }

      void manager_command_interface::set_inhibit_cd(bool icd_)
      {
      DT_THROW_IF(is_initialized(), std::logic_error,
      "Manager command interface is already initialized!");
      _inhibit_cd_ = icd_;
      return;
      }
    */

    void manager_command_interface::_add_manager_commands()
    {
      // Populate the interface with manager commands:

      /*
        {
        manager_help_command * cmdHelp = new manager_help_command(grab_manager());
        cmdHelp->initialize_simple();
        add_command(cmdHelp, "help");
        }

        {
        manager_exit_command * cmdExit = new manager_exit_command(grab_manager());
        cmdExit->initialize_simple();
        add_command(cmdExit, "exit");
        }

        {
        manager_man_command * cmdMan = new manager_man_command(grab_manager());
        cmdMan->initialize_simple();
        add_command(cmdMan, "man");
        }

        {
        manager_pwd_command * cmdPwd = new manager_pwd_command(grab_manager());
        cmdPwd->initialize_simple();
        add_command(cmdPwd, "pwd");
        }

        {
        manager_ls_command * cmdLs = new manager_ls_command(grab_manager());
        cmdLs->initialize_simple();
        add_command(cmdLs, "ls");
        }

        if (! is_inhibit_cd()) {
        manager_cd_command * cmdCd = new manager_cd_command(grab_manager());
        cmdCd->initialize_simple();
        add_command(cmdCd, "cd");
        }

        {
        manager_tree_command * cmdTree = new manager_tree_command(grab_manager());
        cmdTree->initialize_simple();
        add_command(cmdTree, "tree");
        }

        if (! is_inhibit_load()) {
        manager_load_command * cmdLoad = new manager_load_command(grab_manager());
        cmdLoad->initialize_simple();
        add_command(cmdLoad, "load");
        }
      */
      return;
    }

    void manager_command_interface::_remove_manager_commands()
    {
      // remove_command("cd");
      // remove_command("exit");
      // remove_command("man");
      // remove_command("pwd");
      // remove_command("ls");
      // remove_command("tree");
      // remove_command("load");
      return;
    }


  } // namespace device

} // namespace vire
