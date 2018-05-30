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
  
namespace vire {

  namespace cmsserver {

    /// \brief Use case time constraints
    ///
    ///     preparation                                                                      termination
    ///   --[----[---------------------------------------------------------------------------[------[----> time
    ///
    ///
    ///             distributable                                              distributable
    ///             up                                                         down
    ///   ----------[-------------[--------------------------------------------[----------[--------------> time
    ///
    ///                             functional                             functional
    ///                             up                                     down
    ///   --------------------------[--------[-----------------------------[---[-------------------------> time
    ///
    ///                                         functional                                       
    ///                                         work                                         
    ///   --------------------------------------[------------------------[-------------------------------> time
    ///                                
    ///
    class uc_time_constraints
      : public datatools::i_tree_dumpable
    {

    public:

      /// Default constructor
      uc_time_constraints();

      /// Destructor
      ~uc_time_constraints();

      //! Reset
      void reset();
 
      //! Check validity
      bool is_valid() const;

      //! Check if the minimum duration for run preparation action is set
      bool has_preparation_duration_min() const;

      //! Set the minimum duration for run preparation action is set
      void set_preparation_duration_min(const boost::posix_time::time_duration &);

      //! Return the minimum duration for run preparation action is set
      const boost::posix_time::time_duration & get_preparation_duration_min() const;

      //! Check if the maximum duration for run preparation action is set
      bool has_preparation_duration_max() const;

      //! Set the maximum duration for run preparation action is set
      void set_preparation_duration_max(const boost::posix_time::time_duration &);

      //! Return the maximum duration for run preparation action is set
      const boost::posix_time::time_duration & get_preparation_duration_max() const;

      //! Check if the minimum duration for run termination action is set
      bool has_termination_duration_min() const;

      //! Set the minimum duration for run termination action is set
      void set_termination_duration_min(const boost::posix_time::time_duration &);

      //! Return the minimum duration for run termination action is set
      const boost::posix_time::time_duration & get_termination_duration_min() const;

      //! Check if the maximum duration for run termination action is set
      bool has_termination_duration_max() const;

      //! Set the maximum duration for run termination action is set
      void set_termination_duration_max(const boost::posix_time::time_duration &);

      //! Return the maximum duration for run termination action is set
      const boost::posix_time::time_duration & get_termination_duration_max() const;

      //! Check if the minimum duration for distributable up action is set
      bool has_distributable_up_duration_min() const;

      //! Set the minimum duration for distributable up action is set
      void set_distributable_up_duration_min(const boost::posix_time::time_duration &);

      //! Return the minimum duration for distributable up action is set
      const boost::posix_time::time_duration & get_distributable_up_duration_min() const;

      //! Check if the maximum duration for distributable up action is set
      bool has_distributable_up_duration_max() const;

      //! Set the maximum duration for distributable up action is set
      void set_distributable_up_duration_max(const boost::posix_time::time_duration &);

      //! Return the maximum duration for distributable up action is set
      const boost::posix_time::time_duration & get_distributable_up_duration_max() const;

      //! Check if the minimum duration for distributable down action is set
      bool has_distributable_down_duration_min() const;

      //! Set the minimum duration for distributable down action is set
      void set_distributable_down_duration_min(const boost::posix_time::time_duration &);

      //! Return the minimum duration for distributable down action is set
      const boost::posix_time::time_duration & get_distributable_down_duration_min() const;

      //! Check if the maximum duration for distributable down action is set
      bool has_distributable_down_duration_max() const;

      //! Set the maximum duration for distributable down action is set
      void set_distributable_down_duration_max(const boost::posix_time::time_duration &);

      //! Return the maximum duration for distributable down action is set
      const boost::posix_time::time_duration & get_distributable_down_duration_max() const;

      //! Check if the minimum duration for functional up action is set
      bool has_functional_up_duration_min() const;

      //! Set the minimum duration for functional up action is set
      void set_functional_up_duration_min(const boost::posix_time::time_duration &);

      //! Return the minimum duration for functional up action is set
      const boost::posix_time::time_duration & get_functional_up_duration_min() const;

      //! Check if the maximum duration for functional up action is set
      bool has_functional_up_duration_max() const;

      //! Set the maximum duration for functional up action is set
      void set_functional_up_duration_max(const boost::posix_time::time_duration &);

      //! Return the maximum duration for functional up action is set
      const boost::posix_time::time_duration & get_functional_up_duration_max() const;

      //! Check if the minimum duration for functional down action is set
      bool has_functional_down_duration_min() const;

      //! Set the minimum duration for functional down action is set
      void set_functional_down_duration_min(const boost::posix_time::time_duration &);

      //! Return the minimum duration for functional down action is set
      const boost::posix_time::time_duration & get_functional_down_duration_min() const;

      //! Check if the maximum duration for functional down action is set
      bool has_functional_down_duration_max() const;

      //! Set the maximum duration for functional down action is set
      void set_functional_down_duration_max(const boost::posix_time::time_duration &);

      //! Return the maximum duration for functional down action is set
      const boost::posix_time::time_duration & get_functional_down_duration_max() const;

      //! Check if the minimum duration for functional work action is set
      bool has_functional_work_duration_min() const;

      //! Set the minimum duration for functional work action is set
      void set_functional_work_duration_min(const boost::posix_time::time_duration &);

      //! Return the minimum duration for functional work action is set
      const boost::posix_time::time_duration & get_functional_work_duration_min() const;

      //! Check if the maximum duration for functional work action is set
      bool has_functional_work_duration_max() const;

      //! Set the maximum duration for functional work action is set
      void set_functional_work_duration_max(const boost::posix_time::time_duration &);

      //! Return the maximum duration for functional work action is set
      const boost::posix_time::time_duration & get_functional_work_duration_max() const;

      // //! Return the minimum duration of all stages
      // boost::posix_time::time_duration get_total_safe_duration_min() const;

      // //! Return the maximum duration of all stages
      // boost::posix_time::time_duration get_total_safe_duration_max() const;

      // bool run_total_duration_match(const boost::posix_time::time_duration & total_duration_) const;
      
      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_
                              = datatools::i_tree_dumpable::empty_options()) const;


      
    private:
      
