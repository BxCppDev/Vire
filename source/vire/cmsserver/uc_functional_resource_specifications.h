//! \file  vire/cmsserver/uc_functional_resource_specifications.h
//! \brief The CMS server use case functional resource specifications
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

#ifndef VIRE_CMSSERVER_UC_FUNCTIONAL_RESOURCE_SPECIFICATIONS_H
#define VIRE_CMSSERVER_UC_FUNCTIONAL_RESOURCE_SPECIFICATIONS_H

// Standard Library:
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/introspection/data_type.h>

// This project:
#include <vire/resource/utils.h>
#include <vire/utility/path.h>
#include <vire/cmsserver/uc_resource_mount_link.h>

namespace vire {

  namespace cmsserver {

    /// \brief Description of a functional resource
    class uc_functional_resource_description 
      : public datatools::i_tree_dumpable
    {
    public:
        
      uc_functional_resource_description();

      /// Constructor for a mounted relative resource
      explicit uc_functional_resource_description(const uc_resource_mount_link & mlink_);

      /// Constructor for an absolute resource
      explicit uc_functional_resource_description(const vire::utility::path & path_);

      virtual ~uc_functional_resource_description();

      /// Check if an absolute resource is set
      bool is_absolute() const;

      /// Check if a relative resource is set
      bool is_relative() const;

      /// Check if the description is complete
      bool is_complete() const;

      bool has_absolute_path() const;
      
      void set_absolute_path(const vire::utility::path &);
      
      const vire::utility::path & get_absolute_path() const;

      bool has_mount_link() const;
 
      const uc_resource_mount_link & get_mount_link() const;
     
      void set_mount_link(const uc_resource_mount_link &);
 
      void configure(const datatools::properties & config_);
      
      /// Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options() ) const override;
  
    public:
        
      /// Functional resource absolute path
      vire::utility::path _absolute_path_;       

      /// Mount link
      uc_resource_mount_link _mount_link_;
      
      BOOST_SERIALIZATION_BASIC_DECLARATION();

    };
 
    /// \brief Use case's parametrised resource specifications
    class uc_functional_resource_specifications
      : public datatools::i_tree_dumpable
    {
    public:
     
      typedef std::shared_ptr<uc_functional_resource_description> spec_ptr_type;
      typedef std::map<std::string, spec_ptr_type> spec_dict_type;

      /// Default constructor
      uc_functional_resource_specifications();

      /// Destructor
      virtual ~uc_functional_resource_specifications();
     
      /// Check if a functional resource with a given key exists
      bool has(const std::string & key_) const;

      /// Build the list of all specifications' keys
      void build_keys(std::set<std::string> & keys_) const;

      /// Return the list of all specifications' keys
      std::set<std::string> keys() const;

      bool is_absolute(const std::string & key_) const;

      /// Add a new relative functional resource description
      void add_relative_resource(const std::string & key_,
                                 const uc_resource_mount_link & mlink_);
      
      /// Add a new absolute functional resource description
      void add_absolute_resource(const std::string & key_,
                                 const vire::utility::path & path_);

      /// Return the record associated to as existing functional resource description
      const uc_functional_resource_description & get_description(const std::string & key_) const;

      /// Remove all resource specifications (only is not locked)
      void clear();

      /// Check if no resource specifications are set
      bool is_empty() const;

      /// Return the number of resource specifications 
      std::size_t size() const;

      // /// Load a set of resource specifications from a configuration container
      // void load(const datatools::properties & config_);

      /// Check if the container of functional resource descriptions is locked
      bool is_locked() const;

      /// Lock the container of functional resource descriptions
      void lock();
      
      /// Reset the container of functional resource descriptions (unlock and clear)
      void reset();
      
      /// Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options() ) const override;

    private:
      
      void _add_(const std::string & key_,
                 const uc_functional_resource_description & frd_);

    private:

      bool           _locked_ = false; //!< Lock flag
      spec_dict_type _specs_;          //!< Dictionary of functional resource descriptions
 
      BOOST_SERIALIZATION_BASIC_DECLARATION();
     
    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_RESOURCE_RESOURCE_SPECIFICATIONS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
