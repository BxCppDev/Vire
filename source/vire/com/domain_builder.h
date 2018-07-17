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
#include <vire/com/utils.h>
#include <vire/utility/model_identifier.h>

namespace vire {

  namespace com {

    class domain;

    //! \brief Communication domain builder
    class domain_builder
    {
    public:

      // Validate a domain name prefix
      static bool validate_domain_name(const std::string & setup_name_);

      static bool validate_domain_name_prefix(const std::string & setup_name_);

      // Build  the domain name prefix from the name of a setup
      static std::string domain_name_prefix(const std::string & setup_name_);

      // Build the name of a domain associated to a given topic
      static std::string build_cms_topic_name(const std::string & name_prefix_,
                                              const std::string & topic_);

      // Build the name of the vire control domain
      static std::string build_cms_control_name(const std::string & name_prefix_);

      // Build the name of the vire monitoring domain
      static std::string build_cms_monitoring_name(const std::string & name_prefix_);

      // Build the name of the vire system domain associated to a subcontractor
      static std::string build_cms_subcontractor_system_name(const std::string & name_prefix_,
                                                             const std::string & subcontractor_id_);

      // Build the name of the vire system domain associated to a client
      static std::string build_cms_client_system_name(const std::string & name_prefix_,
                                                      const std::string & client_id_);

      // Build the name of the vire client gate domain
      static std::string build_cms_clients_gate_name(const std::string & name_prefix_);

      bool extract_client_id(const std::string & domain_name_prefix_,
                             const std::string & domain_name_,
                             std::string & client_id_);

      bool extract_subcontractor_id(const std::string & domain_name_prefix_,
                                    const std::string & domain_name_,
                                    std::string & subcontractor_id_);
      
      //! Default constructor
      domain_builder();

      //! Constructor
      domain_builder(const std::string & domain_name_prefix_);

      //! Destructor
      virtual ~domain_builder();

      //! Check if the domain is valid
      bool is_valid() const;

      //! Check domain name prefix
      bool has_domain_name_prefix() const;

      //! Set domain name prefix
      //!
      //! Supported formats:
      //! - "/supernemo"
      //! - "/supernemo/demonstrator"
      //! - "/supernemo/test_bench"
      void set_domain_name_prefix(const std::string & name_);

      //! Return domain name prefix
      const std::string & get_domain_name_prefix() const;

      //! Check encoding type identifier
      bool has_encoding_type_id() const;

      //! Set encoding type identifier
      void set_encoding_type_id(const vire::utility::model_identifier & encoding_type_id_);

      //! Return encoding type identifier
      const vire::utility::model_identifier & get_encoding_type_id() const;

      //! Check transport type identifier
      bool has_transport_type_id() const;

      //! Set transport type identifier
      void set_transport_type_id(const vire::utility::model_identifier & transport_type_id_);

      //! Return transport type identifier
      const vire::utility::model_identifier & get_transport_type_id() const;

      //! Build the clients gate domain
      void build_clients_gate_domain(domain &) const;

      //! Build the control domain
      void build_control_domain(domain &) const;

      //! Build the monitoring domain
      void build_monitoring_domain(domain &) const;

      //! Build the client system domain
      void build_client_system_domain(domain &, const std::string & client_id_) const;

      //! Build the subcontractor system domain
      void build_subcontractor_system_domain(domain &, const std::string & subcontractor_id_) const;

      void reset();

    private:

      void _set_defaults_();

    private:

      std::string _domain_name_prefix_; //!< Domain name prefix
      vire::utility::model_identifier _transport_type_id_; //!< Transport type identifier
      vire::utility::model_identifier _encoding_type_id_;  //!< Encoding type identifier

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_DOMAIN_BUILDER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
