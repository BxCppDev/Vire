//! \file vire/cmsserver/use_case_db.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/use_case_db.h>

// This project:
#include <vire/cmsserver/server.h>
#include <vire/cmsserver/use_case_info.h>

namespace vire {

  namespace cmsserver {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(use_case_db, "vire::cmsserver::use_case_db")

    use_case_db::use_case_db(uint32_t /* flags_ */)
      : datatools::base_service("use_cases", "Use case database service", "")
    {
      _set_defaults_();
      return;
    }

    use_case_db::~use_case_db()
    {
      return;
    }


    bool use_case_db::has_users() const
    {
      return _users_ !=  nullptr;
    }

    void use_case_db::set_users(const vire::user::manager & mgr_)
    {
      _users_ = &mgr_;
      return;
    }

    const vire::user::manager & use_case_db::get_users() const
    {
      DT_THROW_IF(!has_users(), std::logic_error, "No user manager is set!");
      return *_users_;
    }

    bool use_case_db::has_devices() const
    {
      return _devices_ !=  nullptr;
    }

    void use_case_db::set_devices(const vire::device::manager &)
    {
      _devices_ = &mgr_;
      return;
    }

    const vire::device::manager & use_case_db::get_devices() const
    {
      DT_THROW_IF(!has_devices(), std::logic_error, "No device manager is set!");
      return *_devices_;
    }

    bool use_case_db::has_resources() const
    {
      return _resources_ !=  nullptr;
    }

    void use_case_db::set_resources(const vire::resource::manager &)
    {
      _resources_ = &mgr_;
      return;
    }

    const vire::resource::manager & use_case_db::get_resources() const
    {
      DT_THROW_IF(!has_resources(), std::logic_error, "No resource manager is set!");
      return *_resources_;
    }

    void use_case_db::_set_defaults_()
    {
      return;
    }

    // virtual
    bool use_case_db::is_initialized() const
    {
      return _initialized_;
    }

