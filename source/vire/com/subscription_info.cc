//! \file vire/com/subscription_info.cc
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

// Standard Library:
#include <sstream>

// Ourselves:
#include <vire/com/subscription_info.h>

namespace vire {

  namespace com {
    
    subscription_info::subscription_info(const std::string & mailbox_name_,
                                         const address & address_)
      : mailbox_name(mailbox_name_)
      , addr(address_)
    {
      return;
    }

    bool subscription_info::is_complete() const
    {
      if (mailbox_name.empty()) return false;
      if (!addr.is_complete()) return false;
      return true;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_, const subscription_info & sinfo_)
    {
      std::ostringstream outs;
      outs << "{mailbox=" << sinfo_.mailbox_name << ";addr=" << sinfo_.addr << '}';
      out_ << outs.str();
      return out_;
    }
    
  } // namespace com

} // namespace vire
