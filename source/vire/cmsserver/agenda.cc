//! \file vire/cmsserver/agenda.cc
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
#include <vire/cmsserver/agenda.h>

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

namespace vire {

  namespace cmsserver {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(agenda, "vire::cmsserver::agenda")

    agenda::agenda(uint32_t flags_)
    {
      _initialized_ = false;
      _reservations_changed_ = false;
      _set_defaults_();
      return;
    }

    agenda::~agenda()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    void agenda::tree_dump(std::ostream & out_,
                           const std::string & title_,
                           const std::string & indent_,
                           bool inherit_) const
    {
      this->datatools::base_service::tree_dump(out_, title_, indent_, true);

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
           << "Reservations store     : "
           << "'" << _reservations_store_ << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Reservations changed   : "
           << std::boolalpha << _reservations_changed_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Last reservation ID    : "
           << "[" << _last_reservation_id_ << "]" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Running    : " << std::boolalpha
           << _running_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Must be stopped : " << std::boolalpha
           << must_be_stopped() << std::endl;

      return;
    }

    // virtual
    bool agenda::is_initialized() const
    {
      return _initialized_;
    }

    // virtual
    int agenda::initialize(const datatools::properties & config_,
                           datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error, "Agenda is already initialized!");

      this->::datatools::base_service::common_initialize(config_);

      // std::string devices_service_name   = _devices_service_name_;
      // if (_devices_ == nullptr) {
      //   if (_devices_service_name_.empty()) {
      //     if (config_.has_key("devices_service_name")) {
      //       set_devices_service_name(config_.fetch_string("devices_service_name"));
      //     }
      //   }
      //   if (_devices_service_name_.empty()) {
      //     set_devices_service_name(service::default_devices_name());
      //   }
      //   const std::string device_service_type_id = "vire::device::manager";
      //   // Automatically pickup the first device manager from the service manager:
      //   DT_THROW_IF(!datatools::find_service_name_with_id(service_dict_,
      //                                                     device_service_type_id,
      //                                                     _devices_service_name_),
      //               std::logic_error,
      //               "Cannot find a service named '" << _devices_service_name_ << "' of type '"
      //               << device_service_type_id << "'!");
      //   set_devices(datatools::get<vire::device::manager>(service_dict_,
      //                                                     get_devices_service_name()));
      // }

      std::string resources_service_name = _resources_service_name_;
      if (_resources_ == nullptr) {
        if (_resources_service_name_.empty()) {
          if (config_.has_key("resources_service_name")) {
            set_resources_service_name(config_.fetch_string("resources_service_name"));
          }
        }
        if (_resources_service_name_.empty()) {
          set_resources_service_name(service::default_resources_name());
        }
        const std::string resource_service_type_id = "vire::resource::manager";
        // Automatically pickup the first device manager from the service manager:
        DT_THROW_IF(!datatools::find_service_name_with_id(service_dict_,
                                                          resource_service_type_id,
                                                          _resources_service_name_),
                    std::logic_error,
                    "Cannot find a service named '" << _resources_service_name_ << "' of type '"
                    << resource_service_type_id << "'!");
        set_resources(datatools::get<vire::resource::manager>(service_dict_,
                                                              get_resources_service_name()));
      }

      std::string users_service_name   = _users_service_name_;
      if (_users_ == nullptr) {
        if (_users_service_name_.empty()) {
          if (config_.has_key("users_service_name")) {
            set_users_service_name(config_.fetch_string("users_service_name"));
          }
        }
        if (_users_service_name_.empty()) {
          set_users_service_name(service::default_users_name());
        }
        const std::string user_service_type_id = "vire::user::manager";
        // Automatically pickup the first user manager from the service manager:
        DT_THROW_IF(!datatools::find_service_name_with_id(service_dict_,
                                                          user_service_type_id,
                                                          _users_service_name_),
                    std::logic_error,
                    "Cannot find a service named '" << _users_service_name_ << "' of type '"
                    << user_service_type_id << "'!");
        set_users(datatools::get<vire::user::manager>(service_dict_,
                                                          get_users_service_name()));
      }

