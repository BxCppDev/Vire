//! \file  vire/com/domain_builder.h
//! \brief Vire communication domain builder
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                            Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_DOMAIN_BUILDER_H
#define VIRE_COM_DOMAIN_BUILDER_H

// Standard library:
#include <string>

// This project:
#include <vire/com/domain.h>

namespace vire {

  namespace com {

    //! \brief Communication domain builder
    class domain_builder
    {
    public:

      static std::string build_cms_topic_name(const std::string & setup_id_, const std::string & topic_);

      static std::string build_cms_control_name(const std::string & setup_id_);

      static std::string build_cms_monitoring_name(const std::string & setup_id_);

      static std::string build_cms_subcontractor_system_name(const std::string & setup_id_,
                                                             const std::string & subcontractor_id_);

      static std::string build_cms_client_system_name(const std::string & setup_id_,
                                                      const std::string & client_id_);

      static std::string build_cms_clients_gate_name(const std::string & setup_id_);

      //! Default constructor
      domain_builder();

      //! Constructor
      domain_builder(const std::string & setup_name_);

      //! Destructor
      virtual ~domain_builder();

      //! Check if the domain is valid
      bool is_valid() const;

      //! Check setup name
      bool has_setup_name() const;

      //! Set setup name
      //!
      //! Supported formats:
      //! - "supernemo"
      //! - "supernemo/demonstrator"
      //! - "supernemo/test"
      void set_setup_name(const std::string & name_);

      //! Return setup name
      const std::string & get_setup_name() const;

      //! Build the clients gate domain
      void build_clients_gate_domain(domain &);

      //! Build the control domain
      void build_control_domain(domain &);

      //! Build the monitoring domain
      void build_monitoring_domain(domain &);

      //! Build the client system domain
      void build_client_system_domain(domain &, const std::string & client_id_);

      //! Build the subcontractor system domain
      void build_subcontractor_system_domain(domain &, const std::string & subcontractor_id_);

    private:

      void _set_defaults_();

    private:

      std::string _setup_name_; //!< Name of the setup
      vire::utility::model_identifier _default_transport_type_id_; //!< Default transport type identifier
      vire::utility::model_identifier _default_encoding_type_id_;  //!< Default encoding type identifier

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_DOMAIN_BUILDER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
