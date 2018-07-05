//! \file  vire/cmsserver/uc_resource_mount_link.h
//! \brief Vire CMS use case resource mount link
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

#ifndef VIRE_CMSSERVER_UC_RESOURCE_MOUNT_LINK_H
#define VIRE_CMSSERVER_UC_RESOURCE_MOUNT_LINK_H

// Standard Library:
#include <string>
#include <iostream>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/serialization_macros.h>

// This project:
#include <vire/cmsserver/uc_utils.h>
#include <vire/utility/path.h>

namespace vire {

  namespace cmsserver {

    /// \brief Local resource mounting entry within an use case
    ///
    /// It represents a link from a scope resource/device port in
    /// a daughter use case or from a functional resource to a scope
    /// resource port.
    ///
    /// \code
    ///      __________
    ///     / Use case \
    ///    +------------+--------+
    ///    |                     |
    ///  V o <---------o R1      |  R1->V           (functional resources)
    ///    |                     |
    ///    | [Foo/Bar]           |
    ///  W o <---------o R2      |  R2->W[Foo/Bar]  (functional resources)
    ///    |             ___     |
    ///    |            / A \    |  A is a daughter use case
    ///    |           +-----+-+ |
    ///    |  [M/N/P]  |       | |
    ///  U o <---------o X     | |  @A:X->U[M/N/P]  (distributable resources/devices)
    ///    |           |       | |
    ///    |           |       | |
    ///  Z o <---------o Y     | |  @A:Y->Z         (distributable resources/devices)
    ///    |           |       | |
    ///    |           +-------+ |
    ///    |                     |
    ///    +---------------------+
    /// \endcode
    
    class uc_resource_mount_link
    {
    public:
      
      uc_resource_mount_link();
      
      uc_resource_mount_link(const uc_port_id & from_id_,
                             const std::string & to_key_);
      
      uc_resource_mount_link(const uc_port_id & from_id_,
                             const std::string & to_key_,
                             const vire::utility::relative_path & relpath_);
      
      uc_resource_mount_link(const uc_port_id & from_id_,
                             const std::string & to_key_,
                             const std::string & relpath_repr_ = "");

      bool is_complete() const;
      
      bool has_from_id() const;
 
      const uc_port_id & get_from_id() const;

      void set_from_id(const uc_port_id &);
      
      bool has_to_key() const;

      void set_to_key(const std::string & to_key_);
      
      const std::string & get_to_key() const;
     
      void reset_to_key();
       
      bool has_relative_path() const;
      
      void set_relative_path(const vire::utility::relative_path & relpath_);
      
      void set_relative_path(const std::string & relpath_repr_);
      
      void reset_relative_path();
      
      const vire::utility::relative_path & get_relative_path() const;

      std::string to_string() const;

      /** Parse from a string
       * Examples:
       * \code 
       * "@Foo:PS->HVPS"
       * "@Foo:T1->Dev1[Monitoring/Temperature]"
       * \endcode
       *
       */
      bool from_string(const std::string & repr_);
      
      friend std::ostream & operator<<(std::ostream & out_,
                                       const uc_resource_mount_link & mlink_);
      
    private:
      
      uc_port_id                   _from_id_;  
      std::string                  _to_key_;
      vire::utility::relative_path _relpath_; ///< Relative path (optional)
 
      BOOST_SERIALIZATION_BASIC_DECLARATION();
      
    };
  
  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_RESOURCE_MOUNT_LINK_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
