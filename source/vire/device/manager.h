//! \file  vire/device/manager.h
//! \brief Vire device manager
//
// Copyright (c) 2015-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_DEVICE_MANAGER_H
#define VIRE_DEVICE_MANAGER_H

// Standard Library:
#include <string>
#include <sstream>
#include <typeinfo>
#include <stdexcept>
#include <memory>

// Third Party:
// - Boost:
#include <boost/cstdint.hpp>

// Third Party:
// - Bayeux/datatools:
#include <datatools/base_service.h>
#include <datatools/properties.h>
#include <datatools/bit_mask.h>
// - Bayeux/geomtools:
#include <geomtools/id_mgr.h>

// This project:
#include <vire/device/device_utils.h>
#include <vire/device/base_device_model.h>
#include <vire/device/base_port_model.h>
#include <vire/device/mapping.h>
#include <vire/device/instance_tree.h>
#include <vire/utility/instance_identifier.h>

namespace datatools {

  // Forward class declarations:
  class properties;
  class multi_properties;

}

namespace vire {

  namespace device {

    //! \brief Device management service
    class manager
      : public ::datatools::base_service
    {

    public:

      friend class device_model_entry;

      //! Dictionary of logical device handles
      typedef std::map<std::string, const logical_device *> logical_device_dict_type;

      //! Dictionary of logical port handles
      typedef std::map<std::string, const logical_port *> logical_port_dict_type;

      //! Register of device model factory
      typedef base_device_model::factory_register_type device_factory_register_type;

      //! Register of port model factory
      typedef base_port_model::factory_register_type port_factory_register_type;

      static const std::string & default_service_name();

      //! Return the default name for the top-level device model
      static const std::string & default_top_level_device_model_name();

      //! \brief Construction flag
      enum flag_type {
        BLANK              = 0,
        NO_FACTORY_PRELOAD = datatools::bit_mask::bit00,
        FORCE_INITIALIZATION_AT_LOAD = datatools::bit_mask::bit01,
        FACTORY_VERBOSE    = datatools::bit_mask::bit02,
        NO_BUILD_MAPPING   = datatools::bit_mask::bit03
      };

      //! Default constructor
      manager(uint32_t flags_ = 0);

      //! Destructor
      virtual ~manager();

      //! Set the name of the setup
      void set_setup_label(const std::string & name_);

      //! Set the description of the setup
      void set_setup_description(const std::string & description_);

      //! Set the version of the setup
      void set_setup_version(const std::string & version_);

      //! Return the Setup ID
      vire::utility::instance_identifier get_setup_id() const;

      //! Check the setup label
      bool has_setup_label() const;

      //! Return the name of the setup
      const std::string & get_setup_label() const;

      //! Check the setup description
      bool has_setup_description() const;

      //! Return the description of the setup
      const std::string & get_setup_description() const;

      //! Check the setup version
      bool has_setup_version() const;

      //! Return the version of the setup
      const std::string & get_setup_version() const;

      //! Flag to propagate logging to devices
      void set_propagate_logging_to_devices(bool);

      //! Check the propagate logging to devices flag
      bool is_propagate_logging_to_devices() const;

      //! Set the flag to preload all device models from the system factory
      void set_factory_preload_system_all(bool);

      //! Check the factory preload flag
      bool is_factory_preload_system_all() const;

      //! Add a device model ID to be preloaded from the system factory
      void add_factory_preload_system_only(const std::string & id_);

      void set_top_level_device_model_name(const std::string &);

      const std::string & get_top_level_device_model_name() const;

      //! Set the build mapping flag
      void set_mapping_requested(bool);

      //! Check the build mapping flag
      bool is_mapping_requested() const;

      //! Set the 'force initialization at load' flag
      void set_force_initialization_at_load(bool);

      //! Check the 'force initialization at load' flag
      bool is_force_initialization_at_load() const;

      //! Add a property prefix to be preserved in models
      void add_auxiliary_property_prefix(const std::string & prefix_);

      //! Return the array of exported property prefixes
      const std::vector<std::string> & get_auxiliary_property_prefixes() const;

      //! Check the initialization flag
      virtual bool is_initialized() const;

      //! Initialize the manager from a container of properties
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the manager
      virtual int reset();

      //! Check if some device type is known
      bool has_device_type(const std::string & id_) const;

      //! Check if some port type is known
      bool has_port_type(const std::string & id_) const;

      //! Register some device type
      template <class DeviceClass>
      void register_device_type(const std::string & id_);

      //! Register some port type
      template <class PortClass>
      void register_port_type(const std::string & id_);

      //! Unregister some device typz
      void unregister_device_type(const std::string & id_);

      //! Unregister some port typz
      void unregister_port_type(const std::string & id_);

      /**  @param name_ The name of the device to be checked
       *   @return true if the manager hosts the device requested by name
       */
      bool has_device_model(const std::string & name_) const;

      /**  @param name_ The name of the port to be checked
       *   @return true if the manager hosts the port requested by name
       */
      bool has_port_model(const std::string & name_) const;

      /**  @param name_ The name of the device to be checked
       *   @return true if the device is initialized
       */
      bool is_device_model_initialized(const std::string & name_) const;

      /**  @param name_ The name of the port to be checked
       *   @return true if the port is initialized
       */
      bool is_port_model_initialized(const std::string & name_) const;

      /**  @param name_ The name of the device to be checked
       *   @return true if the device model is of the requested type
       */
      template <class T>
      bool is_device_model_a(const std::string & name_) const;

      /**  @param name_ The name of the port to be checked
       *   @return true if the port model is of the requested type
       */
      template <class T>
      bool is_port_model_a(const std::string & name_) const;

      /**  Return a mutable reference to a base port model
       *   @param name_ The name of the port to be accessed
       *   @return a mutable reference to the port model instance requested by name and type
       */
      base_port_model & grab_port_model(const std::string & name_);

      /**  Return a non mutable reference to a base port model
       *   @param name_ The name of the port to be accessed
       *   @return a mutable reference to the port model instance requested by name and type
       */
      const base_port_model & get_port_model(const std::string & name_) const;

      /**  Return a mutable reference to a base device model
       *   @param name_ The name of the device to be accessed
       *   @return a mutable reference to the device model instance requested by name and type
       */
      base_device_model & grab_device_model(const std::string & name_);

      /**  Return a non mutable reference to a base device model
       *   @param name_ The name of the device to be accessed
       *   @return a mutable reference to the device model instance requested by name and type
       */
      const base_device_model & get_device_model(const std::string & name_) const;

      /**  Return a mutable reference to a typed device model
       *   @param name_ The name of the device to be accessed
       *   @return a mutable reference to the device model instance requested by name and type
       */
      template<class T>
      T & grab_device_model_as(const std::string & name_);

      /**  Return a mutable reference to a typed port model
       *   @param name_ The name of the port to be accessed
       *   @return a mutable reference to the port model instance requested by name and type
       */
      template<class T>
      T & grab_port_model_as(const std::string & name_);

      /**  Return a non mutable reference to a typed device model
       *   @param name_ The name of the device to be accessed
       *   @return a const reference to the device model instance requested by name and type
       */
      template<class T>
      const T & get_device_model_as(const std::string & name_) const;

      /**  Return a non mutable reference to a typed port model
       *   @param name_ The name of the port to be accessed
       *   @return a const reference to the port model instance requested by name and type
       */
      template<class T>
      const T & get_port_model_as(const std::string & name_) const;

      //! Load a device model
      void load_device_model(const std::string & name_,
                             const std::string & id_,
                             const datatools::properties & config_);

      //! Load a port model
      void load_port_model(const std::string & name_,
                           const std::string & id_,
                           const datatools::properties & config_);

      //! Load a collection of models from a multi properties configuration dictionary
      void load_models(const datatools::multi_properties & config_);

      //! Load a collection of models from a configuration dictionary
      void load_model_definitions(const std::string & models_def_filename);

      //! Load a collection of model definition files from a list of files
      void load_model_definitions_list(const std::string & models_def_list_filename_);

      //! Return a const reference to the embedded device ID manager
      const geomtools::id_mgr & get_mapping_manager() const;

      //! Load mapping categories from a file
      void load_mapping_categories_from_file(const std::string &);

      //! Return a const reference to the embedded pool of models
      const model_pool_type & get_models() const;

      //! Return a const reference to the embedded dictionary of logical devices
      const logical_device_dict_type & get_logical_devices() const;

      //! Return a const reference to the embedded dictionary of port devices
      const logical_port_dict_type & get_logical_ports() const;

      //! Return the mapping
      const mapping & get_mapping() const;

      //! Return the device instance tree
      const instance_tree & get_tree() const;

      //! Dump device models
      void dump_device_models(std::ostream & out_ = std::clog,
                              const std::string & title_  = "",
                              const std::string & indent_ = "") const;

      //! Dump port models
      void dump_port_models(std::ostream & out_ = std::clog,
                            const std::string & title_  = "",
                            const std::string & indent_ = "") const;
      //! Smart print
      //!
      //! Supported options:
      //! \code
      //! {
      //!   "title"    : "My title: ",
      //!   "indent"   : "[debug] ",
      //!   "inherit"  : true,
      //!   "models.list" : true,
      //!   "models.full" : true,
      //!   "tree.list_instances" : true,
      //!   "tree.full_instances" : false,
      //!   "mapping.list_categories" : true,
      //!   "mapping.full_categories" : false,
      //!   "mapping.list_ids" : false,
      //!   "logicals.list" : true,
      //!   "ports.list" : true
      //! }
      //! \endcode
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_ = datatools::i_tree_dumpable::empty_options()) const;

