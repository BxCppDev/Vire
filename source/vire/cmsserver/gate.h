//! \file  vire/cmsserver/gate.h
//! \brief The CMS server gate for remote client connections
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_GATE_H
#define VIRE_CMSSERVER_GATE_H

// Standard Library:
#include <string>

// Third party:
// - Boost:
#include <boost/date_time/posix_time/posix_time.hpp>
// - Bayeux/datatools:
#include <datatools/base_service.h>
#include <datatools/properties.h>
#include <datatools/bit_mask.h>

// This project:
// #include <vire/time/utils.h>
#include <vire/cmsserver/session_possibility.h>

namespace vire {

  namespace cmsserver {

    /// \brief The CMS server gate service for client connection
    class gate
      : public ::datatools::base_service
    {

    public:

      /// Default maximum number of connections
      static const std::size_t DEFAULT_MAX_NUMBER_OF_CONNECTIONS = 100;

      /// Default constructor
      gate(uint32_t flags_ = 0);

      /// Destructor
      virtual ~gate();

      /// \brief Connection info
      class connection_info
      {
      public:

        /// Default constructor
        connection_info(uint32_t flags_ = 0);

        /// Destructor
        ~connection_info();

        bool has_id() const;

        void set_id(int32_t id_);

        void clear_id();

        int32_t get_id() const;

        bool has_key() const;

        void set_key(const std::string & key_);

        void clear_key();

        const std::string & get_key() const;

        bool has_user_login() const;

        void set_user_login(const std::string & user_login_);

        void clear_user_login();

        const std::string & get_user_login() const;

        bool has_time_interval() const;

        void set_time_interval(const boost::posix_time::time_period &);

        void clear_time_interval();

        const boost::posix_time::time_period & get_time_interval() const;

      private:

        int32_t     _id_;
        std::string _key_;
        std::string _user_login_;
        boost::posix_time::time_period _time_interval_;

      };

      /// Check if login is set
      bool has_login() const;

      /// Set login
      void set_login(const std::string &);

      /// Return login
      const std::string & get_login() const;

      /// Check if password is set
      bool has_password() const;

      /// Set password
      void set_password(const std::string &);

      /// Return password
      const std::string & get_password() const;

      /// Authenticate
      bool authenticate(const std::string & login_, const std::string & password_) const;

      /// Set the maximum number of connections
      void set_max_number_of_connections(std::size_t);

      /// Return the maximum number of connections
      std::size_t get_max_number_of_connections() const;

      // /// Process a client connection request
      // int process(const client_connection_request & request_,
      // client_connection_request_response_type & response_);

      //! Check the initialization flag
      virtual bool is_initialized() const;

      //! Initialize the manager from a container of properties
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the manager
      virtual int reset();

      //! Main
      void run();

      void get_possible(const std::string & vire_user_login_,
                        const std::string & vire_user_password_,
                        const boost::posix_time::ptime & at_,
                        const std::string & role_id_,
                        const uint32_t flags_,
                        std::vector<session_possibility> & possible_);

      void create_session(const std::string & vire_user_login_,
                          const std::string & vire_user_password_,
                          const boost::posix_time::time_period & from_to_,
                          const std::string & usecase_id_,
                          const datatools::properties & usecase_params_,
                          int32_t & session_id_);

      void enter_session(const std::string & vire_user_login_,
                         const std::string & vire_user_password_,
                         int32_t & session_id_,
                         std::string & session_key_,
                         boost::posix_time::time_period & from_to_);

    private:

      //! Set default attributes' values
      void _set_defaults_();

    private:

      // Management:
      bool _initialized_; //!< Initialization flag

      // Configuration:
      std::string _login_;
      std::string _password_;
      std::size_t _max_number_of_connections_ = DEFAULT_MAX_NUMBER_OF_CONNECTIONS; ///< Maximum number of connections

      // Working data:

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(gate)

    };

  } // namespace cmsserver

} // namespace vire

// // OCD support :
// #include <datatools/ocd_macros.h>
// // @arg vire::cmsserver::gate the name of the class with OCD support
// DOCD_CLASS_DECLARATION(vire::cmsserver::gate)

// #ifndef Q_MOC_RUN
// // Activate reflection layer:
// DR_CLASS_INIT(vire::cmsserver::gate);
// #endif // Q_MOC_RUN

#endif // VIRE_CMSSERVER_GATE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
