//! \file  vire/device/mapping.h
//! \brief Vire device mapping
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

#ifndef VIRE_DEVICE_MAPPING_H
#define VIRE_DEVICE_MAPPING_H

// Standard library:
#include <string>
#include <map>
#include <stack>
#include <set>
#include <memory>

// Third party:
// - Boost:
// #include <boost/scoped_ptr.hpp>
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>
#include <datatools/logger.h>
// - Bayeux/geomtools:
#include <geomtools/id_mgr.h>
#include <geomtools/geom_id.h>

// This project:
#include <vire/device/mapping_info.h>

namespace vire {

  namespace device {

    // Forward declaration:
    class manager;
    class base_device_model;

    //! \brief The mapping class for the devices in a setup
    class mapping : public datatools::i_tree_dumpable
    {
    public:

      //! Constant representing unlimited mapping depth
      static unsigned int depth_no_limit();

      //! Constant representing the default setup category
      static const std::string & default_setup_category();

      //! Default constructor
      mapping();

      //! Destructor
      virtual ~mapping();

      //! Return the logging priority threshold
      datatools::logger::priority get_logging_priority() const;

      //! Set the logging priority threshold
      void set_logging_priority(datatools::logger::priority);

      //! Check manager
      bool has_manager() const;

      //! Set the device manager
      void set_manager(const manager & mgr_);

      //! Return the device manager
      const manager & get_manager() const;

      //! Check if the top-level device is mapped
      bool is_top_level_mapping() const;

      //! Set the flag to map the top-level device
      void set_top_level_mapping(bool);

      //! Check if a mapping identifier (MID) manager is defined
      bool has_mapping_manager() const;

      //! Set the mapping identifier (MID) manager
      void set_mapping_manager(const geomtools::id_mgr & mgr_);

      //! Return a reference to the handled mapping identifier (MID) manager
      const geomtools::id_mgr & get_mapping_manager() const;

      //! Check if the mapping has a maximum mapping depth
      bool has_max_depth() const;

      //! Set the maximum depth of mapping
      void set_max_depth(unsigned max_depth_);

      //! Return the maximum depth of mapping
      unsigned int get_max_depth() const;

      //! Set port mapping
      void set_port_mapping(bool);

      //! Return port mapping
      bool is_port_mapping() const;

      //! Set link mapping
      void set_link_mapping(bool);

      //! Return link mapping
      bool is_link_mapping() const;

      //! Set the name of the top-level device
      void set_top_level_name(const std::string &);

      //! Returnt the name of the top-level device
      const std::string & get_top_level_name() const;

      //! Check if mapping should be built only for a specific set of categories
      bool is_mode_only() const;

      //! Check if mapping should be built excluding a specific set of categories
      bool is_mode_excluded() const;

      //! Add a category of which mapping should be built
      void add_only(const std::string & category_);

      //! Add a category of which mapping should not be built
      void add_excluded(const std::string & category_);

      //! Check initialization status
      bool is_initialized() const;

      //! Configure the mapping algorithm
      void configure(const datatools::properties & config_);

      //! Initialization
      void initialize();

      //! Reset
      void reset();

      //! Build the mapping
      void build(const std::string & setup_ = "");

      // //! Validate a mapping identifier
      // bool validate_id(const geomtools::geom_id & id_) const;

      // //! Validate a mapping path
      // bool validate_path(const std::string & path_) const;

      //! Return the number of mapped entries
      std::size_t get_number_of_entries() const;

      //! Build the set of paths
      void build_paths(std::set<std::string> & paths_, uint32_t flags_ = 0) const;

      //! Check if the top level path is available
      bool has_top_path() const;

      //! Return the top level path
      std::string get_top_path() const;

      //! Check if a mapping information record exists for a given identifier
      bool has_mapping_info(const geomtools::geom_id & id_) const;

      //! Return a mapping information record given its identifier
      const mapping_info & get_mapping_info(const geomtools::geom_id & id_) const;

      //! Check if a mapping information record exists for a given path
      bool has_mapping_info(const std::string & path_) const;

      //! Return a mapping information record given its path
      const mapping_info & get_mapping_info(const std::string & path_) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_         = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    protected:

      typedef std::stack<const mapping_info *> info_stack_type;

      //! Set default attributes' values
      void _set_defaults();

      //! Build the mapping records
      void _build();

      //! Build the mapping records associated to internal objects in a logical device
      void _build_device_mapping(const logical_device & mother_log_,
                                 info_stack_type & ancestors_);

      //! Build the mapping records associated to embedded device in a logical device
      void _build_embedded_devices_mapping(const logical_device & mother_log_,
                                           info_stack_type & ancestors_);

      //! Build the mapping records associated to ports in a logical device
      void _build_ports_mapping(const logical_device & mother_log_,
                                info_stack_type & ancestors_);

      //! Return a mapping information record given its ID
      mapping_info & _grab_mapping_info_(const geomtools::geom_id & id_);

    private:

      // Management:
      datatools::logger::priority  _logging_priority_;    //!< Logging priority threshold
      bool                         _initialized_;         //!< Initialization flag

      // Configuration attributes:
      const manager *              _manager_;             //!< Handle to the device model manager
      bool                         _top_level_mapping_;   //!< Flag to activate the mapping of the top-level logical
      unsigned int                 _max_depth_;           //!< Maximum mapping depth at build
      std::string                  _top_level_name_;      //!< Name of the top-level device model
      std::vector<std::string>     _only_categories_;     //!< List of device categories to be mapped
      std::vector<std::string>     _excluded_categories_; //!< List of device categories to be excluded from the mapping
      bool                         _port_mapping_;        //!< Flag to activate the mapping of devices' internal ports
      bool                         _link_mapping_;        //!< Flag to activate the mapping of devices' internal links

      // Working/internal data:
      const logical_device *    _top_level_logical_;  //!< Handle to the top-level logical device
      const geomtools::id_mgr * _mapping_manager_;    //!< Mapping identifier (ID) manager
      unsigned int              _mapping_depth_;      //!< Running depth at build

      // PIMPL-ized working data with embedded dictionary of mapping info:
      struct work;
      std::unique_ptr<work>   _work_; //!< Internal working data

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_MAPPING_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
