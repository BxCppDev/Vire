//! \file vire/simulation/thermometer.cc
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
#include <vire/simulation/thermometer.h>

// Standard library:
#include <random>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/clhep_units.h>

namespace vire {

  namespace simulation {

    thermometer::thermometer()
    {
      _seed_ = 0;
      datatools::invalidate(_temperature_min_);
      datatools::invalidate(_temperature_max_);
      datatools::invalidate(_temperature_resolution_);
      datatools::invalidate(_last_measured_temperature_);
      return;
    }

    // virtual
    thermometer::~thermometer()
    {
      return;
    }

    void thermometer::set_seed(long seed_)
    {
      _seed_ = seed_;
      return;
    }
    
    bool thermometer::is_built() const
    {
      if (_measured_object_ == nullptr) return false;
      if (datatools::is_valid(_temperature_min_)) return false; 
      if (datatools::is_valid(_temperature_max_)) return false; 
      if (datatools::is_valid(_temperature_resolution_)) return false;
      return true;
    }

    void thermometer::set(const double min_,
                          const double max_,
                          const double sigma_)
    {
      DT_THROW_IF(min_ >= max_, std::logic_error,
                  "Invalid temperature measurement range!");
      DT_THROW_IF(min_ <= 0.0, std::logic_error,
                  "Invalid temperature measurement range!");
      DT_THROW_IF(sigma_ <= 0.0, std::logic_error,
                  "Invalid temperature measurement range!");
      _temperature_min_ = min_;
      _temperature_max_ = max_;
      _temperature_resolution_ = sigma_;
      return;
    }

    void thermometer::set_measured_object(const i_measurable_temperature & obj_)
    {
      _measured_object_ = &obj_;
      return;
    }
    
    bool thermometer::is_last_measured_temperature_available() const
    {
      return datatools::is_valid(_last_measured_temperature_);
    }

    double thermometer::get_last_measured_temperature() const
    {
      if (!is_last_measured_temperature_available()) {
        thermometer * mutable_this = const_cast<thermometer*>(this);
        mutable_this->update_measurement();
      }
      return _last_measured_temperature_;
    }

    struct thermometer::random_pack_type
    {
      random_pack_type(long seed_, double sigma_)
        : generator(seed_)
        , distribution(0.0, sigma_)
      {
      }
      
      double random_value()
      {
        return distribution(generator);
      }
      
      std::default_random_engine       generator;
      std::normal_distribution<double> distribution;
    };

    thermometer::random_pack_type & thermometer::_grab_rp_()
    {
      if (!_rp_) {
        _rp_.reset(new random_pack_type(_seed_, _temperature_resolution_));  
      }
      return *_rp_;
    }
                        
    void thermometer::reset_measurement()
    {
      datatools::invalidate(_last_measured_temperature_);
      return;
    }

    void thermometer::update_measurement()
    {
      double current_value = _measured_object_->get_temperature();
      if (current_value < _temperature_min_) {
        _last_measured_temperature_ = _temperature_min_;
      } else if (current_value > _temperature_max_) {
        _last_measured_temperature_ = _temperature_max_;
      } else {
        double error = _grab_rp_().random_value();
        // Randomize:
        _last_measured_temperature_ = current_value + error;
      }
      return;
    }

    // virtual
    void thermometer::print_tree(std::ostream & out_,
                          const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      this->base_object::print_tree(out_, base_print_options::force_inheritance(options_));

      out_ <<  popts.indent << tag
           << "Measured object : ";
      if (_measured_object_ != nullptr) {
        const base_object * bo = dynamic_cast<const base_object*>(_measured_object_);
        if (bo) {
          out_ << "'" << bo->get_name() << "'";
        } else {
          out_ << "@" << _measured_object_;
        }
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ <<  popts.indent << tag
           << "Temperature min : ";
      if (datatools::is_valid(_temperature_min_)) {
        out_ << _temperature_min_ / CLHEP::kelvin << " K";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ <<  popts.indent << tag
           << "Temperature max : ";
      if (datatools::is_valid(_temperature_max_)) {
        out_ << _temperature_max_ / CLHEP::kelvin << " K";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ <<  popts.indent << tag
           << "Temperature resolution : ";
      if (datatools::is_valid(_temperature_resolution_)) {
        out_ << _temperature_resolution_ / CLHEP::kelvin << " K";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
 
      out_ <<  popts.indent << tag
           << "Seed : ";
      out_ << _seed_;
      out_ << std::endl;
 
      out_ <<  popts.indent << inherit_tag(popts.inherit)
           << "Last measured temperature : ";
      if (datatools::is_valid(_last_measured_temperature_)) {
        out_ << _last_measured_temperature_ / CLHEP::kelvin << " K";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // namespace simulation

} // namespace vire
