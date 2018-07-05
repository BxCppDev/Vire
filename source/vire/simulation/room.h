//! \file  vire/simulation/room.h
//! \brief Simulated room
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

#ifndef VIRE_SIMULATION_ROOM_H
#define VIRE_SIMULATION_ROOM_H

// This project:
#include <vire/simulation/base_object.h>
#include <vire/simulation/i_measurable_temperature.h>
#include <vire/simulation/i_measurable_pressure.h>
#include <vire/simulation/i_measurable_humidity.h>

namespace vire {

  namespace simulation {

    /// Simulated base object
    class room
      : public base_object
      , public i_measurable_temperature
      , public i_measurable_pressure
      , public i_measurable_humidity
    {
    public:
      
      room();

      virtual ~room();

      bool has_floor_surface() const;

      void set_floor_surface(const double surface_);

      double get_floor_surface() const;

      bool has_volume() const;

      void set_volume(const double volume_);

      double get_volume() const;
 
      bool has_temperature() const;

      void set_temperature(const double temperature_);

      double get_temperature() const override;

      bool has_pressure() const;

      void set_pressure(const double pressure_);

      double get_pressure() const override;

      bool has_humidity() const;

      void set_humidity(const double humidity_);

      double get_humidity() const override;
     
      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;
     
    private:

      double _floor_surface_;
      double _volume_;
      double _temperature_;
      double _pressure_;
      double _humidity_;
       
    };
    
  } // namespace simulation

} // namespace vire

#endif // VIRE_SIMULATION_ROOM_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
