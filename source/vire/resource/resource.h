//! \file    vire/resource/resource.h
//! \brief   Representation of any accessible resource
//! \details The Vire library is able to describe resource objects
//!          published within an environment. Resources can be accessed/reserved through sessions.
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

#ifndef VIRE_RESOURCE_RESOURCE_H
#define VIRE_RESOURCE_RESOURCE_H

// Standard Library:
#include <string>
#include <set>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
// - Bayeux/datatools:
#include <datatools/enriched_base.h>
#include <datatools/properties.h>

// This project:
#include <vire/utility/rw_access.h>
#include <vire/resource/base_resource_instance.h>

namespace vire {

  //! Nested namespace for the Vire library's resource module
  namespace resource {

    class manager;

    //! \brief Description of a resource
    class resource
      : public datatools::enriched_base
    {
    public:

      static const int32_t  INVALID_ID = -1; //!< Invalid resource identifier
      static const int32_t  MIN_ID     =  0; //!< Minimum allowed value for resource identifier
      static const uint32_t MAX_TOKENS_INVALID   = 0; //!< Invalid max number of tokens
      static const uint32_t MAX_TOKENS_SINGLETON = 1; //!< Only one token can reserve the resource
      static const uint32_t MAX_TOKENS_UNLIMITED = 0xFFFFFFFF; //!< Unlimited number of tokens

      // Check if a name is valid
      virtual bool is_name_valid(const std::string & name_) const;

      //! Default constructor
      resource();

      //! Copy constructor
      resource(const resource & r_);

      //! Destructor
      virtual ~resource();

      //! Assignment operator
      resource & operator=(const resource & res_);

      // =============== Initialization/termination methods ===============

      //! Set the resource identifier
      //! Only available at configuration
      void set_id(int32_t);

      //! Set the path
      //! Only available at configuration
      void set_path(const std::string &);

      //! Set the maximum number of tokens
      //! Only available at configuration
      void set_max_number_of_tokens(uint32_t);

      //! Set the access mode
      //! Only available at configuration
      void set_access(vire::utility::rw_access_type);

      //! Set the subscription flag
      void set_allowed_subscription(bool as_);

      //! Set the non blocking execution flag
      void set_allowed_non_blocking(bool anb_);

      //! Set the resource instance
      //! Only available at configuration
      void set_resource_instance(base_resource_instance *);

      //! Set the resource instance
      //! Only available at configuration
      void set_resource_instance(boost::shared_ptr<base_resource_instance> &);

      //! Return resource instance
      base_resource_instance & grab_resource_instance();

      //! Add a dependee
      void add_dependee(const resource & dependee_, bool update_cache_ = false);

      //! Remove dependee given its identifier
      void remove_dependee_with_id(int id_, bool update_cache_ = false);

      //! Remove dependee given its path
      void remove_dependee_with_path(const std::string & path_, bool update_cache_ = false);

      //! Initialization
      void initialize(const datatools::properties &);

      //! Initialization
      void initialize_simple();

      //! Reset
      void reset();

      // =============== Usage methods ===============

      //! Check the validity of the resource
      bool is_valid() const;

      //! Check initialization flag
      bool is_initialized() const;

      //! Check the validity of the RID
      bool has_id() const;

      //! Return the RID
      int32_t get_id() const;

      //! Less operator
      bool operator<(const resource & res_) const;

      //! Check the validity of the path
      bool has_path() const;

      //! Return the path
      const std::string & get_path() const;

      //! Check the validity of the max number of tokens
      bool has_max_number_of_tokens() const;

      //! Return the maximum number of tokens
      uint32_t get_max_number_of_tokens() const;

      //! Check if the number of tokens is unlimited
      bool is_number_of_tokens_unlimited() const;

      //! Check if the number of tokens is a singleton (only one token at a time)
      bool is_number_of_tokens_singleton() const;

      //! Check if the number of tokens is limited
      bool is_number_of_tokens_limited() const;

      //! Check the validity of the access
      bool has_access() const;

      //! Return the access mode
      vire::utility::rw_access_type get_access() const;

      //! Check if the resource is readable
      bool is_readable() const;

      //! Check if the resource is writable
      bool is_writable() const;

      //! Check if resource instance is set
      bool has_resource_instance() const;

      //! Return resource instance
      const base_resource_instance & get_resource_instance() const;

      // //! Check if the resource has an owner device
      // bool has_owner_device() const;

      // //! Set the resource's owner device
      // void set_owner_device(const device & dev_);

      // //! Return the  resource's owner device
      // const device & get_owner_device() const;

      //!  Check if the resource has a depender resources
      bool has_dependers() const;

      //! Return the number of dependers
      std::size_t get_number_of_dependers() const;

      //!  Check if the resource has a dependee resources
      bool has_dependees() const;

      //! Return the number of dependees
      std::size_t get_number_of_dependees() const;

      //! Check if the resource has depender with given path
      bool has_depender_with_path(const std::string & path) const;

      //! Check if the resource has depender with given id
      bool has_depender_with_id(int id_) const;

      //! Check if the resource has dependee with given path
      bool has_dependee_with_path(const std::string & path) const;

      //! Check if the resource has dependee with given id
      bool has_dependee_with_id(int id_) const;

      //! Return the depender resource with given path
      const resource & get_depender_by_path(const std::string & path_) const;

      //! Return the dependee resource with given id
      const resource & get_depender_by_id(int id_) const;

      //! Return the dependee resource with given path
      const resource & get_dependee_by_path(const std::string & path_) const;

      //! Return the dependee resource with given id
      const resource & get_dependee_by_id(int id_) const;

      //! Return the collection of dependers
      const std::set<const resource *> & get_dependers() const;

      //! Return the collection of dependees
      const std::set<const resource *> & get_dependees() const;

      //! Check if subscription is allowed for this resource
      bool is_allowed_subscription() const;

      //! Check if non blocking execution is allowed for this resource
      bool is_allowed_non_blocking() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Return the set of depender resources' identifiers
      const std::set<int32_t> & get_dependers_ids() const;

      //! Return the set of dependee resources' identifiers
      const std::set<int32_t> & get_dependees_ids() const;

      //! Build the cached set of dependee resource identifiers (RID)
      void build_cached_dependee_resource_ids();

      //! Build the cached set of depender resource identifiers (RID)
      void build_cached_depender_resource_ids();

      // =============== Utilities ===============

      //! \brief Comparison by RID
      struct compare_by_id {
        bool operator()(int32_t id_, const resource & r_) const {
          return id_ < r_._id_;
        }
        bool operator()(const resource & r_, int32_t id_) const {
          return r_._id_ < id_;
        }
      };

      //! \brief Comparison by path
      struct compare_by_path {
        bool operator()(const std::string & path_, const resource & r_) const {
          return path_ < r_._path_;
        }
        bool operator()(const resource & r_, const std::string & path_) const {
          return r_._path_ < path_;
        }
      };

      // =============== Protected/private methods ===============

    protected:

      //! Add a depender
      void _add_depender(const resource &, bool update_cache_ = false);

      //! Remove depender given its path
      void _remove_depender_with_path(const std::string & path_, bool update_cache_ = false);

      //! Remove depender given its identifier
      void _remove_depender_with_id(int id_, bool update_cache_ = false);

      //! Set default attribute values
      void _set_defaults();

      //! Set the resource identifier
      void _set_id(int32_t);

    private:

      //! Copy method
      void _copy_from_(const resource &);

      // =============== Attributes ===============

    private:

      // Management:
      bool              _initialized_; //!< Initialization flag

      // Configuration:
      int32_t           _id_;     //!< Resource unique identifier (RID)
      std::string       _path_;   //!< Resource unique path
      vire::utility::rw_access_type _access_; //!< Access (RO, WO, RW)
      uint32_t          _max_number_of_tokens_; //!< Maximum number of tokens
      bool              _allowed_subscription_; //!< Allow subscription
      bool              _allowed_non_blocking_; //!< Allow non blocking execution
      boost::shared_ptr<base_resource_instance> _resource_instance_; //!< Handle to the instance of the resource
      // const device * _owner_device_; //!< Owner device
      // Dependency scheme:
      //
      //   +----+  +----+  +----+
      //   | A1 |  | A2 |  | A3 | these dependees are depended on by the resource
      //   +----+  +----+  +----+
      //      ^      ^      ^
      //       \     |     / dependency
      //     +---------------+
      //     | the resource  | the resource depends on its dependees
      //     +---------------+
      //         ^      ^
      //        /        \ dependency
      //     +----+    +----+
      //     | B1 |    | B2 | these dependers/dependents depend on the resource
      //     +----+    +----+
      //
      std::set<const resource *> _dependers_; //!< List of resources that depends on by this resource
      std::set<const resource *> _dependees_; //!< List of resources by which the resources depends on

      // Cached data:
      boost::scoped_ptr<std::set<int32_t> > _cached_dependee_ids_; //!< Cached enumerated set of dependee resource identifiers
      boost::scoped_ptr<std::set<int32_t> > _cached_depender_ids_; //!< Cached enumerated set of depender resource identifiers

      friend manager;

      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION_ADVANCED(resource);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI();
#endif // Q_MOC_RUN

    };

  } // namespace resource

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::resource::resource);
#endif // Q_MOC_RUN

#endif // VIRE_RESOURCE_RESOURCE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
