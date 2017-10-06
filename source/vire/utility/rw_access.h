//! \file  vire/utility/rw_access.h
//! \brief Readable/writable access type
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_UTILITY_RW_ACCESS_H
#define VIRE_UTILITY_RW_ACCESS_H

// Standard library:
#include <string>

#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

namespace vire {

  namespace utility {

    //! \brief Read/write access type
    enum rw_access_type {
      RW_INVALID           = -1, //!< Invalid R/W access
      RW_NONE              =  0, //!< No R/W access
      RW_READABLE          =  1, //!< RO access
      RW_WRITABLE          =  2, //!< WO access
      RW_READABLE_WRITABLE =  3  //!< R/W access
    };

    //! Convert an access type to a string
    //! @arg t_ the R/W access value
    //! @return the text representation of the R/W access value
    const std::string & to_string(const rw_access_type t_);

    //! Convert a string to an access type
    //! @arg label_ the input text to be decoded
    //! @arg t_ the R/W access value represented by the input text
    //! @return true in case of successful decoding, false otherwise
    bool from_string(const std::string & label_, rw_access_type & t_);

    //! Check the validity of an access type
    //! @arg t_ the R/W access value
    //! @return true if the R/W access value is valid
    bool is_valid(const rw_access_type t_);

    //! Check if an access type is input
    //! @arg t_ the R/W access value
    //! @return true if the R/W access value is readable
    bool is_readable(const rw_access_type t_);

    //! Check if an access type is output
    //! @arg t_ the R/W access value
    //! @return true if the R/W access value is writable
    bool is_writable(const rw_access_type t_);

  } // namespace utility

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the 'vire::utility::rw_access_type' enum:
DR_TYPE_INIT(::vire::utility::rw_access_type);
#endif // Q_MOC_RUN

#endif // VIRE_UTILITY_RW_ACCESS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
