//! \file  vire/resource/role.h
//! \brief Vire role with respect on resources
//
// Copyright (c) 2015-2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_RESOURCE_ROLE_H
#define VIRE_RESOURCE_ROLE_H

// Standard library:
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <memory>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
// - Bayeux/datatools:
#include <datatools/enriched_base.h>
#include <datatools/handle.h>
#include <datatools/properties.h>
#include <datatools/bit_mask.h>
// - Bayeux/cuts:
#include <cuts/cut_tools.h>
#include <cuts/i_cut.h>

namespace vire {

  namespace resource {

    // Forward declaration:
    class resource;
    class manager;

    //! \brief Role description
    class role
      : public ::datatools::enriched_base
    {
    public:

      static const int32_t UNIVERSE_ID          =  0; //!< Universe role identifier
      static const int32_t INVALID_ID           = -1; //!< Invalid role identifier
      static const int32_t ANONYMOUS_DYNAMIC_ID = -2; //!< Anonymous dynamic/private role (local scope)
      static const int32_t MAX_DYNAMIC_ID       = -3; //!< Maximum dynamic/private role (local scope)

      //! \brief Flag for set of resources
      enum resource_set_flag {
        RESOURCE_SET_NONE          = 0, //!< No resource at all
        RESOURCE_SET_FUNCTIONAL    = datatools::bit_mask::bit00, //!< Tag for functional resources
        RESOURCE_SET_DISTRIBUTABLE = datatools::bit_mask::bit01, //!< Tag for distributable resources
        RESOURCE_SET_SCOPE         = RESOURCE_SET_FUNCTIONAL | RESOURCE_SET_DISTRIBUTABLE //!< Tag for scope resources
      };

      //! Default constructor
      role();

      //! Copy constructor
      role(const role & role_);

      //! Destructor
      virtual ~role();

      //! Assignment operator
      role & operator=(const role & role_);

      // =============== Initialization/termination methods ===============

      //! Initialization
      void initialize_simple();

      //! Initialization from a container of configuration parameters
      /**
       * Configuration example:
       * @code
       * id   : integer = 42
       * path : string  = "calo_expert"
       * functional_resource.selector_type_id   : string    = "vire::resource::enumerated_resource_selector"
       * functional_resource.selector.resources : string[4] = \
       *   "Calo/HV/Start" \
       *   "Calo/HV/Stop" \
       *   "Calo/HV/Lock" \
       *   "Calo/HV/Unlock"
       * distributable_resource.selector_type_id   : string    = "vire::resource::enumerated_resource_selector"
       * distributable_resource.selector.resources : string[3] = \
       *   "Calo/HV/PS/Initialize" \
       *   "Calo/HV/PS/LoadConfiguration" \
       *   "Calo/HV/PS/Reset"
       * build_cache    : boolean   = false
       * allowed_users  : string[3] = "johndoe" "bill" "foo"
       * allowed_groups : string[2] = "grp1" "grp2"
       * @endcode
       *
       */
      void initialize(const datatools::properties & config_);

      //! Reset
      void reset();

      //! Set the role identifier
      void set_id(int32_t);

      //! Reset the role identifier
      void reset_id();

      //! Set the path
      void set_path(const std::string &);

      //! Set the functional resource selector handle
      void set_functional_resource_selector_handle(cuts::cut_handle_type &);

      //! Reset the functional resource selector handle
      void reset_functional_resource_selector_handle();

      //! Return the functional resource selector handle
      cuts::cut_handle_type & grab_functional_resource_selector_handle();

      //! Set the distributable resource selector handle
      void set_distributable_resource_selector_handle(cuts::cut_handle_type &);

      //! Reset the distributable resource selector handle
      void reset_distributable_resource_selector_handle();

      //! Return the distributable resource selector handle
      cuts::cut_handle_type & grab_distributable_resource_selector_handle();

      //! Set the resource manager
      void set_resource_manager(const manager & rmgr_);

      //! Add allowed user
      void add_allowed_user(int32_t uid_);

      //! Remove allowed user
      void remove_allowed_user(int32_t uid_);

      //! Add allowed group
      void add_allowed_group(int32_t gid_);

      //! Remove allowed group
      void remove_allowed_group(int32_t gid_);

      // =============== Usage methods ===============

      //! Check initialization flag
      bool is_initialized() const;

      //! Check if the role is valid
      bool is_valid() const;

      //! Check if the role has a valid identifier
      bool has_id() const;

      //! Return the role identifier
      int32_t get_id() const;

      //! Check if the given role identifier is associated to a registered role
      bool is_registered() const;

      //! Check if the role is the universe role
      bool is_universe() const;

      //! Check if the given role identifier is associated to a dynamic role
      bool is_dynamic() const;

      //! Check the validity of the path
      bool has_path() const;

      //! Return the path
      const std::string & get_path() const;

      //! Check if a resource, given its RID, belongs to the role
      bool has_resource_id(int32_t resource_id_, uint32_t flags_ = RESOURCE_SET_SCOPE) const;

      //! Check if a resource, given its path, belongs to the role
      bool has_resource_path(const std::string & rpath_, uint32_t flags_ = RESOURCE_SET_SCOPE) const;

      //! Check if the functional resource selector is defined
      bool has_functional_resource_selector() const;

      //! Return the functional resource selector handle
      const cuts::cut_handle_type & get_functional_resource_selector_handle() const;

      //! Return a non mutable reference to the embedded functional resource selector
      const cuts::i_cut & get_functional_resource_selector() const;

      //! Check if the distributable resource selector is defined
      bool has_distributable_resource_selector() const;

      //! Return the distributable resource selector handle
      const cuts::cut_handle_type & get_distributable_resource_selector_handle() const;

      //! Return a non mutable reference to the embedded distributable resource selector
      const cuts::i_cut & get_distributable_resource_selector() const;

      //! Check if a resource manager is referenced
      bool has_resource_manager() const;

      //! Return the resource manager
      const manager & get_resource_manager() const;

      //! Check if the set of scope resource identifiers (RID) is cached
      bool has_cached_scope_resource_ids() const;

      //! Check if the set of functional resource identifiers (RID) is cached
      bool has_cached_functional_resource_ids() const;

      //! Check if the set of distributable resource identifiers (RID) is cached
      bool has_cached_distributable_resource_ids() const;

      //! Build the cached set of scope resource identifiers (RID)
      void build_cached_scope_resource_ids();

      //! Build the cached set of functional resource identifiers (RID)
      void build_cached_functional_resource_ids();

      //! Build the cached set of distributable resource identifiers (RID)
      void build_cached_distributable_resource_ids();

      //! Build all the cached set of resource identifiers (RID)
      void build_cached();

      //! Return the set of scope resource IDs in the role with respect of a set of reference (universe) in the manager
      const std::set<int32_t> & get_set_of_scope_resource_ids() const;

      //! Return the set of functional resource IDs in the role with respect of a set of reference (universe) in the manager
      const std::set<int32_t> & get_set_of_functional_resource_ids() const;

      //! Return the set of distributable resource IDs in the role with respect of a set of reference (universe) in the manager
      const std::set<int32_t> & get_set_of_distributable_resource_ids() const;

      //! Check if a given user is allowed
      bool has_allowed_user(int32_t uid_) const;

      //! Check if a given user group is allowed
      bool has_allowed_group(int32_t gid_) const;

      //! Return the set of allowed users
      const std::set<int32_t> & get_allowed_users() const;

      //! Return the set of allowed groups
      const std::set<int32_t> & get_allowed_groups() const;

      //! \brief Flags for export to a container of properties
      enum role_xc_flags {
        ROLE_XC_ID                = (datatools::enriched_base::EXPORT_CONFIG_LAST << 1),
        ROLE_XC_PATH              = (ROLE_XC_ID << 2),
        ROLE_XC_ALLOWED_USERS     = (ROLE_XC_ID << 3),
        ROLE_XC_ALLOWED_GROUPS    = (ROLE_XC_ID << 4),
        ROLE_XC_FUNCTIONAL_RESOURCE_SELECTOR = (ROLE_XC_ID << 5),
        ROLE_XC_DISTRIBUTABLE_RESOURCE_SELECTOR = (ROLE_XC_ID << 6),
        ROLE_XC_DEFAULT = (datatools::enriched_base::EXPORT_CONFIG_DEFAULT
                           | datatools::enriched_base::EXPORT_CONFIG_NAME
                           | ROLE_XC_ID
                           | ROLE_XC_PATH
                           | ROLE_XC_ALLOWED_USERS
                           | ROLE_XC_ALLOWED_GROUPS
                           | ROLE_XC_FUNCTIONAL_RESOURCE_SELECTOR
                           | ROLE_XC_DISTRIBUTABLE_RESOURCE_SELECTOR),
        ROLE_XC_LAST    = (ROLE_XC_DISTRIBUTABLE_RESOURCE_SELECTOR)
      };

      //! Export to a container of configuration parameters
      void export_to_config(datatools::properties & config_,
                            uint32_t flags_ = ROLE_XC_DEFAULT,
                            const std::string & prefix_ = "") const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      // =============== Utilities ===============

      //! Return the instance of the cut factory register
      static cuts::i_cut::factory_register_type & resource_selector_factory_register();

      //! Build the set of RIDs with respect of a set of reference (universe)
      static void build_set_of_resource_ids(const role & r_,
                                            uint32_t flags_,
                                            const std::set<int32_t> & universe_,
                                            std::set<int32_t> & resource_ids_);

      //! \brief Comparison by role ID
      struct compare_by_id {
        bool operator()(int32_t id_, const role & r_) const {
          return id_ < r_._id_;
        }
        bool operator()(const role & r_, int32_t id_) const {
          return r_._id_ < id_;
        }
      };

      //! \brief Comparison by name
      struct compare_by_name {
        bool operator()(const std::string & name_, const role & r_) const {
          return name_ < r_.get_name();
        }
        bool operator()(const role & r_, const std::string & name_) const {
          return r_.get_name() < name_;
        }
      };

      //! \brief Comparison by path
      struct compare_by_path {
        bool operator()(const std::string & path_, const role & r_) const {
          return path_ < r_._path_;
        }
        bool operator()(const role & r_, const std::string & path_) const {
          return r_._path_ < path_;
        }
      };

      // =============== Private/protected methods ===============

    protected:

      //! Set default attribute values
      void _set_defaults();

      //! Set the role identifier
      void _set_id(int32_t);

    private:

      //! Copy method
      void _copy_from_(const role &);

      //! Build the scope selector from functional and distributable resource selector
      void _build_scope_selector();

      // =============== Attributes ===============

    private:

      // Management:
      bool                  _initialized_; //!< Initialization flag

      // External resources:
      const manager *       _resource_manager_; //!< Handle to an external resource manager

      // Configuration:
      int32_t               _id_;             //!< Role unique identifier
      std::string           _path_;           //!< Role unique path
      std::set<int32_t>     _allowed_users_;  //!< Collection of allowed users
      std::set<int32_t>     _allowed_groups_; //!< Collection of allowed groups
      cuts::cut_handle_type _functional_resource_selector_handle_;    //!< Handle to the selector of public functional resources
      cuts::cut_handle_type _distributable_resource_selector_handle_; //!< Handle to the selector of public distributable resources

      // Working data:
      cuts::cut_handle_type _scope_resource_selector_handle_;         //!< Handle to the selector of public scope resources

      // Cached data:
      std::unique_ptr<std::set<int32_t> > _cached_distributable_resource_ids_; //!< Cached enumerated set of distributable resource identifiers
      std::unique_ptr<std::set<int32_t> > _cached_functional_resource_ids_;    //!< Cached enumerated set of functional resource identifiers
      std::unique_ptr<std::set<int32_t> > _cached_scope_resource_ids_;         //!< Cached enumerated set of scope resource identifiers

      friend manager;

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace resource

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::resource::role);
// DR_CLASS_INIT(vire::role::role_handle_type);
// DR_CLASS_INIT(vire::role::role_dict_type);
#endif // Q_MOC_RUN

#endif // VIRE_RESOURCE_ROLE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
