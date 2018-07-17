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

    enum address_category_type {
      ADDR_CATEGORY_INVALID  = 0,
      ADDR_CATEGORY_PROTOCOL = 1,
      ADDR_CATEGORY_DEVICE   = 2,
      ADDR_CATEGORY_RESOURCE = 3
    };

    //! Return the label associated to an actor category
    std::string to_string(const address_category_type);
       
    //! Return the address category type associated to a label
    bool from_string(const std::string &, address_category_type &);

    /// \brief Address description for Vire messenging
    class address
    {
    public:

      address(const address_category_type, const std::string & value_);

      bool is_complete() const;
      
      bool is_protocol() const;

      bool is_device() const;

      bool is_resource() const;

      const std::string & get_value() const;

      bool has_path() const;

      const vire::utility::path & get_path() const;

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
