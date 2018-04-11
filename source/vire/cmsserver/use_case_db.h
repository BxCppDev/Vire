//! \file  vire/cmsserver/use_case_db.h
//! \brief The CMS use case database associated to an experiment
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

#ifndef VIRE_CMSSERVER_USE_CASE_DB_H
#define VIRE_CMSSERVER_USE_CASE_DB_H

// Standard Library:
#include <string>
#include <memory>
#include <map>
#include <mutex>

// Third party:
// - Bayeux/datatools:
#include <datatools/base_service.h>
#include <datatools/properties.h>

// This project:
#include <vire/user/manager.h>
#include <vire/device/manager.h>
#include <vire/resource/manager.h>

namespace vire {

  namespace cmsserver {

    class use_case_info;

    /// \brief The CMS use case database service
    class use_case_db
      : public ::datatools::base_service
    {
    public:

      typedef std::shared_ptr<use_case_info> use_case_info_ptr_type;
      typedef std::map<std::string, use_case_info_ptr_type> db_dict_type;

      /// Default constructor
      use_case_db(uint32_t flags_ = 0);

      /// Destructor
      virtual ~use_case_db();

      bool has_users() const;

      void set_users(const vire::user::manager &);

      const vire::user::manager & get_users() const;

      bool has_devices() const;

      void set_devices(const vire::device::manager &);

      const vire::device::manager & get_devices() const;

      bool has_resources() const;

      void set_resources(const vire::resource::manager &);

      const vire::resource::manager & get_resources() const;

      //! Check the initialization flag
      virtual bool is_initialized() const;

      //! Initialize the manager from a container of properties
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the manager
      virtual int reset();

      //! Add record
      void add(const std::string & name_,
               const std::string & description_,
               const std::string & role_definition_,
               const std::string & use_case_type_id_,
               const std::string & use_case_config_path_,
               const datatools::properties & use_case_config_);

      //! Remove a record
      void remove(const std::string & name_);

      //! Check if a record exists
      bool has(const std::string & name_) const;

      //! Check if a record exists and is of given type
      bool is_a(const std::string & name_,
                const std::string & use_case_type_id_) const;

      //! Load some
      void load(const std::string &);

    private:

      //! Set default attributes' values
      void _set_defaults_();

    private:

      // Management:
      bool _initialized_ = false;
      std::mutex _db_mutex_;

      // Configuration:
      vire::user::manager * _users_ = nullptr;
      vire::device::manager * _devices_ = nullptr;
      vire::resource::manager * _resources_ = nullptr;

      // Working data:
      db_dict_type _db_;

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(use_case_db)

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_USE_CASE_DB_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
