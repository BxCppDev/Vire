//! \file  vire/com/manager.h
//! \brief Vire com manager
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

#ifndef VIRE_COM_MANAGER_H
#define VIRE_COM_MANAGER_H

// Standard library:
#include <cstdint>
#include <string>
#include <map>
#include <set>
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/bit_mask.h>
#include <bayeux/datatools/base_service.h>

// // This project:
#include <vire/com/actor.h>
#include <vire/com/utils.h>
#include <vire/com/domain_builder.h>

namespace vire {

  namespace resource {
    class manager;
  }

  namespace com {

    class domain;

    //! \brief Communication manager service
    class manager
      : public ::datatools::base_service
    {
    public:

      //! Dictionary of shared pointers on domain objects
      typedef std::map<std::string, std::shared_ptr<domain>> domain_dict_type;

      //! Default constructor
      manager(uint32_t flags_ = 0);

      //! Destructor
      virtual ~manager();

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

      //! Check the actor
      bool has_actor() const;

      //! Set the actor
      void set_actor(const actor &);

      //! Set the actor
      void reset_actor();

      //! Return the actor
      const actor & get_actor() const;

      //! Check the transport type identifier
      bool has_transport_type_id() const;

      //! Set the transport type identifier
      void set_transport_type_id(const vire::utility::model_identifier &);

      //! Return the transport type identifier
      const vire::utility::model_identifier & get_transport_type_id() const;

      //! Check the encoding type identifier
      bool has_encoding_type_id() const;

      //! Set the encoding type identifier
      void set_encoding_type_id(const vire::utility::model_identifier &);

      //! Return the encoding type identifier
      const vire::utility::model_identifier & get_encoding_type_id() const;

      bool has_subcontractor(const std::string &) const;

      void add_subcontractor(const std::string &);

      //! Check the resources service
      bool has_resources() const;

      //! Return the resources service
      void set_resources(const vire::resource::manager & resources_);

      //! Return a const handle to the resources service
      const vire::resource::manager & get_resources() const;

      const domain_builder & get_domain_maker() const;

      //! Check the domains
      bool has_domains() const;

      //! Check a domain by name
      bool has_domain(const std::string & domain_name_) const;

      //! Return a domain by name
      const domain & get_domain(const std::string & domain_name_) const;

      //! Return a domain by name
      domain & grab_domain(const std::string & domain_name_);

      //! Create and insert a new plug of given identifier and category
      domain & create_domain(const std::string & domain_name_,
                             const std::string & domain_category_,
                             const std::string & domain_protocol_id_repr_,
                             const std::string & domain_encoding_id_repr_);

      //! Create and insert a new plug of given identifier and category
      domain & create_domain(const std::string & domain_name_,
                             const std::string & domain_category_,
                             const vire::utility::model_identifier & domain_protocol_id_,
                             const vire::utility::model_identifier & domain_encoding_id_);

      //! Remove a domain given its name
      void remove_domain(const std::string & domain_name_);

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Check if the service is initialized
      virtual bool is_initialized() const;

      //! Initialize the service using a list of properties with access to a dictionary of services
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the service
      virtual int reset();

    private:

      //! Set default attribute values
      void _set_defaults_();

      void _at_init_();

      void _at_reset_();

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration:
     const vire::resource::manager * _resources_ = nullptr; //!< Handle to the resources service
      std::string _setup_name_; //!< Name of the setup
      actor       _actor_;      //!< Actor
      vire::utility::model_identifier _transport_type_id_; //!< Transport type identifier associated to the domain
      vire::utility::model_identifier _encoding_type_id_;  //!< Encoding type identifier associated to the domain
      std::set<std::string> _subcontractors_; //!< Set of subcontractors

      domain_builder   _domain_maker_; //!< Domain builder

      // Working data:
      domain_dict_type _domains_;   //!< Dictionary of domains

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(manager);

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_MANAGER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
