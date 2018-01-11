// @file virecmsserver/ui/shell.h - Vire CMS server CLI shell support
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRECMSSERVER_UI_SHELL_H
#define VIRECMSSERVER_UI_SHELL_H

// Standard Library:
#include <memory>

// Third Party:
// - Bayeux/datatools:
#include <datatools/ui/basic_shell.h>
#include <datatools/ui/ihs.h>
#include <datatools/logger.h>
#include <datatools/properties.h>

// This project:
#include <vire/cmsserver/server.h>
#include <virecmsserver/ui/server_command_interface.h>

namespace virecmsserver {

  namespace ui {

    /// \brief Vire CMS Server Shell
    class shell
      : public datatools::ui::basic_shell
    {
    public:

      shell(vire::cmsserver::server &);

      virtual ~shell();

      void set_interactive(bool);

      bool is_interactive() const;

    protected:

      void _at_init(const datatools::properties & config_);

      void _at_reset();

      void _init_impl();

      void _reset_impl();

      void _build_ihs();

      vire::cmsserver::server & grab_server();

      const vire::cmsserver::server & get_server() const;

      virtual void _at_run_stop();

      virtual bool _compute_continue_condition();

    public:

      datatools::logger::priority logging = datatools::logger::PRIO_FATAL;

    private:

      // Configuration:
      bool _interactive_ = false; ///< Interactive flag
      vire::cmsserver::server * _server_ = nullptr; ///< Handle to the CMS server

      // Working:
      std::unique_ptr<datatools::ui::ihs>       _ihs_handler_;      ///< Embedded Interface Hierarchical System
      std::unique_ptr<server_command_interface> _commands_handler_; ///< Command inteface

    };

  } // namespace ui

} // namespace virecmsserver

#endif // VIRECMSSERVER_UI_SHELL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
