//! \file vire/running/scheduling.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/running/scheduling.h>

namespace vire {

  namespace running {

    std::string scheduling_to_label(const scheduling_type sched_)
    {
      switch (sched_) {
      case SCHED_PARALLEL : return std::string("parallel");
      case SCHED_SERIAL   : return std::string("serial");
      case SCHED_REPEAT   : return std::string("repeat");
      }
      return std::string("");
    }

    scheduling_type scheduling_from_label(const std::string & label_)
    {
      if (label_ == scheduling_to_label(SCHED_PARALLEL)) return SCHED_PARALLEL;
      if (label_ == scheduling_to_label(SCHED_SERIAL)) return SCHED_SERIAL;
      if (label_ == scheduling_to_label(SCHED_REPEAT)) return SCHED_REPEAT;
      return SCHED_UNDEF;
    }
    
  } // namespace running

} // namespace vire
