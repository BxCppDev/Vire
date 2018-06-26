//! \file  vire/time/rt_utils.h
//! \brief Real-time (RT) time utilities
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

#ifndef VIRE_TIME_RT_UTILS_H
#define VIRE_TIME_RT_UTILS_H

// Standard library:
#include <chrono>

// Third party:
// - Boost:
#include <boost/optional.hpp>

namespace vire {

  namespace time {

    typedef std::chrono::system_clock::duration   system_duration;
    
    typedef std::chrono::system_clock::time_point system_time_point;
    
    typedef boost::optional<system_time_point> maybe_system_time_point;

    const system_time_point & system_epoch();
    
    const system_duration & system_latency();

    double to_real_with_unit(const system_duration & d_);

    
  } // namespace session

} // namespace vire

#endif // VIRE_TIME_RT_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
