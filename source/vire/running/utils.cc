//! \file vire/running/utils.cc
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
#include <vire/running/utils.h>

namespace vire {

  namespace running {

    std::string run_status_to_label(const run_status_type status_)
    {
      switch (status_) {
      case RUN_STATUS_READY : return std::string("ready");
      case RUN_STATUS_STARTING  : return std::string("starting");
      case RUN_STATUS_RUNNING   : return std::string("running");
      case RUN_STATUS_STOPPING  : return std::string("stopping");
      case RUN_STATUS_STOPPED  : return std::string("stopped");
      case RUN_STATUS_FAILED  : return std::string("failed");
      }
      return std::string("");
    }

    run_status_type run_status_from_label(const std::string & label_)
    {
      if (label_ == run_status_to_label(RUN_STATUS_READY)) return RUN_STATUS_READY;
      if (label_ == run_status_to_label(RUN_STATUS_STARTING)) return RUN_STATUS_STARTING;
      if (label_ == run_status_to_label(RUN_STATUS_RUNNING)) return RUN_STATUS_RUNNING;
      if (label_ == run_status_to_label(RUN_STATUS_STOPPING)) return RUN_STATUS_STOPPING;
      if (label_ == run_status_to_label(RUN_STATUS_STOPPED)) return RUN_STATUS_STOPPED;
      if (label_ == run_status_to_label(RUN_STATUS_FAILED)) return RUN_STATUS_FAILED;
      return RUN_STATUS_UNDEF;
    }
    
  } // namespace running

} // namespace vire
