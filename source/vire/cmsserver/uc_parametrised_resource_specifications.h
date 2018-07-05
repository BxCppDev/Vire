//! \file  vire/cmsserver/uc_parametrised_resource_specifications.h
//! \brief The CMS server use case parametrised resource specifications
//
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_UC_PARAMETRISED_RESOURCE_SPECIFICATIONS_H
#define VIRE_CMSSERVER_UC_PARAMETRISED_RESOURCE_SPECIFICATIONS_H

// Standard Library:
#include <string>
#include <set>
#include <map>
#include <iostream>
#include <tuple>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/introspection/data_type.h>

// This project:
#include <vire/utility/model_identifier.h>
#include <vire/resource/utils.h>
#include <vire/device/device_types.h>

namespace vire {

  namespace cmsserver {

    /// \brief Use case's parametrised resource specifications
    class uc_parametrised_resource_specifications
      : public datatools::i_tree_dumpable
    {
    public:

      /// \brief Category of the specification
      enum category_type {
        CATEGORY_UNDEF    = 0, ///< Undefined category
        CATEGORY_DEVICE   = 1, ///< Required device
        CATEGORY_RESOURCE = 2  ///< Required resource
      };

      /// \brief Requirement specification entry
      struct resource_spec_entry 
        : public datatools::i_tree_dumpable
      {
      public:
        
        resource_spec_entry();

        virtual ~resource_spec_entry();

        /// Check is a specification is complete
        bool is_complete() const;

        /// Check is a specification is related to a resource
        bool is_resource() const;
        
        /// Check is a specification is related to a device
        bool is_device() const;

        bool has_device_model_id() const;

        bool has_device_type() const;

        bool has_resource_name() const;

        bool has_resource_category() const;

        bool has_resource_access() const;

        bool has_resource_unit_dimension() const;

        bool has_resource_datatype_id() const;

        std::tuple<bool,std::string> match(const resource_spec_entry & other_) const;

        /// Smart print
        void print_tree(std::ostream & out_ = std::clog,
                        const boost::property_tree::ptree & options_
                        = datatools::i_tree_dumpable::empty_options() ) const override;
        
        /// Category (device or resource) 
        category_type                          category = CATEGORY_UNDEF;

        /// Device model ID (class/model ID for devices)   
        std::string                            device_model_id;  

        /// Device type ID
        vire::device::device_type              device_type_id = vire::device::TYPE_UNDEFINED;

        /// Resource name
        std::string                            resource_name;  
        
        /// Resource category (datapoint or method)
        vire::resource::resource_category_type resource_category = vire::resource::RESOURCE_CATEGORY_INVALID;

        /// Resource access mode ("ro", "wo", "rw)
        vire::resource::access_type            resource_access = vire::resource::ACCESS_INVALID;

        /// Resource data type (read method return type/write method parameter)
        datatools::introspection::data_type    resource_datatype_id = datatools::introspection::DATA_TYPE_INVALID;
        
        /// Explicit resource unit dimension (only for resource of real type)
        std::string                            resource_unit_dimension;

      };

      typedef std::shared_ptr<resource_spec_entry> spec_ptr_type;
      typedef std::map<std::string, spec_ptr_type> spec_dict_type;

      /// Default constructor
      uc_parametrised_resource_specifications();

      /// Destructor
      virtual ~uc_parametrised_resource_specifications();
      
      /// Check if a specification with a given path exists
      bool has(const std::string & key_) const;

      /// Build the list of all specifications' paths
      void build_keys(std::set<std::string> & keys_) const;

      /// Return the list of all specifications' paths
      std::set<std::string> keys() const;

      /// Check is a specification is related to a resource
      bool is_resource(const std::string & key_) const;
      
      /// Check is a specification is related to a device
      bool is_device(const std::string & key_) const;

      /// Add a new device specification
      void add_device(const std::string & key_,
                      const std::string & device_model_id_ = "",
                      const vire::device::device_type device_type_id_ = vire::device::TYPE_GENERIC);

      /// Add a new resource specification
      void add_resource(const std::string & key_,
                        const vire::resource::resource_category_type resource_category_ = vire::resource::RESOURCE_CATEGORY_INVALID,
                        const vire::resource::access_type resource_access_ = vire::resource::ACCESS_INVALID,
                        const datatools::introspection::data_type resource_datatype_id_ = datatools::introspection::DATA_TYPE_INVALID,
                        const std::string & resource_unit_dimension_ = "");

      /// Remove an existing resource specification 
      void remove(const std::string & key_);

      /// Return the record associated to as existing specification
      const resource_spec_entry & get_specification(const std::string & key_) const;

      /// Return the record associated to as existing specification
      const spec_ptr_type & get_specification_ptr(const std::string & key_) const;

      /// Remove all resource specifications (only is not locked)
      void clear();

      /// Check if no resource specifications are set
      bool is_empty() const;

      /// Return the number of resource specifications 
      std::size_t size() const;

      /// Load a set of resource specifications from a configuration container
      void load(const datatools::properties & config_);

      void configure(const datatools::properties & config_);
 
      /// Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_ = datatools::i_tree_dumpable::empty_options()) const;

      /// Check if the container of resource specifications is locked
      bool is_locked() const;

      /// Lock the container of resource specifications
      void lock();
      
      /// Reset the container of resource specifications (unlock and clear)
      void reset();
      
    private:

      /// Private add a resource specification record
      void _add_(const std::string & key_, const resource_spec_entry & e_);
      
    private:

      bool           _locked_ = false; //!< Lock flag
      spec_dict_type _specs_;          //!< Dictionary of resource/device specifications
      
    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_PARAMETRISED_RESOURCE_SPECIFICATIONS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
