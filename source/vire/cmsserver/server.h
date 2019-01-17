//! \file  vire/cmsserver/server.h
//! \brief Vire CMS server
//
// Copyright (c) 2017-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                            Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_SERVER_H
#define VIRE_CMSSERVER_SERVER_H

// Standard library:
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>
#include <ctime>
//#include <atomic>

// Third party:
// - Qt:
#include <QObject>
// - Boost:
#include <boost/noncopyable.hpp>
// - Bayeux:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/multi_properties.h>
#include <bayeux/datatools/service_manager.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/library_loader.h>

// This project:
#include <vire/utility/instance_identifier.h>
#include <vire/running/run_control.h>
#include <vire/running/i_runnable.h>

namespace vire {

  namespace cmsserver {

    class server;
      
    /// \brief Vire CMS server Qt-based signal emitter
    struct server_signal_emitter
      : public QObject
    {
      Q_OBJECT
    public:
      
      server_signal_emitter(server & server_);

      const server & get_server() const;
      
      server & grab_server() const;

      void emit_stop();

    signals:
      
      void sig_stop();
  
    private:

      server & _server_;

    };

    /// \brief Vire CMS server
    class server
      : public datatools::i_tree_dumpable
      , public vire::running::i_runnable
      , private boost::noncopyable
    {
    public:

      static std::string log_service_name();

      static std::string devices_service_name();

      static std::string resources_service_name();

      static std::string users_service_name();

      static std::string sc_service_name();

      static std::string auth_service_name();

      static std::string com_service_name();

      static std::string sessions_service_name();

      static std::string agenda_service_name();

      static std::string pubsub_service_name();

      static std::string control_service_name();

      static std::string clients_service_name();

      static std::string gate_service_name();

      //! Default constructor
      server();

      //! Destructor
      virtual ~server();

      //! Return the logging threshold
      datatools::logger::priority get_logging() const;

      //! Set the logging threshold
      void set_logging(const datatools::logger::priority);

      //! Check if setup ID is set
      bool has_setup_id() const;

      //! Return the setup ID
      const ::vire::utility::instance_identifier & get_setup_id() const;

      //! Set the setup ID
      //!
      //! @code
      //! vire::cmssserver::server svr;
      //! vire::utility::instance_identifier sid("SuperNEMO");
      //! svr.set_setup_id(sid);
      //! @endcode
      void set_setup_id(const ::vire::utility::instance_identifier &);

      //! Check if CMS top path is set
      bool has_cms_top_path() const;

      //! Set the CMS top path
      //!
      //! @code
      //! vire::cmssserver::server svr;
      //! svr.set_cms_top_path("SuperNEMO:/Demonstrator/CMS");
      //! @endcode
      //!
      //! If setup ID is not set, set it to the scheme of the top_path.
      void set_cms_top_path(const std::string &);

      //! Return the CMS top path
      const std::string & get_cms_top_path() const;

      //! Return the DLL loading default path
      const std::string & get_dll_load_default_path() const;

      //! Set the DLL loading default path
      void set_dll_load_default_path(const std::string &);

      //! Add a DLL load directive
      void add_dll_load_directive(const std::string &);

      //! Return the DLL load directives
      std::vector<std::string> get_dll_load_directives() const;

      //! Check initialization status
      bool is_initialized() const;

      //! Initialize the server
      void initialize(const datatools::properties &);

      //! Terminate the server
      void reset();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Return the mutable service manager
      datatools::service_manager & grab_services();

      //! Return the non mutable service manager
      const datatools::service_manager & get_services() const;

      //! Run the server
      void run() override;

      //! Request stop
      void stop() override;

      const vire::running::run_control & get_rc() const override;
      
      const server_signal_emitter & get_emitter() const; 
     
      server_signal_emitter & grab_emitter(); 

    private:

      void _at_init_(const datatools::properties &);

      void _init_dlls_(const datatools::properties &);

      void _init_core_(const datatools::properties &);

      void _start_pre_system_services();

      void _start_business_services();

      void _compute_resource_responsible_();
      
      void _start_post_system_services();

      void _stop_post_system_services();

      void _stop_business_services();

      void _stop_pre_system_services();

      void _reset_core_();

      void _reset_dlls_();

      void _at_reset_();

      // Running:
      void _at_run_();

      void _at_run_start_();

      void _at_run_stop_();

      void _at_run_loop_();

    private:

      // Management:
      bool                        _initialized_ = false;
      datatools::logger::priority _logging_;

      // Configuration:
      ::vire::utility::instance_identifier _setup_id_;     //!< Setup ID
      std::string                          _cms_top_path_; //!< CMS top path
      datatools::multi_properties          _mconfig_;      //!< Server configuration parameters
      std::string                          _dll_load_default_path_;
      std::vector<std::string>             _dll_load_directives_;
      std::size_t                          _tick_ms_ = 500;

      // Working:
      std::unique_ptr<datatools::library_loader>  _dll_loader_;
      std::unique_ptr<datatools::service_manager> _services_;
      vire::running::run_control  _rc_; ///< Run control

      struct pimpl_type;
      std::unique_ptr<pimpl_type> _pimpl_;
      
      // Qt signal emitter wrapper:
      std::unique_ptr<server_signal_emitter> _emitter_;

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_SERVER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
