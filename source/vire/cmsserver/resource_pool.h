//! \file    vire/cmsserver/resource_pool.h
//! \brief   High level wrapper for a set of resources
//
// Copyright (c) 2016 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jérôme Poincheval <poincheval@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_RESOURCE_POOL_H
#define VIRE_CMSSERVER_RESOURCE_POOL_H

// Standard Library:
#include <string>
#include <set>
#include <map>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>

// This project:
#include <vire/cmsserver/resource_cardinality.h>

namespace vire {

  namespace resource {
    class manager;
  }

  namespace cmsserver {

    //! \brief Description of a pool of resources (addressed by identifiers)
    //!        with their accounting informations.
    class resource_pool
      : public datatools::i_tree_dumpable
    {
    public:

      /// \brief Cardinality profile at pool initialization:
      enum cardinality_profile_type {
        CARD_ALL          = 0,
        CARD_ONLY_ONE     = 1,
        CARD_ALL_PLUS_ONE = 2
      };

      //! Default constructor
      resource_pool();

      //! Destructor
      virtual ~resource_pool();

      //! Reset
      void reset();

      //! Load/initialize the daughter pool from a parent pool and token cardinality directives
      //! @arg parent_   the parent resource pool from which to transfer the resource tokens
      //! @arg daughter_ the daughter resource pool that is filled with the resource tokens
      //! @arg resource_mgr_ the resource manager which knows the static properties
      //!                    of each resource and maximum cardinalities associated to it
      //! @arg requested_cardinalities_ the cardinality request on some specific resource identifiers
      static void load_daughter_from_parent(resource_pool & parent_,
                                            resource_pool & daughter_,
                                            const vire::resource::manager & resource_mgr_,
                                            const cardinalities_request_type & requested_cardinalities_);

      //! Restore the parent pool state from a daughter pool by
      //! transfering back all allocated resource tokens
      //! @arg parent_   the parent resource pool to be restored
      //! @arg daughter_ the daughter resource pool from which to fetch the resource tokens
      static void restore_parent_from_daughter(resource_pool & parent_,
                                               resource_pool & daughter_);

      //! Initialize the root resource pool from an explicit request on resource cardinalities
      //! and a resource manager which knows the static properties of each resource.
      //!
      //! @arg root_          the target resource pool considered as the root pool
      //! @arg resource_mgr_  the resource manager which knows the static properties
      //!                     of each resource and maximum cardinalities
      //! @arg requested_cardinalities_ the cardinality request on some specific resource identifiers
      //! @arg profile_       the cardinality profile that describes the default strategy
      //!                     to set the token cardinalities if no specific request is set
      //!                     for a resource
      static void init_root(resource_pool & root_,
                            const vire::resource::manager & resource_mgr_,
                            const cardinalities_request_type & requested_cardinalities_,
                            cardinality_profile_type profile_ = CARD_ALL);

      //! Check if a resource belongs to the resource set
      bool has_resource(int32_t) const;

      //! Check if a resource is limited
      bool is_limited(int32_t) const;

      //! Check if a limited resource is exhausted
      bool is_exhausted(int32_t) const;

      //! Check if a resource is unlimited
      bool is_unlimited(int32_t) const;

      //! Add an unlimited resource ID
      void add_unlimited(int32_t);

      //! Add a limited resource with initial number of tokens
      void add_limited(int32_t rid_, std::size_t init_ = 0);

      //! Increment the number of tokens by some amount (limited resources only)
      void increment_limited_tokens(int32_t, std::size_t = 1);

      //! Decrement the number of tokens by some amount (limited resources only)
      void decrement_limited_tokens(int32_t, std::size_t = 1);

      //! Return the number of remaining tokens for a limited resource
      std::size_t remaining_limited_tokens(int32_t) const;

      //! Check if pool can transfer a given amount of tokens
      bool can_tranfer(int32_t, std::size_t);

      //! Build the set of all resource identifiers
      void build_all(std::set<int32_t> & all_, bool append_ = false) const;

      //! Build the set of all limited resource identifiers
      void build_limited(std::set<int32_t> & limited_, bool append_ = false) const;

      //! Build the set of all unlimited resource identifiers
      void build_unlimited(std::set<int32_t> & unlimited_, bool append_ = false) const;

      //! Return the map of limited resource identifier-token pairs
      const std::map<int32_t, std::size_t> & get_limited_tokens() const;

      //! Return the set of unlimited resource identifiers
      const std::set<int32_t> & get_unlimited() const;

      //! Compute the difference between two resource pools
      static void diff(const resource_pool & pool1_,
                       const resource_pool & pool2_,
                       std::map<int32_t, std::size_t> & result_);

      //! Set operations
      // //! Check if the pool contains/includes another pool
      // bool contains(const resource_pool & rp_) const;

      // static void and(const resource_pool & rp1_, const resource_pool & rp2_,
      //                resource_pool & rp3_);

      // static void or(const resource_pool & rp1_, const resource_pool & rp2_,
      //                resource_pool & rp3_);

      // friend resource_pool operator+(const resource_pool & rp1_, const resource_pool & rp2_);

      // friend resource_pool operator-(const resource_pool & rp1_, const resource_pool & rp2_);

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Print
      void print(std::ostream & out_, uint32_t flags_ = 0) const;

      //! Equality test operator
      bool operator==(const resource_pool &) const;

      //! Inequality test operator
      bool operator!=(const resource_pool &) const;

    private:

      std::map<int32_t, std::size_t> _limited_tokens_; //!< Accounting for limited resources
      std::set<int32_t>              _unlimited_;      //!< List of unlimited resource identifiers

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_RESOURCE_POOL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
