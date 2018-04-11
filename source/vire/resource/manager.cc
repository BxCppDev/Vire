// vire/resource/manager.cc
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

// Ourselves:
#include <vire/resource/manager.h>

// Standard library:
#include <typeinfo>

// Third party:
// - Boost:
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/system/error_code.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/multi_properties.h>
#include <bayeux/datatools/introspection/method.h>
// - Bayeux/cuts:
#include <bayeux/cuts/accept_cut.h>

// This project:
#include <vire/resource/resource.h>
#include <vire/resource/role.h>
#include <vire/utility/rw_access.h>
#include <vire/device/manager.h>
#include <vire/resource/devices_to_resources_builder.h>
#include <vire/user/user.h>
#include <vire/user/group.h>

namespace vire {

  namespace resource {

    // Auto-registration of this service class in a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(manager, "vire::resource::manager");

    // Tags for accessing the corresponding indices of resource_set :
    struct resource_tag_id {};
    struct resource_tag_path {};
    struct resource_tag_name {};

    // static
    const std::string & manager::default_service_name()
    {
      static std::string _name("resources");
      return _name;
    }

    //! Multi index container of resources
    typedef boost::multi_index_container<
      resource,
      boost::multi_index::indexed_by<

        // The first key is the unique resource identifier (RID):
        boost::multi_index::ordered_unique<
          boost::multi_index::tag<resource_tag_id>,
          boost::multi_index::const_mem_fun<resource,
                                            int32_t,
                                            &resource::get_id
                                            >
          >,

        // The second key is the unique resource name:
        boost::multi_index::ordered_unique<
          boost::multi_index::tag<resource_tag_name>,
          boost::multi_index::const_mem_fun<datatools::enriched_base,
                                            const std::string &,
                                            &datatools::enriched_base::get_name
                                            >
          >,

        // The third key is the unique resource path:
        boost::multi_index::ordered_unique<
          boost::multi_index::tag<resource_tag_path>,
          boost::multi_index::const_mem_fun<resource,
                                            const std::string &,
                                            &resource::get_path
                                            >
          >
        >
      > resource_set;

    typedef resource_set::index<resource_tag_id>::type   resource_set_by_id;
    typedef resource_set::index<resource_tag_path>::type resource_set_by_path;
    typedef resource_set::index<resource_tag_name>::type resource_set_by_name;

    // Tags for accessing the corresponding indices of role_set :
    struct role_tag_id {};
    struct role_tag_name {};
    struct role_tag_path {};

    //! Multi index container of roles
    typedef boost::multi_index_container<
      role,
      boost::multi_index::indexed_by<

        // The first key is the unique role identifier (role ID):
        boost::multi_index::ordered_unique<
          boost::multi_index::tag<role_tag_id>,
          boost::multi_index::const_mem_fun<role,
                                            int32_t,
                                            &role::get_id
                                            >
          >,

        // The first key is the unique role identifier (role path):
        boost::multi_index::ordered_unique<
          boost::multi_index::tag<role_tag_path>,
          boost::multi_index::const_mem_fun<role,
                                            const std::string &,
                                            &role::get_path
                                            >
          >,

        // The second key is the unique role name path:
        boost::multi_index::ordered_unique<
          boost::multi_index::tag<role_tag_name>,
          boost::multi_index::const_mem_fun<datatools::enriched_base,
                                            const std::string &,
                                            &datatools::enriched_base::get_name
                                            >
          >
        >
      > role_set;

    typedef role_set::index<role_tag_id>::type   role_set_by_id;
    typedef role_set::index<role_tag_path>::type role_set_by_path;
    typedef role_set::index<role_tag_name>::type role_set_by_name;

    struct manager::data
    {
      data();
      ~data();
      const vire::device::manager * devices = nullptr;
      resource_set resources; //!< Set of resources
      role_set     roles;     //!< Set of roles
      std::unique_ptr<std::set<int32_t> > cached_resource_ids; //!< Cached set of resource ids
    };

    manager::data::data()
    {
      return;
    }

    manager::data::~data()
    {
      cached_resource_ids.reset();
      roles.clear();
      resources.clear();
      devices = nullptr;
      return;
    }

    // static
    const std::string & manager::default_universe_role_name()
    {
      static const std::string _name("__universe__");
      return _name;
    }

    bool manager::has_roles_table_path() const
    {
      return !_roles_table_path_.empty();
    }

    const std::string & manager::get_roles_table_path() const
    {
      return _roles_table_path_;
    }

    void manager::set_roles_table_path(const std::string & ut_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Role manager is already initialized!");
      _roles_table_path_ = ut_;
      return;
    }

    bool manager::dont_load_tables() const
    {
      return _dont_load_tables_;
    }

