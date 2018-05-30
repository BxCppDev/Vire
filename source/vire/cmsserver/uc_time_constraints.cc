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

namespace vire {

  namespace cmsserver {

    void uc_time_constraints::reset()
    {
      vire::time::invalidate_time_duration(_preparation_duration_min_);
      vire::time::invalidate_time_duration(_preparation_duration_max_);
      vire::time::invalidate_time_duration(_termination_duration_min_);
      vire::time::invalidate_time_duration(_termination_duration_max_);
      vire::time::invalidate_time_duration(_distributable_up_duration_min_);
      vire::time::invalidate_time_duration(_distributable_up_duration_max_);
      vire::time::invalidate_time_duration(_distributable_down_duration_min_);
      vire::time::invalidate_time_duration(_distributable_down_duration_max_);
      vire::time::invalidate_time_duration(_functional_up_duration_min_);
      vire::time::invalidate_time_duration(_functional_up_duration_max_);
      vire::time::invalidate_time_duration(_functional_down_duration_min_);
      vire::time::invalidate_time_duration(_functional_down_duration_max_);
      vire::time::invalidate_time_duration(_functional_work_duration_min_);
      vire::time::invalidate_time_duration(_functional_work_duration_max_);
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

    bool uc_time_constraints::is_valid() const
    {
      if ( vire::time::is_valid(_preparation_duration_min_)
           && vire::time::is_valid(_preparation_duration_max_)) {
        if (_preparation_duration_min_ > _preparation_duration_max_) {
          return false;
        }
      }
      if ( vire::time::is_valid(_termination_duration_min_)
           && vire::time::is_valid(_termination_duration_max_)) {
        if (_termination_duration_min_ > _termination_duration_max_) {
          return false;
        }
      }
      if ( vire::time::is_valid(_functional_work_duration_min_)
           && vire::time::is_valid(_functional_work_duration_max_)) {
        if (_functional_work_duration_min_ > _functional_work_duration_max_) {
          return false;
        }
      }
      if ( vire::time::is_valid(_functional_up_duration_min_)
           && vire::time::is_valid(_functional_up_duration_max_)) {
        if (_functional_up_duration_min_ > _functional_up_duration_max_) {
          return false;
        }
      }
      if ( vire::time::is_valid(_functional_down_duration_min_)
           && vire::time::is_valid(_functional_down_duration_max_)) {
        if (_functional_down_duration_min_ > _functional_down_duration_max_) {
          return false;
        }
      }
      if ( vire::time::is_valid(_distributable_up_duration_min_)
           && vire::time::is_valid(_distributable_up_duration_max_)) {
        if (_distributable_up_duration_min_ > _distributable_up_duration_max_) {
          return false;
        }
      }
      if ( vire::time::is_valid(_distributable_down_duration_min_)
           && vire::time::is_valid(_distributable_down_duration_max_)) {
        if (_distributable_down_duration_min_ > _distributable_down_duration_max_) {
          return false;
        }
      }
      return true;
    }

    /* run preparation */
    
    bool uc_time_constraints::has_preparation_duration_min() const
    {
      return vire::time::is_valid(_preparation_duration_min_);
    }

    void uc_time_constraints::set_preparation_duration_min(const boost::posix_time::time_duration & d_)
    {
      _preparation_duration_min_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_preparation_duration_min() const
    {
      return _preparation_duration_min_;
    }
   
    bool uc_time_constraints::has_preparation_duration_max() const
    {
      return vire::time::is_valid(_preparation_duration_max_);
    }

    void uc_time_constraints::set_preparation_duration_max(const boost::posix_time::time_duration & d_)
    {
      _preparation_duration_max_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_preparation_duration_max() const
    {
      return _preparation_duration_max_;
    }

    /* run termination */  
  
    bool uc_time_constraints::has_termination_duration_min() const
    {
      return vire::time::is_valid(_termination_duration_min_);
    }

    void uc_time_constraints::set_termination_duration_min(const boost::posix_time::time_duration & d_)
    {
      _termination_duration_min_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_termination_duration_min() const
    {
      return _termination_duration_min_;
    }
   
    bool uc_time_constraints::has_termination_duration_max() const
    {
      return vire::time::is_valid(_termination_duration_max_);
    }

    void uc_time_constraints::set_termination_duration_max(const boost::posix_time::time_duration & d_)
    {
      _termination_duration_max_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_termination_duration_max() const
    {
      return _termination_duration_max_;
    }

    /* distributable up */  
  
    
    bool uc_time_constraints::has_distributable_up_duration_min() const
    {
      return vire::time::is_valid(_distributable_up_duration_min_);
    }

    void uc_time_constraints::set_distributable_up_duration_min(const boost::posix_time::time_duration & d_)
    {
      _distributable_up_duration_min_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_distributable_up_duration_min() const
    {
      return _distributable_up_duration_min_;
    }
    
    bool uc_time_constraints::has_distributable_up_duration_max() const
    {
      return vire::time::is_valid(_distributable_up_duration_max_);
    }

    void uc_time_constraints::set_distributable_up_duration_max(const boost::posix_time::time_duration & d_)
    {
      _distributable_up_duration_max_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_distributable_up_duration_max() const
    {
      return _distributable_up_duration_max_;
    }

    /* distributable down */  

    bool uc_time_constraints::has_distributable_down_duration_min() const
    {
      return vire::time::is_valid(_distributable_down_duration_min_);
    }

    void uc_time_constraints::set_distributable_down_duration_min(const boost::posix_time::time_duration & d_)
    {
      _distributable_down_duration_min_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_distributable_down_duration_min() const
    {
      return _distributable_down_duration_min_;
    }

    bool uc_time_constraints::has_distributable_down_duration_max() const
    {
      return vire::time::is_valid(_distributable_down_duration_max_);
    }

    void uc_time_constraints::set_distributable_down_duration_max(const boost::posix_time::time_duration & d_)
    {
      _distributable_down_duration_max_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_distributable_down_duration_max() const
    {
      return _distributable_down_duration_max_;
    }

    /* functional up */  
  
    bool uc_time_constraints::has_functional_up_duration_max() const
    {
      return vire::time::is_valid(_functional_up_duration_max_);
    }

    void uc_time_constraints::set_functional_up_duration_max(const boost::posix_time::time_duration & d_)
    {
      _functional_up_duration_max_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_functional_up_duration_max() const
    {
      return _functional_up_duration_max_;
    }
  
    bool uc_time_constraints::has_functional_up_duration_min() const
    {
      return vire::time::is_valid(_functional_up_duration_min_);
    }

    void uc_time_constraints::set_functional_up_duration_min(const boost::posix_time::time_duration & d_)
    {
      _functional_up_duration_min_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_functional_up_duration_min() const
    {
      return _functional_up_duration_min_;
    }

    /* functional down */  

    bool uc_time_constraints::has_functional_down_duration_min() const
    {
      return vire::time::is_valid(_functional_down_duration_min_);
    }

    void uc_time_constraints::set_functional_down_duration_min(const boost::posix_time::time_duration & d_)
    {
      _functional_down_duration_min_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_functional_down_duration_min() const
    {
      return _functional_down_duration_min_;
    }
 
    bool uc_time_constraints::has_functional_down_duration_max() const
    {
      return vire::time::is_valid(_functional_down_duration_max_);
    }

    void uc_time_constraints::set_functional_down_duration_max(const boost::posix_time::time_duration & d_)
    {
      _functional_down_duration_max_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_functional_down_duration_max() const
    {
      return _functional_down_duration_max_;
    }

    /* functional work */  
 
    bool uc_time_constraints::has_functional_work_duration_min() const
    {
      return vire::time::is_valid(_functional_work_duration_min_);
    }

    void uc_time_constraints::set_functional_work_duration_min(const boost::posix_time::time_duration & d_)
    {
      //DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already intialized!");
      if (has_functional_work_duration_max() && vire::time::is_valid(d_)) {
        DT_THROW_IF(_functional_work_duration_max_ < d_,
                    std::range_error, "Invalid functional work duration range!");
      }
      _functional_work_duration_min_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_functional_work_duration_min() const
    {
      return _functional_work_duration_min_;
    }

    bool uc_time_constraints::has_functional_work_duration_max() const
    {
      return vire::time::is_valid(_functional_work_duration_max_);
    }

    void uc_time_constraints::set_functional_work_duration_max(const boost::posix_time::time_duration & d_)
    {
      // DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already intialized!");
      if (has_functional_work_duration_min() && vire::time::is_valid(d_)) {
        DT_THROW_IF(d_ < _functional_work_duration_min_,
                    std::range_error, "Invalid functional work duration range!");
      }
      _functional_work_duration_max_ = d_;
      return;
    }

    const boost::posix_time::time_duration & uc_time_constraints::get_functional_work_duration_max() const
    {
      return _functional_work_duration_max_;
    }

    /*
    boost::posix_time::time_duration uc_time_constraints::get_total_safe_duration_min() const
    {
      boost::posix_time::time_duration d = boost::posix_time::seconds(0);
      if (has_distributable_up_duration_min()) {
        d += _distributable_up_duration_min_;
      }
      if (has_functional_up_duration_min()) {
        d += _functional_up_duration_min_;
      }
      if (has_functional_work_duration_min()) {
        d += _functional_work_duration_min_;
      }
      if (has_functional_down_duration_min()) {
        d += _functional_down_duration_min_;
      }
      if (has_distributable_down_duration_min()) {
        d += _distributable_down_duration_min_;
      }
      return d;
    }
    */
    /*
    boost::posix_time::time_duration uc_time_constraints::get_total_safe_duration_max() const
    {
      boost::posix_time::time_duration d = boost::posix_time::seconds(0);
      if (has_distributable_up_duration_max()) {
        d += _distributable_up_duration_max_;
      }
      if (has_functional_up_duration_max()) {
        d += _functional_up_duration_max_;
      }
      if (has_functional_work_duration_max()) {
        d += _functional_work_duration_max_;
      }
      if (has_functional_down_duration_max()) {
        d += _functional_down_duration_max_;
      }
      if (has_distributable_down_duration_max()) {
        d += _distributable_down_duration_max_;
      }
      return d;
    }
    */

    /*
    bool uc_time_constraints::run_total_duration_match(const boost::posix_time::time_duration & total_duration_) const
    {
      boost::posix_time::time_duration max_duration;
      boost::posix_time::time_duration min_duration;
      vire::time::invalidate(max_duration);
      vire::time::invalidate(min_duration);
    
      if (has_distributable_up_duration_max()) {
        if (!vire::time::is_valid(max_duration)) {
          max_duration = get_distributable_up_duration_max();
        } else {
          max_duration += get_distributable_up_duration_max();
        }
      }
      
      if (has_distributable_down_duration_max()) {
        if (!vire::time::is_valid(max_duration)) {
          max_duration = get_distributable_down_duration_max();
        } else {
          max_duration += get_distributable_down_duration_max();
        }
      }
          
      if (has_functional_up_duration_max()) {
        if (!vire::time::is_valid(max_duration)) {
          max_duration = get_functional_up_duration_max();
        } else {
          max_duration += get_functional_up_duration_max();
        }
      }
      
      if (has_functional_down_duration_max()) {
        if (!vire::time::is_valid(max_duration)) {
          max_duration = get_functional_down_duration_max();
        } else {
          max_duration += get_functional_down_duration_max();
        }
      }
     
      if (has_functional_work_duration_max()) {
        if (!vire::time::is_valid(max_duration)) {
          max_duration = get_functional_work_duration_max();
        } else {
          max_duration += get_functional_work_duration_max();
        }
      }
      
      if (has_distributable_up_duration_min()) {
        if (!vire::time::is_valid(min_duration)) {
          min_duration = get_distributable_up_duration_min();
        } else {
          min_duration += get_distributable_up_duration_min();
        }
      }
      
      if (has_distributable_down_duration_min()) {
        if (!vire::time::is_valid(min_duration)) {
          min_duration = get_distributable_down_duration_min();
        } else {
          min_duration += get_distributable_down_duration_min();
        }
      }
          
      if (has_functional_up_duration_min()) {
        if (!vire::time::is_valid(min_duration)) {
          min_duration = get_functional_up_duration_min();
        } else {
          min_duration += get_functional_up_duration_min();
        }
      }
      
      if (has_functional_down_duration_min()) {
        if (!vire::time::is_valid(min_duration)) {
          min_duration = get_functional_down_duration_min();
        } else {
          min_duration += get_functional_down_duration_min();
        }
      }
     
      if (has_functional_work_duration_min()) {
        if (!vire::time::is_valid(min_duration)) {
          min_duration = get_functional_work_duration_min();
        } else {
          min_duration += get_functional_work_duration_min();
        }
      }
      
      if (vire::time::is_valid(max_duration) && total_duration_ > max_duration) {
        return false;
      }
     
      if (vire::time::is_valid(min_duration) && total_duration_ < min_duration) {
        return false;
      }
      
      return true;
    }
    */
    
    void uc_time_constraints::print_tree(std::ostream & out_,
                                         const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      if (!popts.title.empty()) {
        out_ << popts.indent << popts.title << std::endl;
      }
     
      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Preparation duration min        : ";
        if (has_preparation_duration_min()) {
          out_ << _preparation_duration_min_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }
       
      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Preparation duration max        : ";
        if (has_preparation_duration_max()) {
          out_ << _preparation_duration_max_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }
      
      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Termination duration min        : ";
        if (has_termination_duration_min()) {
          out_ << _termination_duration_min_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }
       
      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Termination duration max        : ";
        if (has_termination_duration_max()) {
          out_ << _termination_duration_max_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }
    
      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Distributable up duration min   : ";
        if (has_distributable_up_duration_min()) {
          out_ << _distributable_up_duration_min_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }
     
      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Distributable up duration max   : ";
        if (has_distributable_up_duration_max()) {
          out_ << _distributable_up_duration_max_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }

      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Distributable down duration min : ";
        if (has_distributable_down_duration_min()) {
          out_ << _distributable_down_duration_min_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }

      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Distributable down duration max : ";
        if (has_distributable_down_duration_max()) {
          out_ << _distributable_down_duration_max_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }

      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Functional up duration min      : ";
        if (has_functional_up_duration_min()) {
          out_ << _functional_up_duration_min_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }

      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Functional up duration max      : ";
        if (has_functional_up_duration_max()) {
          out_ << _functional_up_duration_max_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }

      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Functional down duration min    : ";
        if (has_functional_down_duration_min()) {
          out_ << _functional_down_duration_min_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }

      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Functional down duration max    : ";
        if (has_functional_down_duration_max()) {
          out_ << _functional_down_duration_max_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }
 
      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Functional work duration min    : ";
        if (has_functional_work_duration_min()) {
          out_ << _functional_work_duration_min_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }

      {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Functional work duration max    : ";
        if (has_functional_work_duration_max()) {
          out_ << _functional_work_duration_max_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }
 
      // {
      //   out_ << popts.indent << datatools::i_tree_dumpable::tag
      //        << "Total safe duration min         : ";
      //   if (is_valid()) {
      //     out_ << get_total_safe_duration_min();
      //   } else {
      //     out_ << "<none>";
      //   }
      //   out_ << std::endl;
      // }
  
      // {
      //   out_ << popts.indent << datatools::i_tree_dumpable::tag
      //        << "Total safe duration max         : ";
      //   if (is_valid()) {
      //     out_ << get_total_safe_duration_max();
      //   } else {
      //     out_ << "<none>";
      //   }
      //   out_ << std::endl;
      // }
 
      // if (! popts.inherit)
      {
        out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
             << "Validity : " << std::boolalpha << is_valid();
        out_ << std::endl;
      }
     
      return;
    }
  } // namespace cmsserver

} // namespace vire
