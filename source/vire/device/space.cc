// vire/device/space.cc
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

// Ourselves:
#include <vire/device/space.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>

namespace vire {

  namespace device {

    // static
    const uint32_t space::COORDINATE_INVALID;
    const uint32_t space::COORDINATE_ANY;
    const uint32_t space::COORDINATE_NA;
    const uint32_t space::COORDINATE_MAX;

    // static
    bool space::at_least_dimension_1(dimension_type dim_)
    {
      switch(dim_) {
      case DIM_1 : return true;
      case DIM_2 : return true;
      case DIM_3 : return true;
      default: return false;
      }
    }

    // static
    bool space::at_least_dimension_2(dimension_type dim_)
    {
      switch(dim_) {
      case DIM_2 : return true;
      case DIM_3 : return true;
      default: return false;
      }
    }

    // static
    bool space::at_least_dimension_3(dimension_type dim_)
    {
      switch(dim_) {
      case DIM_3 : return true;
      default: return false;
      }
    }

    // static
    std::ostream & space::coordinate_symbol(std::ostream & out_, uint32_t value_)
    {
      if (value_ == COORDINATE_INVALID) {
        out_ << "!";
      } else if (value_ == COORDINATE_ANY) {
        out_ << "*";
      } else if (value_ == COORDINATE_NA) {
        out_ << "-";
      } else {
        out_ << value_;
      }
      return out_;
    }

    // static
    bool space::coordinate_is_special(uint32_t value_)
    {
      if (value_ == COORDINATE_INVALID || value_ == COORDINATE_ANY || value_ == COORDINATE_NA) {
        return true;
      }
      return false;
    }

  } // namespace device

} // namespace vire
