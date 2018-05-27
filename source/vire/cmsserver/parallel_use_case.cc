//! \file vire/cmsserver/parallel_use_case.cc
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
#include <vire/cmsserver/parallel_use_case.h>

namespace vire {

  namespace cmsserver {

    parallel_use_case::parallel_use_case(const uint32_t flags_)
      : base_use_case(flags_)
    {
      vire::time::invalidate(_duration_);
      return;
    }
    
    // virtual
    parallel_use_case::~parallel_use_case()
    {
      return;
    }
    
    void parallel_use_case::_set_defaults_()
    {
      return;
    }

    void parallel_use_case::set_duration(const boost::posix_time::time_duration & duration_)
    {
      DT_THROW_IF(duration_.is_negative() || duration_.total_microseconds() < 1,
                  std::domain_error,
                  "Invalid duration time duration!");
      _duration_ = duration_;
      return;
    }

    const boost::posix_time::time_duration & parallel_use_case::get_duration() const
    {
      return _duration_;
    }
                     
    
    void parallel_use_case::_at_initialize_(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());

      //
      

      
      if (config_.has_key("duration")) {
        std::string duration_repr = config_.fetch_string("duration");
        // XXX
      }

      // Autocompute duration:
      
      DT_THROW_IF(!vire::time::is_valid(_duration_),
                  std::logic_error,
                  "Missing duration!");
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
    
    void parallel_use_case::_at_finalize_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      vire::time::invalidate(_duration_);
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    // virtual
    std::shared_ptr<uc_time_constraints> parallel_use_case::_build_time_constraints()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      std::shared_ptr<uc_time_constraints> utc_p;
      utc_p.reset(new uc_time_constraints);

      // Scan daughter use case:
      // utc_p->set_functional_work_duration_max(_duration_);

      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return utc_p;
    }

    void parallel_use_case::_at_run_prepare_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());

      // _run_start_time_ = vire::time::now_utc();
      // _run_stop_time_ = _run_start_time_ + _duration_;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void parallel_use_case::_at_run_functional_work_loop_iteration_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    // virtual
    void parallel_use_case::print_tree(std::ostream & out_,
                                       const boost::property_tree::ptree & options_) const
    {
      {
        boost::property_tree::ptree popts = i_tree_dumpable::base_print_options::force_inheritance(options_);
        this->base_use_case::print_tree(out_, popts);
      }
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Duration : "
           << vire::time::to_string(_duration_)
           << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
           << "End. "
           << std::endl;
             
      return;
    }

    
  } // namespace cmsserver

} // namespace vire
