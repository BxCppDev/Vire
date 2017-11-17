//! \file  vire/user/group.h
//! \brief Vire user group
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

#ifndef VIRE_USER_GROUP_H
#define VIRE_USER_GROUP_H

// Standard library:
#include <iostream>
#include <string>
#include <set>
#include <map>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
// - Bayeux/datatools:
#include <datatools/enriched_base.h>
#include <datatools/handle.h>
// - Bayeux/cuts:
#include <cuts/cut_tools.h>
#include <cuts/i_cut.h>

namespace vire {

  namespace user {

    // Forward declaration:
    class group;
    class manager;

    //! Type alias for a handle to an user group object
    typedef datatools::handle<group> group_handle_type;

    //! Type alias for a dictionary of user groups with GID as the primary key
    typedef std::map<int32_t, group_handle_type> group_dict_type;

    //! \brief User group description
    //!
    //! A group is defined by its unique group identifier (GUI) and name.
    //! It is also associated to a special embedded object, the user selector,
    //! which can determine if a given user belongs to the group or not.
    class group
      : public datatools::enriched_base
    {
    public:

      static const int32_t INVALID_GID = -1;         //!< Invalid user group identifier (GID)
      static const int32_t ADMIN_GID   =  0;         //!< Super administrator user group identifier (GID)
      static const int32_t MIN_GID     =  ADMIN_GID; //!< Minimum allowed value for user group identifier (GID)

      /// Return the admin group name
      static const std::string & admin_group();

      //! Default constructor
      group();

      //! Destructor
      virtual ~group();

      //! Check if the user group is valid
      bool is_valid() const;

      //! Check if the user group has a valid identifier (GID)
      bool has_valid_gid() const;

      //! Set the user group identifier (GID)
      void set_gid(int32_t);

      //! Return the user group identifier (GID)
      int32_t get_gid() const;

      //! Check if an user, given its UID, belongs to the group
      bool has_uid(int32_t uid_) const;

      //! Build the set of UIDs with respect of a set of reference (universe)
      void build_set_of_uids(const std::set<int32_t> & universe_,
                             std::set<int32_t> & uids_) const;

      //! Return the set of UIDs in the group with respect of a set of reference (universe)
      std::set<int32_t> set_of_uids(const std::set<int32_t> & universe_) const;

      //! Check if the user selector is defined
      bool has_user_selector() const;

      //! Set the user selector handle
      void set_user_selector_handle(cuts::cut_handle_type &);

      //! Return the user selector handle
      cuts::cut_handle_type & grab_user_selector_handle();

      //! Return the user selector handle
      const cuts::cut_handle_type & get_user_selector_handle() const;

      //! Return a non mutable reference to the embedded user selector
      const cuts::i_cut & get_user_selector() const;

      //! Check user group status
      void check() const;

      //! Initialization
      void initialize_simple();

      //! Initialization from a container of configuration parameters
      void initialize_standalone(const datatools::properties & config_);

      //! Initialization from a container of configuration parameters and
      //! a user manager
      void initialize(const datatools::properties & config_,
                      manager & user_mgr_);

      //! \brief Flags for export to a container of properties
      enum export_config_flags {
        GROUP_XC_GID      = (datatools::enriched_base::EXPORT_CONFIG_LAST << 1),
        GROUP_XC_SELECTOR = (GROUP_XC_GID << 1),
        GROUP_XC_DEFAULT  = (datatools::enriched_base::EXPORT_CONFIG_DEFAULT
                             | datatools::enriched_base::EXPORT_CONFIG_NAME
                             | GROUP_XC_GID
                             | GROUP_XC_SELECTOR),
        GROUP_XC_LAST     = (GROUP_XC_SELECTOR)
      };

      //! Export to a container of configuration parameters
      void export_to_config(datatools::properties & config_,
                            uint32_t flags_ = GROUP_XC_DEFAULT,
                            const std::string & prefix_ = "") const;

      //! Reset
      void reset();

      //! Smart print
      // virtual
      void tree_dump(std::ostream & out_ = std::clog,
                     const std::string & title_  = "",
                     const std::string & indent_ = "",
                     bool inherit_ = false) const;

      //! Return the instance of the cut factory register
      static cuts::i_cut::factory_register_type & user_selector_factory_register();

    protected:

      //! Set default attribute values
      void _set_defaults();

    private:

      int32_t               _gid_; //!< User group unique identifier (GID)
      cuts::cut_handle_type _users_selector_handle_; //!< Handle to an user selector

      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION_ADVANCED(group);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace user

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::user::group);
DR_CLASS_INIT(vire::user::group_handle_type);
DR_CLASS_INIT(vire::user::group_dict_type);
#endif // Q_MOC_RUN

#endif // VIRE_USER_GROUP_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