    bool manager::is_load_tables() const
    {
      return _dont_load_tables_ == false;
    }

    bool manager::dont_store_tables() const
    {
      return _dont_store_tables_;
    }

    bool manager::is_store_tables() const
    {
      return _dont_store_tables_ == false;
    }

    bool manager::dont_backup_tables() const
    {
      return _dont_backup_tables_;
    }

    bool manager::is_backup_tables() const
    {
      return _dont_backup_tables_ == false;
    }

    void manager::set_dont_load_tables(bool flag_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Role manager is already initialized!");
      _dont_load_tables_ = flag_;
      DT_LOG_TRACE(get_logging_priority(), "Set dont_load_tables = " << std::boolalpha << _dont_load_tables_);
      return;
    }

    void manager::set_dont_store_tables(bool flag_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Role manager is already initialized!");
      _dont_store_tables_ = flag_;
      DT_LOG_TRACE(get_logging_priority(), "Set dont_store_tables = " << std::boolalpha << _dont_store_tables_);
      return;
    }

    void manager::set_dont_backup_tables(bool flag_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Role manager is already initialized!");
      _dont_backup_tables_ = flag_;
      DT_LOG_TRACE(get_logging_priority(), "Set dont_backup_tables = " << std::boolalpha << _dont_backup_tables_);
      return;
    }

    void manager::_construct_data_()
    {
      _data_.reset(new data);
      return;
    }

    void manager::_destroy_data_()
    {
      _data_.reset();
      return;
    }

    manager::manager(uint32_t flags_)
    {
      _initialized_ = false;
      set_logging_priority(datatools::logger::PRIO_FATAL);
      _set_defaults();
      if (flags_ & LOG_TRACE) {
        set_logging_priority(datatools::logger::PRIO_TRACE);
      }
      if (flags_ & DONT_LOAD_TABLES) {
        set_dont_load_tables(true);
      }
      if (flags_ & DONT_STORE_TABLES) {
        set_dont_store_tables(true);
      }
      if (flags_ & DONT_BACKUP_TABLES) {
        set_dont_backup_tables(true);
      }
      _construct_data_();
      return;
    }

    manager::~manager()
    {
      if (is_initialized()) {
        reset();
      }
      _destroy_data_();
      return;
    }

    std::size_t manager::get_number_of_resources() const
    {
      return _data_->resources.size();
    }

    std::size_t manager::get_number_of_roles() const
    {
      return _data_->roles.size();
    }

    void manager::add_resource(const resource & r_)
    {
      DT_THROW_IF(! r_.has_path(), std::logic_error, "Resource has no path!");
      if (!r_.has_id()) {
        resource & r = const_cast<resource &>(r_);
        r._set_id(candidate_resource_id());
      } else {
        DT_THROW_IF(has_resource_by_id(r_.get_id()),
                    std::logic_error,
                    "Resource manager already has a resource with ID=[" << r_.get_id() << "]");
      }
      _data_->resources.insert(r_);
      return;
    }

    bool manager::has_resource_by_name(const std::string & name_) const
    {
      const resource_set_by_name & name_index = _data_->resources.get<resource_tag_name>();
      resource_set_by_name::const_iterator found = name_index.find(name_);
      return found != name_index.end();
    }

    bool manager::has_resource_by_path(const std::string & path_) const
    {
      const resource_set_by_path & path_index = _data_->resources.get<resource_tag_path>();
      resource_set_by_path::const_iterator found = path_index.find(path_);
      return found != path_index.end();
    }

    bool manager::has_resource_by_id(int32_t id_) const
    {
      const resource_set_by_id & id_index = _data_->resources.get<resource_tag_id>();
      resource_set_by_id::const_iterator found = id_index.find(id_);
      return found != id_index.end();
    }

    const resource & manager::get_resource_by_name(const std::string & name_) const
    {
      const resource_set_by_name & name_index = _data_->resources.get<resource_tag_name>();
      resource_set_by_name::const_iterator found = name_index.find(name_);
      DT_THROW_IF(found == name_index.end(), std::logic_error,
                  "Cannot find resource with name='" << name_ << "'!");
      return *found;
    }

    const resource & manager::get_resource_by_path(const std::string & path_) const
    {
      const resource_set_by_path & path_index = _data_->resources.get<resource_tag_path>();
      resource_set_by_path::const_iterator found = path_index.find(path_);
      DT_THROW_IF(found == path_index.end(), std::logic_error,
                  "Cannot find resource with PATH='" << path_ << "'!");
      return *found;
    }

    const resource & manager::get_resource_by_id(int32_t id_) const
    {
      const resource_set_by_id & id_index = _data_->resources.get<resource_tag_id>();
      resource_set_by_id::const_iterator found = id_index.find(id_);
      DT_THROW_IF(found == id_index.end(), std::logic_error,
                  "Cannot find resource with RID=[" << id_ << "]!");
      return *found;
    }

