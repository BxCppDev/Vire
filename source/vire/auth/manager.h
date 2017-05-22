//! \file  vire/auth/manager.h
//! \brief Vire authentication manager
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_AUTH_MANAGER_H
#define VIRE_AUTH_MANAGER_H

// Standard library:
#include <string>
#include <set>
#include <map>

// Third party:
// - Bayeux/datatools:
#include <datatools/bit_mask.h>
#include <datatools/base_service.h>

// This package:
#include <vire/auth/credentials.h>

namespace vire {

  namespace user {
    class manager;
  }

  namespace auth {

    //! \brief Authentication manager
    class manager : public ::datatools::base_service
    {
    public:

      /// \brief Setup flags
      enum setup_flags {
        DONT_STORE_TABLES  = datatools::bit_mask::bit00, //!< Do not store tables at reset
        DONT_BACKUP_TABLES = datatools::bit_mask::bit01, //!< Do not backup tables at store
        DONT_LOAD_TABLES   = datatools::bit_mask::bit02, //!< Do not load tables at init
        LOG_TRACE          = datatools::bit_mask::bit03  //!< Activate trace logging
      };

      static const int DEFAULT_EXPIRE_AFTER = 90;

      enum auth_status {
        AUTH_SUCCESS          = 0,
        AUTH_INVALID_LOGIN    = 1,
        AUTH_LOCKED           = 2,
        AUTH_EXPIRED          = 3,
        AUTH_INVALID_PASSWORD = 4
      };

      //! Default constructor
      manager(uint32_t flags_ = 0);

      //! Destructor
      virtual ~manager();

      //! Check the users service
      bool has_users() const;

      //! Return the users service
      void set_users(const vire::user::manager & users_);

      //! Return a const handle to the users service
      const vire::user::manager & get_users() const;

      //! Check the existence of a credentials table path
      bool has_credentials_table_path() const;

      //! Return the path of the credentials table path
      const std::string & get_credentials_table_path() const;

      //! Set the path of the credentials table path
      void set_credentials_table_path(const std::string &);

      //! Check the flag for not loading tables
      bool dont_load_tables() const;

      //! Check the flag for loading tables
      bool is_load_tables() const;

      //! Check the flag for not storing tables
      bool dont_store_tables() const;

      //! Check the flag for storing tables
      bool is_store_tables() const;

      //! Check the flag for not backuping tables
      bool dont_backup_tables() const;

      //! Check the flag for backuping tables
      bool is_backup_tables() const;

      //! Set the flag for denying tables loading
      void set_dont_load_tables(bool);

      //! Set the flag for denying tables storage
      void set_dont_store_tables(bool);

      //! Set the flag for not backuping tables
      void set_dont_backup_tables(bool);

      //! Check the existence of a credentials given its login
      bool has_credentials(const std::string & login_) const;

      //! Check the validity of a credentials given its login
      bool is_credentials_valid(const std::string & login_) const;

      //! Return a const reference to an existing credentials given its login
      const credentials & get_credentials(const std::string & login_) const;

      //! Return a mutable reference to an existing credentials given its login
      credentials & grab_credentials(const std::string & login_);

      /// \brief Credentials creation flag
      enum creation_flags {
        create_defaults        = datatools::bit_mask::bit00, ///< Set default min/max/warn time
        create_lock            = datatools::bit_mask::bit01, ///< Lock the account ar creation
        create_no_expiration   = datatools::bit_mask::bit02, ///< Set no expiration date
        create_with_password   = datatools::bit_mask::bit03, ///< Set password at start
        create_check_login     = datatools::bit_mask::bit04  ///< Check validity of login, using user service
      };

      //! Create a new credentials given its login
      credentials & create_new_credentials(const std::string & login_,
                                           uint32_t flags_ = 0,
                                           const std::string & password_ = "");

      //! Remove an existing credentials given its login
      void remove_credentials(const std::string & login_);

      //! Remove all credentials
      void remove_all_credentials();

      //! Load the tables
      void load_tables(uint32_t load_flags_ = 0);

      //! Store the tables
      void store_tables(uint32_t store_flags_ = 0) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Check if the service is initialized
      virtual bool is_initialized() const;

      //! Initialize the service using a list of properties with access to a dictionary of services
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the service
      virtual int reset();

      //! Authentication
      auth_status authenticate(const std::string & login_, const std::string & password_) const;

    protected:

      //! Set default attribute values
      void _set_defaults();

      //! Create a new credentials given its login
      credentials & _create_new_credentials(const std::string & login_,
                                            uint32_t flags_,
                                            const std::string & password_);

      //! Create a credentials given its login
      void _remove_credentials(const std::string & login_);

      //! Load credentials table
      void _load_credentials_table(const std::string & source_, uint32_t flags_);

      //! Store credentials table
      void _store_credentials_table(const std::string & target_, uint32_t flags_) const;

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration attributes:
      const vire::user::manager * _users_ = nullptr; //!< Users service
      std::string _credentials_table_path_;     //!< Path to the credentials table storage
      bool        _dont_load_tables_ = false;   //!< Flag to load tables at initialization
      bool        _dont_store_tables_ = false;  //!< Flag to store tables at reset
      bool        _dont_backup_tables_ = false; //!< Flag to not backup tables

      // Working data:
      std::map<std::string, credentials> _credentials_; //!< Dictionary of credentials

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(manager);

    };

  } // namespace auth

} // namespace vire

// OCD support:
#include <datatools/ocd_macros.h>
// @arg vire::auth::manager the name of the class with OCD support
DOCD_CLASS_DECLARATION(vire::auth::manager)

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::auth::manager);
DR_TYPE_INIT(vire::auth::manager::auth_status);
#endif // Q_MOC_RUN

#endif // VIRE_AUTH_MANAGER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
