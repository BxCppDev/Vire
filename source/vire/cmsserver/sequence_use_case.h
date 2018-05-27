//! \file  vire/cmsserver/sequence_use_case.h
//! \brief Vire CMS sequence use case
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

#ifndef VIRE_CMSSERVER_SEQUENCE_USE_CASE_H
#define VIRE_CMSSERVER_SEQUENCE_USE_CASE_H

// This project:
#include <vire/cmsserver/base_use_case.h>

namespace vire {

  namespace cmsserver {
    
    /// \brief Sequence agregator use case which runs of set of N daughter use cases
    ///        in sequence for a given period.
    ///
    /// Time layout:
    /// \code
    /// 
    ///                        duration
    ///      :--------------------------------------------------->
    /// -----[-------------------------------------------------+--[------------> time
    ///      start                                            min stop
    ///      :                                                 :  :
    ///      +----------------------------------------------------+
    ///      | Sequence                                        :  |
    ///      +----------------------------------------------------+
    ///      |+-------------+ +-----------------++-------------+  |       
    ///      || use case #0 | | use case #1     || use case #2 |  | 
    ///      |+-------------+ +-----------------++-------------+  |
    ///      +----------------------------------------------------+
    /// \encode
    ///
    class sequence_use_case
      : public base_use_case
    {
    public:
    
      //! Default constructor
      sequence_use_case(const uint32_t flags_ = 0);

      //! Destructor
      virtual ~sequence_use_case();
     
      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_
                              = datatools::i_tree_dumpable::empty_options()) const;
    
      void set_duration(const boost::posix_time::time_duration & duration_);

      const boost::posix_time::time_duration & get_duration() const;

    private:

      void _set_defaults_();
 
      virtual void _at_initialize_(const datatools::properties & config_) override;

      virtual void _at_finalize_() override;

      virtual std::shared_ptr<uc_time_constraints> _build_time_constraints() override;
       
      virtual void _at_run_prepare_() override;

      // virtual void _at_run_distributable_up_() override;

      // virtual void _at_run_functional_up_() override;
      
      virtual void _at_run_functional_work_loop_iteration_() override;

      // virtual void _at_run_functional_down_() override;

      // virtual void _at_run_distributable_down_() override;
      
      // virtual void _at_run_terminate_() override;
 
      // Configuration:
      boost::posix_time::time_duration _duration_;  //!< Time duration
      
      // Working data:
 
    };

 
  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_SEQUENCE_USE_CASE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
