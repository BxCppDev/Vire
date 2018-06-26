//! \file  vire/running/scheduling.h
//! \brief Vire process running utilities
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

#ifndef VIRE_RUNNING_SCHEDULING_H
#define VIRE_RUNNING_SCHEDULING_H

// Standard Library:
#include <string>

namespace vire {

  namespace running {

    /// \brief Scheduling model
    enum scheduling_type {
      SCHED_UNDEF    = 0,
      SCHED_PARALLEL = 1, ///< Daughter use cases run in parallel (default scheduling model)
      SCHED_SERIAL   = 2, ///< Daughter use cases run in sequence 
      SCHED_REPEAT   = 3  ///< One unique use case is repeated
    };

    /// Return the label associated to a scheduling model
    std::string scheduling_to_label(const scheduling_type);

    /// Return the scheduling model associated to a label
    scheduling_type scheduling_from_label(const std::string &);
     
  } // namespace running

} // namespace vire

#endif // VIRE_RUNNING_SCHEDULING_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
