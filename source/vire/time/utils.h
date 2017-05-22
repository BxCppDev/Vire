//! \file  vire/time/utils.h
//! \brief Time utilities
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

#ifndef VIRE_TIME_UTILS_H
#define VIRE_TIME_UTILS_H

// Standard library:
#include <string>

// Third party:
// - Boost:
#include <boost/date_time/posix_time/posix_time.hpp>
// - Bayeux/datatools:
#include <datatools/properties.h>

namespace vire {

  namespace time {

    // ----------- time point operations -----------

    //! Return a const reference to a invalid time point singleton
    const boost::posix_time::ptime & invalid_time();

    //! Check if a time point is valid
    bool is_valid(const boost::posix_time::ptime &);

    //! Invalidate a time point
    void invalidate_time(boost::posix_time::ptime &);

    //! Invalidate a time point
    void invalidate(boost::posix_time::ptime &);

    //! Return an ISO representation of a time point
    std::string to_string(const boost::posix_time::ptime &);

    //! Parse a time point
    bool parse_time(const std::string & token_, boost::posix_time::ptime &);

    //! Set a time point at current local time (now)
    void now(boost::posix_time::ptime &);

    //!  Return a time point at current local time (now)
    boost::posix_time::ptime now();

    // ----------- time duration operations -----------

    //! Return a const reference to a invalid time duration
    const boost::posix_time::time_duration & invalid_time_duration();

    //! Check if a time duration is valid
    bool is_valid(const boost::posix_time::time_duration &);

    //! Invalidate a time duration
    void invalidate_time_duration(boost::posix_time::time_duration &);

    //! Invalidate a time duration
    void invalidate(boost::posix_time::time_duration &);

    //! Return an ISO representation of a time duration
    std::string to_string(const boost::posix_time::time_duration &);

    //! Parse a time duration
    bool parse_time_duration(const std::string & token_, boost::posix_time::time_duration &);

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
