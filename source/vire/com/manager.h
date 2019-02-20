//! \file  vire/com/manager.h
//! \brief Vire com manager
//
// Copyright (c) 2016-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2016-2019 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/com/utils.h>
#include <vire/com/domain_builder.h>
#include <vire/cms/application.h>

namespace vire {

  namespace resource {
    class manager;
  }

  namespace com {

    class domain;
    class access_profile;

    /// \brief Communication manager service
    class manager
      : public ::datatools::base_service
    {
    public:

      /// Dictionary of shared pointers on domain objects
      typedef std::map<std::string, std::shared_ptr<domain>> domain_dict_type;

      /// Dictionary of shared pointers on access profile objects
      typedef std::map<std::string, std::shared_ptr<access_profile>> access_profile_dict_type;

      /// \brief Access profile information record
      struct access_profile_info_type
      {
        access_category_type category = ACCESS_CATEGORY_INVALID; ///< Access category
        std::string name; 
        std::string password; 
      };
      typedef std::map<std::string, access_profile_info_type> access_profile_info_dict_type;
      
      /// \brief Domain information record
      struct domain_info_type
      {
        domain_category_type            category = DOMAIN_CATEGORY_INVALID; ///< Domain category
        std::string                     topic; ///< Domain topic (unused?)
        vire::utility::model_identifier transport_driver_type_id; ///< Transport type ID for the domain
        vire::utility::model_identifier encoding_driver_type_id;  ///< Encoding type ID for the domain
      };
      typedef std::map<std::string, domain_info_type> domain_info_dict_type;

      /// Default name of the Vire communication service
      static const std::string & default_service_name();
    
      /// Default constructor
      manager(uint32_t flags_ = 0);

      /// Destructor
      virtual ~manager();

      /// Check if the resources service name is set
      bool has_resources_service_name() const;

      /// Set the resources service name is set
      void set_resources_service_name(const std::string &);

      /// Return the resources service name is set      
      const std::string & get_resources_service_name() const;

      /// Check if the system access login prefix is set
      bool has_system_access_login_prefix() const;
      
      /// Set the system access name prefix 
      void set_system_access_login_prefix(const std::string &);
      
      /// Return the system access login prefix
      const std::string & get_system_access_login_prefix() const;   

      /// Check if the domain name prefix is set
      bool has_domain_name_prefix() const;
      
      /// Set the domain name prefix 
      void set_domain_name_prefix(const std::string &);
      
      /// Return the domain name prefix
      const std::string & get_domain_name_prefix() const;   

      /// Check the default transport type identifier
      bool has_default_transport_driver_type_id() const;

      /// Set the default transport type identifier
      void set_default_transport_driver_type_id(const vire::utility::model_identifier &);

      /// Return the default transport type identifier
      const vire::utility::model_identifier & get_default_transport_driver_type_id() const;

      /// Check the default encoding type identifier
      bool has_default_encoding_driver_type_id() const;

      /// Set the default encoding type identifier
      void set_default_encoding_driver_type_id(const vire::utility::model_identifier &);

      /// Return the default encoding type identifier
      const vire::utility::model_identifier & get_default_encoding_driver_type_id() const;

      /// Check the resources service handle is set
      bool has_resources() const;

      /// Set the resources service handle
      void set_resources(const vire::resource::manager & resources_);

      /// Return a const handle to the resources service
      const vire::resource::manager & get_resources() const;

      /// Reset the resources service handle
      void reset_resources();

      /// Retrurn the domain builder
      const domain_builder & get_domain_maker() const;

      /// Check the access profiles
      bool has_access_profiles() const;

      /// Build the list of access profile names
      void build_access_profile_names(std::set<std::string> & names_) const;
      
      /// Check an access profile by name
      bool has_access_profile(const std::string & access_profile_name_) const;

      /// Create and insert a new access profile of given name and category
      access_profile & create_access_profile(const std::string & access_profile_name_,
                                             const std::string & access_profile_password_,
                                             const access_category_type access_category_,
                                             const std::string & target_id_ = "");
      
      /// Return a access profile by name
      const access_profile & get_access_profile(const std::string & access_profile_name_) const;

      /// Return a access profile by name
      access_profile & grab_access_profile(const std::string & access_profile_name_);

      /// Remove an access profile of given name
      void remove_access_profile(const std::string & access_profile_name_);
      
      /// Check the domains
      bool has_domains() const;

      /// Build the set of domain names
      void build_domain_names(std::set<std::string> & names_) const;

      /// Check a domain by name
      bool has_domain(const std::string & domain_name_) const;

      /// Return a domain by name
      const domain & get_domain(const std::string & domain_name_) const;

      /// Return a domain by name
      domain & grab_domain(const std::string & domain_name_);

      /// Create and insert a new domain of given name and category
      domain & create_domain(const std::string & domain_name_,
                             const domain_category_type domain_category_,
                             const vire::utility::model_identifier & domain_transport_driver_type_id_,
                             const vire::utility::model_identifier & domain_encoding_driver_type_id_);

      /// Remove a domain given its name
      void remove_domain(const std::string & domain_name_);

      /// Create all transport resources associated to a subcontractor
      ///
      /// \code
      /// +--------+          +-------------------+        +-------+   
      /// | Server |          |    Monitoring     |        |  S/C  | 
      /// +---o----+          |      domain       |<-.     +---o---+   
      ///     |               +-------------------+   \        |
      /// +---o--------+                               \ +-----o-----+
      /// | S/C system |      +-------------------+     \| S/C user  |
      /// |   profile  |----->|    S/C system     |<-----|  profile  |
      /// +------------+      |      domain       |      +-----------+
      ///                     +-------------------+      
      /// \endcode
      ///
      void create_sc_transport(const subcontractor_info & sc_info_);
      
      /// Remove all transport resources associated to a subcontractor
      void remove_sc_transport(const subcontractor_info & sc_info_);

      bool has_app_category() const;
      
      const vire::cms::application::category_type get_app_category() const;

      void set_app_category(const vire::cms::application::category_type);

      bool has_transport_management() const;

      /// Check if the service is initialized
      virtual bool is_initialized() const;

      /// Initialize the service using a list of properties with access to a dictionary of services
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      /// Reset the service
      virtual int reset();
   
      /// Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_ = empty_options()) const override;
  
    private:

      /// Build default domains addressed by the communication manager for a specific application category
      void _build_default_access_profiles_();

      /// Build default domains addressed by the communication manager for a specific application category
      void _build_default_domains_();

      /// Set default attribute values
      void _set_defaults_();

      void _at_init_(const datatools::properties & config_);

      void _at_init_transport_managers_(const datatools::properties & config_);

      void _at_reset_transport_managers_();

      void _at_reset_();

    private:

      // Management:
      bool _initialized_ = false; ///< Initialization flag

      // Configuration:
      vire::cms::application::category_type _app_category_ = vire::cms::application::CATEGORY_UNDEF; ///< Application category (server, client, subcontractor)
      bool                            _transport_management_ = false;     ///< Transport management activation (for server category only)
      std::string                     _resources_service_name_;           ///< Name of the resources management service
      std::string                     _system_access_login_prefix_;        ///< Prefix of system access logins (for server category only)
      std::string                     _domain_name_prefix_;               ///< Prefix of domain names
      vire::utility::model_identifier _default_transport_driver_type_id_; ///< Default transport type identifier associated to the domains
      vire::utility::model_identifier _default_encoding_driver_type_id_;  ///< Default encoding type identifier associated to the domains
      
      access_profile_info_dict_type   _access_profile_infos_; ///< Access profile informations
      domain_builder                  _domain_maker_; ///< Domain builder
      domain_info_dict_type           _domain_infos_; ///< Domain informations
      
      // Working data:
      const vire::resource::manager * _resources_ = nullptr; ///< Handle to the resources service
      domain_dict_type                _domains_;             ///< Dictionary of domains
      access_profile_dict_type        _access_profiles_;     ///< Dictionary of access profiles
      
      struct pimpl_type;
      std::unique_ptr<pimpl_type>     _pimpl_; ///< Private working data implementation
      
      /// Auto-registration of this service class in a central service database of Bayeux/datatools
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
