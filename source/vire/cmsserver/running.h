//! \file  vire/cmsserver/running.h
//! \brief Vire CMS use case running utilities and types
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

#ifndef VIRE_CMSSERVER_RUNNING_H
#define VIRE_CMSSERVER_RUNNING_H

// Standard Library:
#include <string>
#include <map>
#include <mutex>
#include <atomic>

// Third party:
// - Boost:
#include <boost/property_tree/ptree.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
// - Bayeux/mygsl:
#include <bayeux/mygsl/min_max.h>
#include <bayeux/mygsl/mean.h>

// This project:
// #include <vire/running/scheduling.h>

namespace vire {

  namespace cmsserver {

    namespace running {
      
      /// \brief Run stages
      ///
      /// [READY]
      ///    |
      ///    | action [SYSTEM_PREPARING]
      ///    |
      ///    v
      /// [SYSTEM_PREPARED]
      ///    |
      ///    | action [FUNCTIONAL_UP_RUNNING]
      ///    |
      ///    v
      /// [FUNCTIONAL_UP_DONE]
      ///    |
      ///    | action [FUNCTIONAL_WORK_RUNNING]
      ///    |
      ///    v
      /// [FUNCTIONAL_WORK_DONE]
      ///    |
      ///    | action [FUNCTIONAL_DOWN_RUNNING]
      ///    |
      ///    v
      /// [FUNCTIONAL_DOWN_DONE]
      ///    |
      ///    | action [SYSTEM_TERMINATING]
      ///    |
      ///    v
      /// [SYSTEM_TERMINATED]
      ///
      ///
      enum run_stage_type {
        RUN_STAGE_NA = -1,                         ///< Not applicable running concept
        RUN_STAGE_UNDEF = 0,                       ///< Run stage is not defined (yet)
        RUN_STAGE_READY = 1,                       ///< Ready for running stage
        RUN_STAGE_SYSTEM_PREPARING = 2,            ///< System preparing stage
        RUN_STAGE_SYSTEM_PREPARED = 3,             ///< System prepared stage
        RUN_STAGE_FUNCTIONAL_UP_RUNNING = 4,       ///< Functional up running stage
        RUN_STAGE_FUNCTIONAL_UP_DONE = 5,          ///< Functional up done stage
        RUN_STAGE_FUNCTIONAL_WORK_RUNNING = 6,     ///< Functional work running stage
        RUN_STAGE_FUNCTIONAL_WORK_DONE = 7,        ///< Functional work done stage
        RUN_STAGE_FUNCTIONAL_DOWN_RUNNING = 8,     ///< Functional down running stage
        RUN_STAGE_FUNCTIONAL_DOWN_DONE = 9,        ///< Functional down done stage
        RUN_STAGE_SYSTEM_TERMINATING = 10,         ///< System terminating stage
        RUN_STAGE_SYSTEM_TERMINATED = 11           ///< Terminated stage
      };
      
      /// Return the label associated to a given run stage
      std::string run_stage_label(const run_stage_type);

      enum run_termination_type {
        RUN_TERMINATION_UNDEF       = 0,
        RUN_TERMINATION_NORMAL      = 1, ///< Normal termination by the usecase itself
        RUN_TERMINATION_ANTICIPATED = 2, ///< Normal anticipated termination (requested by the session through a termination_signal)
        RUN_TERMINATION_ERROR       = 3, ///< Abnormal termination by the usecase itself because an exception was thrown
        RUN_TERMINATION_TIMEOUT     = 4  ///< Forced termination requested by the session because of the timeout (through a timeout_signal)
      };

      //! \brief Run report data structure returned by each running steps of the use case
      struct run_stage_completion
      {
        run_stage_type              run_stage       = RUN_STAGE_UNDEF;       ///< Run stage at termination (mandatory)
        boost::posix_time::ptime    timestamp;      ///< Report UTC timestamp (mandatory)
        run_termination_type        run_termination = RUN_TERMINATION_UNDEF; ///< Type of run termination (mandatory)
        std::string                 error_class_id; ///< Error/exception type/class identifier (optional)
        boost::property_tree::ptree error_data;     ///< Specific data associated to the error/exception (optional)

        run_stage_completion();
        bool is_terminated() const;
        bool is_error() const;
        bool is_normal_termination() const;
      };

      // typedef std::future<stage_completion> run_future_type;

      struct run_stage_time_statistics
      {
        run_stage_time_statistics();
        std::size_t                    loop_counter = 0;
        boost::posix_time::time_period start_stop;
        boost::posix_time::time_period last_run_start_stop;
        // Use templatized min/max and mean/sigma calculation ?
        mygsl::min_max                 min_max_info;
        mygsl::arithmetic_mean         mean_info;
      };

      /// \brief Stage report record with embedded both completion status and time statistics 
      struct run_stage_report_record
      {
        run_stage_completion      completion; ///< Completion status
        run_stage_time_statistics time_stats; ///< Time statistics
      };

      /// \brief Dictionary of stage report records associated to different running stages
      typedef std::map<run_stage_type, run_stage_report_record> run_report_type;

      /// \brief Control status for functional work loop
      enum run_work_loop_status_type {
        RUN_WORK_LOOP_CONTINUE = 0, 
        RUN_WORK_LOOP_STOP     = 1
      };

      /// \brief Resource running depth 
      enum run_depth_type {
        RUN_DEPTH_NONE   = 0, ///< No run at all (skip all running operations) 
        RUN_DEPTH_SYSTEM = 1, ///< Run system prepare/terminate 
        RUN_DEPTH_AUTO   = 2, ///< Run functional resource up/down
        RUN_DEPTH_WORK   = 3  ///< Run functional resource work
      };

      std::string run_depth_label(const run_depth_type);

      class base_use_base;
      // class session;
      
      //! \brief Run control structure for a given use case 
      struct run_control
      {
        /// Constructor
        run_control(const run_depth_type = running::RUN_DEPTH_WORK);

        /// Request a run stop
        void run_stop_request();

        /// Check is run stop is requested
        bool check_run_stop_requested() const;

        /// Return the run depth
        run_depth_type get_run_depth() const;

        /// Return the current run stage
        run_stage_type get_run_stage() const;

        /// Set the current run stage
        void set_run_stage(const run_stage_type);

        /// Check if the current run stage has a given value
        bool is_run_stage(const run_stage_type stage_) const;

        /// Return the current run work loop counter
        std::size_t get_run_work_loop_counter() const;

        /// Increment the current run work loop counter
        void increment_run_work_loop_counter();
        
      public:
        
        run_report_type run_report;  //!< Run report                  

      private:

        // Configuration:
        run_depth_type _run_depth_ = running::RUN_DEPTH_WORK; //!< Running depth

        // Running informations:
        run_stage_type   _run_stage_ = RUN_STAGE_UNDEF;   //!< Run stage status     
        std::atomic<std::size_t> _run_work_loop_counter_; //!< Counter of functional work loop      
        std::atomic<bool> _run_stop_requested_;           //!< Stop request flag
        std::mutex       _run_mutex_;                     //!< Mutex for run actions

        friend class base_use_base;
        friend class session;
       
      };
     
    } // namespace running
 
  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_RUNNING_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