    void manager::remove_resource_by_name(const std::string & name_)
    {
      resource_set_by_name & name_index = _data_->resources.get<resource_tag_name>();
      resource_set_by_name::iterator found = name_index.find(name_);
      DT_THROW_IF(found == name_index.end(), std::logic_error,
                  "Cannot find resource with NAME='" << name_ << "'!");
      name_index.erase(found);
      return;
    }

    void manager::remove_resource_by_path(const std::string & path_)
    {
      resource_set_by_path & path_index = _data_->resources.get<resource_tag_path>();
      resource_set_by_path::iterator found = path_index.find(path_);
      DT_THROW_IF(found == path_index.end(), std::logic_error,
                  "Cannot find resource with PATH='" << path_ << "'!");
      path_index.erase(found);
      return;
    }

    void manager::remove_resource_by_id(int32_t id_)
    {
      resource_set_by_id & id_index = _data_->resources.get<resource_tag_id>();
      resource_set_by_id::iterator found = id_index.find(id_);
      DT_THROW_IF(found == id_index.end(), std::logic_error,
                  "Cannot find resource with RID=[" << id_ << "]!");
      id_index.erase(found);
      return;
    }

    bool manager::has_cached_resource_ids() const
    {
      return _data_->cached_resource_ids.get() != 0;
    }

    void manager::build_cached_resource_ids()
    {
      if (_data_->cached_resource_ids.get() == nullptr) {
        _data_->cached_resource_ids.reset(new std::set<int32_t>);
        const resource_set_by_id & id_index = _data_->resources.get<resource_tag_id>();
        for (resource_set_by_id::const_iterator it = id_index.begin();
             it != id_index.end();
             it++) {
          const resource & r = *it;
          _data_->cached_resource_ids->insert(r.get_id());
        }
      }
      return;
    }

    const std::set<int32_t> & manager::get_set_of_resource_ids() const
    {
      bool build_it = false;
      if (!has_cached_resource_ids()) {
        build_it = true;
      } else if (_data_->cached_resource_ids->size() != _data_->resources.size()) {
        build_it = true;
      }
      if (build_it) {
        manager * mutable_this = const_cast<manager*>(this);
        mutable_this->build_cached_resource_ids();
      }
      return *_data_->cached_resource_ids;
    }

    void manager::add_role(const role & role_)
    {
      DT_THROW_IF(! role_.has_path(), std::logic_error, "Role has no path!");
      if (! role_.has_id()) {
        role & r = const_cast<role &>(role_);
        r._set_id(candidate_role_id());
      } else {
        DT_THROW_IF(has_role_by_id(role_.get_id()),
                    std::logic_error,
                    "Resource manager already has a role with ID=[" << role_.get_id() << "]");
      }
      // role_.tree_dump(std::cerr, "ADD ROLE: " + role_.get_name(), "DEVEL: ");
      _data_->roles.insert(role_);
      return;
    }

    bool manager::has_role_by_name(const std::string & role_name_) const
    {
      const role_set_by_name & name_index = _data_->roles.get<role_tag_name>();
      role_set_by_name::const_iterator found = name_index.find(role_name_);
      return found != name_index.end();
    }

    bool manager::has_role_by_path(const std::string & path_) const
    {
      const role_set_by_path & path_index = _data_->roles.get<role_tag_path>();
      role_set_by_path::const_iterator found = path_index.find(path_);
      return found != path_index.end();
    }

    bool manager::has_role_by_id(int32_t id_) const
    {
      const role_set_by_id & role_index = _data_->roles.get<role_tag_id>();
      role_set_by_id::const_iterator found = role_index.find(id_);
      return found != role_index.end();
    }

    const role & manager::get_role_by_name(const std::string & name_) const
    {
      const role_set_by_name & name_index = _data_->roles.get<role_tag_name>();
      role_set_by_name::const_iterator found = name_index.find(name_);
      DT_THROW_IF(found == name_index.end(), std::logic_error,
                  "Cannot find role with name='" << name_ << "'!");
      return *found;
    }

    const role & manager::get_role_by_path(const std::string & path_) const
    {
      const role_set_by_path & path_index = _data_->roles.get<role_tag_path>();
      role_set_by_path::const_iterator found = path_index.find(path_);
      DT_THROW_IF(found == path_index.end(), std::logic_error,
                  "Cannot find role with path='" << path_ << "'!");
      return *found;
    }

