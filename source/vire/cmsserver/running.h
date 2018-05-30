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

// - Boost:
#include <boost/property_tree/ptree.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
// - Bayeux/mygsl:
#include <bayeux/mygsl/min_max.h>
#include <bayeux/mygsl/mean.h>

namespace vire {

  namespace cmsserver {

    namespace running {
      
      /// \brief Supported run stages
      ///
      /// [READY]
      ///    |
      ///    v
      /// [PREPARING]
      ///    |
      ///    v
      /// [PREPARED]
      ///    |
      ///    v
      /// [DISTRIBUTABLE_UP_RUNNING]
      ///    |
      ///    v
      /// [DISTRIBUTABLE_UP_DONE]
      ///    |
      ///    v
      /// [FUNCTIONAL_UP_RUNNING]
      ///    |
      ///    v
      /// [FUNCTIONAL_UP_DONE]
      ///    |
      ///    v
      /// [FUNCTIONAL_WORK_RUNNING]
      ///    |
      ///    v
      /// [FUNCTIONAL_WORK_DONE]
      ///    |
      ///    v
      /// [FUNCTIONAL_DOWN_RUNNING]
      ///    |
      ///    v
      /// [FUNCTIONAL_DOWN_DONE]
      ///    |
      ///    v
      /// [DISTRIBUTABLE_DOWN_RUNNING]
      ///    |
      ///    v
      /// [DISTRIBUTABLE_DOWN_DONE]
      ///    |
      ///    v
      /// [TERMINATING]
      ///    |
      ///    v
      /// [TERMINATED]
      ///
      ///
      enum run_stage_type {
        RUN_STAGE_NA = -1,                         ///< Not applicable running concept
        RUN_STAGE_UNDEF = 0,                       ///< Run stage is not defined (yet)
        RUN_STAGE_READY = 1,                       ///< Ready for running stage
        RUN_STAGE_PREPARING = 2,                   ///< Preparing stage
        RUN_STAGE_PREPARED = 3,                    ///< Prepared stage
        RUN_STAGE_DISTRIBUTABLE_UP_RUNNING = 4,    ///< Distributable up running stage
        RUN_STAGE_DISTRIBUTABLE_UP_DONE = 5,       ///< Distributable up done stage
        RUN_STAGE_FUNCTIONAL_UP_RUNNING = 6,       ///< Functional up running stage
        RUN_STAGE_FUNCTIONAL_UP_DONE = 7,          ///< Functional up done stage
        RUN_STAGE_FUNCTIONAL_WORK_RUNNING = 8,     ///< Functional work running stage
        RUN_STAGE_FUNCTIONAL_WORK_DONE = 9,        ///< Functional work done stage
        RUN_STAGE_FUNCTIONAL_DOWN_RUNNING = 10,    ///< Functional down running stage
        RUN_STAGE_FUNCTIONAL_DOWN_DONE = 11,       ///< Functional down done stage
        RUN_STAGE_DISTRIBUTABLE_DOWN_RUNNING = 12, ///< Distributable down running stage
        RUN_STAGE_DISTRIBUTABLE_DOWN_DONE = 13,    ///< Distributable down done stage
        RUN_STAGE_TERMINATING = 14,                ///< Terminating stage
        RUN_STAGE_TERMINATED = 15                  ///< terminated stage
      };

      /// \brief Resource running depth 
      enum run_resource_depth_type {
        RUN_RESOURCE_DEPTH_DISTRIBUTABLE   = 1, ///< Run distributable up/down 
        RUN_RESOURCE_DEPTH_FUNCTIONAL_AUTO = 2, ///< Run functional up/down
        RUN_RESOURCE_DEPTH_FUNCTIONAL_WORK = 3  ///< Run functional work
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
        boost::posix_time::ptime    timestamp;      ///< Report UTC timestamp (mandatory)
        run_termination_type        run_termination = RUN_TERMINATION_UNDEF; ///< Type of run termination (mandatory)
        run_stage_type              run_stage       = RUN_STAGE_UNDEF;       ///< Run stage at termination (mandatory)
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

      /// \brief Preparation run status
      enum run_preparation_status_type {
        RUN_PREPARE_STATUS_OK = 0, 
        RUN_PREPARE_STATUS_ERROR = 1
      };

      /// \brief Control status for functional work loop
      enum run_functional_work_loop_status_type {
        RUN_FUNCTIONAL_WORK_LOOP_CONTINUE = 0, 
        RUN_FUNCTIONAL_WORK_LOOP_STOP     = 1
      };

      // struct run_base_signal {};
      // struct run_termination_signal : public run_base_signal {};
      // struct run_timeout_signal : public run_base_signal {};
      // struct run_user1_signal : public run_base_signal {};
      // struct run_user2_signal : public run_base_signal {};

      //! \brief Run control structure for an use case 
      struct run_control
      {
        /// Constructor
        run_control();

        /// Check run inhibition
        bool can_run() const;

        /// Request a run stop
        void run_stop_request();

        /// Check is run stop is requested
        bool check_run_stop_requested() const;
        
        //! Run stage status
        run_stage_type                       run_stage   = RUN_STAGE_UNDEF;
        //! Counter of functional work loop
        std::size_t                          run_functional_work_loop_counter = 0;
        //! Run report
        run_report_type                      run_report;                    

      private:
        // std::thread  _thread(s)_ // ??? main thread/stage thread/watchdog thread (or in the use case's mother session)
        std::mutex                _run_stop_request_mutex_;        //!< Mutex for the stop request flag
        // Atomic bool
        bool                      _run_stop_requested_ = false;    //!< Stop request flag
        
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