    protected:

      //! Set default attributes' values
      void _set_defaults();

      device_factory_register_type & _grab_device_factory_register();

      port_factory_register_type & _grab_port_factory_register();

      void _load_device_model(const std::string & name_,
                              const std::string & id_,
                              const datatools::properties & config_);

      void _load_port_model(const std::string & name_,
                            const std::string & id_,
                            const datatools::properties & config_);

      void _create_device_model(model_entry & entry_);

      void _create_port_model(model_entry & entry_);

      void _initialize_device_model(model_entry & entry_);

      void _initialize_port_model(model_entry & entry_);

      void _reset_device_model(model_entry & entry_);

      void _reset_port_model(model_entry & entry_);

      void _reset_model(model_entry & entry_);

    private:

      void _post_init_(const datatools::properties & config_);

      void _pre_reset_();

      void _init_tree_(const datatools::properties & config_);

      void _reset_tree_();

      void _init_mapping_(const datatools::properties & config_);

      void _reset_mapping_();

    private:

      // Management:
      bool _initialized_; //!< Initialization flag
      bool _propagate_logging_to_devices_ = false;

      // Configuration parameters:
      std::string _setup_label_;       //!< The label associated to the setup
      std::string _setup_version_;     //!< The version tag of the setup
      std::string _setup_description_; //!< The description of the setup
      std::vector<std::string> _factory_preload_system_only_; //!< The list of model factories to be loaded from the system
      bool _factory_preload_system_all_;   //!< Factory preload flag
      bool _force_initialization_at_load_; //!< Flag for triggering device initialization at load (rather than first use)
      std::vector<std::string> _auxiliary_property_prefixes_; //!< List of property prefixes to be preserved in device models auxiliary properties
      std::string _top_level_device_model_name_; //!< Name of the top-level device model
      bool _mapping_requested_; //!< Mapping build request flag
      datatools::multi_properties _external_mapping_rules_; //!< Mapping rules associated to device models

      // Internal working data and resources:
      std::unique_ptr<device_factory_register_type> _device_factory_register_; //!< Handle to a register of device model factories
      std::unique_ptr<port_factory_register_type>   _port_factory_register_;   //!< Handle to a register of port model factories
      model_pool_type          _models_;          //!< Dictionary of device/port model entries
      logical_device_dict_type _logical_devices_; //!< Dictionary of logical devices
      logical_port_dict_type   _logical_ports_;   //!< Dictionary of logical ports
      geomtools::id_mgr        _mapping_manager_; //!< Manager of mapping IDs
      mapping                  _mapping_;         //!< Default mapping
      instance_tree            _tree_;            //!< Device instance tree

      friend class model_entry;

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(manager)

    };

  } // end of namespace device

} // end of namespace vire

#include <vire/device/manager-inl.h>

// OCD support :
#include <datatools/ocd_macros.h>
// @arg vire::device::manager the name of the class with OCD support
DOCD_CLASS_DECLARATION(vire::device::manager)

#endif // VIRE_DEVICE_MANAGER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
