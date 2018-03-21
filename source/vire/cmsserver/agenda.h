//! \file  vire/cmsserver/agenda.h
//! \brief Vire CMS server agenda
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

#ifndef VIRE_CMSSERVER_AGENDA_H
#define VIRE_CMSSERVER_AGENDA_H

// Standard Library:
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <thread>

// Third party:
// - Bayeux:
#include <bayeux/datatools/base_service.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/resource/manager.h>
#include <vire/device/manager.h>
#include <vire/user/manager.h>
#include <vire/cmsserver/session_reservation.h>
#include <vire/cmsserver/session_info.h>

namespace vire {

  namespace cmsserver {

    //! \brief The CMS server agenda service
    class agenda
      : public ::datatools::base_service
    {
    public:

      typedef std::map<int32_t, session_reservation> reservation_dict_type;

      //! Default constructor
      agenda(uint32_t flags_ = 0);

      //! Destructor
      virtual ~agenda();

      //! Check if the service is initialized
      virtual bool is_initialized() const;

      //! Initialize the service using a list of properties with access to a dictionary of services
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the service
      virtual int reset();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      void set_resources_service_name(const std::string &);

      const std::string & get_resources_service_name() const;

      void set_devices_service_name(const std::string &);

      const std::string & get_devices_service_name() const;

      void set_users_service_name(const std::string &);

      const std::string & get_users_service_name() const;

      void set_resources(const vire::resource::manager & resources_);

      const vire::resource::manager & get_resources() const;

      void set_devices(const vire::device::manager & devices_);

      const vire::device::manager & get_devices() const;

      void set_users(const vire::user::manager & users_);

      const vire::user::manager & get_users() const;

      int32_t get_next_reservation_id() const;

      int32_t get_last_reservation_id() const;

      bool has_reservation(const int32_t id_) const;

      int32_t add_reservation(const session_reservation & r_);

      void remove_reservation(const int32_t id_);

      void set_reservations_store(const std::string &);

      void set_purge_store(const std::size_t);

      std::size_t get_purge_store() const;

      const std::string & get_reservations_store() const;

      /// Return the list of IDs of all sessions
      void compute_all_sessions(std::vector<int32_t> & future_) const;

      /// Return the list of IDs of sessions which contains a given time
      void compute_simultaneous_sessions(const boost::posix_time::ptime & time_,
                                         std::vector<int32_t> & simultaneous_) const;

      /// Return the list of IDs of current sessions
      void compute_current_sessions(std::vector<int32_t> & current_) const;

      /// Return the list of IDs of future sessions (not started)
      void compute_future_sessions(std::vector<int32_t> & future_) const;

      /// Return the list of IDs of past sessions (finished)
      void compute_past_sessions(std::vector<int32_t> & past_) const;

      /// Return the list of IDs of sessions which intersect with a period
      void compute_intersection_sessions(const boost::posix_time::time_period & interval_,
                                         std::vector<int32_t> & intersection_) const;

      /// Return the ID of the next session reservation to be started
      int32_t next_session() const;

      void set_stop_file(const std::string &);

      bool is_running() const;

      void start();

      void stop();

      bool check_stop_file() const;

      bool must_be_stopped() const;

    private:

      void _set_defaults_();

      static void _run_agenda_(agenda * ag_);

      void _at_run_start_();

      void _at_run_loops_();

      void _at_run_stop_();

      void _load_reservations_();

      void _store_reservations_();

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration:
      std::string _devices_service_name_;   //!< Name of the device manager service
      std::string _resources_service_name_; //!< Name of the resource manager service
      std::string _users_service_name_;     //!< Name of the users manager service
      std::string _reservations_store_;     //!< Name of the reservations store file
      std::string _stop_file_;              //!< Name of the forced-stop file
      std::size_t _purge_store_;            //!< Flag to automatically purge the reservation store

      // Working data:
      std::unique_ptr<std::thread> _thread_;
      bool _running_         = false; //!< Running state flag
      bool _must_be_stopped_ = false; //!< Must be stopped flag

      const vire::resource::manager * _resources_ = nullptr;
      const vire::device::manager   * _devices_   = nullptr;
      const vire::user::manager     * _users_     = nullptr;
      std::mutex            _reservations_mutex_;
      reservation_dict_type _reservations_;
      // reservation sequences ???
      bool                  _reservations_changed_;
      int32_t               _last_reservation_id_  = session_reservation::INVALID_ID;

      //! Auto-registration of this service class in the Bayeux/datatools' central service database
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(agenda)

    };

  } // namespace cmsserver

} // namespace vire

// OCD support:
#include <datatools/ocd_macros.h>
// @arg vire::auth::manager the name of the class with OCD support
DOCD_CLASS_DECLARATION(vire::cmsserver::agenda)

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::cmsserver::agenda);
#endif // Q_MOC_RUN

#endif // VIRE_CMSSERVER_AGENDA_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
