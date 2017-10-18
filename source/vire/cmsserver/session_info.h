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
// - Bayeux/datatools:
#include <datatools/i_tree_dump.h>

// This project:
#include <vire/cmsserver/resource_cardinality.h>
// #include <vire/resource/role.h>

namespace vire {

  namespace user {
    class manager;
  }

  namespace resource {
    class manager;
  }

  namespace cmsserver {

    /// \brief Session information
    class session_info
      : public datatools::i_tree_dumpable
    {
    public:

      static const int32_t INVALID_ID     = -1; ///< Invalid session ID
      static const int32_t ROOT_ID        =  0; ///< Root session ID
      static const int32_t MAX_USER_ID    = -2; ///< Maximum value for user session
      static const int32_t MIN_BLESSED_ID =  1; ///< Minimum value for system/blessed session

      /// Default constructor
      session_info();

      /// Destructor
      virtual ~session_info();

      /// Check validity flag
      bool is_valid() const;

      /// Check if identifier is set
      bool has_id() const;

      /// Return identifier
      int32_t get_id() const;

      /// Check if key is set
      bool has_key() const;

      /// Return the key
      const std::string & get_key() const;

      /// Check if the record is blessed
      bool is_blessed() const;

      /// Check if the record is user defined
      bool is_user() const;

      /// Check if description is set
      bool has_description() const;

      /// Return description
      const std::string & get_description() const;

      /// Check if role identifier is set
      bool has_role_id() const;

      /// Return role identifier
      int32_t get_role_id() const;

      /// Check if usecase identifier is set
      bool has_usecase_id() const;

      /// Return usecase identifier
      const std::string & get_usecase_id() const;

      /// Check if session time period is set
      bool has_when() const;

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

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      /// Set unique identifier
      void set_id(int32_t id_);

      /// Set unique key
      void set_key(const std::string & key_);

      /// Set description
      void set_description(const std::string & description_);

      /// Set role identifier
      void set_role_id(int32_t rid_);

      /// Set session time period
      void set_when(const boost::posix_time::time_period & when_);

      /// Set usecase
      void set_usecase_id(const std::string & usecase_id_);

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

    protected:

      /// Initialization
      void _initialize(const datatools::properties & config_,
                       const vire::user::manager * user_mgr_ = nullptr,
                       const vire::resource::manager * resource_mgr_ = nullptr);

      /// Set default attribute values
      void _set_defaults();

    private:

      // Configuration:
      int32_t     _id_;                      ///< Session identifier
      std::string _key_;                     ///< Session key
      std::string _description_;             ///< Description of the session
      int32_t     _role_id_;                 ///< Role identifier
      // int32_t     _uid_;                     ///< User identifier
      // int32_t     _gid_;                     ///< Group identifier
      boost::posix_time::time_period _when_; ///< Session time period
      std::string _usecase_id_;              ///< Usecase identifier
      cardinalities_request_type _special_functional_cardinalities_;    ///< Special cardinality request on functional resource tokens
      cardinalities_request_type _special_distributable_cardinalities_; ///< Special cardinality request on distributable resource tokens

    };

  } // namespace cmsserver

} // namespace vire

// #ifndef Q_MOC_RUN
// // Activate reflection layer:
// DR_CLASS_INIT(vire::cmsserver::session_info);
// #endif // Q_MOC_RUN

#endif // VIRE_CMSSERVER_SESSION_INFO_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
