// virecmsserver/ui/server_stop_command.cc
//
// Copyright (c) 2015-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <virecmsserver/ui/server_stop_command.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/detail/command_macros.h>

namespace virecmsserver {

  namespace ui {

    server_stop_command::server_stop_command(vire::cmsserver::server & server_)
      : datatools::ui::target_command<vire::cmsserver::server>(server_,
                                                               "stop",
                                                               "Stop informations about the CMS server",
                                                               datatools::version_id(1, 0))
    {
      return;
    }

    server_stop_command::~server_stop_command()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void server_stop_command::_init(const datatools::properties & config_)
    {
      this->base_command::_init(config_);

      // Arguments description:
      _grab_opts().add_options()


        ; // end of options description

      return;
    }

    void server_stop_command::_run(datatools::command::returned_info & cri_, uint32_t /* flags_ */)
    {
      cri_.reset();
      cri_.set_error_code(datatools::command::CEC_SUCCESS);

      vire::cmsserver::server & server = _grab_target();

      if (server.is_running() && !server.is_requested_stop()) {
        server.request_stop();
      } else {
        DT_COMMAND_RETURNED_ERROR(cri_,
                                  datatools::command::CEC_FAILURE,
                                  get_name() + ": " + "Server is not running or stopping has been already requested!");

      }

      return;
    }

  } // namespace ui

} // namespace virecmsserver
