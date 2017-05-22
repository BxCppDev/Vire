//! \file  vire/com/base_plug.h
//! \brief Vire com base plug
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

#ifndef VIRE_COM_BASE_PLUG_H
#define VIRE_COM_BASE_PLUG_H

// Standard library:
#include <cstdint>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/properties.h>

// This project:
#include <vire/utility/model_identifier.h>
// #include <vire/com/i_protocol_driver.h>
#include <vire/com/i_encoding_driver.h>

namespace vire {

  namespace com {

    class mailbox;

    //! \brief Base communication plug
    class base_plug
      : public datatools::i_tree_dumpable
    {
    public:

      //! Default constructor
      base_plug();

      //! Constructor
      base_plug(const mailbox & mailbox_);

      //! Destructor
      virtual ~base_plug();

      //! Check mailbox
      bool has_mailbox() const;

      //! Set the mailbox handle
      void set_mailbox(const mailbox & mailbox_);

      //! Return the mailbox handle
      const mailbox & get_mailbox() const;

      //! Check identifier
      bool has_name() const;

      //! Set identifier
      void set_name(const std::string & name_);

      //! Get identifier
      const std::string & get_name() const;

      // //! Check domain identifier
      // bool has_domain_name() const;

      // //! Set domain identifier
      // void set_domain_name(const std::string & domain_name_);

      // //! Get domain identifier
      // const std::string & get_domain_name() const;

      // //! Set the configuration parameters for drivers
      // void set_drivers_config(const datatools::properties &);

      // //! Return the configuration parameters for drivers
      // const datatools::properties & get_drivers_config() const;

      // //! Return the number of sent messages
      // std::size_t get_sent_messages_counter() const;

      // //! Return the number of received messages
      // std::size_t get_received_messages_counter() const;

      // //! Reset the messages counter
      // void reset_messages_counters();

      //! Check initialization flag
      bool is_initialized() const;

      //! Check if the encoding driver is set
      bool has_encoding_driver() const;

      //! Return the handle to the embedded encoding driver
      i_encoding_driver & grab_encoding_driver();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;
    protected:

      void _set_initialized(bool);

      // void _increment_sent_messages_counter();

      // void _decrement_sent_messages_counter();

      // void _increment_received_messages_counter();

      // void _decrement_received_messages_counter();

      void _base_init();

      void _base_reset();

    private:

      void _init_encoding_driver_();

      void _reset_encoding_driver_();

    private:

      // Management:
      bool _initialized_ = false; //!< Initialization flag

      // Configuration:
      std::string     _name_;              //!< Identifier
      const mailbox * _mailbox_ = nullptr; //!< Handle to the parent manager
      // const manager * _mgr_ = nullptr; //!< Handle to the parent manager
      // std::string     _domain_name_;   //!< Domain identifier
      datatools::properties _config_; //!< Various configuration parameters

      // Working data:
      // const domain * _dom_ = nullptr;
      std::shared_ptr<i_encoding_driver> _encoding_;
      // std::size_t _sent_messages_counter_     = 0; //!< Sent messages counter
      // std::size_t _received_messages_counter_ = 0; //!< Received messages counter

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_BASE_PLUG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
