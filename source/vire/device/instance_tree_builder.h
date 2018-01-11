//! \file  vire/device/instance_tree_builder.h
//! \brief Builder of Vire device instance tree
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_DEVICE_INSTANCE_TREE_BUILDER_H
#define VIRE_DEVICE_INSTANCE_TREE_BUILDER_H

// Standard library:
#include <string>
#include <set>

// Third Party:
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/properties.h>

// This project:
#include <vire/device/instance_info.h>
#include <vire/device/instance_tree.h>
#include <vire/device/manager.h>

namespace vire {

  namespace device {

    //! \brief Basic information about a device/port instance in a hierarchy of devices
    class instance_tree_builder
    {
    public:

      typedef std::vector<std::string> path_stack_type;

      //! Constant representing unlimited mapping depth
      static std::size_t depth_no_limit();

      //! Default constructor
      instance_tree_builder();

      //! Destructor
      virtual ~instance_tree_builder();

      //! Return the logging priority threshold
      datatools::logger::priority get_logging_priority() const;

      //! Set the logging priority threshold
      void set_logging_priority(const datatools::logger::priority);

      //! Check if max depth is set
      bool has_max_depth() const;

      //! Return the maximum depth of mapping
      std::size_t get_max_depth() const;

      //! Set the maximum depth of mapping
      void set_max_depth(const std::size_t max_depth_);

      //! Set the name of the top-level device model
      void set_top_level_device_model_name(const std::string &);

      //! Return the name of the top-level device model
      const std::string & get_top_level_device_model_name() const;

      //! Set the name of the top-level device model
      void set_top_level_device_instance_name(const std::string &);

      //! Return the name of the top-level device instance
      const std::string & get_top_level_device_instance_name() const;

      bool build_no_devices() const;

      void set_no_devices(bool);

      bool build_no_ports() const;

      void set_no_ports(bool);

      void add_only_from_path(const std::string &);

      bool has_only_from_path(const std::string &) const;

      //! Return the set of only-from paths
      const std::set<std::string> & get_only_from_paths() const;

      /** Configure the builder
       *
       * Supported configuration parameters:
       * \code
       * top_level_device_model : string = "setup"
       * top_level_device_instance : string = "mySetup"
       * no_devices : boolean = false
       * no_ports   : boolean = false
       * max_depth  : integer = 0 # No limit
       * from       : string  = "path"
       *
       */
      void configure(const datatools::properties & config_);

      void build(const manager & devmgr_, instance_tree & tree_);

      void reset();

    private:

      //! Build the mapping from the top-level
      void _top_build_();

      //! Build the mapping records
      void _build_();

      //! Build the mapping records associated to internal objects in a logical device
      void _build_device_(const logical_device & mother_log_);

      //! Build the mapping records associated to embedded device in a logical device
      void _build_embedded_devices_(const logical_device & mother_log_);

      //! Build the mapping records associated to ports in a logical device
      void _build_ports_(const logical_device & mother_log_);

    private:

      // Management:
      datatools::logger::priority _logging_priority_; //!< Logging priority

      // Configuration:
      std::size_t _max_depth_;                      //!< Maximum mapping depth at build
      bool        _no_devices_ = false;             //!< Inhibit devices
      bool        _no_ports_ = false;               //!< Inhibit ports
      std::string _top_level_device_model_name_;    //!< Name of the top-level device model
      std::string _top_level_device_instance_name_; //!< Name of the top-level device instance
      std::set<std::string> _only_from_paths_;      //!< Names of the instances of which only daughters instances are stored

      // PIMPL-ized working data with embedded dictionary of mapping info:
      struct work;
      std::unique_ptr<work> _work_; //!< Internal working data

    };

  } // end of namespace device

} // end of namespace vire

#endif // VIRE_DEVICE_INSTANCE_TREE_BUILDER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
