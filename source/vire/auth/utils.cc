//! \file vire/auth/utils.cc
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/auth/utils.h>

// Third party:
// - Boost:
#include <boost/date_time/gregorian/gregorian.hpp> //no i/o just types

namespace vire {

  namespace auth {

    int32_t today_from_epoch()
    {
      boost::gregorian::date epoch_day(boost::gregorian::from_simple_string("1970-01-01"));
      boost::gregorian::date today_day(boost::gregorian::day_clock::local_day());
      boost::gregorian::days from_epoch = today_day - epoch_day;
      return (int32_t) from_epoch.days();
    }

  } // namespace auth

} // namespace vire
