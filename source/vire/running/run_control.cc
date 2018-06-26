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
#include <vire/running/run_control.h>

// Third party:
// - Bayeux:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace running {
    
    run_control::run_control()
    {
      set_sys_tick(std::chrono::milliseconds(1000));
      _start_time_ = vire::time::system_epoch();
      return;
    }

    bool run_control::has_label() const
    {
      return !_label_.empty();
    }
    
    const std::string & run_control::get_label() const
    {
      return _label_;
    }
      
    void run_control::set_label(const std::string & label_)
    {
      _label_ = label_;
      return;
    }

    bool run_control::has_status() const
    {
      return _status_ != RUN_STATUS_UNDEF;
    }
    
    run_status_type run_control::get_status() const
    {
      return _status_;
    }

    bool run_control::is_ready() const
    {
      return _status_ == RUN_STATUS_READY;
    }

    bool run_control::is_running() const
    {
      return _status_ == RUN_STATUS_RUNNING;
    }

    bool run_control::is_stopped() const
    {
      return _status_ == RUN_STATUS_STOPPED;
    }
  
    bool run_control::has_failed() const
    {
      return _status_ == RUN_STATUS_FAILED;
    }
   
    void run_control::set_status(const run_status_type status_)
    {
      _status_ = status_;
      return;
    }

    bool run_control::has_stage_label() const
    {
      return !_stage_label_.empty();
    }
    
    const std::string & run_control::get_stage_label() const
    {
      return _stage_label_;
    }
      
    void run_control::set_stage_label(const std::string & label_)
    {
      _stage_label_ = label_;
      return;
    }
     
    void run_control::reset_stage_label()
    {
      _stage_label_.clear();
      return;
    }

    bool run_control::has_stage_index() const
    {
      return _stage_index_ > INVALID_STAGE_INDEX;
    }

    int run_control::get_stage_index() const
    {
      return _stage_index_;
    }

    void run_control::set_stage_index(const int index_)
    {
      DT_THROW_IF(index_ <= INVALID_STAGE_INDEX,
                  std::logic_error,
                  "Invalid run stage index ['" << index_ << "'!");
      _stage_index_ = index_;
      return;
    }
      
    void run_control::reset_stage_index()
    {
      _stage_index_ = INVALID_STAGE_INDEX;
      return;
    }

    void run_control::increment_loop_counter()
    {
      _loop_counter_++;
      return;
    }

    std::size_t run_control::get_loop_counter() const
    {
      return _loop_counter_;
    }

    void run_control::set_loop_counter(const std::size_t count_)
    {
      _loop_counter_ = count_;
      return;
    }

    void run_control::request_stop()
    {
      std::lock_guard<std::mutex> lock(_stop_mutex_);
      _stop_requested_ = true;
      return;
    }

    bool run_control::is_stop_requested() const
    {
      return _stop_requested_;
    }

    bool run_control::has_sys_tick() const
    {
      return _sys_tick_.count() > 0;
    }
    
    const vire::time::system_duration & run_control::get_sys_tick() const
    {
      return _sys_tick_;
    }

    void run_control::set_sys_tick(const vire::time::system_duration & tick_)
    {
      _sys_tick_ = tick_;
      return;
    }
 
    bool run_control::has_start_time() const
    {
      return _start_time_ == vire::time::system_epoch();
    }
      
    void run_control::set_start_time(const vire::time::system_time_point & start_time_)
    {
      _start_time_ = start_time_;
      return;
    }
  
    const vire::time::system_time_point & run_control::get_start_time() const
    {
      return _start_time_;
    }
 
    bool run_control::has_stop_time() const
    {
      return _stop_time_ == vire::time::system_epoch();
    }
      
    void run_control::set_stop_time(const vire::time::system_time_point & stop_time_)
    {
      _stop_time_ = stop_time_;
      return;
    }
  
    const vire::time::system_time_point & run_control::get_stop_time() const
    {
      return _stop_time_;
    }

    void run_control::compute_uptime(vire::time::system_duration & uptime_) const
    {
      if (has_start_time()) {
        std::chrono::system_clock::time_point last = std::chrono::system_clock::now();
        if (has_stop_time()) {
          last = _stop_time_;
        }
        uptime_ = last - _start_time_;
      }
      uptime_ = std::chrono::milliseconds(-1);
      return;
    }

  } // namespace running

} // namespace vire
