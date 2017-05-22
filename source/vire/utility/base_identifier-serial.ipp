//! \file  vire/utility/base_identifier-serial.ipp
//! \brief Base identifier body serialization
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

#ifndef VIRE_UTILITY_BASE_IDENTIFIER_SERIAL_IPP
#define VIRE_UTILITY_BASE_IDENTIFIER_SERIAL_IPP

// Ourselves:
#include <vire/utility/base_identifier.h>

// Third Party:
// - Boost:
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/optional.hpp>
// - Bayeux/datatools:
// #include <bayeux/datatools/version_id.h>

namespace vire {
  namespace utility {

    template<class Archive>
    void base_identifier::serialize (Archive & archive_,
                                  const unsigned int /* version_*/)
    {
      // archive_ & DATATOOLS_SERIALIZATION_I_SERIALIZABLE_BASE_OBJECT_NVP;
      archive_ & boost::serialization::make_nvp("name",    _name_);
      archive_ & boost::serialization::make_nvp("version", _version_);
      return;
    }

  } // end of namespace utility

} // end of namespace vire

#endif // VIRE_UTILITY_BASE_IDENTIFIER_SERIAL_IPP
