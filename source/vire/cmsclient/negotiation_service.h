//! \file  vire/cmsclient/negotiation_service.h
//! \brief Negotiation service
//
// Copyright (c) 2019 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSCLIENT_NEGOTIATION_SERVICE_H
#define VIRE_CMSCLIENT_NEGOTIATION_SERVICE_H

// Standard Library
#include <string>

// Third party:
// - Bayeux:
#include <bayeux/datatools/base_service.h>

// This project:
#include <vire/com/manager.h>
#include <vire/cmsclient/session_connection_parameters.h>
#include <vire/com/access_hub.h>

namespace vire {

  namespace cmsclient {

    class client;

    /// \brief Negotiation service
    class negotiation_service
      : public ::datatools::base_service
    {
    public:

      // Negotiation worker interface
      struct i_worker
      {
        i_worker(negotiation_service & parent_);
        virtual ~i_worker();
        virtual bool run(session_connection_parameters &) = 0;
      protected:
        negotiation_service & _nego;
      };

      /// Default constructor
      negotiation_service();

      /// Destructor
      virtual ~negotiation_service();

      void set_client(const client & client_);

      void set_com(vire::com::manager & com_);

      bool has_gate_user_login() const;

      void set_gate_user_login(const std::string &);

      const std::string & get_gate_user_login() const;

      bool has_gate_user_password() const;

      void set_gate_user_password(const std::string &);

      const std::string & get_gate_user_password() const;

      //! Check initialization status
      bool is_initialized() const override;

      //! Initialize the service from a list of properties
      int initialize(const datatools::properties & config_,
                     datatools::service_dict_type & services_) override;

      //! Reset
      int reset() override;

      /// Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_ = empty_options()) const override;

      bool run(session_connection_parameters & nego_result_);

    private:

      void _at_init_();

      void _at_reset_();
      
    private:

      // Management:
      bool _initialized_ = false;            //!< Initialization status

      // Configuration:
      const client * _client_ = nullptr;     //!< Parent client
      vire::com::manager * _com_ = nullptr;  //!< Handle to the com service
      std::string _gate_user_login_;         //!< Client side gate user login
      std::string _gate_user_password_;      //!< Client side gate user password

      // Working data:
      vire::com::access_hub_ptr_type _client_gate_hub_;
      
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(negotiation_service)

    };

  } // namespace cmsclient

} // namespace vire

#endif // VIRE_CMSCLIENT_NEGOTIATION_SERVICE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
