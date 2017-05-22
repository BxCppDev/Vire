//! \file  vire/utility/instance_identifier-serial.ipp
///! \brief Instance identifier serialization
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

#ifndef VIRE_UTILITY_INSTANCE_IDENTIFIER_SERIAL_IPP
#define VIRE_UTILITY_INSTANCE_IDENTIFIER_SERIAL_IPP

// Ourselves:
#include <vire/utility/instance_identifier.h>

// Third Party:
// - Boost:
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/base_object.hpp>

namespace vire {
  namespace utility {

    template<class Archive>
    void instance_identifier::serialize(Archive & archive_, const unsigned int /* version_*/)
    {
      archive_ & boost::serialization::make_nvp("vire__utility__base_identifier",
                                                boost::serialization::base_object<vire::utility::base_identifier>(*this));
      return;
    }

  } // end of namespace utility

} // end of namespace vire

#endif // VIRE_UTILITY_INSTANCE_IDENTIFIER_SERIAL_IPP
