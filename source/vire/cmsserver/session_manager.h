//! \file  vire/cmsserver/session_manager.h
//! \brief Vire CMS server session manager
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

#ifndef VIRE_CMSSERVER_SESSION_MANAGER_H
#define VIRE_CMSSERVER_SESSION_MANAGER_H

// Standard Library:
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <memory>

// Third party:
// - Bayeux:
#include <bayeux/datatools/base_service.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/resource/manager.h>
#include <vire/device/manager.h>
#include <vire/user/manager.h>
#include <vire/cmsserver/session_reservation.h>
// #include <vire/cmsserver/session_info.h>

namespace vire {

  namespace cmsserver {

    class session;

    //! \brief The CMS server session manager
    class session_manager
      : public ::datatools::base_service
    {
    public:

      typedef std::shared_ptr<session>           session_handle_type;
      typedef std::map<int, session_handle_type> session_dict_type;

      //! Default constructor
      session_manager(uint32_t flags_ = 0);

      //! Destructor
      virtual ~session_manager();

      void set_users_name(const std::string &);

      const std::string & get_users_name() const;

      void set_user_manager(const vire::user::manager & mgr_);

      void set_devices_name(const std::string &);

      const std::string & get_devices_name() const;

      void set_device_manager(const vire::device::manager & mgr_);

      void set_resources_name(const std::string &);

      const std::string & get_resources_name() const;

      void set_resource_manager(const vire::resource::manager & mgr_);

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

      //! Run
      void run();

    private:

      void _set_defaults_();

      void _create_root_session_();

      void _run_root_session_();

      void _destroy_root_session_();

      // std::shared_ptr<session> _create_subsession_(const session_ptr_type & parent_,
      //                                              int);

      // Working data:
      const vire::user::manager     * _users_ = nullptr;     ///< Handle to the user manager
      const vire::device::manager   * _devices_ = nullptr;   ///< Handle to the device manager
      const vire::resource::manager * _resources_ = nullptr; ///< Handle to the resource manager
      int _next_id_ = -1;
      session_handle_type _root_session_;

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(session_manager)

    };

  } // namespace cmsserver

} // namespace vire

// OCD support:
#include <datatools/ocd_macros.h>
DOCD_CLASS_DECLARATION(vire::cmsserver::session_manager)

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::cmsserver::session_manager);
#endif // Q_MOC_RUN

#endif // VIRE_CMSSERVER_SESSION_MANAGER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
