//! \file  vire/cmsserver/uc_mounting_table.h
//! \brief Vire CMS use case mounting table
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_UC_MOUNTING_TABLE_H
#define VIRE_CMSSERVER_UC_MOUNTING_TABLE_H

// Standard Library:
#include <string>
#include <list>
#include <memory>
#include <set>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/serialization_macros.h>

// This project:
#include <vire/cmsserver/uc_utils.h>
#include <vire/cmsserver/uc_resource_mount_link.h>

namespace vire {

  namespace cmsserver {
     
    /// \brief Local resource mounting table
    class uc_resource_mounting_table
      : public datatools::i_tree_dumpable
    {
    public:

      /// \brief List of resource mount links
      typedef std::list<uc_resource_mount_link> link_list_type;
     
      uc_resource_mounting_table();
      
      virtual ~uc_resource_mounting_table();

      bool has(const uc_port_id & from_id_) const;
      
      void add(const uc_resource_mount_link & mount_link_);
     
      void add(const uc_port_id & from_id_,
               const std::string & to_key_,
               const vire::utility::relative_path & relpath_ = vire::utility::empty_relative_path());

      void add(const std::string & from_id_name_,
               const std::string & from_id_port_key_,
               const std::string & to_key_,
               const vire::utility::relative_path & relpath_ = vire::utility::empty_relative_path());
     
      void add(const uc_port_id & from_id_,
               const std::string & to_key_,
               const std::string & relpath_repr_ = "");

      void add(const std::string & from_id_name_,
               const std::string & from_id_port_key_,
               const std::string & to_key_,
               const std::string & relpath_repr_ = "");
  
      const uc_resource_mount_link & get(const uc_port_id & from_key_) const;
      
      /// Return the list of mount links
      const link_list_type & get_mount_links() const;
 
      void lock();

      bool is_locked() const;
 
      void configure(const datatools::properties & config_);
 
      void configure_only_for(const datatools::properties & config_,
                              const std::set<std::string> & allowed_from_names_);
    
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options() ) const override;
      
    private:

      bool            _locked_ = false; //!< Lock flag
      link_list_type  _mount_links_;    //!< List of mount links 

      BOOST_SERIALIZATION_BASIC_DECLARATION();
        
    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_MOUNTING_TABLE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
