//! \file  vire/cmsserver/session_info.h
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

#ifndef VIRE_CMSSERVER_SESSION_INFO_H
#define VIRE_CMSSERVER_SESSION_INFO_H

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

    /// \brief Description of a session for the agenda
    class session_info
      : public datatools::i_tree_dumpable
      , public datatools::i_serializable
    {
    public:

      /// Default constructor
      session_info();

      /// Destructor
      virtual ~session_info();

      /// Check if key is set
      bool has_key() const;

      /// Set unique key
      void set_key(const std::string & key_);

      /// Return the key
      const std::string & get_key() const;

      /// Check if description is set
      bool has_description() const;

      /// Set description
      void set_description(const std::string & description_);

      /// Return description
      const std::string & get_description() const;

      /// Check if role identifier is set
      bool has_role_identifier() const;

      /// Set role identifier
      void set_role_identifier(const std::string & rid_);

      /// Return role identifier
      const std::string & get_role_identifier() const;

      /// Check if session time period is set
      bool has_when() const;

      /// Set session time period
      void set_when(const boost::posix_time::time_period & when_);

      /// Return session time period
      const boost::posix_time::time_period & get_when() const;

      /// Check if special request on functional resource cardinalities are set
      bool has_special_functional_cardinalities() const;

      /// Check if special request on distributable resource cardinalities are set
      bool has_special_distributable_cardinalities() const;

      /// Return special request on functional resource cardinalities
      const cardinalities_request_type & get_special_functional_cardinalities() const;

      /// Return special request on distributable resource cardinalities
      const cardinalities_request_type & get_special_distributable_cardinalities() const;

      /// Add special functional resource token limited cardinality
      void set_special_functional_limited(int32_t resource_id_, std::size_t cardinality_);

      /// Add special functional unlimited resource token
      void set_special_functional_unlimited(int32_t resource_id_);

      /// Unset special functional resource
      void unset_special_functional(int32_t resource_id_);

      /// Add special distributable resource token limited cardinality
      void set_special_distributable_limited(int32_t resource_id_, std::size_t cardinality_);

      /// Add special distributable unlimited resource token
      void set_special_distributable_unlimited(int32_t resource_id_);

      /// Unset special distributable resource
      void unset_special_distributable(int32_t resource_id_);

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

      // Configuration:
      std::string                    _key_;             ///< Unique session key
      std::string                    _description_;     ///< Description of the session
      std::string                    _role_identifier_; ///< Role identifier (name)
      boost::posix_time::time_period _when_;            ///< Session time period
      cardinalities_request_type     _special_functional_cardinalities_;    ///< Special cardinality request on functional resource tokens
      cardinalities_request_type     _special_distributable_cardinalities_; ///< Special cardinality request on distributable resource tokens
      std::string                    _use_case_type_id_;        ///< Use case type identifier
      std::string                    _use_case_config_path_;    ///< Path of the use case configuration
      boost::optional<datatools::properties> _use_case_config_; ///< Explicit use case configuration

      //! Serialization interface
      DATATOOLS_SERIALIZATION_DECLARATION()

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_SESSION_INFO_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
