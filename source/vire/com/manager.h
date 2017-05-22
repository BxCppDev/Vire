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
#include <memory>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/bit_mask.h>
#include <bayeux/datatools/base_service.h>

// // This project:
// #include <vire/com/base_plug.h>
#include <vire/utility/base_event.h>
#include <vire/utility/base_request.h>
#include <vire/utility/base_response.h>
#include <vire/com/actor.h>
#include <vire/com/mailbox.h>
#include <vire/com/utils.h>

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

      //! Check the actor
      bool has_actor() const;

      //! Set the actor
      void set_actor(const actor &);

      //! Set the actor
      void reset_actor();

      //! Return the actor
      const actor & get_actor() const;

      //! Check the resources service
      bool has_resources() const;

      //! Return the resources service
      void set_resources(const vire::resource::manager & resources_);

      //! Return a const handle to the resources service
      const vire::resource::manager & get_resources() const;

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

      //! Remove a domain given its name
      void remove_domain(const std::string & domain_name_);

      //! Check a mailbox by name
      bool has_mailbox(const std::string & domain_name_,
                       const std::string & mailbox_name_) const;

      //! Create a new private event mailbox in a given domain
      std::string create_private_event_mailbox(const std::string & domain_name_);

      //! Create a new private service mailbox in a given domain
      std::string create_private_service_mailbox(const std::string & domain_name_);

      //! Create a new private event mailbox from a given domain
      void remove_private_event_mailbox(const std::string & domain_name_,
                                        const std::string & mailbox_name_);

      //! Create a new private service rpc mailbox from a given domain
      void remove_private_service_mailbox(const std::string & domain_name_,
                                          const std::string & mailbox_name_);

      // //! Subscribe to an existing public service mailbox in a given domain
      // //!
      // std::string subscribe_to_service_mailbox(const std::string & domain_name_,
      //                                          const std::string & mailbox_name_
      //                                         const std::string & topic_,
      //                                         const std::string & );

      // //! Subscribe to an existing public event mailbox in a given domain
      // //!
      // std::string subscribe_to_event_mailbox(const std::string & domain_name_,
      //                                        const std::string & mailbox_name_);

      //! Return a mailbox by name
      const mailbox & get_mailbox(const std::string & domain_name_,
                                  const std::string & mailbox_name_) const;

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

      //! Notify an event payload to a given domain/mailbox
      com_status notify_event(const std::string & domain_name_,
                              const std::string & mailbox_name_,
                              const utility::base_event & event_);

      //! Perform a request-response (Remote Procedure Call) addressed to a given domain/mailbox
      com_status request_response(const std::string & domain_name_,
                                  const std::string & mailbox_name_,
                                  const utility::base_request & request_,
                                  utility::base_response & response_,
                                  const std::string & mailbox_async_name_ = "");

    private:

      //! Set default attribute values
      void _set_defaults_();

      void _at_init_();

      void _at_reset_();

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration:
      actor       _actor_; //!< Actor
      const vire::resource::manager * _resources_ = nullptr; //!< Handle to the resources service

      // Working data:
      domain_dict_type _domains_; //!< Dictionary of domains

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
