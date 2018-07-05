//! \file  vire/simulation/thermometer.h
//! \brief Simulated thermometer
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

#ifndef VIRE_SIMULATION_THERMOMETER_H
#define VIRE_SIMULATION_THERMOMETER_H

// This project:
#include <vire/simulation/base_object.h>
#include <vire/simulation/i_measurable_temperature.h>

namespace vire {

  namespace simulation {

    /// Simulated thermometer
    class thermometer
      : public base_object
    {
    public:
      
      thermometer();

      virtual ~thermometer();

      bool is_built() const override;

      void set_seed(long seed_);

      void set(const double min_, const double max_, const double sigma_); 
      
      void set_measured_object(const i_measurable_temperature &);

      bool is_last_measured_temperature_available() const;

      double get_last_measured_temperature() const;

      void update_measurement();
      
      void reset_measurement();
     
      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;

    private:

      struct random_pack_type;

      random_pack_type & _grab_rp_();
      
    private:
      
      // Configuration:
      long   _seed_;
      double _temperature_min_;
      double _temperature_max_;
      double _temperature_resolution_;
      const i_measurable_temperature * _measured_object_ = nullptr;
        
      // Working data:
      std::unique_ptr<random_pack_type> _rp_;
      double _last_measured_temperature_; ///< Last temperature value read from the measured object (cache)
      
    };
    
  } // namespace simulation

} // namespace vire

#endif // VIRE_SIMULATION_THERMOMETER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
