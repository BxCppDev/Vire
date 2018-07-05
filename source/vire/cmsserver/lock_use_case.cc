//! \file vire/cmsserver/lock_use_case.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/lock_use_case.h>

namespace vire {

  namespace cmsserver {

    VIRE_USE_CASE_REGISTRATION_IMPLEMENT(lock_use_case, "vire::cmsserver::lock_use_case")

    lock_use_case::lock_use_case()
    {
      vire::time::invalidate(_duration_);
      _set_defaults_();
      // _tick_ = boost::posix_time::seconds(1);
      return;
    }
    
    // virtual
    lock_use_case::~lock_use_case()
    {
      return;
    }

    void lock_use_case::_set_defaults_()
    {
      _tick_ = boost::posix_time::time_duration(0, 0, 1); // one second
      return;
    }

    void lock_use_case::set_tick(const boost::posix_time::time_duration & tick_)
    {
      DT_THROW_IF(tick_.is_negative() || tick_.total_microseconds() < 10,
                  std::domain_error,
                  "Invalid tick time duration!");
      _tick_ = tick_;
      return;
    }

    const boost::posix_time::time_duration & lock_use_case::get_tick() const
    {
      return _tick_;
    }

    bool lock_use_case::has_duration() const
    {
      return vire::time::is_valid(_duration_);
    }

    void lock_use_case::set_duration(const boost::posix_time::time_duration & duration_)
    {
      DT_THROW_IF(duration_.is_negative() || duration_.total_microseconds() < 1,
                  std::domain_error,
                  "Invalid duration time duration!");
      _duration_ = duration_;
      return;
    }

    const boost::posix_time::time_duration & lock_use_case::get_duration() const
    {
      return _duration_;
    }

    void lock_use_case::_at_initialize_(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());

      if (!has_duration()) {
        if (config_.has_key("duration")) {
          std::string duration_repr = config_.fetch_string("duration");
          boost::posix_time::time_duration duration;
          DT_THROW_IF(!vire::time::parse_positive_time_duration(duration_repr, duration),
                      std::logic_error,
                      "Invalid time duration format '" << duration_repr << "'!");
          set_duration(duration);
        }
      }
      
      // DT_THROW_IF(!vire::time::is_valid(_duration_),
      //             std::logic_error,
      //             "Missing duration!");
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
    
    void lock_use_case::_at_finalize_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    // virtual
    std::shared_ptr<uc_time_constraints> lock_use_case::_build_time_constraints_() const
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      std::shared_ptr<uc_time_constraints> utc_p;
      if (has_duration()) {
        utc_p.reset(new uc_time_constraints);
        utc_p->add_constraint(running::RUN_STAGE_FUNCTIONAL_WORK_RUNNING,
                              vire::time::duration_interval(_duration_, _duration_));
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return utc_p;
    }

    void lock_use_case::_at_run_prepare_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      _run_start_time_ = vire::time::now_utc();
      _run_stop_time_ = _run_start_time_ + _duration_;
      DT_LOG_TRACE(get_logging_priority(), "Run start time = " << _run_start_time_);
      DT_LOG_TRACE(get_logging_priority(), "Run stop time  = " << _run_stop_time_);
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    running::run_work_loop_status_type
    lock_use_case::_at_run_work_loop_iteration_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      running::run_work_loop_status_type ret = running::RUN_WORK_LOOP_STOP;
      
      // Note: Algorithm to be reviewed and use std::this_thread::sleep_until

      boost::posix_time::ptime now = vire::time::now_utc();
      DT_LOG_TRACE(get_logging_priority(), "Now = " << now);
      // Compute remaining time:
      boost::posix_time::time_duration sleep_amount = _run_stop_time_ - now;
      DT_LOG_TRACE(get_logging_priority(), "Remaining time " << sleep_amount << " us");
      if (sleep_amount > _tick_) {
        sleep_amount = _tick_;
      }
      unsigned int usecs = sleep_amount.total_microseconds();
      // Loop sleep duration
      std::chrono::duration<int,std::ratio<1,1000000>> sleep_time
        = std::chrono::microseconds(usecs);
      if (now < _run_stop_time_) {
        DT_LOG_TRACE(get_logging_priority(), "Sleep for " << sleep_time.count() << " us");
        std::this_thread::sleep_for(sleep_time);
        ret = running::RUN_WORK_LOOP_CONTINUE;
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return ret;
    }

    // virtual
    void lock_use_case::print_tree(std::ostream & out_,
                                   const boost::property_tree::ptree & options_) const
    {
      {
        boost::property_tree::ptree popts
          = i_tree_dumpable::base_print_options::force_inheritance(options_);
        this->base_use_case::print_tree(out_, popts);
      }
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Duration : ";
      if (has_duration()) {
        out_ << vire::time::to_string(_duration_);
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      
      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Tick : "
           << _tick_
           << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Run start time : "
           << vire::time::to_string(_run_start_time_)
           << std::endl;
 
      out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
           << "Run stop time : "
           << vire::time::to_string(_run_stop_time_)
           << std::endl;
     
      return;
    }
  
  } // namespace cmsserver

} // namespace vire
