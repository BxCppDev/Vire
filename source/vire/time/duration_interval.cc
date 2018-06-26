//! \file vire/time/duration_interval.cc
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

// Ourselves:
#include <vire/time/duration_interval.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/clhep_units.h>

// This project:
#include <vire/time/utils.h>
 
namespace vire {

  namespace time {
 
    // static
    bool duration_interval::validate(const boost::posix_time::time_duration & min_,
                                     const boost::posix_time::time_duration & max_)
    {
      if (vire::time::is_valid(min_) && vire::time::is_valid(max_)) {
        if (min_ > max_) {
          return false;
        }
      }
      return true;      
    }
    
    // static
    duration_interval duration_interval::make_min(const boost::posix_time::time_duration & min_)
    {
      return duration_interval(min_);
    }
    
    // static
    duration_interval duration_interval::make_max(const boost::posix_time::time_duration & max_)
    {
      duration_interval di;
      di.set_max(max_);
      return di;
    }
    
    // static
    duration_interval duration_interval::make_min_max(const boost::posix_time::time_duration & min_,
                                                      const boost::posix_time::time_duration & max_)
    {
      return duration_interval(min_, max_);
    }
   
    duration_interval::duration_interval()
    {
      reset();
      return;
    }
  
    duration_interval::duration_interval(const boost::posix_time::time_duration & min_)
    {
      reset();
      set_min(min_);
      return;
    }
   
    duration_interval::duration_interval(const boost::posix_time::time_duration & min_,
                                         const boost::posix_time::time_duration & max_)
    {
      reset();
      set_min_max(min_, max_);
      return;
    }

    duration_interval::~duration_interval()
    {
      return;
    }

    void duration_interval::reset()
    {
      vire::time::invalidate_time_duration(_min_);
      vire::time::invalidate_time_duration(_max_);
      return;
    }

    bool duration_interval::has_min() const
    {
      return vire::time::is_valid(_min_);
    }

    void duration_interval::set_min(const boost::posix_time::time_duration & d_)
    {
      DT_THROW_IF(!validate(d_, _max_), std::logic_error,
                  "Invalid minimum duration with respect to the value of the maximum duration!");
      _min_ = d_;
      return;
    }

    const boost::posix_time::time_duration & duration_interval::get_min() const
    {
      return _min_;
    }
    
    bool duration_interval::has_max() const
    {
      return vire::time::is_valid(_max_);
    }

    void duration_interval::set_max(const boost::posix_time::time_duration & d_)
    {
      DT_THROW_IF(!validate(_min_, d_), std::logic_error,
                  "Invalid maximum duration with respect to the value of the minimum duration!");
      _max_ = d_;
      return;
    }

    const boost::posix_time::time_duration & duration_interval::get_max() const
    {
      return _max_;
    }
 
    void duration_interval::set_min_max(const boost::posix_time::time_duration & min_,
                                        const boost::posix_time::time_duration & max_)
    {
      DT_THROW_IF(!validate(min_, max_), std::logic_error,
                  "Invalid minium/maximum durations!");
      reset();
      set_min(min_);
      set_max(max_);
      return;
    }

    void duration_interval::set_min_max(const double time_min_, const double time_max_)
    {
      reset();
      double time_unit = CLHEP::microsecond;
       if (datatools::is_valid(time_min_)) {
        DT_THROW_IF(time_max_ <= 0.0, std::range_error, "Invalid negative min duration!");
        std::size_t min_us = static_cast<std::size_t>(time_min_ / time_unit);
        set_min(boost::posix_time::microseconds(min_us));
      }
      if (datatools::is_valid(time_max_)) {
        DT_THROW_IF(time_max_ <= 0.0, std::range_error, "Invalid negative max duration!");
        std::size_t max_us = static_cast<std::size_t>(time_max_ / time_unit);
        set_max(boost::posix_time::microseconds(max_us));
      }
      return;
    }
  
    void duration_interval::print_tree(std::ostream & out_,
                                       const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }
     
      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Duration min : ";
        if (has_min()) {
          out_ << _min_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }
       
      {
        out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
             << "Duration max : ";
        if (has_max()) {
          out_ << _max_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }
      
      return;
    }
      
  } // namespace time

} // namespace vire