      if (_reservations_store_.empty()) {
        if (config_.has_key("reservations_store")) {
          std::string reservations_store = config_.fetch_string("reservations_store");
          set_reservations_store(reservations_store);
        }
      }

      if (_stop_file_.empty()) {
        if (config_.has_key("stop_file")) {
          std::string stop_file = config_.fetch_string("stop_file");
          set_stop_file(stop_file);
        }
      }

      if (config_.has_key("purge_store")) {
        std::size_t purge_store = config_.fetch_positive_integer("purge_store");
        set_purge_store(purge_store);
      }

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    int agenda::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error, "Agenda is not initialized!");
      DT_THROW_IF(is_running(), std::logic_error, "Agenda is still running!");
      if (_thread_) {
        _thread_.reset();
      }
      _initialized_ = false;

      _users_   = nullptr;
      _devices_   = nullptr;
      _resources_ = nullptr;

      _set_defaults_();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    void agenda::_set_defaults_()
    {
      _resources_service_name_.clear();
      _devices_service_name_.clear();
      _users_service_name_.clear();
      _purge_store_ = 0;
      _resources_ = nullptr;
      _devices_   = nullptr;
      _reservations_store_.clear();
      return;
    }

    void agenda::set_resources_service_name(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Agenda is already initialized!");
      _resources_service_name_ = name_;
      return;
    }

    const std::string & agenda::get_resources_service_name() const
    {
      return _resources_service_name_;
    }

    void agenda::set_devices_service_name(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Agenda is already initialized!");
      _devices_service_name_ = name_;
      return;
    }

    const std::string & agenda::get_devices_service_name() const
    {
      return _devices_service_name_;
    }

    void agenda::set_users_service_name(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Agenda is already initialized!");
      _users_service_name_ = name_;
      return;
    }

    const std::string & agenda::get_users_service_name() const
    {
      return _users_service_name_;
    }

    void agenda::set_resources(const vire::resource::manager & resources_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Agenda is initialized!");
      _resources_ = &resources_;
      return;
    }

    const vire::resource::manager & agenda::get_resources() const
    {
      return *_resources_;
    }

    void agenda::set_devices(const vire::device::manager & devices_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Agenda is initialized!");
      _devices_ = &devices_;
      return;
    }

    const vire::device::manager & agenda::get_devices() const
    {
      return *_devices_;
    }

    void agenda::set_users(const vire::user::manager & users_)
    {
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Agenda is initialized!");
      _users_ = &users_;
      return;
    }

    const vire::user::manager & agenda::get_users() const
    {
      return *_users_;
    }

    bool agenda::has_reservation(const int32_t id_) const
    {
      return _reservations_.count(id_) == 1;
    }

    int32_t agenda::get_last_reservation_id() const
    {
      return _last_reservation_id_;
    }

    int32_t agenda::get_next_reservation_id() const
    {
      return _last_reservation_id_ + 1;
    }

    int32_t agenda::add_reservation(const session_reservation & r_)
    {
      std::lock_guard<std::mutex> guard(_reservations_mutex_);
      int32_t id = session_info::INVALID_ID;
      if (r_.get_sinfo().has_id()) {
        id = r_.get_sinfo().get_id();
      }
      DT_THROW_IF(has_reservation(id),
                  std::logic_error,
                  "Reservation with ID=" << id << " already exists!");
      _reservations_[id] = r_;
      _last_reservation_id_ = id;
      _reservations_changed_ = true;
      return id;
    }

    void agenda::remove_reservation(const int32_t id_)
    {
      std::lock_guard<std::mutex> guard(_reservations_mutex_);
      DT_THROW_IF(!has_reservation(id_),
                  std::logic_error,
                  "No reservation with ID=" << id_ << " exists!");
      _reservations_.erase(id_);
      _reservations_changed_ = true;
      return;
    }

    void agenda::set_reservations_store(const std::string & path_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Agenda is already initialized!");
      _reservations_store_ = path_;
      return;
    }

    const std::string & agenda::get_reservations_store() const
    {
      return _reservations_store_;
    }

    void agenda::set_purge_store(const std::size_t ps_)
    {
      _purge_store_ = ps_;
      return;
    }

    std::size_t agenda::get_purge_store() const
    {
      return _purge_store_;
    }

