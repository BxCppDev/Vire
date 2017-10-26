//! \file  vire/com/utils.h
//! \brief Vire com utilities
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_COM_UTILS_H
#define VIRE_COM_UTILS_H

// Standard library:
#include <vector>

// Third party:
#include <bayeux/datatools/properties.h>

namespace vire {

  namespace com {

    enum com_status {
      COM_OK          = 0, //!< Success
      COM_FAILURE     = 1, //!< Generic failure
      COM_UNAVAILABLE = 2, //!< Unavailable resource error
      COM_PERMISSION  = 3, //!< Permission error
      COM_TIMEOUT     = 4  //!< Timeout error
    };

    struct raw_message_type
    {
      typedef std::vector<char>     buffer_type;

      //! Clear the raw message internals
      void reset();

      datatools::properties metadata; //!< List of metadata (key/value pairs)
      buffer_type           buffer;   //!< Encoded message buffer

    };

  } // namespace com

} // namespace vire

#endif // VIRE_COM_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
