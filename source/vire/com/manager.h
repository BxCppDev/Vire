//! \file  vire/com/manager.h
//! \brief Vire com manager
//
// Copyright (c) 2016-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2016-2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

// This project:
#include <vire/com/actor.h>
#include <vire/com/utils.h>
#include <vire/com/domain_builder.h>
#include <vire/cms/application.h>

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

      //! Dictionary of shared pointers on actor objects
      typedef std::map<std::string, std::shared_ptr<actor>> actor_dict_type;

      static const std::string & default_service_name();

      //! Default constructor
      manager(uint32_t flags_ = 0);

      //! Destructor
      virtual ~manager();

      bool has_resource_service_name() const;

      void set_resource_service_name(const std::string &);
      
      const std::string & get_resource_service_name() const;

      //! Set the domain name prefix 
      void set_domain_name_prefix(const std::string &);
      
      //! Return the domain name prefix
      const std::string & get_domain_name_prefix() const;   

      //! Check the default transport type identifier
      bool has_default_transport_type_id() const;

      //! Set the default transport type identifier
      void set_default_transport_type_id(const vire::utility::model_identifier &);

      //! Return the default transport type identifier
      const vire::utility::model_identifier & get_default_transport_type_id() const;

      //! Check the default encoding type identifier
      bool has_default_encoding_type_id() const;

      //! Set the default encoding type identifier
      void set_default_encoding_type_id(const vire::utility::model_identifier &);

      //! Return the default encoding type identifier
      const vire::utility::model_identifier & get_default_encoding_type_id() const;

      //! Check the resources service handle is set
      bool has_resources() const;

      //! Set the resources service handle
      void set_resources(const vire::resource::manager & resources_);

      //! Return a const handle to the resources service
      const vire::resource::manager & get_resources() const;

      //! Reset the resources service handle
      void reset_resources();

      const domain_builder & get_domain_maker() const;

      //! Check the actors
      bool has_actors() const;

      void build_actor_names(std::set<std::string> & names_) const;
      
      //! Check a actors by name
      bool has_actor(const std::string & actor_name_) const;

      //! Create and insert a new actor of given name and category
      void create_actor(const std::string & actor_name_,
                        const std::string & actor_password_,
                        const actor::category_type & actor_category_,
                        const std::string & target_id_ = "");

      //! Return a actor by name
      const actor & get_actor(const std::string & actor_name_) const;

      //! Return a actor by name
      actor & grab_actor(const std::string & actor_name_);

      //! Remove an actor of given name
      void remove_actor(const std::string & actor_name_);
      
      //! Check the domains
      bool has_domains() const;

      void build_domain_names(std::set<std::string> & names_) const;

      //! Check a domain by name
      bool has_domain(const std::string & domain_name_) const;

      //! Return a domain by name
      const domain & get_domain(const std::string & domain_name_) const;

      //! Return a domain by name
      domain & grab_domain(const std::string & domain_name_);

      //! Create and insert a new domain of given name and category
      domain & create_domain(const std::string & domain_name_,
                             const domain::category_type & domain_category_,
                             const vire::utility::model_identifier & domain_protocol_id_,
                             const vire::utility::model_identifier & domain_encoding_id_);

      //! Remove a domain given its name
      void remove_domain(const std::string & domain_name_);

      bool has_app_category() const;
      
      const vire::cms::application::category_type get_app_category() const;

      void set_app_category(const vire::cms::application::category_type);

      bool has_transport_management() const;
      
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

      void _build_default_domains_();

      //! Set default attribute values
      void _set_defaults_();

      void _at_init_(const datatools::properties & config_);

      void _at_init_transport_managers_(const datatools::properties & config_);

      void _at_reset_transport_managers_();

      void _at_reset_();

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration:
      vire::cms::application::category_type _app_category_ = vire::cms::application::CATEGORY_UNDEF;
      bool                            _transport_management_ = false;
      std::string                     _resource_service_name_;     //!< Name of the resource management service
      std::string                     _domain_name_prefix_;        //!< Prefix of domain names
      vire::utility::model_identifier _default_transport_type_id_; //!< Default transport type identifier associated to the domains
      vire::utility::model_identifier _default_encoding_type_id_;  //!< Default encoding type identifier associated to the domains
      
      domain_builder                  _domain_maker_; //!< Domain builder

      // Working data:
      const vire::resource::manager * _resources_ = nullptr; //!< Handle to the resources service
      domain_dict_type                _domains_;             //!< Dictionary of domains
      actor_dict_type                 _actors_;              //!< Dictionary of actors
      
      struct pimpl_type;
      std::unique_ptr<pimpl_type>     _pimpl_;
      
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
