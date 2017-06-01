//! \file  vire/user/user.h
//! \brief Vire user representing an user account
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_USER_USER_H
#define VIRE_USER_USER_H

// Standard library:
#include <string>
#include <set>
#include <map>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
// - Bayeux/datatools:
#include <datatools/enriched_base.h>
#include <datatools/handle.h>

namespace vire {

  //! Nested namespace for the Vire library's user module
  namespace user {

    //! \brief User description
    class user : public datatools::enriched_base
    {
    public:

      static const int32_t INVALID_UID = -1; //!< Invalid user identifier (UID)
      static const int32_t ADMIN_UID   =  0; //!< Super administrator user identifier (UID)
      static const int32_t MIN_UID     =  ADMIN_UID; //!< Minimum allowed value for user identifier (UID)

      /// Return the admin user login
      static const std::string & admin_user();

      //! Default constructor
      user();

      //! Destructor
      virtual ~user();

      //! Check if the user is valid
      bool is_valid() const;

      //! Check the validity of the UID
      bool has_valid_uid() const;

      //! Set the UID
      void set_uid(int32_t);

      //! Return the UID
      int32_t get_uid() const;

      //! Check if the user is administrator
      bool is_administrator() const;

      //! Check the user
      void check() const;

      //! Initialization
      void initialize_simple();

      //! Initialization from a container of configuration parameters
      void initialize(const datatools::properties & config_);

      //! \brief Flags for export to a container of properties
      enum user_xc_flags {
        USER_XC_UID     = (datatools::enriched_base::EXPORT_CONFIG_LAST << 1),
        USER_XC_DEFAULT = (datatools::enriched_base::EXPORT_CONFIG_DEFAULT
                           | datatools::enriched_base::EXPORT_CONFIG_NAME
                           | USER_XC_UID),
        USER_XC_LAST    = (USER_XC_UID)
      };

      //! Export to a container of configuration parameters
      void export_to_config(datatools::properties & config_,
                            uint32_t flags_ = USER_XC_DEFAULT,
                            const std::string & prefix_ = "") const;

      //! Reset
      void reset();

      //! Smart print
      // virtual
      void tree_dump(std::ostream & out_ = std::clog,
                     const std::string & title_  = "",
                     const std::string & indent_ = "",
                     bool inherit_ = false) const;

    protected:

      //! Set default attribute values
      void _set_defaults();

    private:

      int32_t _uid_; //!< User unique identifier (UID)

      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION_ADVANCED(user);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

    //! Type alias for a handle to an user object
    typedef datatools::handle<user> user_handle_type;

    //! Type alias for a dictionary of users with UID as the primary key
    typedef std::map<int32_t, user_handle_type> user_dict_type;

  } // namespace user

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::user::user);
DR_CLASS_INIT(vire::user::user_handle_type);
DR_CLASS_INIT(vire::user::user_dict_type);
#endif // Q_MOC_RUN

#endif // VIRE_USER_USER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