    const role & manager::get_role_by_id(int32_t id_) const
    {
      const role_set_by_id & id_index = _data_->roles.get<role_tag_id>();
      role_set_by_id::const_iterator found = id_index.find(id_);
      DT_THROW_IF(found == id_index.end(), std::logic_error,
                  "Cannot find role with role ID=[" << id_ << "]!");
      return *found;
    }

    void manager::remove_role_by_name(const std::string & name_)
    {
      role_set_by_name & name_index = _data_->roles.get<role_tag_name>();
      role_set_by_name::iterator found = name_index.find(name_);
      DT_THROW_IF(found == name_index.end(), std::logic_error,
                  "Cannot find role with name='" << name_ << "'!");
      name_index.erase(found);
      return;
    }

    void manager::remove_role_by_path(const std::string & path_)
    {
      role_set_by_path & path_index = _data_->roles.get<role_tag_path>();
      role_set_by_path::iterator found = path_index.find(path_);
      DT_THROW_IF(found == path_index.end(), std::logic_error,
                  "Cannot find role with path='" << path_ << "'!");
      path_index.erase(found);
      return;
    }

    void manager::remove_role_by_id(int32_t id_)
    {
      role_set_by_id & id_index = _data_->roles.get<role_tag_id>();
      role_set_by_id::iterator found = id_index.find(id_);
      DT_THROW_IF(found == id_index.end(), std::logic_error,
                  "Cannot find role with role ID=[" << id_ << "]!");
      id_index.erase(found);
      return;
    }

    void manager::remove_all_resources()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Resource manager is not initialized!");
      _data_->resources.clear();
      return;
    }

