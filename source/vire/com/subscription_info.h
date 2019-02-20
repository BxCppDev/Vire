//! \file  vire/com/subscription_info.h
//! \brief Vire com subscription info
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

#ifndef VIRE_COM_SUBSCRIPTION_INFO_H
#define VIRE_COM_SUBSCRIPTION_INFO_H

// Standard library:
#include <string>
#include <list>

// This project:
#include <vire/com/addressing.h>

namespace vire {

  namespace com {

    /// \brief Server plug subscription description
    struct subscription_info
    {
      /// Constructor
      subscription_info(const std::string & mailbox_name_,
                        const address & address_);

      /// Check the completeness
      bool is_complete() const;
      
      friend std::ostream & operator<<(std::ostream & out_, const subscription_info &);

      std::string mailbox_name; ///< Name of the target mailbox
      address     addr;         ///< Address of the subscription
      
    };

    typedef std::list<subscription_info> subscription_info_list;
  
  } // namespace com

} // namespace vire

#endif // VIRE_COM_SUBSCRIPTION_INFO_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
