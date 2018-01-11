//! \file  vire/cmsserver/resource_cardinality.h
//! \brief The CMS server resource cardinality utilities
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_RESOURCE_CARDINALITY_H
#define VIRE_CMSSERVER_RESOURCE_CARDINALITY_H

// Standard Library:
#include <map>
#include <iostream>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
// - Bayeux/datatools:
#include <datatools/properties.h>

// This project:
#include <vire/cmsserver/utils.h>
#include <vire/resource/role.h>

namespace vire {

  namespace resource {

    class manager;

  }

  namespace cmsserver {

    class session_info;

    /// \brief A wrapper for resource cardinality request.
    ///
    /// This object wraps a dictionary which defines a specific number of
    /// tokens requested for each resource, addressed by its resource
    /// identifier.
    ///
    /// Example:
    /// @code
    /// vire::cmsserver::cardinalities_request_type functional_needed_tokens;
    /// functional_needed_tokens[1034] = 2; // 2 requested resource tokens
    /// functional_needed_tokens[1035] = 4; // 4 requested resource tokens
    /// functional_needed_tokens[1036] = 0; // inhibit the resource
    ///
    /// vire::cmsserver::cardinalities_request_type distributable_needed_tokens;
    /// distributable_needed_tokens[1034] = 2;
    /// distributable_needed_tokens[1054] = 6;
    /// distributable_needed_tokens[1055] = 2;
    /// distributable_needed_tokens[1056] = 3;
    /// @endcode
    ///
    /// When initializing, an use case specifies not only the requested
    /// role (and  thus the resources  implied by this role)  but also
    /// the number of resource tokens needed to fulfill its tasks.  As
    /// a session contains a pool of functional resources and a set of
    /// distributable resources,  the use case  will be able  to define
    /// two   cardinalities_request_type    objects   in    order   to
    /// specifically  set  the number  of  tokens  associated to  some
    /// resources  to  some  arbitrary  values.   By  default,  if  no
    /// specific   request  is   defined  for   some  (functional   or
    /// distributable) resource, the number of tokens should be set to
    /// 1 for any limited resource.

    struct resource_cardinality
    {

      /// Default constructor
      resource_cardinality(cardinalities_request_type & request_);

      /// Destructor
      virtual ~resource_cardinality();

      /// Check if a given resource is explicitely set
      bool has_resource(const int32_t resource_id_) const;

      /// Return the cardinality associated to a resource
      std::size_t at(const int32_t resource_id_) const;

      /// Explicitely inhibit the (limited or unlimited) resource by setting zero cardinality
      /// @arg resource_id_ the identifier of the resource to be inhibited
      void unset_resource(const int32_t resource_id_);

      /// Check if a given resource is explicitely unset
      bool has_unset_resource(const int32_t resource_id_) const;

      /// Set the limited resource with a given cardinality
      /// @arg resource_id_ the identifier of the resource with limited token cardinality
      /// @arg cardinality_ the requested cardinality for the limited resource (default is 1)
      void set_limited_resource(const int32_t resource_id_,
                                const std::size_t cardinality_ = 1);

      /// Check if a given limited resource is set
      bool has_limited_resource(const int32_t resource_id_) const;

      /// Set the unlimited resource
      /// @arg resource_id_ the identifier of the unlimited resource
      void set_unlimited_resource(const int32_t resource_id_);

      /// Check if a given unlimited resource is set
      bool has_unlimited_resource(const int32_t resource_id_) const;

      /// Print
      void print(std::ostream & out_) const;

      /// Initialize from a set of configuration properties and a resource manager
      void initialize(const datatools::properties & config_,
                      const vire::resource::manager * resource_mgr_ = nullptr);

      /// Initialize from a set of configuration properties and a resource manager
      void initialize(const datatools::properties & config_,
                      const vire::resource::manager & resource_mgr_);

      /// Reset
      void reset();

      /// Build a 'cardinality request' from a role and optional special request
      void build_from_role(const vire::resource::manager & resource_mgr_,
                           const vire::resource::role & r_,
                           const vire::resource::role::resource_set_flag flag_,
                           const resource_cardinality & special_);

    private:

      cardinalities_request_type & _request_; ///< Wrapped cardinality request dictionary

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_RESOURCE_CARDINALITY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
