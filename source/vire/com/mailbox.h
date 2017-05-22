//! \file  vire/com/mailbox.h
//! \brief Vire communication mailbox
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_MAILBOX_H
#define VIRE_COM_MAILBOX_H

// Standard library:
#include <string>
#include <bitset>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/bit_mask.h>
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/com/actor.h>

namespace vire {

  namespace com {

    //! \brief Mailbox utility
    class mailbox
      : public ::datatools::i_tree_dumpable
    {
    public:

      //! \brief Mailbox privacy
      enum privacy_type {
        PRIVACY_INVALID = 0,
        //! Public mailbox, accessible by many actors (corresponds to a RabbitMQ's exchange)
        PRIVACY_PUBLIC  = 1,
        //! Private mailbox, generally accessible by only one actor (corresponds to a RabbitMQ's exclusive queue)
        PRIVACY_PRIVATE = 2
      };

      //! Return the label associated to a privacy type
      static std::string privacy_label(const privacy_type);

      //! Return the privacy type associated to a label
      static privacy_type privacy(const std::string &);

      //! \brief Mailbox mode (service or event)
      enum mode_type {
        MODE_INVALID = 0,
        //!< RPC like functionality with query/response transaction between a client and a server
        MODE_SERVICE = 1,
        //!< Asynchronous event publishing functionality
        MODE_EVENT   = 2
      };

      //! Return the label associated to a mode
      static std::string mode_label(const mode_type);

      //! Return the mode associated to a label
      static mode_type mode(const std::string &);

      //! Usage permission associated to mailbox
      enum usage_permission_flag {
        USAGE_PERMISSION_NULL             = 0x0,
        USAGE_PERMISSION_PUT              = datatools::bit_mask::bit00,
        USAGE_PERMISSION_PRIVATE_PICKUP   = datatools::bit_mask::bit01,
        USAGE_PERMISSION_PUBLIC_SUBSCRIBE = datatools::bit_mask::bit02
      };

      //! Type alias for permissions bitset
      typedef std::bitset<9> permissions_type;

      //! Return the label associated to a permission flag
      static std::string usage_permission_label(const usage_permission_flag);

      //! Return the permission flag associated to a label
      static usage_permission_flag usage_permission(const std::string &);

      //! Return the symbol associated to an usage permission flag
      static char usage_permission_symbol(const usage_permission_flag);

      //! Decode permissions from a string
      static bool usage_permission_from_string(const std::string &, permissions_type &);

      //! Return permissions from a string
      static permissions_type usage_permission_from_string(const std::string &);

      //! Return the string representation of a permissions
      static std::string usage_permission_to_string(const permissions_type &);

      //! Default constructor
      mailbox();

      //! Constructor
      mailbox(const std::string & name_,
              const mode_type,
              const privacy_type,
              const std::string & address_,
              const permissions_type & permissions_);

      //! Constructor
      mailbox(const std::string & name_,
              const mode_type,
              const privacy_type,
              const std::string & address_,
              const std::string & perms_repr_);

      //! Destructor
      virtual ~mailbox();

      //! Check validity
      bool is_valid() const;

      //! Initialize
      void initialize(const datatools::properties & config_);

      //! Reset
      void reset();

      bool has_name() const;

      void set_name(const std::string &);

      const std::string & get_name() const;

      bool has_address() const;

      void set_address(const std::string &);

      const std::string & get_address() const;

      bool has_mode() const;

      void set_mode(const mode_type &);

      mode_type get_mode() const;

      bool is_event() const;

      bool is_service() const;

      bool has_privacy() const;

      void set_privacy(const privacy_type &);

      privacy_type get_privacy() const;

      bool is_public() const;

      bool is_private() const;

      void set_permissions(const permissions_type &);

      void set_permissions(const std::string &);

      const permissions_type & get_permissions() const;

      bool check_permissions(const actor &,
                             const usage_permission_flag) const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Check lock flag
      bool is_locked() const;

      //! Check domain flag
      bool is_domain() const;

      //! Lock the mailbox's configuration
      void lock(bool domain_);

    private:

      // Management:
      bool _locked_ = false;  //!< Lock flag
      bool _domain_ = false;  //!< Domain flag (indicate a domain system mailbox that cannot be removed by any *users*)

      // Configuration:
      std::string  _name_;    //!< Mailbox unique name
      std::string  _address_; //!< Address of the mailbox
      mode_type    _mode_;    //!< Mode
      privacy_type _privacy_; //!< Privacy
      permissions_type _permissions_; //!< Actor permissions

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_MAILBOX_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
