//! \file  vire/resource/manager.h
//! \brief Vire resource manager
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

#ifndef VIRE_RESOURCE_MANAGER_H
#define VIRE_RESOURCE_MANAGER_H

// Standard library:
#include <string>
#include <set>
#include <map>
#include <memory>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>
// - Bayeux/datatools:
#include <datatools/base_service.h>
#include <datatools/properties.h>
#include <datatools/bit_mask.h>

namespace vire {

  namespace device {
    class manager;
  }

  namespace resource {

    // Forward declarations:
    class resource;
    class role;

    //! \brief Resource and role management service
    class manager
      : public ::datatools::base_service
      , private boost::noncopyable
    {
    public:

      /// \brief Setup flags
      enum setup_flags {
        DONT_STORE_TABLES  = datatools::bit_mask::bit00, //!< Do not store tables at reset
        DONT_BACKUP_TABLES = datatools::bit_mask::bit01, //!< Do not backup tables at store
        DONT_LOAD_TABLES   = datatools::bit_mask::bit02, //!< Do not load tables at init
        LOG_TRACE          = datatools::bit_mask::bit03  //!< Activate trace logging
      };

      //! Type alias for a set of resource identifiers (RIDs)
      typedef std::set<int32_t> set_of_resource_ids;

      static const std::string & default_service_name();

      //! Return the default universe role name
      static const std::string & default_universe_role_name();

      //! Default constructor
      manager(uint32_t flags_ = 0);

      //! Destructor
      virtual ~manager();

      //! Check if the name of the device manager service is set
      bool has_devices_service_name() const;

      //! Set the name of the device manager service
      void set_devices_service_name(const std::string &);

      //! Return the name of the device manager service
      const std::string & get_devices_service_name() const;

      //! Set the path of the roles table path
      void set_roles_table_path(const std::string &);

      //! Set the flag for denying tables loading
      void set_dont_load_tables(bool);

      //! Set the flag for denying tables storage
      void set_dont_store_tables(bool);

      //! Set the flag for denying tables backup
      void set_dont_backup_tables(bool);

      //! Add a resource
      void add_resource(const resource &);

      //! Remove an existing resource given its name
      void remove_resource_by_name(const std::string & name_);

      //! Remove an existing resource given its path
      void remove_resource_by_path(const std::string & path_);

      //! Remove an existing resource given its identifier (RID)
      void remove_resource_by_id(int32_t id_);

      //! Remove all resources
      void remove_all_resources();

      //! Add a role
      void add_role(const role &);

      //! Remove an existing role given its path
      void remove_role_by_path(const std::string & path_);

      //! Remove an existing role given its name
      void remove_role_by_name(const std::string & name_);

      //! Remove an existing role given its identifier
      void remove_role_by_id(int32_t role_id_);

      //! Remove all roles
      void remove_all_roles();

      //! Load the tables
      void load_tables(uint32_t load_flags_ = 0);

      //! Initialize the manager from a container of properties
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the manager
      virtual int reset();

      //! Build resources from internal services
      void build_resources_from_internal_services(const datatools::service_manager & service_manager_,
                                                  uint32_t flags_ = 0);

      //! Build resources from a device manager
      void build_resources_from_devices(const vire::device::manager & device_manager_,
                                        uint32_t flags_ = 0);

      //! Check the initialization flag
      virtual bool is_initialized() const;

      //! Check the existence of a role table path
      bool has_roles_table_path() const;

      //! Return the path of the roles table path
      const std::string & get_roles_table_path() const;

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

      //! Return the number of resources
      std::size_t get_number_of_resources() const;

      //! Return the number of roles
      std::size_t get_number_of_roles() const;

      //! Check the existence of a resource given its path
      bool has_resource_by_path(const std::string & path_) const;

      //! Check the existence of a resource given its name
      bool has_resource_by_name(const std::string & name_) const;

      //! Check the existence of a resource given its RID
      bool has_resource_by_id(int32_t id_) const;

      //! Return a const reference to an existing resource given its path
      const resource & get_resource_by_path(const std::string & path_) const;

      //! Return a const reference to an existing resource given its name
      const resource & get_resource_by_name(const std::string & name_) const;

      //! Return a const reference to an existing resource given its RID
      const resource & get_resource_by_id(int32_t id_) const;

      //! Check if the set of resource identifiers (RID) is cached
      bool has_cached_resource_ids() const;

      //! Buil the cached set of resource identifiers (RID)
      void build_cached_resource_ids();

      //! Return the set of RIDs
      const std::set<int32_t> & get_set_of_resource_ids() const;

      //! Build a set of resource identifiers (RID) from a regexp applied to paths
      //!
      //! Path regular expressions use the POSIX Extended Regular Expression Syntax
      //! from the Boost/Regex library.
      void build_set_of_resource_ids_from_path_regexp(const std::string & path_regexp_,
                                                      std::set<int32_t> & resource_ids_) const;

      //! Build a set of resource identifiers (RID) from a regexp applied to paths
      //!
      //! Path regular expressions use the POSIX Extended Regular Expression Syntax
      //! from the Boost/Regex library.
      void build_set_of_resource_ids_from_path_regexp_within_device(const std::string & device_path_,
                                                                  const std::string & path_regexp_,
                                                                  std::set<int32_t> & resource_ids_) const;

      //! Build a set of resource identifiers (RID) from a list of resource paths
      void build_set_of_resource_ids_from_paths(const std::vector<std::string> & rpath_,
                                                std::set<int32_t> & resource_ids_) const;

      //! Build a set of resource identifiers (RID) from a responsible
      void build_set_of_resource_ids_from_responsible(const std::string & responsible_,
                                                      std::set<int32_t> & resource_ids_) const;

      //! Check the existence of a role given its path
      bool has_role_by_path(const std::string & path_) const;

      //! Check the existence of a role given its name
      bool has_role_by_name(const std::string & name_) const;

      //! Check the existence of a role given its identifier
      bool has_role_by_id(int32_t role_id_) const;

      //! Return a const reference to an existing role given its path
      const role & get_role_by_path(const std::string & path_) const;

      //! Return a const reference to an existing role given its name
      const role & get_role_by_name(const std::string & name_) const;

      //! Return a const reference to an existing role given its identifier
      const role & get_role_by_id(int32_t role_rid_) const;

      //! Build a set of role identifiers (RLID)
      void build_set_of_role_ids(std::set<int32_t> & role_ids_) const;

      //! Build a set of role names
      void build_set_of_role_names(std::set<std::string> & role_names_) const;

      //! Build a set of role paths
      void build_set_of_role_paths(std::set<std::string> & role_paths_) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Compute a new (unused) resource identifier (RID)
      int32_t candidate_resource_id() const;

      //! Compute a new (unused) role identifier (RLID)
      int32_t candidate_role_id() const;

      //! Store the tables
      void store_tables(uint32_t store_flags_ = 0) const;

      //! Build a universe role (root/system role that contains all resources)
      //!
      //! The cached functional, distributable and scope set of resources
      //! are automatically built.
      void build_universe_role(role & ur_);

      //! Fetch the identifier of a valid resource from a textual representation
      bool fetch_resource_id(const std::string & repr_, int32_t & resource_id_) const;

    protected:

      //! Set default attribute values
      void _set_defaults();

      //! Build and load the universe role
      void _do_build_universe_role();

      //! Load roles table
      virtual void _load_roles_table(const std::string & source_, uint32_t flags_);

      //! Store roles tables
      virtual void _store_roles_table(const std::string & target_, uint32_t flags_) const;

    private:

      //! Construct the PIMPL-ized data
      void _construct_data_();

      //! Destroy the PIMPL-ized data
      void _destroy_data_();

      // =============== Attributes ===============

    private:

      // Management:
      bool _initialized_; //!< Initialization flag

      // Configuration parameters:
      std::string _roles_table_path_;     //!< Path to the blessed roles table storage
      bool        _dont_load_tables_;     //!< Flag to load tables at initialization
      bool        _dont_store_tables_;    //!< Flag to store tables at reset
      bool        _dont_backup_tables_;   //!< Flag to backup tables
      std::string _devices_service_name_; //!< Device manager service name

      // Working data:
      // PIMPL-ized data structure:
      struct data;
      std::unique_ptr<data> _data_; //!< Pimpl-ized internal data

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(manager);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace resource

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::resource::manager);
#endif // Q_MOC_RUN

#endif // VIRE_RESOURCE_MANAGER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
