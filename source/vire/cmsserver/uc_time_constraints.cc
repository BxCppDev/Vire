//! \file vire/cmsserver/uc_time_constraints.cc
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
#include <vire/cmsserver/uc_time_constraints.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

// This project:
#include <vire/time/utils.h>
#include <vire/cmsserver/utils.h>

namespace vire {

  namespace cmsserver {

    // static
    const std::set<running::run_stage_type> & uc_time_constraints::supported_stages()
    {
      static std::set<running::run_stage_type> _stages;
      if (_stages.size() == 0) {
        _stages.insert(running::RUN_STAGE_SYSTEM_PREPARING);
        _stages.insert(running::RUN_STAGE_FUNCTIONAL_UP_RUNNING);
        _stages.insert(running::RUN_STAGE_FUNCTIONAL_WORK_RUNNING);
        _stages.insert(running::RUN_STAGE_FUNCTIONAL_DOWN_RUNNING);
        _stages.insert(running::RUN_STAGE_SYSTEM_TERMINATING);
      }
      return _stages;
    }

    void uc_time_constraints::reset()
    {
      _constraints_.clear();
      return;
    }
    
    uc_time_constraints::uc_time_constraints()
    {
      reset();
      return;
    }

    uc_time_constraints::~uc_time_constraints()
    {
      return;
    }
    
    bool uc_time_constraints::has_constraint(const running::run_stage_type stage_) const
    {
      return _constraints_.count(stage_);
    }

    void uc_time_constraints::add_constraint(const running::run_stage_type stage_,
                                             const time::duration_interval & interval_)
    {
      if (supported_stages().count(stage_) == 0 ) {
        DT_THROW(std::logic_error, "Use case time constraints are not applicable to running stage '"
                 << running::run_stage_label(stage_) << "!");
      }
      _constraints_[stage_] = interval_;
      return;
    }
    
    void uc_time_constraints::remove_constraint(const running::run_stage_type stage_)
    {
      _constraints_.erase(stage_);
      return;
    }
    
    const time::duration_interval &
    uc_time_constraints::get_constraint(const running::run_stage_type stage_) const
    {
      time_constraint_dict_type::const_iterator found
        = _constraints_.find(stage_);
      DT_THROW_IF(found == _constraints_.end(), std::logic_error,
                  "No time constraints is associated to running stage '"
                  << running::run_stage_label(stage_) << "!");
      return found->second;
    }

    void uc_time_constraints::print_tree(std::ostream & out_,
                                         const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }
     
      {
        running::run_stage_type stage = running::RUN_STAGE_SYSTEM_PREPARING;
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "System preparation duration interval : ";
        if (!has_constraint(stage)) {
          out_ << "<none>";
        }
        out_ << std::endl;
        if (has_constraint(stage)) {
          get_constraint(stage).tree_dump(out_, "", popts.indent + tags::skip_item());
        }
      }
    
      {
        running::run_stage_type stage = running::RUN_STAGE_SYSTEM_TERMINATING;
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "System termination duration interval : ";
        if (!has_constraint(stage)) {
          out_ << "<none>";
        }
        out_ << std::endl;
        if (has_constraint(stage)) {
          get_constraint(stage).tree_dump(out_, "", popts.indent + tags::skip_item());
        }
      }
    
      {
        running::run_stage_type stage = running::RUN_STAGE_FUNCTIONAL_UP_RUNNING;
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Functional up duration interval : ";
        if (!has_constraint(stage)) {
          out_ << "<none>";
        }
        out_ << std::endl;
        if (has_constraint(stage)) {
          get_constraint(stage).tree_dump(out_, "", popts.indent + tags::skip_item());
        }
      }
    
      {
        running::run_stage_type stage = running::RUN_STAGE_FUNCTIONAL_DOWN_RUNNING;
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Functional down duration interval : ";
        if (!has_constraint(stage)) {
          out_ << "<none>";
        }
        out_ << std::endl;
        if (has_constraint(stage)) {
          get_constraint(stage).tree_dump(out_, "", popts.indent + tags::skip_item());
        }
      }
     
      {
        running::run_stage_type stage = running::RUN_STAGE_FUNCTIONAL_WORK_RUNNING;
        out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
             << "Functional work duration interval : ";
        if (!has_constraint(stage)) {
          out_ << "<none>";
        }
        out_ << std::endl;
        if (has_constraint(stage)) {
          get_constraint(stage).tree_dump(out_, "", popts.indent + tags::last_skip_item());
        }
      }
      
      return;
    }
    
  } // namespace cmsserver

} // namespace vire
