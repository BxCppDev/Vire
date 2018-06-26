//! \file  vire/automaton/utils.h
//! \brief Automaton utilities
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

#ifndef VIRE_AUTOMATON_UTILS_H
#define VIRE_AUTOMATON_UTILS_H

// Standard Library:
#include <string>
#include <atomic>
#include <chrono>
#include <iostream>

// This project:
#include <vire/time/rt_utils.h>

namespace vire {

  namespace automaton {
     
    struct automation_control
    {
      /// Constructor
      automation_control();

      /// Reset
      void reset();

      //! Print
      void print(std::ostream & out_ = std::clog) const;
     
      std::atomic<bool> stop_requested;
      vire::time::system_time_point start_time;
      vire::time::system_time_point requested_stop_time;
      vire::time::system_time_point stop_time;
      vire::time::system_time_point loop_start_time;
      vire::time::system_time_point loop_stop_time;
      std::size_t loop = 0;
      int         error_code = 0;
      std::string error_type_id;
      std::string error_message;
    };

  } // namespace automaton

} // namespace vire

#endif // VIRE_AUTOMATON_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
