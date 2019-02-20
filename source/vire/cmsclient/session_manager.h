//! \file  vire/cmsclient/session_manager.h
//! \brief Session manager
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

#ifndef VIRE_CMSCLIENT_SESSION_MANAGER_H
#define VIRE_CMSCLIENT_SESSION_MANAGER_H

// Third party:
// - Bayeux:
#include <bayeux/datatools/base_service.h>

// This project:
#include <vire/com/manager.h>
#include <vire/device/manager.h>
#include <vire/resource/manager.h>

// This project:
#include <vire/cmsclient/client.h>
#include <vire/cmsclient/session_connection_parameters.h>

namespace vire {

  namespace cmsclient {

    /// \brief Session manager
    class session_manager
      : public ::datatools::base_service
    {
    public:

      enum fsm_state {
        FSM_STATE_INVALID,
        FSM_STATE_IDLE,
        FSM_STATE_NEGOTIATING,
        FSM_STATE_CONNECTED,
        FSM_STATE_STOPPED,
        FSM_STATE_ERROR
      };

      static std::string state_to_label(const fsm_state);

      /// Default constructor
      session_manager();

      /// Destructor
      virtual ~session_manager();

      //! Return the logging threshold
      datatools::logger::priority get_logging() const;

      //! Set the logging threshold
      void set_logging(const datatools::logger::priority);

      bool has_client() const;

      void set_client(client & client_);

      bool has_com() const;

      void set_com(vire::com::manager & com_);

      const vire::com::manager & get_com() const;

      void reset_com();

      bool has_devices() const;

      void set_devices(const vire::device::manager & devices_);

      const vire::device::manager & get_devices() const;

      void reset_devices();

      bool has_resources() const;

      void set_resources(const vire::resource::manager & resources_);

      const vire::resource::manager & get_resources() const;

      void reset_resources();

      bool is_idle() const;

      bool is_negotiating() const;

      bool is_connected() const;

      bool is_stopped() const;

      bool is_error() const;

      bool is_stop_run_requested() const;

      void run();

      //! Check initialization status
      virtual bool is_initialized() const;

      //! Initialize the service from a list of properties
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & services_);

      //! Reset
      virtual int reset();

      /// Smart print
      virtual void tree_dump(std::ostream & out = std::clog,
                             const std::string & title = "",
                             const std::string & indent = "",
                             bool inherit = false) const;
   private:

      void _configure_com_for_negotiation();

      void _deconfigure_com();

      void _configure_com_for_connexion();

      void _loop_();

    private:

      // Management:
      bool _initialized_ = false;            //!< Initialization status
      fsm_state _state_ = FSM_STATE_INVALID; //!< Session management FSM current state
      bool _stop_run_requested_ = false;     //!< Requested stop flag

      // Configuration:
      client * _client_ = nullptr;           //!< Parent client
      vire::com::manager * _com_ = nullptr;  //!< Handle to the com service

      // Working data:
      const vire::device::manager   * _devices_ = nullptr;   //!< Handle to the device service
      const vire::resource::manager * _resources_ = nullptr; //!< Handle to the resource service
      session_connection_parameters   _nego_result_;

      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(session_manager)

    };

  } // namespace cmsclient

} // namespace vire

#endif // VIRE_CMSCLIENT_SESSION_MANAGER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
