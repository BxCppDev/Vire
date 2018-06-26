//! \file vire/automaton/utils.cc
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
#include <vire/automaton/utils.h>

// Standard Library:
#include <memory>
#include <thread>

namespace vire {

  namespace automaton {

    automation_control::automation_control()
    {
      reset();
      return;
    }

    void automation_control::reset()
    {
      static std::chrono::system_clock::time_point tp_epoch;
      stop_requested.store(false);
      start_time = tp_epoch;
      requested_stop_time = tp_epoch;
      stop_time = tp_epoch;
      loop_start_time = tp_epoch;
      loop_stop_time = tp_epoch;
      loop = 0;
      error_code = 0;
      error_type_id.clear();
      error_message.clear();
      return;
    }

    void automation_control::print(std::ostream & out_) const
    {
      out_ << "Stop requested : " << stop_requested.load() << std::endl;
      out_ << "Start time     : " << start_time.time_since_epoch().count() << std::endl;
      out_ << "Stop time      : " << requested_stop_time.time_since_epoch().count()
           << " (requested)" << std::endl;
      out_ << "Stop time      : " << stop_time.time_since_epoch().count() << std::endl;
      out_ << "Loop #         : " << loop << std::endl;
      out_ << "Error code     : " << error_code << std::endl;
      out_ << "Error type ID  : " << error_type_id << std::endl;
      out_ << "Error message  : '" << error_message << "'" << std::endl;
      return;
    }

  } // namespace automaton

} // namespace vire
