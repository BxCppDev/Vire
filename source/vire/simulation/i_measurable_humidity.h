//! \file  vire/simulation/i_measurable_humidity.h
//! \brief Interface for object with a measurable humidity
//
// Copyright (c) 2018 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_SIMULATION_I_MEASURABLE_HUMIDITY_H
#define VIRE_SIMULATION_I_MEASURABLE_HUMIDITY_H

namespace vire {

  namespace simulation {

    struct i_measurable_humidity
    {
      virtual ~i_measurable_humidity() {}
      
      virtual double get_humidity() const = 0;
      
    };
    
  } // namespace simulation

} // namespace vire

#endif // VIRE_SIMULATION_I_MEASURABLE_HUMIDITY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
