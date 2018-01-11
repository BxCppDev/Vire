//! \file vire/cmsserver/session_manager.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/session_manager.h>

// Standard library:
#include <unistd.h>
#include <chrono>

// Third party:
// - Boost:
#include <boost/serialization/nvp.hpp>
#include <boost/filesystem.hpp>
// - Bayeux:
#include <datatools/io_factory.h>
#include <datatools/service_tools.h>

// This project:
#include <vire/utility/backup_file.h>
#include <vire/time/utils.h>
#include <vire/cmsserver/utils.h>
#include <vire/cmsserver/server.h>

namespace vire {

  namespace cmsserver {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(session_manager, "vire::cmsserver::session_manager")

    session_manager::session_manager(uint32_t flags_)
    {
      _initialized_ = false;

      _set_defaults_();
      return;
    }

    session_manager::~session_manager()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void session_manager::set_resources_name(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Session manager is already initialized!");
      _resources_name_ = name_;
      return;
    }

    const std::string & session_manager::get_resources_name() const
    {
      return _resources_name_;
    }

    void session_manager::set_resource_manager(const vire::resource::manager & mgr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Session manager is already initialized!");
      _resources_ = mgr_;
      return;
    }

    void session_manager::tree_dump(std::ostream & out_,
                           const std::string & title_,
                           const std::string & indent_,
                           bool inherit_) const
    {
      this->datatools::base_service::tree_dump(out_, title_, indent_, true);

      /*
      out_ << indent_ << i_tree_dumpable::tag
           << "Devices service name   : "
           << "'" << _devices_service_name_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Resources service name : "
           << "'" << _resources_service_name_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Users service name : "
           << "'" << _users_service_name_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Stop file              : "
           << "'" << _stop_file_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Running    : " << std::boolalpha
           << _running_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Must be stopped : " << std::boolalpha
           << must_be_stopped() << std::endl;
      */

      return;
    }

    // virtual
    bool session_manager::is_initialized() const
    {
      return _initialized_;
    }

    // virtual
    int session_manager::initialize(const datatools::properties & config_,
                                    datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error, "Session manager is already initialized!");

      this->::datatools::base_service::common_initialize(config_);

      if (_resources_ != nullptr) {
        if (_resources_name_.empty()) {
          if (config_.has_key("resources_label")) {
            set_resources_name(config_.fetch_string("resources_label"));
          }
        }

        if (_resources_name_.empty()) {
          set_resources_name(server::resources_service_name());
        }

        set_resource_manager(datatools::get<vire::resource::manager>(service_dict_,
                                                                     _resources_name_));
      }

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    int session_manager::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error, "Session manager is not initialized!");

      _set_defaults_();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    void session_manager::_set_defaults_()
    {
      _resources_name_.clear();
      _resources_ = nullptr;
      _next_id_ = -1;
      _top_session_.reset();
      return;
    }

    void session_manager::_init_top_session_()
    {
      return;
    }

  } // namespace cmsserver

} // namespace vire
