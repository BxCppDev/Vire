//! \file  vire/cmsserver/session.h
//! \brief The CMS server session
//
// Copyright (c) 2016-2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_SESSION_H
#define VIRE_CMSSERVER_SESSION_H

// Standard Library:
#include <string>
#include <set>
#include <memory>

// Third party:
// - Boost:
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/i_tree_dump.h>

// This project:
#include <vire/resource/role.h>
#include <vire/cmsserver/resource_pool.h>

namespace vire {

  namespace resource {
    class manager;
  }

  namespace cmsserver {

    class base_use_case;

    /// \brief Session
    class session
      : private boost::noncopyable
      , public datatools::i_tree_dumpable
    {
    public:

      static const int32_t INVALID_ID     = -1; ///< Invalid session ID
      static const int32_t ROOT_ID        =  0; ///< Root session ID

      typedef std::shared_ptr<base_use_case> use_case_ptr_type;
      typedef std::shared_ptr<session> session_ptr_type;
      typedef std::map<std::string, session_ptr_type> session_dict_type;

      /// Default constructor
      session();

      /// Destructor
      virtual ~session();

      /// Set the logging priority threshold
      void set_logging_priority(datatools::logger::priority lp_);

      /// Return the logging priority threshold
      datatools::logger::priority get_logging_priority() const;

      /// Check if the session identifier is set
      bool has_id() const;

      /// Set the session identifier
      void set_id(const int32_t);

      /// Return the session identifier
      int32_t get_id() const;

      /// Check if a parent session is set
      bool has_parent() const;

      /// Set the parent session
      void set_parent(const session_ptr_type & parent_);

      /// Return the handle to the parent, if any
      const session_ptr_type & get_parent() const;

      /// Check if session is the root session
      bool is_root() const;

      /// Chick if the use case is set
      bool has_use_case() const;

      /// Set the use case handle
      void set_use_case(const use_case_ptr_type &);

      /// Return the handle to the use case
      const use_case_ptr_type & get_use_case() const;

      /// Return the functional resources
      const resource_pool & get_functional() const;

      /// Return the functional resources
      resource_pool & grab_functional();

      /// Set of functional resources
      void set_functional(const resource_pool &);

      /// Return the distributable resources
      const resource_pool & get_distributable() const;

      /// Return the distributable resources
      resource_pool & grab_distributable();

      /// Set of distributable resources
      void set_distributable(const resource_pool &);

      // /// Check if subsessions are set
      // bool has_subsessions() const;

      // /// Return the set of subsessions
      // const session_set_type & get_subsessions() const;

      // /// Return the set of subsessions
      // session_set_type & grab_subsessions();

      // /// Print the list of subsessions
      // void list_subsessions(std::ostream & out_ = std::clog,
      //                       const std::string & title_  = "",
      //                       const std::string & indent_ = "",
      //                       uint32_t flags_ = 0) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      /// Initialization
      void initialize_simple();

      /// Reset
      void reset();

      /// Check initialization flag
      bool is_initialized() const;

      //! Main run
      void run();

      /// Check running flag
      bool is_running() const;

    private:

      // /// Register a subsession
      // void _add_subsession_(const session_ptr_type & s_);

      // /// Unregister a subsession
      // void _sys_remove_subsession_(session & s_);

      /// Initialization of embedded pools
      // void _initialize_pools_(const vire::resource::manager & rmgr_,
      //                         const session_info & sinfo_);

      /// Post initialization
      void _post_init_();

      // /// System initialization
      // void _at_init_(const session_ptr_type & parent_,
      //                 const vire::resource::manager & rmgr_,
      //                 const session_info & sinfo_,
      //                 uint32_t flags_ = 0);

      /// System reset
      void _at_reset_();

      /// System run
      void _at_run_();

    private:

      // Management:
      datatools::logger::priority _logging_ = datatools::logger::PRIO_FATAL; ///< Logging priority
      bool _initialized_ = false; ///< Initialization flag
      bool _running_     = false; ///< Running flag

      // Work:
      int               _id_ = INVALID_ID;     ///< Session unique identifier
      session_ptr_type  _parent_  = nullptr;   ///< Handle to parent session, if any
      // when
      resource_pool     _functional_;          ///< Pool of functional resources (accounting)
      resource_pool     _distributable_;       ///< Pool of distributable resources (accounting)
      use_case_ptr_type _use_case_;            ///< Handle to the use case
      session_dict_type _subsessions_;         ///< Daughter sessions

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_SESSION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
