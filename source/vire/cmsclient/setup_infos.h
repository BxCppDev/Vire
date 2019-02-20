//! \file  vire/cmsclient/setup_infos.h
//! \brief Server infos
//
// Copyright (c) 2017-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2017-2019 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSCLIENT_SETUP_INFOS_H
#define VIRE_CMSCLIENT_SETUP_INFOS_H

// Standard library:
#include <string>

// Third party:
// - Bayeux:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/utility/instance_identifier.h>
#include <vire/utility/model_identifier.h>

namespace vire {

  namespace cmsclient {

    /// \brief Informations about the setup
    class setup_infos
      : public datatools::i_tree_dumpable
    {
    public:

      //! Default constructor
      setup_infos();

      //! Destructor
      virtual ~setup_infos();

      //! Check if the setup ID is set
      bool has_setup_id() const;

      //! Set the setup ID
      void set_setup_id(const ::vire::utility::instance_identifier &);

      //! Return the setup ID
      const ::vire::utility::instance_identifier & get_setup_id() const;

      //! Check if the host is set
      bool has_host() const;

      //! Set the host
      void set_host(const std::string &);

      //! Return the host
      const std::string & get_host() const;

      //! Check if the port is set
      bool has_port() const;

      //! Set the port
      void set_port(const int);

      //! Return the port
      int get_port() const;

      //! Check if the domain name prefix is set
      bool has_domain_name_prefix() const;

      //! Set the domain name prefix
      void set_domain_name_prefix(const std::string &);

      //! Return the domain name prefix
      const std::string & get_domain_name_prefix() const;

      //! Check if the transport driver type ID is set
      bool has_transport_driver_type_id() const;

      //! Set the transport driver type ID
      void set_transport_driver_type_id(const ::vire::utility::model_identifier &);

      //! Return the transport driver type ID
      const ::vire::utility::model_identifier & get_transport_driver_type_id() const;

      //! Check if the encoding driver type ID is set
      bool has_encoding_driver_type_id() const;

      //! Set the encoding driver type ID
      void set_encoding_driver_type_id(const ::vire::utility::model_identifier &);

      //! Return the encoding driver type ID
      const ::vire::utility::model_identifier & get_encoding_driver_type_id() const;

      //! Check if the client side gate login is set
      bool has_gate_client_user_login() const;

      //! Set the client side gate login
      void set_gate_client_user_login(const std::string &);

      //! Return the client side gate login
      const std::string & get_gate_client_user_login() const;

      //! Check if the client side gate password is set
      bool has_gate_client_user_password() const;

      //! Set the client side gate password
      void set_gate_client_user_password(const std::string &);

      //! Return the client side gate password
      const std::string & get_gate_client_user_password() const;

      //! Check validity
      bool is_valid() const;

      //! Check initialization status
      bool is_initialized() const;

      //! Initialize the server infos
      void initialize(const datatools::properties &);

      //! Terminate the server infos
      void reset();

      //! Lock the server infos
      void lock();

      //! Unlock the server infos
      void unlock();

      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_ = empty_options()) const override;

    private:

      // Management:
      bool _initialized_ = false; ///< Initialization flag

      // Configuration:
      ::vire::utility::instance_identifier _setup_id_; ///< The experimental setup identifier handled by the server
      std::string _host_;               ///< The server host identifier
      int         _port_ = -1;          ///< The server port
      std::string _domain_name_prefix_; ///< The domain name prefix
      /// The transport driver type ID (ex: "vire::rabbitmq::transport_driver")
      ::vire::utility::model_identifier _transport_driver_type_id_;
      /// The encoding driver type ID (ex: "vire::com::protobuf_encoding_driver")
      ::vire::utility::model_identifier _encoding_driver_type_id_;
      std::string _gate_client_user_login_;    ///< The client side login used to connect to the server's client gate
      std::string _gate_client_user_password_; ///< The client side password used to connect to the server's client gate

    };

  } // namespace cmsclient

} // namespace vire

#endif // VIRE_CMSCLIENT_SETUP_INFOS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
