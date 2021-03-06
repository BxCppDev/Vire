//! \file  vire/cmsserver/session.h
//! \brief The CMS server session
//
// Copyright (c) 2016-2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                         François Mauger <mauger@lpccaen.in2p3.fr>
//                         Jérôme Poincheval <poincheval@lpccaen.in2p3.fr>
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
#include <bayeux/datatools/bit_mask.h>
#include <bayeux/datatools/i_tree_dump.h>

// This project:
#include <vire/resource/role.h>
#include <vire/cmsserver/resource_pool.h>
#include <vire/cmsserver/session_info.h>

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

      static const int32_t INVALID_ID     =  -1; ///< Invalid session ID
      static const int32_t ROOT_ID        =   0; ///< Root session ID
      static const int32_t MAX_SYSTEM_ID  = 999; ///< Max system session ID
      
      struct session_entry
      {
        ~session_entry();
        std::string       name;     //!< Unique name for the sub-session
        session_ptr_type  instance; //!< Sub-session instance
        // use_case_ptr_type usecase;  //!< Sub-session use case
      };
      typedef std::map<std::string, session_entry> session_dict_type;

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

      /// Check if the session is a system session
      bool is_system() const;

      /// Check if a parent session is set
      bool has_parent() const;

      /// Reset the parent session
      void reset_parent();

      /// Return the handle to the parent, if any
      const session & get_parent() const;

      /// Check if session is the root session
      bool is_root() const;

      /// Chick if the use case is set
      bool has_use_case() const;

      /// Set the use case handle
      void set_use_case(base_use_case &);

      /// Return the handle to the use case
      const base_use_case & get_use_case() const;

      /// Check if the session period is set (UTC)
      bool has_when() const;

      /// Return the session period (UTC)
      const boost::posix_time::time_period & get_when() const;

      /// Set the session period (UTC)
      void set_when(const boost::posix_time::time_period &);

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

      /// Check if subsessions are set
      bool has_subsessions() const;

      /// Return the set of subsessions
      const session_dict_type & get_subsessions() const;

      /// Build the list of subsessions' names
      void build_subsession_names(std::set<std::string> & names_) const;
      
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

      /// Terminate
      void terminate();

      /// Check initialization flag
      bool is_initialized() const;

      //! Set the check only flag
      void set_check_only(bool check_);

      /// Check the check only flag
      bool is_check_only() const;

      // //! Main run
      // void run();

      // /// Check if the session can run
      // bool can_run() const;

      /// Check running flag
      bool is_running() const;
      
      /// Check running flag
      bool is_completed() const;
 
      /// Start the session
      void start();

      /// Stop the session
      void stop();
      
    public:

      enum root_session_flag_type {
        CHECK_ONLY = datatools::bit_mask::bit00 //!< 
      };
      
      static session_ptr_type create_root_session(const session_info &,
                                                  const uint32_t flags_ = 0);
      
    private:

      void _create_subsession_(const std::string & name_, const session_info &);

      void _destroy_subsession_(const std::string & name_);
      
      static session_ptr_type _create_session_(session * parent_,
                                               const session_info & sinfo_);
                               
    private:

      /// Initialization of embedded pools
      // void _initialize_pools_(const vire::resource::manager & rmgr_,
      //                         const session_info & sinfo_);

      /// Post initialization
      //! Setup all structural internal mechanisms for resource execution
      //! - bind "write" resources on Control domain's exchange
      //! - ...
      void _at_init_();

      //! Check the use case
      void _check_use_case_();

      // /// System run
      // void _at_run_();

      /// System terminate
      void _at_terminate_();

      /// Destroy the embedded use case
      void _destroy_use_case_();

    private:

      // Management:
      datatools::logger::priority _logging_ = datatools::logger::PRIO_FATAL; ///< Logging priority
      bool _initialized_ = false; ///< Initialization flag
      bool _check_only_  = false; ///< Check only flag
      bool _running_     = false; ///< Running flag
      //   bool _completed_  = false; ///< completed flag
      //   bool _timeout_    = false; ///< timeout flag

      // Work:
      int               _id_ = INVALID_ID;    ///< Session unique identifier
      const session *   _parent_  = nullptr;  ///< Handle to parent session, if any
      use_case_ptr_type _use_case_;           ///< Handle to the use case
      boost::posix_time::time_period _when_;  ///< Session time period
      resource_pool     _functional_;         ///< Pool of functional resources (accounting)
      resource_pool     _distributable_;      ///< Pool of distributable resources (accounting)
      session_dict_type _subsessions_;        ///< Daughter sessions

      // Pimpl-ized internals:
      // struct internals_type;
      // std::unique_ptr<internals_type> _internals_;

      friend class base_use_case;
      
    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_SESSION_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
