//! \file  vire/running/run_control.h
//! \brief Vire process run control utilities
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

#ifndef VIRE_RUNNING_RUN_CONTROL_H
#define VIRE_RUNNING_RUN_CONTROL_H

// Standard Library:
#include <string>
#include <mutex>

// This project:
#include <vire/time/rt_utils.h>
#include <vire/running/utils.h>

namespace vire {

  namespace running {

    /// \brief Run control record
    class run_control
    {
    public:

      static const int INVALID_STAGE_INDEX = -1;

      run_control();

      bool has_label() const;
      
      const std::string & get_label() const;
      
      void set_label(const std::string & label_);

      bool has_status() const;
      
      run_status_type get_status() const;

      void set_status(const run_status_type);
     
      //! Check if the run is ready
      bool is_ready() const;
     
      //! Check if the run is running
      bool is_running() const;

      //! Check if the run is stopped
      bool is_stopped() const;

      //! Check if the run has failed
      bool has_failed() const;

      bool has_stage_index() const;

      int get_stage_index() const;

      void set_stage_index(const int);
      
      void reset_stage_index();

      bool has_stage_label() const;
      
      const std::string & get_stage_label() const;
      
      void set_stage_label(const std::string & label_);
      
      void reset_stage_label();

      void increment_loop_counter();

      std::size_t get_loop_counter() const;

      void set_loop_counter(const std::size_t);

      void request_stop();

      bool is_stop_requested() const;

      bool has_sys_tick() const;

      const vire::time::system_duration & get_sys_tick() const;

      void set_sys_tick(const vire::time::system_duration &);

      bool has_start_time() const;
      
      void set_start_time(const vire::time::system_time_point & start_time_);
  
      const vire::time::system_time_point & get_start_time() const;

      bool has_stop_time() const;
      
      void set_stop_time(const vire::time::system_time_point & stop_time_);
  
      const vire::time::system_time_point & get_stop_time() const;

      /// Compute the run duration
      void compute_uptime(vire::time::system_duration &) const;
      
    private:

      std::string _label_;  ///! User friendly label (optional)
      vire::time::system_duration           _sys_tick_;

      run_status_type _status_ = RUN_STATUS_UNDEF; ///! Current run status
      std::size_t _loop_counter_ =  0; ///! Current loop counter (optional)
      int         _stage_index_  = -1; ///! Current stage index (optional)
      std::string _stage_label_; ///! Current stage label (optional)
      std::mutex  _stop_mutex_;
      bool        _stop_requested_ = false;
      vire::time::system_time_point _start_time_;
      vire::time::system_time_point _stop_time_;
      
    };
    
  } // namespace running

} // namespace vire

#endif // VIRE_RUNNING_RUN_CONTROL_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
