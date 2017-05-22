//! \file  vire/device/space.h
//! \brief Space utils
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

#ifndef VIRE_DEVICE_SPACE_H
#define VIRE_DEVICE_SPACE_H

// Standard library:
#include <iostream>

// Third party:
// - Boost:
#include <boost/cstdint.hpp>
// - Bayeux/datatools:
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

namespace vire {

  namespace device {

    //! \brief Instantiation space utilities
    class space
    {
    public:

      //! \brief Dimension of instantiation space
      enum dimension_type {
        DIM_0 = 0, //!< Dimensionless space
        DIM_1 = 1, //!< 1-dimension space
        DIM_2 = 2, //!< 2-dimension space
        DIM_3 = 3, //!< 3-dimension space
        DIM_MAX = DIM_3, //!< Maximum dimension of the instantiation space
        DIM_INVALID = 0xFFFFFFFF //!< Invalid dimension
      };

      //! Check if a dimension is at least 1
      static bool at_least_dimension_1(dimension_type);

      //! Check if a dimension is at least 2
      static bool at_least_dimension_2(dimension_type);

      //! Check if a dimension is at least 3
      static bool at_least_dimension_3(dimension_type);

      //! Invalid slot index
      static const uint32_t COORDINATE_INVALID = 0xFFFFFFFF;

      //! Any slot index
      static const uint32_t COORDINATE_ANY     = 0xFFFFFFFE;

      //! Not applicable slot index
      static const uint32_t COORDINATE_NA      = 0xFFFFFFFD;

      //! Max slot index
      static const uint32_t COORDINATE_MAX     = 0x7FFFFFFF;

      //! Return the symbol
      static std::ostream & coordinate_symbol(std::ostream &, uint32_t);

      //! Check if a coordinate value is special
      static bool coordinate_is_special(uint32_t);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI()
#endif // Q_MOC_RUN

    };

  } // namespace device

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer for the '::vire::device::space' class:
DR_CLASS_INIT(::vire::device::space) // mandatory to access the embedded enum
// Activate reflection layer for the '::vire::device::space::dimension_type' enumeration:
DR_TYPE_INIT(::vire::device::space::dimension_type)
#endif // Q_MOC_RUN

#endif // VIRE_DEVICE_SPACE_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
