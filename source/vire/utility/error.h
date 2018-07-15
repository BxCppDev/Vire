//! \file  vire/utility/error.h
//! \brief Error utilities
//
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_UTILITY_ERROR_H
#define VIRE_UTILITY_ERROR_H

namespace vire {

  namespace utility {

    struct error
    {
      enum code_type {
        CODE_WARNING = -1,
        CODE_SUCCESS =  0,
        CODE_FAILURE =  1
      };
    };
   
  } // namespace utility

} // namespace vire

#endif // VIRE_UTILITY_EXEC_REPORT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
