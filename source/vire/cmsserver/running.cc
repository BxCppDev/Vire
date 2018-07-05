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
        case RUN_STAGE_SYSTEM_PREPARING: return std::string("system-preparing");
        case RUN_STAGE_SYSTEM_PREPARED: return std::string("system-prepared");
        case RUN_STAGE_FUNCTIONAL_UP_RUNNING: return std::string("functional-up-running");
        case RUN_STAGE_FUNCTIONAL_UP_DONE: return std::string("functional-up-done");
        case RUN_STAGE_FUNCTIONAL_WORK_RUNNING: return std::string("functional-work-running");
        case RUN_STAGE_FUNCTIONAL_WORK_DONE: return std::string("functional-work-done");
        case RUN_STAGE_FUNCTIONAL_DOWN_RUNNING: return std::string("functional-down-running");
        case RUN_STAGE_FUNCTIONAL_DOWN_DONE: return std::string("functional-down-done");
        case RUN_STAGE_SYSTEM_TERMINATING: return std::string("system-terminating");
        case RUN_STAGE_SYSTEM_TERMINATED: return std::string("system-terminated");
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

      std::string run_depth_label(const run_depth_type depth_)
      {
        switch (depth_) {
        case RUN_DEPTH_NONE: return std::string("none");
        case RUN_DEPTH_SYSTEM: return std::string("system");
        case RUN_DEPTH_AUTO: return std::string("auto");
        case RUN_DEPTH_WORK: return std::string("work");
        }
      }

      run_stage_time_statistics::run_stage_time_statistics()
        : loop_counter(0)
        , start_stop(vire::time::invalid_time_interval())
        , last_run_start_stop(vire::time::invalid_time_interval())
      {
        return;
      }
      
      run_control::run_control(const run_depth_type depth_)
      {
        _run_depth_ = depth_;
        _run_stop_requested_.store(false);
        _run_work_loop_counter_.store(0);
        return;
      }

      run_depth_type run_control::get_run_depth() const
      {
        return _run_depth_;
      }

      run_stage_type run_control::get_run_stage() const
      {
        return _run_stage_;
      }

      void run_control::set_run_stage(const run_stage_type stage_)
      {
        _run_stage_ = stage_;
        return;
      }

      bool run_control::is_run_stage(const run_stage_type stage_) const
      {
        return _run_stage_ == stage_;
      }
      
      std::size_t run_control::get_run_work_loop_counter() const
      {
        return _run_work_loop_counter_.load();
      }

      void run_control::increment_run_work_loop_counter()
      {
        _run_work_loop_counter_++;
        return;
      }
 
      void run_control::run_stop_request()
      {
        _run_stop_requested_.store(true);
        return;
      }
      
      bool run_control::check_run_stop_requested() const
      {
        return _run_stop_requested_.load();
      }
      
    } // namespace running

  } // namespace cmsserver

} // namespace vire

/*
    // void base_use_case::_run_work_init_()
    // {
    //   return;
    // }

    // void base_use_case::_run_work_begin_()
    // {
    //   // run_report_record * record = nullptr;
    //   // run_report_record_dict_type::iterator found = _run_reports_.find(_stage_);
    //   // if (found == _run_reports_.end()) {
    //   //   run_report_record new_record = run_report_record;
    //   //   new_record.start_stop = boost::posix_time::time_period(vire::time::now_utc(),
    //   //                                                                 vire::time::invalid_time());
    //   //   _run_reports_[_stage_] = new_record;
    //   // } else {
    //   //   record = &found->second;
    //   // }
    //   return;
    // }

    // void base_use_case::_run_work_end_()
    // {
    //   return;
    // }

    // void base_use_case::_run_work_terminate_()
    // {
    //   return;
    // }
*/
