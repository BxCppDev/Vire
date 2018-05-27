//! \file vire/cmsserver/running.cc
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
#include <vire/cmsserver/running.h>

// This project:
#include <vire/time/utils.h>

namespace vire {

  namespace cmsserver {

    namespace running {
    
      std::string run_stage_label(const run_stage_type stage_)
      {
        switch (stage_) {
        case RUN_STAGE_UNDEF: return std::string("undefined");
        case RUN_STAGE_READY: return std::string("ready");
        case RUN_STAGE_PREPARING: return std::string("preparing");
        case RUN_STAGE_PREPARED: return std::string("prepared");
        case RUN_STAGE_DISTRIBUTABLE_UP_RUNNING: return std::string("distributable-up-running");
        case RUN_STAGE_DISTRIBUTABLE_UP_DONE: return std::string("distributable-up-done");
        case RUN_STAGE_FUNCTIONAL_UP_RUNNING: return std::string("functional-up-running");
        case RUN_STAGE_FUNCTIONAL_UP_DONE: return std::string("functional-up-done");
        case RUN_STAGE_FUNCTIONAL_WORK_RUNNING: return std::string("functional-work-running");
        case RUN_STAGE_FUNCTIONAL_WORK_DONE: return std::string("functional-work-done");
        case RUN_STAGE_FUNCTIONAL_DOWN_RUNNING: return std::string("functional-down-running");
        case RUN_STAGE_FUNCTIONAL_DOWN_DONE: return std::string("functional-down-done");
        case RUN_STAGE_DISTRIBUTABLE_DOWN_RUNNING: return std::string("distributable-down-running");
        case RUN_STAGE_DISTRIBUTABLE_DOWN_DONE: return std::string("distributable-down-done");
        case RUN_STAGE_TERMINATING: return std::string("terminating");
        case RUN_STAGE_TERMINATED: return std::string("terminated");
        default:
          return std::string("no-run"); 
        }
      }

      run_stage_completion::run_stage_completion() 
      {
        vire::time::invalidate(timestamp);
        run_termination = RUN_TERMINATION_UNDEF;
        run_stage       = RUN_STAGE_UNDEF;
        return;
      }

      bool run_stage_completion::is_terminated() const
      {
        return run_termination != RUN_TERMINATION_UNDEF;
      }
 
      bool run_stage_completion::is_error() const
      {
        return this->run_termination == RUN_TERMINATION_ERROR;
      }

      bool run_stage_completion::is_normal_termination() const
      {
        return this->run_termination == RUN_TERMINATION_NORMAL;
      }

      run_stage_time_statistics::run_stage_time_statistics()
        : loop_counter(0)
        , start_stop(vire::time::invalid_time_interval())
        , last_run_start_stop(vire::time::invalid_time_interval())
      {
        return;
      }
      
      run_control::run_control()
      {
        // Nothing special
        return;
      }
      
      bool run_control::can_run() const
      {
        return run_stage != RUN_STAGE_NA;
      }
 
      void run_control::run_stop_request()
      {
        std::lock_guard<std::mutex> lck(_run_stop_request_mutex_);
        _run_stop_requested_ = true;
        return;
      }
      
      bool run_control::check_run_stop_requested() const
      {
        run_control * mutable_this = const_cast<run_control *>(this);
        std::lock_guard<std::mutex> lck(mutable_this->_run_stop_request_mutex_);
        return _run_stop_requested_;
      }
      
    } // namespace running

  } // namespace cmsserver

} // namespace vire