    // virtual
    int use_case_db::initialize(const datatools::properties & config_,
                                datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Manager is already initialized!");

      this->::datatools::base_service::common_initialize(config_);

      if (!has_users()) {
        std::string users_service_name;
        if (config_.has_key("users_service_name")) {
          users_service_name = config_.fetch_string("users_service_name");
        } else {
          users_service_name = server::users_service_name();
        }
        const std::string user_service_type_id("vire::user::manager");
        DT_THROW_IF(!datatools::find_service_name_with_id(service_dict_,
                                                          user_service_type_id,
                                                          users_service_name),
                    std::logic_error,
                    "Cannot find a service named '" << users_service_name << "' of type '"
                    << user_service_type_id << "'!");
        set_users(datatools::get<vire::user::manager>(service_dict_,
                                                      users_service_name));
      }

      if (!has_devices()) {
        std::string devices_service_name;
        if (config_.has_key("devices_service_name")) {
          devices_service_name = config_.fetch_string("devices_service_name");
        } else {
          devices_service_name = server::devices_service_name();
        }
        const std::string device_service_type_id("vire::device::manager");
        DT_THROW_IF(!datatools::find_service_name_with_id(service_dict_,
                                                          device_service_type_id,
                                                          devices_service_name),
                    std::logic_error,
                    "Cannot find a service named '" << devices_service_name << "' of type '"
                    << device_service_type_id << "'!");
        set_devices(datatools::get<vire::device::manager>(service_dict_,
                                                          devices_service_name));
      }

      if (!has_resources()) {
        std::string resources_service_name;
        if (config_.has_key("resources_service_name")) {
          resources_service_name = config_.fetch_string("resources_service_name");
        } else {
          resources_service_name = server::resources_service_name();
        }
        const std::string resource_service_type_id("vire::resource::manager");
        DT_THROW_IF(!datatools::find_service_name_with_id(service_dict_,
                                                          resource_service_type_id,
                                                          resources_service_name),
                    std::logic_error,
                    "Cannot find a service named '" << resources_service_name << "' of type '"
                    << resource_service_type_id << "'!");
        set_resources(datatools::get<vire::resource::manager>(service_dict_,
                                                              resources_service_name));
      }

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    // virtual
    int use_case_db::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Use case database service is not initialized!");
      _initialized_ = false;

      _db_.clear();
      _resources_ = nullptr;
      _devices_ = nullptr;
      _users_ = nullptr;

      _set_defaults_();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    bool use_case_db::has(const std::string & name_) const
    {
      return _db_.find(name_) != _db_.end();
    }

    bool use_case_db::is_a(const std::string & name_,
                           const std::string & use_case_type_id_) const
    {
      DT_THROW_IF(name_.empty(), std::logic_error,
                  "Missing use case record name!");
      DT_THROW_IF(use_case_type_id_.empty(), std::logic_error,
                  "Missing use case type identifier!");
      db_dict_type::const_iterator found = _db_.find(name_);
      if (found == _db_.end()) {
        return false;
      }
      if (found->second.get()->get_use_case_type_id() != use_case_type_id_) {
        return false;
      }
      return true;
    }

    void use_case_db::add(const std::string & name_,
                          const std::string & description_,
                          const std::string & role_definition_,
                          const std::string & use_case_type_id_,
                          const std::string & use_case_config_path_,
                          const datatools::properties & use_case_config_)
    {
      std::lock_guard<std::mutex> guard(_db_mutex_);
      DT_THROW_IF(name_.empty(), std::logic_error,
                  "Missing use case record name!");
      DT_THROW_IF(use_case_type_id_.empty(), std::logic_error,
                  "Missing use case type identifier!");

      use_case_info_ptr_type record_ptr(new use_case_info);
      use_case_info & ucinfo = *record_ptr.get();
      ucinfo.set_name(name_);
      ucinfo.set_description(description_);
      ucinfo.set_role_definition(role_definition_);
      ucinfo.set_use_case_type_id(use_case_type_id_);
      ucinfo.set_use_case_config_path(use_case_config_path_);
      ucinfo.set_use_case_config(use_case_config_);

      _db_[name_] = record_ptr;
      return;
    }

    void use_case_db::remove(const std::string & name_)
    {
      std::lock_guard<std::mutex> guard(_db_mutex_);
      DT_THROW_IF(name_.empty(), std::logic_error,
                  "Missing use case record name!");
      db_dict_type::const_iterator found = _db_.find(name_);
      DT_THROW_IF(found == _db_.end(), std::logic_error,
                  "Missing use case record name!");
      DT_THROW_IF(found.use_count() > 1, std::logic_error,
                  "Cannot remove use case record with name '" << name_ << "'!");
      _db_.erase(found);
      return;
    }

    void use_case_db::load(const std::string & filename_)
    {
      std::string filename = filename_;
      datatools::fetch_path_with_env(filename);
      datatools::multi_properties mp;
      mp.read(filename);
      for (int i = 0; i < mp.size(); i++) {
        const std::string & section_key = mp.ordered_key(i);
        const datatools::multi_properties::entry & mpe = mp.get(section_key);
        std::string name = mpe.get_key();
        std::string use_case_type_id = mpe.get_meta();
        const datatools::properties & config = mpe.get_properties();
        std::string description = "";
        std::string role_definition;
        std::string use_case_config_path;
        if (config.has_key("description")) {
          description = config.fetch_string("description");
        }
        if (config.has_key("role_definition")) {
          role_definition = config.fetch_string("role_definition");
        }
        if (config.has_key("use_case.config_path")) {
          use_case_config_path = config.fetch_string("use_case.config_path");
        }
        // XXX
        // if (config.has_key("use_case.config")) {
        //   use_case_config = config.fetch_string("use_case.config");
        //      }
      }
      return;
    }

  } // namespace cmsserver

} // namespace vire
