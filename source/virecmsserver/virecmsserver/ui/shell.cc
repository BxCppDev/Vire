// shell.cc - Vire CMS server shell support
//
// Copyright (c) 2016-2017 by François MAUGER <mauger@lpccaen.in2p3.fr>
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

// Ourselves:
#include <virecmsserver/ui/shell.h>

// Standard Library:
#include <thread>
#include <future>
#include <functional>

// Third Party:
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/exception.h>
#include <datatools/ui/utils.h>

// This project:
#include <vire/logging/ui/log_command_interface.h>
#include <vire/device/manager.h>
#include <vire/version.h>
#include <vire/utility/path.h>

namespace virecmsserver {

  namespace ui {

    shell::shell(vire::cmsserver::server & server_)
      : datatools::ui::basic_shell("virecmsserver")
      , _server_(&server_)
    {
      return;
    }

    shell::~shell()
    {
      return;
    }

    void shell::set_interactive(bool i_)
    {
      _interactive_ = i_;
      return;
    }

    bool shell::is_interactive() const
    {
      return _interactive_;
    }

    void shell::_at_init(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(logging);
      _init_impl();
      datatools::ui::basic_shell::_at_init(config_);
      DT_LOG_TRACE_EXITING(logging);
      return;
    }

    void shell::_at_reset()
    {
      DT_LOG_TRACE_ENTERING(logging);
      datatools::ui::basic_shell::_at_reset();
      _reset_impl();
      DT_LOG_TRACE_EXITING(logging);
      return;
    }

    vire::cmsserver::server & shell::grab_server()
    {
      return *_server_;
    }

    const vire::cmsserver::server & shell::get_server() const
    {
      return *_server_;
    }

    // virtual
    void shell::_at_run_stop()
    {
      datatools::ui::basic_shell::_at_run_stop();
      if (_server_->is_running() || !_server_->is_requested_stop()) {
        _server_->request_stop();
      }
      return;
    }

    bool shell::_compute_continue_condition()
    {
      if (!_server_->is_running() || _server_->is_requested_stop()) {
        return false;
      }
      return datatools::ui::basic_shell::_compute_continue_condition();
    }

    void shell::_init_impl()
    {
      DT_LOG_TRACE_ENTERING(logging);

      std::string host = _server_->get_setup_id().get_name();

      std::string devices_service_name;
      if (datatools::find_service_name_with_id(_server_->get_services().get_bus_of_services(),
                                               "vire::device::manager",
                                               devices_service_name)) {
        std::string cms_setup_label = _server_->get_setup_id().get_name();
        const vire::device::manager & devices
          = _server_->get_services().get<const vire::device::manager>(devices_service_name);
        std::string dev_setup_label = devices.get_setup_label();
        DT_THROW_IF(dev_setup_label != cms_setup_label, std::logic_error,
                    "Unmatching CMS setup ID '" << cms_setup_label
                    << "' and device setup ID '" << dev_setup_label << "'!");
      }
      if (this->get_server().has_setup_id()) {
        this->set_host(host);
      }

      DT_LOG_DEBUG(logging, "Configuring the shell...");
      this->set_logging(logging);
      this->set_name("virecmsserver"); // Mandatory
      this->set_title("vire cms server");
      datatools::version_id vid(vire::version::get_major(),
                                vire::version::get_minor(),
                                vire::version::get_patch());
      this->set_version(vid);
      this->set_host(host);
      this->set_prompt("%H:%W> ");
      this->set_continuation_prompt("> ");
      this->set_exit_on_error(true);
      this->set_using_splash(false);
      this->set_using_readline(false);
      this->set_using_history(false);
      if (_interactive_) {
        DT_LOG_DEBUG(logging, "Interactive mode...");
        this->set_exit_on_error(false);
        this->set_using_splash(true);
        this->set_using_readline(true);
        this->set_using_history(true);
        this->set_history_add_only_on_success(true);
        this->set_history_filename("~/.virecmsserver.history");
        this->set_history_truncate(100);
      }
      _commands_handler_.reset(new server_command_interface(this->grab_server(),
                                                            "Server",
                                                            "CMS server command interface"));
      _commands_handler_->initialize_simple();
      DT_LOG_DEBUG(logging, "Building IHS...");
      _build_ihs();

      this->set_ihs(*_ihs_handler_.get());
      std::string default_path; // = std::to_string(vire::utility::path::root_symbol());

      {
        std::string setup;
        std::vector<std::string> segments;
        if (vire::utility::path::extract(_server_->get_cms_top_path(),
                                         setup,
                                         segments)) {
          vire::utility::path::segments_to_dirs(segments, default_path);
        }
      }
      this->set_default_path(default_path);
      DT_LOG_DEBUG(logging, "Default path : '" << this->get_default_path() << "'");
      DT_LOG_TRACE_EXITING(logging);
      return;
    }

    void shell::_build_ihs()
    {
      DT_LOG_TRACE_ENTERING(logging);
      _ihs_handler_.reset(new datatools::ui::ihs);
      datatools::ui::ihs & thisIhs = *_ihs_handler_.get();
      thisIhs.set_name("CMSServerIHS");
      thisIhs.set_display_name("The CMS Server Hierarchical System");
      thisIhs.set_scheme(this->_server_->get_setup_id().get_name());

      std::string cms_server_path;
      {
        std::string setup;
        std::vector<std::string> segments;
        if (vire::utility::path::extract(_server_->get_cms_top_path(),
                                         setup,
                                         segments)) {
          std::string parent;
          parent += vire::utility::path::root_symbol();
          for (std::size_t i = 0; i < segments.size(); i++) {
            // std::cerr << "*** DEVEL *** " << "add_interface " <<  segments[i] << std::endl;
            thisIhs.add_interface(parent, segments[i]);
            std::ostringstream parent_out;
            if (parent.size() > 1) {
              parent_out << parent << vire::utility::path::path_separator() << segments[i];
            } else {
              parent_out << vire::utility::path::path_separator() << segments[i];
            }
            parent = parent_out.str();
            // std::cerr << "*** DEVEL *** " << "new parent " << parent << std::endl;
          }
          thisIhs.add_interface(parent, *_commands_handler_.get());
          cms_server_path = parent;
        }
      }
      DT_LOG_DEBUG(logging, "CMS server path = '" << cms_server_path << "'");

      /*
        vire::cmsserver::server & k = _server_->grab_kernel();
        if (k.has_logger()) {
        DT_LOG_DEBUG(logging, "Add interface '" << k.grab_logger().get_name() << "'");

        vire::logging::ui::log_command_interface * logCmsInter
        = new vire::logging::ui::log_command_interface(k.grab_logger(),
        k.grab_logger().get_name(),
        "The Logger interface");
        logCmsInter->initialize_simple();
        _ihs_.add_interface("/", logCmsInter);

        }
        if (k.has_users_manager()) {
        DT_LOG_DEBUG(logging, "Add interface '" << k.grab_users_manager().get_name() << "'");
        _ihs_.add_interface("/", k.grab_users_manager().get_name());
        }
        if (k.has_authentication_manager()) {
        _ihs_.add_interface("/", k.grab_authentication_manager().get_name());
        }

        // kernel print
        // _ihs_.add_interface("/", kernel_command_interface);
      */

      if (datatools::logger::is_debug(get_logging())) {
        thisIhs.tree_dump(std::cerr, "IHS: ", "[debug] ");
      }
      DT_LOG_TRACE_EXITING(logging);
      return;
    }

    void shell::_reset_impl()
    {
      _commands_handler_.reset();
      _ihs_handler_.reset();
      return;
    }

  } // namespace ui

} // namespace virecmsserver