    void manager::remove_all_roles()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Resource manager is not initialized!");
      _data_->roles.clear();
      return;
    }

    void manager::_set_defaults()
    {
      _dont_load_tables_ = false;
      _dont_store_tables_ = false;
      _dont_backup_tables_ = false;
      _roles_table_path_ = "";
      return;
    }

    void manager::load_tables(uint32_t load_flags_)
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Resource manager is not initialized!");
      // load roles:
      // remove_all_roles();
      DT_THROW_IF(!has_roles_table_path(),
                  std::logic_error,
                  "No roles table path is defined!");
      _load_roles_table(_roles_table_path_, load_flags_);
      return;
    }

    void manager::store_tables(uint32_t store_flags_) const
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Resource manager is not initialized!");
      DT_THROW_IF(!has_roles_table_path(),
                  std::logic_error,
                  "No roles table path is defined!");
      _store_roles_table(_roles_table_path_, store_flags_);
      return;
    }

    bool manager::has_devices_service_name() const
    {
      return !_devices_service_name_.empty();
    }

    void manager::set_devices_service_name(const std::string & n_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource manager is already initialized!");
      _devices_service_name_ = n_;
      return;
    }

    const std::string & manager::get_devices_service_name() const
    {
      return _devices_service_name_;
    }

    bool manager::is_initialized() const
    {
      return _initialized_;
    }

    int manager::initialize(const datatools::properties & config_,
                            datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(this->is_initialized(), std::logic_error, "Resource manager is already initialized !");

      this->::datatools::base_service::common_initialize(config_);

      DT_LOG_DEBUG(get_logging_priority(), "Parsing configuration parameters...");
      bool do_build_cache = false;
      bool do_build_from_devices = false;
      // bool do_build_from_services = false;
      bool do_build_universe_role = true;

      // Fetch configuration parameters:
      if (config_.has_key("build_from_devices")) {
        do_build_from_devices = config_.fetch_boolean("build_from_devices");
      }

      if (config_.has_key("build_cache")) {
        do_build_cache = config_.fetch_boolean("build_cache");
      }

      if (config_.has_key("build_universe_role")) {
        do_build_universe_role = config_.fetch_boolean("build_universe_role");
      }

      if (do_build_from_devices) {
        DT_LOG_DEBUG(get_logging_priority(), "About to build resources from devices definitions.");
        if (!has_devices_service_name()) {
          if (config_.has_key("devices_service_name")) {
            std::string  device_manager_name = config_.fetch_string("devices_service_name");
            set_devices_service_name(device_manager_name);
          }
        }
        // if (has_devices_service_name()) {
        //   // Automatically pickup the first device manager from the service manager:
        //   datatools::find_service_name_with_id(service_dict_,
        //                                        "vire::device::manager",
        //                                        get_devices_service_name());
        // }
        DT_THROW_IF(!has_devices_service_name(),
                    std::logic_error,
                    "No device manager service name is set!");
        const vire::device::manager & device_mgr
          = datatools::get<vire::device::manager>(service_dict_,
                                                  get_devices_service_name());

        // datatools::service_dict_type::const_iterator found =
        //   service_dict_.find(device_manager_name);
        // DT_THROW_IF(found == service_dict_.end(),
        //             std::logic_error,
        //             "No device manager found in the service manager!");
        // const vire::device::manager & device_mgr
        //   = dynamic_cast<const vire::device::manager &>(found->second->get_service_handle().get());
        uint32_t dev_flags = 0;
        build_resources_from_devices(device_mgr, dev_flags);
        DT_LOG_DEBUG(get_logging_priority(), "Number of resources : " << get_number_of_resources());
       }

      if (datatools::logger::is_debug(get_logging_priority())) {
        std::set<int32_t> resource_ids;
        build_set_of_resource_ids_from_path_regexp(".*", resource_ids);
        for (auto rid : resource_ids) {
          const resource & res = get_resource_by_id(rid);
          // std::cerr << "DEVEL: Resource [" << rid << "] : " << res.get_path() << std::endl;
          DT_LOG_DEBUG(get_logging_priority(), "Resource [" << rid << "] : " << res.get_path() );
        }
      }

      // Specific actions:
      if (do_build_cache) {
        build_cached_resource_ids();
      }

      if (_roles_table_path_.empty()) {
        if (config_.has_key("roles_table_path")) {
          set_roles_table_path(config_.fetch_string("roles_table_path"));
        }
      }

      if (config_.has_key("dont_load_tables")) {
        set_dont_load_tables(config_.fetch_boolean("dont_load_tables"));
      }

      if (config_.has_key("dont_store_tables")) {
        set_dont_store_tables(config_.fetch_boolean("dont_store_tables"));
      }

      if (is_store_tables()) {
        if (config_.has_key("dont_backup_tables")) {
          set_dont_backup_tables(config_.fetch_boolean("dont_backup_tables"));
        }
      }

      // Checks:
      // if (!has_roles_table_path()) {
      //   // set_roles_table_path("~/.vire/cms/server/roles.defs");
      //   // set_roles_table_path("roles.defs");
      // }
      DT_THROW_IF(!has_roles_table_path(), std::logic_error,
                  "Missing roles table path!");

      if (do_build_universe_role) {
        _do_build_universe_role();
      }

      // Initialization:
      datatools::fetch_path_with_env(_roles_table_path_);
      if (boost::filesystem::exists(_roles_table_path_)) {
        if (is_load_tables()) {
          DT_LOG_DEBUG(get_logging_priority(), "Loading roles table...");
          _load_roles_table(_roles_table_path_, 0);
          DT_LOG_DEBUG(get_logging_priority(), "Loading roles table done.");
        }
      } else {
        DT_LOG_DEBUG(get_logging_priority(), "No roles table file '" << _roles_table_path_ << "' exists!");
      }

      // Set the initialization flag:
      _initialized_ = true;

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    int manager::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!_initialized_, std::logic_error, "Manager is not initialized !");
      _initialized_ = false;
      if (is_store_tables()) {
        DT_LOG_TRACE(get_logging_priority(), "About to store the roles in table...");
        _store_roles_table(_roles_table_path_, 0);
        DT_LOG_TRACE(get_logging_priority(), "Role tables is stored.");
      }
      _data_->roles.erase(_data_->roles.get<role_tag_id>().begin(),
                          _data_->roles.get<role_tag_id>().end());
      _data_->resources.erase(_data_->resources.get<resource_tag_id>().begin(),
                              _data_->resources.get<resource_tag_id>().end());
      _data_->cached_resource_ids.reset();
      _set_defaults();

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    // virtual
    void manager::_load_roles_table(const std::string & source_, uint32_t /* flags_ */)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      std::string source = source_;
      datatools::fetch_path_with_env(source);
      DT_LOG_DEBUG(get_logging_priority(), "Source = '" << source << "'");
      datatools::multi_properties roles_table("id", "type", "Vire roles table");
      roles_table.read(source);
      std::vector<std::string> id_keys;
      roles_table.ordered_keys(id_keys);

      for (auto id_key : id_keys) {
        const datatools::multi_properties::entry & role_entry = roles_table.get(id_key);
        const std::string & meta = role_entry.get_meta();
        if (meta != "vire::resource::role") continue; // Should we throw ?
        std::istringstream id_iss(role_entry.get_key());
        int role_id;
        id_iss >> role_id;
        DT_THROW_IF(!id_iss, std::logic_error,
                    "Invalid format for role ID key ('" << role_entry.get_key() << "')!");
        DT_THROW_IF(role_id <= role::INVALID_ID, std::logic_error,
                    "Invalid registered role ID [" << role_id << "]!");
        const datatools::properties & role_config = role_entry.get_properties();
        {
          role new_role;
          new_role.set_id(role_id);
          new_role.set_resource_manager(*this);
          new_role.initialize(role_config);
          // new_role.tree_dump(std::cerr, "ROLE: ", "DEVEL: ");
          DT_THROW_IF(has_role_by_name(new_role.get_name()), std::logic_error,
                      "Role '" << new_role.get_name() << "' with id '" << role_id << "' already exists!");
          add_role(new_role);
        }
        const role_set_by_id & id_index = _data_->roles.get<role_tag_id>();
        role_set_by_id::const_iterator found = id_index.find(role_id);
        const role & r = *found;
        DT_LOG_DEBUG(get_logging_priority(), "Registered role '" << r.get_name() << "' is loaded with ID=[" << r.get_id() << "]");
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    // virtual
    void manager::_store_roles_table(const std::string & target_, uint32_t /* flags_ */) const
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      std::string target = target_;
      datatools::fetch_path_with_env(target);

      // Ensure parent directory exists:
      boost::filesystem::path p(target);
      if (p.has_parent_path()) {
        boost::filesystem::path parent_dir = p.parent_path();
        if (!boost::filesystem::is_directory(parent_dir)) {
          if (!boost::filesystem::create_directories(parent_dir)) {
            DT_THROW(std::logic_error,
                     "Cannot create directory '" << parent_dir << "'!");
          }
        }
      }

      datatools::multi_properties roles_table("id", "type", "Vire roles table");

      if (boost::filesystem::exists(target)) {
        DT_LOG_DEBUG(get_logging_priority(), "File '" << target << "' exists...");
        DT_THROW_IF(! boost::filesystem::is_regular_file(target),
                    std::runtime_error,
                    "Target '" << target_ << "' is not a regular file!");
        if (is_backup_tables()) {
          DT_LOG_DEBUG(get_logging_priority(), "Backup roles table...");
          std::ostringstream backup_target_oss;
          backup_target_oss << target << "~";
          std::string backup_target = backup_target_oss.str();
          std::string tmp_backup_target = backup_target + ".tmp";
          if (boost::filesystem::exists(backup_target)) {
            DT_LOG_DEBUG(get_logging_priority(), "Backuped roles table '" << backup_target << "' exists...");
            if (boost::filesystem::exists(tmp_backup_target)) {
              boost::filesystem::remove(tmp_backup_target);
            }
            DT_LOG_DEBUG(get_logging_priority(), "Rename in safe tmp file '" << tmp_backup_target << "'...");
            boost::filesystem::rename(backup_target, tmp_backup_target);
          }
          boost::system::error_code ec;
          boost::filesystem::copy(boost::filesystem::path(target),
                                  boost::filesystem::path(backup_target),
                                  ec);
          if (ec) {
            boost::filesystem::rename(tmp_backup_target, backup_target);
            DT_LOG_DEBUG(get_logging_priority(), "Restore the safe tmp file '" << tmp_backup_target << "' to '" << backup_target << "'...");
            DT_THROW(std::runtime_error,
                     "Cannot backup the users table '" << backup_target << "' : "
                     << ec.message() << "!");
          } else {
            if (boost::filesystem::exists(tmp_backup_target)) {
              boost::filesystem::remove(tmp_backup_target);
              DT_LOG_DEBUG(get_logging_priority(), "Remove the safe tmp file '" << tmp_backup_target << "'...");
            }
          }
        } else {
          boost::filesystem::remove(target);
        }
      }

      for (const auto & r : _data_->roles.get<role_tag_id>()) {
        int32_t role_id = r.get_id();
        // Store only blessed roles:
        if (! r.is_registered()) continue;
        std::ostringstream role_id_iss;
        role_id_iss << role_id;
        roles_table.add(role_id_iss.str(), "vire::resource::role");
        datatools::multi_properties::entry & role_entry = roles_table.grab(role_id_iss.str());
        r.export_to_config(role_entry.grab_properties(),
                           datatools::enriched_base::EXPORT_CONFIG_DEFAULT
                           | datatools::enriched_base::EXPORT_CONFIG_NAME
                           | role::ROLE_XC_PATH
                           | role::ROLE_XC_ALLOWED_USERS
                           | role::ROLE_XC_ALLOWED_GROUPS
                           | role::ROLE_XC_FUNCTIONAL_RESOURCE_SELECTOR
                           | role::ROLE_XC_DISTRIBUTABLE_RESOURCE_SELECTOR
                           );
      }
      roles_table.write(target);

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::_do_build_universe_role()
    {
      DT_LOG_TRACE(get_logging_priority(), "Building the universe role...");
      role universe_role;
      build_universe_role(universe_role);
      add_role(universe_role);
      return;
    }

    /*
    role & manager::_insert_role(const datatools::properties & role_config_, int32_t id_)
    {
      int32_t role_id = id_;
      if (role_id != role::INVALID_ID) {
        DT_THROW_IF(has_role_by_id(role_id), std::logic_error,
                    "Role with ID=[" << role_id << "] already exists!");
        DT_THROW_IF(role_id < 0, std::logic_error,
                    "Blessed role cannot use negative role ID=[" << role_id << "]!");
      } else {
        role_id = candidate_role_id();
      }
      {
        role new_role;
        new_role.set_id(role_id);
        new_role.set_resource_manager(*this);
        new_role.initialize(role_config_);
        DT_THROW_IF(_has_role_by_name(new_role.get_name()), std::logic_error,
                    "Role '" << new_role.get_name() << "' already exists!");
        add_role(new_role);
      }
      role_set_by_id & id_index = _data_->roles.grab<role_tag_id>();
      role_set_by_id::iterator found = id_index.find(role_id);
      DT_THROW_IF(found == id_index.end(), std::logic_error,
                  "Cannot find role with role ID=[" << role_id << "]!");
      return *found;
    }
    */

    int32_t manager::candidate_resource_id() const
    {
      int32_t standard_min_resource_id = 1000;
      int32_t last_id = resource::INVALID_ID;
      for (resource_set_by_id::const_iterator i = _data_->resources.get<resource_tag_id>().begin();
           i != _data_->resources.get<resource_tag_id>().end();
           i++) {
        if (i->get_id() >= standard_min_resource_id) {
          last_id = i->get_id();
        }
      }
      if (last_id == resource::INVALID_ID) {
        return standard_min_resource_id;
      }
      return ++last_id;
    }

    int32_t manager::candidate_role_id() const
    {
      int32_t standard_min_role_id = 1000;
      int32_t last_role_id = role::INVALID_ID;
      for (role_set_by_id::const_iterator i = _data_->roles.get<role_tag_id>().begin();
           i != _data_->roles.get<role_tag_id>().end();
           i++) {
        if (i->get_id() >= standard_min_role_id) {
          last_role_id = i->get_id();
        }
      }
      if (last_role_id == role::INVALID_ID) {
        return standard_min_role_id;
      }
      return ++last_role_id;
    }

    void manager::build_set_of_resource_ids_from_path_regexp(const std::string & path_regexp_,
                                                             std::set<int32_t> & resource_ids_) const
    {
      boost::regex e(path_regexp_, boost::regex::extended);
      for (resource_set_by_id::const_iterator i
             = _data_->resources.get<resource_tag_id>().begin();
           i != _data_->resources.get<resource_tag_id>().end();
           i++) {
        const std::string & rpath = i->get_path();
        if (boost::regex_match(rpath, e)) {
          resource_ids_.insert(i->get_id());
        }
      }
      return;
    }

    void manager::build_set_of_resource_ids_from_paths(const std::vector<std::string> & rpaths_,
                                                       std::set<int32_t> & resource_ids_) const
    {
      for (auto rpath : rpaths_) {
        if (has_resource_by_path(rpath)) {
          const resource & r = get_resource_by_path(rpath);
          resource_ids_.insert(r.get_id());
        }
      }
      return;
    }

    void manager::build_set_of_role_ids(std::set<int32_t> & role_ids_) const
    {
      role_ids_.clear();
      const role_set_by_id & role_index = _data_->roles.get<role_tag_id>();
      for (const auto & the_role : role_index) {
        role_ids_.insert(the_role.get_id());
      }
      return;
    }

    void manager::build_set_of_role_names(std::set<std::string> & role_names_) const
    {
      role_names_.clear();
      const role_set_by_name & name_index = _data_->roles.get<role_tag_name>();
      for (const auto & the_role : name_index) {
        role_names_.insert(the_role.get_name());
      }
      return;
    }

    void manager::build_set_of_role_paths(std::set<std::string> & role_paths_) const
    {
      role_paths_.clear();
      const role_set_by_path & path_index = _data_->roles.get<role_tag_path>();
      for (const auto & the_role : path_index) {
        role_paths_.insert(the_role.get_path());
      }
      return;
    }

    void manager::tree_dump(std::ostream & out_,
                            const std::string & title_,
                            const std::string & indent_,
                            bool inherit_) const
    {
      this->datatools::base_service::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Roles table path : '" << _roles_table_path_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Don't load tables : '" << _dont_load_tables_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Don't store tables : '" << _dont_store_tables_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Don't backup tables : '" << _dont_backup_tables_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Next candidate resource ID : [" << manager::candidate_resource_id() << ']' << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Next candidate role ID : [" << manager::candidate_role_id() << ']' << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Cached set of resource IDs: ";
      if (has_cached_resource_ids()) {
        out_ << '[' << _data_->cached_resource_ids->size() << ']';
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Resources : ";
      if (_data_->resources.size() == 0) {
        out_ << "<none>";
      } else {
        out_ << '[' << _data_->resources.size() << ']';
      }
      out_ << std::endl;

      {
        const resource_set_by_id & id_index = _data_->resources.get<resource_tag_id>();
        for (resource_set_by_id::const_iterator it = id_index.begin();
             it != id_index.end();
             it++) {
          resource_set_by_id::const_iterator jt = it;
          out_ << indent_;
          out_ << datatools::i_tree_dumpable::skip_tag;
          if (++jt == id_index.end()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          const resource & r = *it;
          out_ << "Resource ID=[" << r.get_id() << "] (access='" << vire::utility::to_string(r.get_access()) << "'";
          if (r.is_number_of_tokens_unlimited()) {
            out_ << ",unlimited";
          } else if (r.is_number_of_tokens_limited()) {
            out_ << ",limited=" << r.get_max_number_of_tokens();
          }
          out_ << ") : ";
          out_ << "Path='" << r.get_path() << "'";
          out_ << std::endl;
        }
      }

      {
        out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
             << "Roles : ";
        if (_data_->roles.size() == 0) {
          out_ << "<none>";
        } else {
          out_ << '[' << _data_->roles.size() << ']';
        }
        out_ << std::endl;

        const role_set_by_id & id_index = _data_->roles.get<role_tag_id>();
        for (role_set_by_id::const_iterator it = id_index.begin();
             it != id_index.end();
             it++) {
          role_set_by_id::const_iterator jt = it;
          out_ << indent_;
          out_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
          if (++jt == id_index.end()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          const role & r = *it;
          out_ << "Role ID=[" << r.get_id() << "] : ";
          out_ << "Name='" << r.get_name() << "' ";
          out_ << "Path='" << r.get_path() << "' ";
          out_ << std::endl;
        }
      }

      return;
    }

    void manager::build_resources_from_internal_services(const datatools::service_manager & service_manager_,
                                                         uint32_t flags_)
    {
      //
      return;
    }

    void manager::build_resources_from_devices(const vire::device::manager & device_manager_,
                                               uint32_t /* flags_ */)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      try {
        devices_to_resources_builder d2r_builder;
        datatools::logger::priority d2r_builder_logging = get_logging_priority();
        d2r_builder.set_logging_priority(d2r_builder_logging);
        d2r_builder.set_device_manager(device_manager_);
        d2r_builder.set_resource_manager(*this);
        d2r_builder.initialize_simple();
        d2r_builder.build_resources();
        d2r_builder.reset();
      } catch (std::exception & error) {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, error.what());
        throw std::logic_error(error.what());
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::build_universe_role(role & urole_)
    {
      if (!urole_.has_name()) {
        urole_.set_name(default_universe_role_name());
      }
      if (!urole_.has_display_name()) {
        urole_.set_display_name("Universe");
      }
      if (!urole_.has_terse_description()) {
        urole_.set_terse_description("The role that contains all available resources");
      }
      urole_.set_id(role::UNIVERSE_ID);
      urole_.set_path("__universe__");
      urole_.add_allowed_user(vire::user::user::ADMIN_UID);
      urole_.add_allowed_group(vire::user::group::ADMIN_GID);
      urole_.set_resource_manager(*this);

      cuts::cut_handle_type h_universe_selector(new cuts::accept_cut);
      {
        cuts::accept_cut & universe_selector = dynamic_cast<cuts::accept_cut &>(h_universe_selector.grab());
        universe_selector.initialize_simple();
      }
      urole_.set_distributable_resource_selector_handle(h_universe_selector);
      urole_.build_cached_scope_resource_ids();
      urole_.build_cached_functional_resource_ids();
      urole_.build_cached_distributable_resource_ids();
      urole_.initialize_simple();

      return;
    }

    bool manager::fetch_resource_id(const std::string & repr_, int32_t & resource_id_) const
    {
      int32_t resource_id = vire::resource::resource::INVALID_ID;
      try {
        // Attempt to parse the resource integer identifier:
        resource_id = boost::lexical_cast<int32_t>(repr_);
        if (! has_resource_by_id(resource_id)) {
          resource_id = vire::resource::resource::INVALID_ID;
        }
      } catch (boost::bad_lexical_cast &) {
        resource_id = vire::resource::resource::INVALID_ID;
      }
      if (resource_id == vire::resource::resource::INVALID_ID) {
        // Attempt to parse the resource path:
        const std::string & resource_path = repr_;
        if (has_resource_by_path(resource_path)) {
          resource_id = get_resource_by_path(resource_path).get_id();
        }
      }
      resource_id_ = resource_id;
      return resource_id_ != vire::resource::resource::INVALID_ID;
    }

  } // namespace resource

} // namespace vire