      boost::posix_time::time_duration _preparation_duration_min_;        //!< Minimum duration of the run preparation
      boost::posix_time::time_duration _preparation_duration_max_;        //!< Maximum duration of the run preparation
      boost::posix_time::time_duration _termination_duration_min_;        //!< Minimum duration of the run termination
      boost::posix_time::time_duration _termination_duration_max_;        //!< Maximum duration of the run termination
      boost::posix_time::time_duration _distributable_up_duration_min_;   //!< Minimum duration of the distributable up action
      boost::posix_time::time_duration _distributable_up_duration_max_;   //!< Maximum duration of the distributable up action
      boost::posix_time::time_duration _distributable_down_duration_min_; //!< Minimum duration of the distributable down action
      boost::posix_time::time_duration _distributable_down_duration_max_; //!< Maximum duration of the distributable down action
      boost::posix_time::time_duration _functional_up_duration_min_;      //!< Minimum duration of the functional up action
      boost::posix_time::time_duration _functional_up_duration_max_;      //!< Maximum duration of the functional up action
      boost::posix_time::time_duration _functional_down_duration_min_;    //!< Minimum duration of the functional down action
      boost::posix_time::time_duration _functional_down_duration_max_;    //!< Maximum duration of the functional down action
      boost::posix_time::time_duration _functional_work_duration_min_;    //!< Minimum duration of the functional work action
      boost::posix_time::time_duration _functional_work_duration_max_;    //!< Maximum duration of the functional work action
        
    };
    
  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UC_TIME_CONSTRAINTS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
