//! \file  vire/cmsserver/uc_time_constraints.h
//! \brief Vire CMS use case time constraints
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

#ifndef VIRE_CMSSERVER_UC_TIME_CONSTRAINTS_H
#define VIRE_CMSSERVER_UC_TIME_CONSTRAINTS_H

// Standard Library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>

// This project:
#include <vire/time/utils.h>
#include <vire/time/duration_interval.h>
#include <vire/cmsserver/running.h>
  
namespace vire {

  namespace cmsserver {

    typedef std::map<running::run_stage_type, time::duration_interval> time_constraint_dict_type;
    
    /// \brief Use case time constraints per running stage
    ///
    /// Supported running stages are:
    /// \code
    /// running::RUN_STAGE_SYSTEM_PREPARING
    /// running::RUN_STAGE_FUNCTIONAL_UP_RUNNING
    /// running::RUN_STAGE_FUNCTIONAL_WORK_RUNNING
    /// running::RUN_STAGE_FUNCTIONAL_DOWN_RUNNING
    /// running::RUN_STAGE_SYSTEM_TERMINATING
    /// \endcode
    class uc_time_constraints
      : public datatools::i_tree_dumpable
    {

    public:

      static const std::set<running::run_stage_type> & supported_stages();
      
      /// Default constructor
      uc_time_constraints();

      /// Destructor
      ~uc_time_constraints();

      //! Reset
      void reset();
      
      bool has_constraint(const running::run_stage_type) const;

      void add_constraint(const running::run_stage_type, const time::duration_interval &);

      void remove_constraint(const running::run_stage_type);

      const time::duration_interval & get_constraint(const running::run_stage_type) const;
      
      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_
                              = datatools::i_tree_dumpable::empty_options()) const;
      
    private:

      time_constraint_dict_type _constraints_; //!< Dictionary of time constraints per running stage
       
    };
    
  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_TIME_CONSTRAINTS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
