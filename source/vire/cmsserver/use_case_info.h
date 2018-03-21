//! \file  vire/cmsserver/use_case_info.h
//! \brief The CMS server session info
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_USE_CASE_INFO_H
#define VIRE_CMSSERVER_USE_CASE_INFO_H

// Standard Library:
#include <string>
#include <map>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/i_serializable.h>

// This project:
#include <vire/cmsserver/utils.h>
#include <vire/cmsserver/resource_cardinality.h>

namespace vire {

  namespace user {
    class manager;
  }

  namespace resource {
    class manager;
  }

  namespace cmsserver {

    /// \brief Description of a CMS use case
    class use_case_info
      : public datatools::i_tree_dumpable
      , public datatools::i_serializable
    {
    public:

      /// Default constructor
      use_case_info();

      /// Destructor
      virtual ~use_case_info();

      /// Check if the name is set
      bool has_name() const;

      /// Set the name
      void set_name(const std::string & name_);

      /// Return the name
      const std::string & get_name() const;

      /// Check if the description is set
      bool has_description() const;

      /// Set the description
      void set_description(const std::string & description_);

      /// Return the description
      const std::string & get_description() const;

      /// Check if the role identifier is set
      bool has_role_definition() const;

      /// Set the role identifier
      void set_role_definition(const std::string & rid_);

      /// Return the role identifier
      const std::string & get_role_definition() const;

      // /// Check if a special request on functional resource cardinalities is set
      // bool has_special_functional_cardinalities() const;

      // /// Check if a special request on distributable resource cardinalities is set
      // bool has_special_distributable_cardinalities() const;

      // /// Return the special request on functional resource cardinalities
      // const cardinalities_request_type & get_special_functional_cardinalities() const;

      // /// Return the special request on distributable resource cardinalities
      // const cardinalities_request_type & get_special_distributable_cardinalities() const;

      // /// Add the special functional resource token limited cardinality
      // void set_special_functional_limited(int32_t resource_id_, std::size_t cardinality_);

      // /// Add the special functional unlimited resource token
      // void set_special_functional_unlimited(int32_t resource_id_);

      // /// Unset the special functional resource
      // void unset_special_functional(int32_t resource_id_);

      // /// Add the special distributable resource token limited cardinality
      // void set_special_distributable_limited(int32_t resource_id_, std::size_t cardinality_);

      // /// Add the special distributable unlimited resource token
      // void set_special_distributable_unlimited(int32_t resource_id_);

      // /// Unset the special distributable resource
      // void unset_special_distributable(int32_t resource_id_);

      /// Check if the use case type identifier is set
      bool has_use_case_type_id() const;

      /// Set the use case identifier
      void set_use_case_type_id(const std::string & use_case_type_id_);

      /// Return the use case identifier
      const std::string & get_use_case_type_id() const;

      /// Check if the use case configuration path is set
      bool has_use_case_config_path() const;

      /// Set the use case configuration path
      void set_use_case_config_path(const std::string & use_case_config_path_);

      /// Return the use case configuration path (path, URN...)
      const std::string & get_use_case_config_path() const;

      /// Check if the explicit use case configuration is set
      bool has_use_case_config() const;

      /// Return the explicit use case configuration
      const datatools::properties & get_use_case_config() const;

      /// Return the explicit use case configuration
      datatools::properties & grab_use_case_config();

      /// Set the explicit use case configuration
      void set_use_case_config(const datatools::properties &);

      /// Reset the explicit use case configuration
      void reset_use_case_config();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
      /// Initialization
      void initialize_simple();

      /// Initialization
      void initialize(const vire::user::manager & user_mgr_,
                      const vire::resource::manager & resource_mgr_);

      /// Initialization
      void initialize(const datatools::properties & config_,
                      const vire::user::manager & user_mgr_,
                      const vire::resource::manager & resource_mgr_);

      /// Reset/invalidate the session info
      void reset();

      /// Check validity flag
      bool is_valid() const;

    private:

      /// Initialization
      void _initialize_(const datatools::properties & config_,
                        const vire::user::manager * user_mgr_ = nullptr,
                        const vire::resource::manager * resource_mgr_ = nullptr);

      /// Set default attribute values
      void _set_defaults_();

    private:

      std::string                            _name_;                 ///< Unique use case name
      std::string                            _description_;          ///< Description of the use case
      std::string                            _role_definition_;      ///< Role definition (name, path, expression...)
      // /// Special cardinality request on functional resource tokens
      // cardinalities_request_type             _special_functional_cardinalities_;
      // /// Special cardinality request on distributable resource tokens
      // cardinalities_request_type             _special_distributable_cardinalities_;
      std::string                            _use_case_type_id_;     ///< Use case type identifier
      std::string                            _use_case_config_path_; ///< Path of the use case configuration
      boost::optional<datatools::properties> _use_case_config_;      ///< Explicit use case configuration

      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION()

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_USE_CASE_INFO_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
