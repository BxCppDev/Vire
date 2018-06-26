//! \file  vire/running/utils.h
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

#ifndef VIRE_RUNNING_UTILS_H
#define VIRE_RUNNING_UTILS_H

// Standard Library:
#include <string>

namespace vire {

  namespace running {

    /// \brief Run status
      enum run_status_type {
        RUN_STATUS_UNDEF    = 0, ///< Undefined run status
        RUN_STATUS_READY    = 1, ///< Ready run status
        RUN_STATUS_STARTING = 2, ///< Starting run status
        RUN_STATUS_RUNNING  = 3, ///< Running run status
        RUN_STATUS_STOPPING = 4, ///< Stopping run status
        RUN_STATUS_STOPPED  = 5, ///< Stopped run status
        RUN_STATUS_FAILED   = 6  ///< Failed run status
      };
    
    /// Return the label associated to a scheduling model
    std::string run_status_to_label(const run_status_type status_);

    /// Return the scheduling model associated to a label
    run_status_type run_status_from_label(const std::string & label_);
 
  } // namespace running

} // namespace vire

#endif // VIRE_RUNNING_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
