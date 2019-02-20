//! \file  vire/com/addressing.h
//! \brief Vire com event emitter plug interface
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_ADDRESSING_H
#define VIRE_COM_ADDRESSING_H

// Standard library:
#include <string>
#include <iostream>

// This project:
#include <vire/utility/path.h>

namespace vire {

  namespace com {

    /// \brief Address category
    enum address_category_type {
      ADDR_CATEGORY_INVALID  = 0, ///< Invalid address category
      ADDR_CATEGORY_PROTOCOL = 1, ///< Address category dedicated to 'protocol' messenging through a public mailbox
      ADDR_CATEGORY_DEVICE   = 2, ///< Address category dedicated to 'device' addressing through a public mailbox
      ADDR_CATEGORY_RESOURCE = 3, ///< Address category dedicated to 'resource' addressing through a public mailbox
      ADDR_CATEGORY_PRIVATE  = 4  ///< Address category describing a 'private' mailbox
    };

    //! Return the label associated to an address category
    std::string to_string(const address_category_type);
       
    //! Return the address category type associated to a label
    bool from_string(const std::string &, address_category_type &);

    /// \brief Address description for Vire messenging
    class address
    {
    public:

      /// Constructor
      address();

      /// Constructor
      address(const address_category_type category_, const std::string & value_);

      /// Set category and value
      void set(const address_category_type category_, const std::string & value_);

      /// Check if address is complete
      bool is_complete() const;

      /// Check if address is a private address
      bool is_private() const;
      
      /// Check if address is a protocol address
      bool is_protocol() const;

      /// Check if address is a device address
      bool is_device() const;

      /// Check if address is a resource address
      bool is_resource() const;

      /// Return the address value
      const std::string & get_value() const;

      /// Check if path is defined (device/resource only)
      bool has_path() const;

      /// Return the address path  (device/resource only)
      const vire::utility::path & get_path() const;

      /// Print
      friend std::ostream & operator<<(std::ostream & out_, const address &);
      
    private:
      
      address_category_type _category_ = ADDR_CATEGORY_INVALID; ///< The category of the address
      std::string           _value_;       ///< The value of the address
      vire::utility::path   _cached_path_; ///< The device/resource path associated to the value
      
    };
    
  } // namespace com

} // namespace vire

#endif // VIRE_COM_ADDRESSING_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