    void agenda::_load_reservations_()
    {
      DT_LOG_DEBUG(get_logging_priority(), "Loading reservations...");
      std::lock_guard<std::mutex> guard(_reservations_mutex_);
      _reservations_.clear();
      if (boost::filesystem::exists(_reservations_store_)) {
        datatools::data_reader reader(_reservations_store_,
                                      datatools::using_multi_archives);
        std::string version = "1.0";
        reader.load("version", version);
        std::size_t number_of_reservations;
        reader.load("number_of_reservations", number_of_reservations);
        for (std::size_t i = 0; i < number_of_reservations; i++) {
          int32_t id;
          session_reservation res;
          // reader.load("session_id", id);
          reader.load("session_reservation", res);
          id = res.get_sinfo().get_id();
          _reservations_[id] = res;
          _last_reservation_id_ = id;
          DT_LOG_DEBUG(get_logging_priority(), "Loading reservation ID=[" << id << "]");
        }
      }
      return;
    }

    void agenda::_store_reservations_()
    {
      DT_LOG_DEBUG(get_logging_priority(), "Storing reservations...");
      std::lock_guard<std::mutex> guard(_reservations_mutex_);
      vire::utility::backup_file bak(_reservations_store_);
      if (bak.exists()) {
        bak.make_new_backup();
        bak.purge(_purge_store_);
      }
      datatools::data_writer writer(_reservations_store_,
                                    datatools::using_multi_archives);
      std::string version = "1.0";
      writer.store("version", version);
      writer.store("number_of_reservations", _reservations_.size());
      for (reservation_dict_type::const_iterator iter = _reservations_.begin();
           iter != _reservations_.end();
           iter++) {
        int32_t id = iter->first;
        const session_reservation & res = iter->second;
        // writer.store("session_id", id);
        writer.store("session_reservation", res);
      }
      if (_reservations_changed_) {
        _reservations_changed_ = false;
      }
      return;
    }

    void agenda::compute_all_sessions(std::vector<int32_t> & all_) const
    {
      all_.clear();
      for (reservation_dict_type::const_iterator iter = _reservations_.begin();
           iter != _reservations_.end();
           iter++) {
        int32_t id = iter->first;
        const session_reservation & res = iter->second;
        const session_info & sinfo = res.get_sinfo();
        all_.push_back(id);
      }
      return;
    }

    void agenda::compute_simultaneous_sessions(const boost::posix_time::ptime & time_,
                                               std::vector<int32_t> & simultaneous_) const
    {
      simultaneous_.clear();
      for (reservation_dict_type::const_iterator iter = _reservations_.begin();
           iter != _reservations_.end();
           iter++) {
        int32_t id = iter->first;
        const session_reservation & res = iter->second;
        const session_info & sinfo = res.get_sinfo();
        const boost::posix_time::time_period & when = sinfo.get_when();
        if (when.contains(time_)) {
          simultaneous_.push_back(id);
        }
      }
      return;
    }

    void agenda::compute_current_sessions(std::vector<int32_t> & current_) const
    {
      const boost::posix_time::ptime now = vire::time::now();
      compute_simultaneous_sessions(now, current_);
      return;
    }

    void agenda::compute_future_sessions(std::vector<int32_t> & future_) const
    {
      future_.clear();
      const boost::posix_time::ptime now = vire::time::now();
      for (reservation_dict_type::const_iterator iter = _reservations_.begin();
           iter != _reservations_.end();
           iter++) {
        int32_t id = iter->first;
        const session_reservation & res = iter->second;
        const session_info & sinfo = res.get_sinfo();
        const boost::posix_time::time_period & when = sinfo.get_when();
        const boost::posix_time::ptime begin = when.begin();
        if (now < begin) {
          future_.push_back(id);
        }
      }
      return;
    }

    void agenda::compute_past_sessions(std::vector<int32_t> & past_) const
    {
      past_.clear();
      const boost::posix_time::ptime now = vire::time::now();
      for (reservation_dict_type::const_iterator iter = _reservations_.begin();
           iter != _reservations_.end();
           iter++) {
        int32_t id = iter->first;
        const session_reservation & res = iter->second;
        const session_info & sinfo = res.get_sinfo();
        const boost::posix_time::time_period & when = sinfo.get_when();
        const boost::posix_time::ptime last = when.begin();
        if (last < now) {
          past_.push_back(id);
        }
      }
      return;
    }

