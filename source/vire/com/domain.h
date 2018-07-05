//! \file  vire/com/domain.h
//! \brief Vire communication domain
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

#ifndef VIRE_COM_DOMAIN_H
#define VIRE_COM_DOMAIN_H

// Standard library:
#include <string>
#include <memory>

// Third party:
// - Boost:
#include <boost/uuid/uuid_generators.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/utility/model_identifier.h>
#include <vire/com/mailbox.h>
#include <vire/com/actor.h>
#include <vire/com/i_encoding_driver.h>
#include <vire/com/i_transport_driver.h>

namespace vire {

  namespace com {

    //! \brief Description of a Vire communication domain
    //!
    //! The communication domain is a virtual scope that confines
    //! some networked transactions within one specific category or tasks.
    //!
    //! The communication domain corresponds to the concept of *virtual host* in RabbitMQ
    class domain
      : public ::datatools::i_tree_dumpable
    {
    public:

      //! \brief Domain categories
      enum category_type {
        CATEGORY_INVALID              = 0, ///< Invalid domain category
        CATEGORY_GATE                 = 1, ///< Gate category
        CATEGORY_CLIENT_SYSTEM        = 2, ///< Client system category
        CATEGORY_SUBCONTRACTOR_SYSTEM = 3, ///< Subcontractor system category
        CATEGORY_CONTROL              = 4, ///< Control category
        CATEGORY_MONITORING           = 5  ///< Monitoring category
      };

      //! \brief Mailbox entry categories
      struct mailbox_entry {
        mailbox mb;
      };

      //! \brief Dictionary of mailbox entries
      typedef std::map<std::string, mailbox_entry> mailbox_dict_type;

      //! Validate a domain name
      //! Domain name syntax is similar to an absolute filesystem path.
      //! This syntax is compatible with virtual host naming scheme in RabbitMQ.
      //! Examples:
      //!  - "/snemo/test/vire/cms/control"
      //!  - "/snemo/test/vire/cms/monitoring"
      //!  - "/snemo/test/vire/cms/subcontractors/system/cmslapp"
      //!  - "/snemo/test/vire/cms/clients/gate"
      //!  - "/snemo/test/vire/cms/clients/system/xgZTZ87e"
      static bool validate_domain_name(const std::string & candidate_);

      //! Return the domain category associated to a label
      static category_type category_from_label(const std::string & cat_repr_);

      //! Return the label associated to a domain category
      static std::string label_from_category(const category_type cat_);

      //! Default constructor
      domain();

      //! Constructor
      domain(const std::string & name_,
             const category_type cat_,
             const vire::utility::model_identifier & transport_id_,
             const vire::utility::model_identifier & encoding_id_);

      //! Constructor
      domain(const std::string & name_,
             const std::string & cat_repr_,
             const std::string & transport_id_repr_,
             const std::string & encoding_id_repr_);

      //! Destructor
      virtual ~domain();

      //! Check if the domain is valid
      bool is_valid() const;

      //! Check if the name is set
      bool has_name() const;

      //! Set the name
      void set_name(const std::string & name_);

      //! Return the name
      const std::string & get_name() const;

      //! Check the domain type
      bool has_category() const;

      //! Set category
      void set_category(const category_type cat_);

      //! Set category
      void set_category(const std::string & cat_repr_);

      //! Return the category
      category_type get_category() const;

      //! Check if the category is subcontractor system
      bool is_subcontractor_system() const;

      //! Check if the category is client system
      bool is_client_system() const;

      //! Check if the category is gate
      bool is_gate() const;

      //! Check if the category is control
      bool is_control() const;

      //! Check if the category is monitoring
      bool is_monitoring() const;

      //! Check the transport type identifier
      bool has_transport_type_id() const;

      //! Set the transport type identifier
      void set_transport_type_id(const vire::utility::model_identifier &);

      //! Set the transport type identifier
      void set_transport_type_id(const std::string &);

      //! Return the transport type identifier
      const vire::utility::model_identifier & get_transport_type_id() const;

      //! Check the encoding type identifier
      bool has_encoding_type_id() const;

      //! Set the encoding type identifier
      void set_encoding_type_id(const vire::utility::model_identifier &);

      //! Set the encoding type identifier
      void set_encoding_type_id(const std::string &);

      //! Return the encoding type identifier
      const vire::utility::model_identifier & get_encoding_type_id() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Invalidate the domain
      void initialize(const datatools::properties & config_);

      //! Invalidate the domain
      void reset();

      //! Add a mailbox
      void add_mailbox(const std::string & name_,
                       const mailbox::mode_type mode_,
                       const mailbox::privacy_type privacy_,
                       const std::string & address_,
                       const mailbox::permissions_type & perms_,
                       const bool domain_ = false);

      //! Add a mailbox
      void add_mailbox(const std::string & name_,
                       const mailbox::mode_type mode_,
                       const mailbox::privacy_type privacy_,
                       const std::string & address_,
                       const std::string & perms_repr_,
                       const bool domain_ = false);

      // //! Add a private mailbox
      // std::string add_private_mailbox(const std::string & owner_id_,
      //                                 const mailbox::mode_type mode_,
      //                                 const mailbox::permissions_type perms_);

      //! Check a mailbox by name
      bool has_mailbox(const std::string & name_) const;

      //! Remove a mailbox
      void remove_mailbox(const std::string & name_);

      //! Return a non mutable reference to a mailbox
      const mailbox & get_mailbox(const std::string & name_) const;

      //! Check usage permission access to a given mailbox by a category of actor
      bool grant_access(const std::string & mailbox_name_,
                        const actor::category_type actor_cat_,
                        const mailbox::usage_permission_flag useperm_) const;

      // //! Generate a new unique name for a private mailbox
      // //! Format is: "__mb.{OWNER_ID}.{service|event}.{RANDOMIZED_UUID}"
      // std::string generate_private_mailbox_name(const std::string & id_,
      //                                           const mailbox::mode_type);

      //! Return a non mutable handle to the encoding driver
      const i_encoding_driver & get_encoding_driver() const;

      const std::shared_ptr<i_encoding_driver> & get_encoding_driver_ptr() const;
      
      const datatools::properties & get_transport_driver_params() const;

      void set_transport_driver_params(const datatools::properties & params_);

      const datatools::properties & get_encoding_driver_params() const;

      void set_encoding_driver_params(const datatools::properties & params_);

      //! Return a non mutable handle to the transport driver
      const i_transport_driver & get_transport_driver() const;

      //! Return a mutable handle to the transport driver
      i_transport_driver & grab_transport_driver();

      const std::shared_ptr<i_transport_driver> & get_transport_driver_ptr() const;

      std::shared_ptr<i_transport_driver> & grab_transport_driver_ptr();
     
    private:

      i_encoding_driver & _encoding_driver_instance_();

      i_transport_driver & _transport_driver_instance_();

    private:

      // Configuration:
      std::string                     _name_;                           //!< Domain unique name
      category_type                   _category_ = CATEGORY_INVALID;    //!< Domain category
      vire::utility::model_identifier _transport_type_id_;              //!< Transport type identifier associated to the domain
      vire::utility::model_identifier _encoding_type_id_;               //!< Encoding type identifier associated to the domain
      mailbox_dict_type               _mailboxes_;                      //!< Dictionary of mailboxes
      boost::uuids::random_generator  _mailbox_uuid_gen_;
      datatools::properties           _transport_driver_params_;
      datatools::properties           _encoding_driver_params_;

      // Working data:
      encoding_driver_ptr  _encoding_driver_;  //!< Encoding driver instance
      transport_driver_ptr _transport_driver_; //!< Transport driver instance
 
    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_DOMAIN_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
