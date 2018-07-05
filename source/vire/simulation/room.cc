//! \file vire/simulation/room.cc
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

// Ourselves:
#include <vire/simulation/room.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/clhep_units.h>

namespace vire {

  namespace simulation {

    room::room()
    {
      datatools::invalidate(_floor_surface_);
      datatools::invalidate(_volume_);
      datatools::invalidate(_temperature_);
      datatools::invalidate(_pressure_);
      datatools::invalidate(_humidity_);
      return;
    }

    // virtual
    room::~room()
    {
      return;
    }

    bool room::has_floor_surface() const
    {
      return datatools::is_valid(_floor_surface_);
    }

    void room::set_floor_surface(const double surface_)
    {
      _floor_surface_ = surface_;
      return;
    }

    double room::get_floor_surface() const
    {
      return _floor_surface_;
    }

    bool room::has_volume() const
    {
      return datatools::is_valid(_volume_);
    }

    void room::set_volume(const double volume_)
    {
      _volume_ = volume_;
      return;
    }

    double room::get_volume() const
    {
      return _volume_;
    }
 
    bool room::has_temperature() const
    {
      return datatools::is_valid(_temperature_);
    }

    void room::set_temperature(const double temperature_)
    {
      _temperature_ = temperature_;
      return;
    }

    double room::get_temperature() const
    {
      return _temperature_;
    }

    bool room::has_pressure() const
    {
      return datatools::is_valid(_pressure_);
    }

    void room::set_pressure(const double pressure_)
    {
      _pressure_ = pressure_;
      return;
    }

    double room::get_pressure() const
    {
      return _pressure_;
    }
 
    bool room::has_humidity() const
    {
      return datatools::is_valid(_humidity_);
    }

    void room::set_humidity(const double humidity_)
    {
      _humidity_ = humidity_;
      return;
    }

    double room::get_humidity() const
    {
      return _humidity_;
    }
  
    // virtual
    void room::print_tree(std::ostream & out_,
                          const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      this->base_object::print_tree(out_, base_print_options::force_inheritance(options_));

      out_ <<  popts.indent << tag
           << "Floor surface : ";
      if (has_floor_surface()) {
        out_ << get_floor_surface() / CLHEP::m2 << " m2";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ <<  popts.indent << tag
           << "Volume        : ";
      if (has_volume()) {
        out_ << get_volume() / CLHEP::m3 << " m3";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ <<  popts.indent << tag
           << "Temperature   : ";
      if (has_temperature()) {
        out_ << get_temperature() / CLHEP::kelvin << " K";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ <<  popts.indent << tag
           << "Pressure      : ";
      if (has_pressure()) {
        out_ << get_pressure() / CLHEP::bar << " bar";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ <<  popts.indent << inherit_tag(popts.inherit)
           << "Humidity      : ";
      if (has_humidity()) {
        out_ << get_humidity() / CLHEP::perCent << " %";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // namespace simulation

} // namespace vire