    void agenda::compute_intersection_sessions(const boost::posix_time::time_period & interval_,
                                               std::vector<int32_t> & intersection_) const
    {
      intersection_.clear();
      for (reservation_dict_type::const_iterator iter = _reservations_.begin();
           iter != _reservations_.end();
           iter++) {
        int32_t id = iter->first;
        const session_reservation & res = iter->second;
        const session_info & sinfo = res.get_sinfo();
        const boost::posix_time::time_period & when = sinfo.get_when();
        if (interval_.intersects(when)) {
          intersection_.push_back(id);
        }
      }
      return;
    }

    int32_t agenda::next_session() const
    {
      std::vector<int32_t> future;
      compute_future_sessions(future);
      if (future.size()) {
        return future.front();
      }
      return session_info::INVALID_ID;
    }

    // bool agenda::has_session(const std::string & session_key_)
    // {
    //   return _sessions_.find(session_key_) != _sessions_.end();
    // }

    // const session_info & agenda::get_session(const std::string & session_key_) const
    // {
    //   sessions_dict_type::const_iterator found = _sessions_.find(session_key_);
    //   DT_THROW_IF(found == _sessions_.end(),
    //               std::logic_error,
    //               "No session with key '" << session_key_ << "' exists!");
    //   return found->second.sinfo;
    // }

    void agenda::_at_run_start_()
    {
      DT_LOG_DEBUG(get_logging_priority(), "At run start...");
      _load_reservations_();
      return;
    }

    void agenda::_at_run_loops_()
    {
      DT_LOG_DEBUG(get_logging_priority(), "At run loop...");
      _running_ = true;
      bool requested_stop = must_be_stopped();
      while (!requested_stop) {
        DT_LOG_DEBUG(get_logging_priority(), "New loop...");
        std::this_thread::sleep_for(std::chrono::microseconds(500000));

        if (_reservations_changed_) {
          _store_reservations_();
        }

        requested_stop = must_be_stopped();
        if (requested_stop) {
          DT_LOG_DEBUG(get_logging_priority(), "Break the main loop...");
          break;
        }
      }
      _running_ = false;
      DT_LOG_DEBUG(get_logging_priority(), "At run loop...");
      return;
    }

    void agenda::set_stop_file(const std::string & f_)
    {
      _stop_file_ = f_;
      return;
    }

    bool agenda::check_stop_file() const
    {
      if (boost::filesystem::exists(_stop_file_)) {
        return true;
      }
      return false;
    }

    void agenda::_at_run_stop_()
    {
      DT_LOG_DEBUG(get_logging_priority(), "At run stop...");
      if (_reservations_changed_) {
        _store_reservations_();
      }
      return;
    }

    // static
    void agenda::_run_agenda_(agenda * ag_)
    {
      DT_LOG_DEBUG(ag_->get_logging_priority(), "Main thread function...");
      ag_->_at_run_start_();
      // ag_->_running_ = true;
      ag_->_at_run_loops_();
      // ag_->_running_ = false;
      ag_->_at_run_stop_();
      return;
    }

    void agenda::start()
    {
      DT_LOG_DEBUG(get_logging_priority(), "Starting agenda's main thread...");
      DT_THROW_IF(!is_initialized(), std::logic_error, "Agenda is not initialized!");
      DT_THROW_IF(is_running(), std::logic_error, "Agenda is already running!");
      _thread_.reset(new std::thread(agenda::_run_agenda_, this));
      _thread_->detach();
      DT_LOG_DEBUG(get_logging_priority(), "Agenda's main thread is started.");
      return;
    }

    void agenda::stop()
    {
      DT_THROW_IF(!is_running(), std::logic_error, "Agenda should stop!");
      _must_be_stopped_ = true;
      return;
    }

    // virtual
    bool agenda::is_running() const
    {
      return _running_;
    }

    bool agenda::must_be_stopped() const
    {
      if (check_stop_file()) return true;
      return _must_be_stopped_;
    }

  } // namespace cmsserver

} // namespace vire
