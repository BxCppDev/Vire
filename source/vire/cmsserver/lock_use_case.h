//! \file  vire/cmsserver/lock_use_case.h
//! \brief Vire CMS lock use case
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

#ifndef VIRE_CMSSERVER_LOCK_USE_CASE_H
#define VIRE_CMSSERVER_LOCK_USE_CASE_H

// This project:
#include <vire/cmsserver/base_use_case.h>

namespace vire {

  namespace cmsserver {
    
    /// \brief Lock (leaf) use case which reserves a set of functional resources and
    ///        do nothing with them for a period.
    class lock_use_case
      : public base_use_case
    {
    public:
    
      //! Default constructor
      lock_use_case(const uint32_t flags_ = 0);

      //! Destructor
      virtual ~lock_use_case();

      void set_duration(const boost::posix_time::time_duration & duration_);

      const boost::posix_time::time_duration & get_duration() const;

      void set_tick(const boost::posix_time::time_duration & tick_);

      const boost::posix_time::time_duration & get_tick() const;
      
      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_
                              = datatools::i_tree_dumpable::empty_options()) const;
      
    private:

      void _set_defaults_();

      virtual void _at_initialize_(const datatools::properties & config_) override;

      virtual void _at_finalize_() override;

      virtual std::shared_ptr<uc_time_constraints> _build_time_constraints() override;
       
      virtual running::run_preparation_status_type
      _at_run_prepare_() override;

      /**
       *
       *                             duration
       *          :----------------------------------------->
       *   -------[----+----+----+----+----+----+----+----+--[------------> time
       *        start       :--->                           stop
       *                     tick
       *
       */
      virtual running::run_functional_work_loop_status_type
      _at_run_functional_work_loop_iteration_() override;

    private:

      // Configuration:
      boost::posix_time::time_duration _duration_;  //!< Time duration
      boost::posix_time::time_duration _tick_;      //!< Loop tick

      // Working data:
      boost::posix_time::ptime _run_start_time_; //!< Start time (UTC)
      boost::posix_time::ptime _run_stop_time_;  //!< Stop time (UTC)
    
    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_LOCK_USE_CASE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
