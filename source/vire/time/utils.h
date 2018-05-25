//! \file  vire/time/utils.h
//! \brief Time utilities
//
// Copyright (c) 2015-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_TIME_UTILS_H
#define VIRE_TIME_UTILS_H

// Standard library:
#include <string>
#include <chrono>

// Third party:
// - Boost:
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
// - Bayeux/datatools:
#include <datatools/properties.h>

// This project:
#include <vire/utility/comparison.h>

namespace vire {

  namespace time {
  
    // ----------- time point conversions -----------

    /*
    ///! Convenient type alias:
    typedef boost::optional<std::chrono::system_clock::time_point> maybe_std_time_point;
  
    maybe_std_time_point to_std(const boost::posix_time::ptime & from_);

    bool from_std(const std::chrono::system_clock::time_point & from_,
                  boost::posix_time::ptime & to_);
    */
    
    // ----------- date operations -----------

    //! Return an invalid date (presumably UTC) (singleton)
    const boost::gregorian::date & invalid_date();

    //!  Return the current date in UTC time (today)
    boost::gregorian::date date_today_utc();
  
    //!  Return the Epoch date in UTC time
    boost::gregorian::date date_epoch();
    
    // ----------- time point operations -----------
    
    //! Return a const reference to a invalid time point singleton
    const boost::posix_time::ptime & invalid_time();

    //! Check if a time point (presumably UTC) is valid
    bool is_valid(const boost::posix_time::ptime &);

    //! Invalidate a time point
    void invalidate_time(boost::posix_time::ptime &);

    //! Invalidate a time point
    void invalidate(boost::posix_time::ptime &);

    //! Return an ISO representation of a time point (presumably UTC)
    std::string to_string(const boost::posix_time::ptime &);

    //! Parse a time point (presumably UTC)
    bool parse_time(const std::string & token_, boost::posix_time::ptime &);

    //! Set a time point at current time in UTC (now)
    void now_utc(boost::posix_time::ptime &);

    //! Set a time point at current time in UTC (now)
    //! \deprecated
    void now(boost::posix_time::ptime &);

    //! Build a plain time (presumably UTC) from date and time with microsecond resolution
    void make(boost::posix_time::ptime &,
              const unsigned int year_,
              const unsigned int month_,
              const unsigned int day_,
              const unsigned int hours_ = 0,
              const unsigned int minutes_ = 0,
              const unsigned int seconds_ = 0,
              const unsigned int microseconds_ = 0);

    //!  Return a time point at current local time in UTC (now)
    boost::posix_time::ptime now_utc();
  
    //!  Return a time point at current local time in UTC (now)
    //! \deprecated
    boost::posix_time::ptime now();
  
    //!  Return a time point at Epoch in UTC
    boost::posix_time::ptime epoch();

    //! Compare time points
    vire::utility::comparison_result compare(const boost::posix_time::ptime & t1_,
                                             const boost::posix_time::ptime & t2_);
   
    // ----------- time duration operations -----------

    //! Return a const reference to a invalid time duration
    const boost::posix_time::time_duration & invalid_time_duration();
 
    //! Check if a time duration is valid
    bool is_valid(const boost::posix_time::time_duration &);

    //! Invalidate a time duration
    void invalidate_time_duration(boost::posix_time::time_duration &);

    //! Invalidate a time duration
    void invalidate(boost::posix_time::time_duration &);

    //! Return a representation of a time duration
    std::string to_string(const boost::posix_time::time_duration &);

    //! Build a time duration with microsecond resolution
    boost::posix_time::time_duration make_duration(const unsigned int hours_,
                                                   const unsigned int minutes_,
                                                   const unsigned int seconds_,
                                                   const unsigned int microseconds_ = 0);

    //! Parse a positive time duration with microsecond resolution
    //!
    //! Supported formats:
    //! \code
    //! std::string s1 = "01:23:32.000002" // {HH}:{MM}:{SS}.{ffffff}
    //! std::string s2 = "3.24 s"          // {value} {unit}
    //! \endcode
    //!
    //! Note: the following format is (strangely) accepted: 
    //! \code
    //! std::string s = "0:59.000123" // as {HH}:{MM}.{SSSSSS} !!!!
    //! \endcode
    bool parse_positive_time_duration(const std::string & token_, boost::posix_time::time_duration &);

    // ----------- time interval/period operations -----------

    //! Return a const reference to a invalid time interval singleton
    const boost::posix_time::time_period & invalid_time_interval();

    //! Check if a time interval is valid
    bool is_valid(const boost::posix_time::time_period &);

    //! Invalidate a time interval
    void invalidate_time_interval(boost::posix_time::time_period &);

    //! Invalidate a time interval
    void invalidate(boost::posix_time::time_period &);

    //! Parse a time interval
    bool parse_time_interval(const std::string & token_, boost::posix_time::time_period  &);

    //! Return an ISO representation of a time period
    std::string to_string(const boost::posix_time::time_period &);

    //! Initialize a time period from a set of configuration properties
    void initialize(const datatools::properties & config_, boost::posix_time::time_period  &);

  } // namespace session

} // namespace vire

#endif // VIRE_TIME_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
