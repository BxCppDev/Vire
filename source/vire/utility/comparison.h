//! \file  vire/utility/comparison.h
//! \brief Comparison utilities
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_UTILITY_COMPARISON_H
#define VIRE_UTILITY_COMPARISON_H

// #ifndef Q_MOC_RUN
// #include <datatools/reflection_interface.h>
// #endif // Q_MOC_RUN

namespace vire {

  namespace utility {

    //! \brief comparison result
    enum comparison_result {
      COMPARISON_NOT_APPLICABLE = -2,
      COMPARISON_LESS           = -1,
      COMPARISON_EQUAL          =  0,
      COMPARISON_GREATER        =  1
    };

  } // namespace utility

} // namespace vire

// #ifndef Q_MOC_RUN
// // Activate reflection layer for the 'vire::utility::comparison_result' enum:
// DR_TYPE_INIT(::vire::utility::comparison_result);
// #endif // Q_MOC_RUN

#endif // VIRE_UTILITY_COMPARISON_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
