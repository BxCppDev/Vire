//! \file  vire/cmsserver/session_possibility.h
//! \brief Session possibility
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

#ifndef VIRE_CMSSERVER_SESSION_POSSIBILITY_H
#define VIRE_CMSSERVER_SESSION_POSSIBILITY_H

// Standard Library:
#include <string>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>

// This project:
#include <vire/time/utils.h>

namespace vire {

  namespace cmsserver {

    /// \brief Session possibility as computed by the CMS server at client request
    class session_possibility
      : public datatools::i_tree_dumpable
    {
    public:

      /// \brief Types of session action
      enum action_type {
        action_undefined,      //!< Undefined action
        action_enter_session,  //!< Enter an existing running session
        action_create_session  //!< Create a new session
      };

      /// Return the label associated to action
      static std::string action_to_label(const action_type);

      /// Constructor
      session_possibility();

      /// Destructor
      virtual ~session_possibility();

      /// Check validity
      bool is_valid() const;

      /// Reset
      void reset();

      /// Build an enter session record
      void make_enter_session(const std::string & role_id_,
                              const std::string & session_key_,
                              const boost::posix_time::ptime & end_);

      // /// Build a create session record
      // void make_create_session(const std::string & role_id_,
      //                          const std::string & process_info_key_,
      //                          const boost::posix_time::time_period & when_);

      /// Check an enter session record
      bool is_enter_session() const;

      /// Check a create session record
      bool is_create_session() const;

      /// Set the role identifier
      void set_role_id(const std::string &);

      /// Return role identifier
      const std::string & get_role_id() const;

      /// Set the session period
      void set_when(const boost::posix_time::time_period &);

      /// Return session period
      const boost::posix_time::time_period & get_when() const;

      /// Set the session identifier
      void set_session_key(const std::string &);

      /// Return session identifier
      const std::string & get_session_key() const;

      /// Set the process identifier
      void set_process_name(const std::string &);

      /// Return process identifier
      const std::string & get_process_name() const;

      /// Set the process description
      void set_process_description(const std::string &);

      /// Return process description
      const std::string & get_process_description() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      action_type _action_ = action_undefined; ///< Action type
      std::string _role_id_;                   ///< Role identifier
      boost::posix_time::time_period _when_ = vire::time::invalid_time_interval(); ///< Session validity time period
      std::string _session_key_;         ///< Session identifier (only for 'entering' session action: client connection):
      std::string _process_name_;        ///< Process name
      std::string _process_description_; ///< Process description
      std::string _process_type_id_;     ///< Process type identifier

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_SESSION_POSSIBILITY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
