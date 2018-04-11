//! \file  vire/cmsserver/base_use_case.h
//! \brief Vire CMS base use case
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

#ifndef VIRE_CMSSERVER_BASE_USE_CASE_H
#define VIRE_CMSSERVER_BASE_USE_CASE_H

// Standard Library:
#include <string>
#include <memory>
#include <future>
#include <exception>
#include <map>
#include <mutex>

// Third party:
// - Boost:
#include <boost/noncopyable.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/enriched_base.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/factory_macros.h>
#include <bayeux/mygsl/min_max.h>
#include <bayeux/mygsl/mean.h>

// This project:
#include <vire/time/utils.h>
#include <vire/resource/role.h>

namespace vire {

  namespace cmsserver {

    class session;

    /// \brief Vire CMS base use case
    class base_use_case
      : private boost::noncopyable
      , public datatools::enriched_base
    {
    public:

      /// \brief Supported run stages
      enum run_stage_type {
        RUN_STAGE_UNDEF = 0,
        RUN_STAGE_READY = 1,
        RUN_STAGE_PREPARING = 2,
        RUN_STAGE_PREPARED = 3,
        RUN_STAGE_DISTRIBUTABLE_UP_RUNNING = 4,
        RUN_STAGE_DISTRIBUTABLE_UP_DONE = 5,
        RUN_STAGE_FUNCTIONAL_UP_RUNNING = 6,
        RUN_STAGE_FUNCTIONAL_UP_DONE = 7,
        RUN_STAGE_FUNCTIONAL_WORK_RUNNING = 8,
        RUN_STAGE_FUNCTIONAL_WORK_DONE = 9,
        RUN_STAGE_FUNCTIONAL_DOWN_RUNNING = 10,
        RUN_STAGE_FUNCTIONAL_DOWN_DONE = 11,
        RUN_STAGE_DISTRIBUTABLE_DOWN_RUNNING = 12,
        RUN_STAGE_DISTRIBUTABLE_DOWN_DONE = 13,
        RUN_STAGE_TERMINATING = 14,
        RUN_STAGE_TERMINATED = 15
      };

      /// Return the label associated to a given run stage
      static std::string run_stage_label(const run_stage_type);

      ///
      enum run_functional_work_loop_status_type {
        RUN_FUNCTIONAL_WORK_LOOP_ITERATE = 0,
        RUN_FUNCTIONAL_WORK_LOOP_STOP    = 1
      };

      struct base_signal {};
      struct termination_signal : public base_signal {};
      struct timeout_signal : public base_signal {};
      // struct user1_signal : public base_signal {};
      // struct user2_signal : public base_signal {};

      enum run_termination_type {
        RUN_TERMINATION_UNDEF   = 0,
        RUN_TERMINATION_NORMAL  = 1, ///< Normal termination by the usecase itself
        RUN_TERMINATION_SESSION = 2, ///< Normal anticipated termination requested by the session (through a termination_signal)
        RUN_TERMINATION_ERROR   = 3, ///< Abnormal termination by the usecase itself because an exception was thrown
        RUN_TERMINATION_TIMEOUT = 4  ///< Forced termination requested by the session because of the timeout (through a timeout_signal)
      };

      struct base_exception
        : public std::exception
      {
        virtual void export_error_data(boost::property_tree::ptree & error_data_) const = 0;
      };

      /*
      struct resource_exception : public base_exception
      {
        virtual void export(boost::property_tree::ptree & error_data_) const
        {
          error_data_.put("resource_path", resource_path);
          error_data_.put("invalid_value", invalid_value);
        }

        std::string resource_path;
        int invalid_value;
      };
      */

      //! \brief Run report data structure returned by each running steps of the use case
      struct stage_completion
      {
        boost::posix_time::ptime    timestamp;      ///< Report timestamp (mandatory)
        run_termination_type        run_termination = RUN_TERMINATION_UNDEF; ///< Type of run termination (mandatory)
        run_stage_type              run_stage       = RUN_STAGE_UNDEF;       ///< Run stage at termination (mandatory)
        std::string                 error_class_id; ///< Error/exception type/class identifier (optional)
        boost::property_tree::ptree error_data;     ///< Specific data associated to the error/exception (optional)

        bool is_error() const;
        bool is_normal_termination() const;
      };

      // typedef std::future<stage_completion> run_future_type;

      struct stage_time_statistics
      {
        stage_time_statistics();
        std::size_t                    loop_counter = 0;
        boost::posix_time::time_period start_stop;
        boost::posix_time::time_period last_run_start_stop;
        // Use templatized min/max and mean/sigma calculation ?
        mygsl::min_max                 min_max_info;
        mygsl::arithmetic_mean         mean_info;
      };

      struct stage_report_record
      {
        stage_time_statistics time_stats;
        stage_completion      completion;
      };

      typedef std::map<run_stage_type,stage_report_record> run_report_type;

      //! Default constructor
      base_use_case();

      //! Destructor
      virtual ~base_use_case();

      //! Check if the role expression is set
      bool has_role_expression() const;

      //! Set the role expression
      void set_role_expression(const std::string & role_expression_);

      //! Return the role expression
      const std::string & get_role_expression() const;

      //! Check if the mother session is set
      bool has_mother_session() const;

      //! Set the mother session
      void set_mother_session(const session & s_);

      //! Return the mother session
      const session & get_mother_session() const;

      //! Check if the maximum duration for distributable up action is set
      bool has_distributable_up_max_duration() const;

      //! Set the maximum duration for distributable up action is set
      void set_distributable_up_max_duration(const boost::posix_time::time_duration &);

      //! Return the maximum duration for distributable up action is set
      const boost::posix_time::time_duration & get_distributable_up_max_duration() const;

      //! Check if the maximum duration for distributable down action is set
      bool has_distributable_down_max_duration() const;

      //! Set the maximum duration for distributable down action is set
      void set_distributable_down_max_duration(const boost::posix_time::time_duration &);

      //! Return the maximum duration for distributable down action is set
      const boost::posix_time::time_duration & get_distributable_down_max_duration() const;

      //! Check if the maximum duration for functional up action is set
      bool has_functional_up_max_duration() const;

      //! Set the maximum duration for functional up action is set
      void set_functional_up_max_duration(const boost::posix_time::time_duration &);

      //! Return the maximum duration for functional up action is set
      const boost::posix_time::time_duration & get_functional_up_max_duration() const;

      //! Check if the maximum duration for functional down action is set
      bool has_functional_down_max_duration() const;

      //! Set the maximum duration for functional down action is set
      void set_functional_down_max_duration(const boost::posix_time::time_duration &);

      //! Return the maximum duration for functional down action is set
      const boost::posix_time::time_duration & get_functional_down_max_duration() const;

      //! Check if the maximum duration for functional work action is set
      bool has_functional_work_max_duration() const;

      //! Set the maximum duration for functional work action is set
      void set_functional_work_max_duration(const boost::posix_time::time_duration &);

      //! Return the maximum duration for functional work action is set
      const boost::posix_time::time_duration & get_functional_work_max_duration() const;

      //! Check if the minimum duration for functional work action is set
      bool has_functional_work_min_duration() const;

      //! Set the minimum duration for functional work action is set
      void set_functional_work_min_duration(const boost::posix_time::time_duration &);

      //! Return the minimum duration for functional work action is set
      const boost::posix_time::time_duration & get_functional_work_min_duration() const;

      //! Return the minimum duration of all stages
      boost::posix_time::time_duration get_total_min_duration() const;

      //! Return the maximum duration of all stages
      boost::posix_time::time_duration get_total_max_duration() const;

      // //! Check if the tick time for the run functional work loop is set
      // bool has_run_functional_work_loop_tick() const;

      // //! Set the tick time for the run functional work loop
      // void set_run_functional_work_loop_tick(const boost::posix_time::time_duration &);

      // //! Return the tick time for the run functional work loop
      // const boost::posix_time::time_duration & get_run_functional_work_loop_tick() const;

      // Instance initialization

      bool is_initialized() const;

      void initialize_simple();

      void initialize(const datatools::properties & config_);

      void finalize();

      // Business running:
      run_stage_type get_run_stage() const;

      /// Check if run is ready to process
      bool is_run_ready() const;

      /// Check if run is terminated
      bool is_run_terminated() const;

      // /// Check if run is broken
      // bool is_run_broken() const;

      // void set_run_broken(const bool b_ = true);

      stage_completion run_prepare();

      stage_completion run_distributable_up();

      stage_completion run_functional_up();

      stage_completion run_functional_work();

      stage_completion run_functional_down();

      stage_completion run_distributable_down();

      stage_completion run_terminate();

      // Signal handler based on the few signals and possible overidden handler methods in the UC

      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_ = datatools::i_tree_dumpable::empty_options()) const;

      const ::vire::resource::role & get_minimal_role() const;

      void run_stop_request();

    private:

      // Initialization/destruction:

      void _basic_initialize_(const datatools::properties & config_);

      void _basic_time_calibration_(const datatools::properties & config_);

      void _basic_finalize_();

      virtual const ::vire::resource::role * _create_minimal_role_() = 0;

      virtual void _compute_run_distributable_times_();

      virtual void _compute_run_functional_times_();

      virtual void _at_initialize_(const datatools::properties & config_);

      virtual void _at_finalize_();

      // Running:

      virtual void _at_run_prepare_();

      virtual void _at_run_distributable_up_();

      virtual void _at_run_functional_up_();

      virtual void _at_run_functional_work_loop_iteration_();

      virtual void _at_run_functional_down_();

      virtual void _at_run_distributable_down_();

      virtual void _at_run_terminate_();

      // In case of broken run at some given stage, we should be able
      // to build a recursive snapshot of the use case (and its daughter
      // use cases) and report.

      std::size_t get_run_work_loop_count() const;

      void _run_work_init_();

      void _run_work_begin_();

      void _run_work_end_();

      void _run_work_terminate_();

    protected:

      bool _check_run_stop_requested() const;

      void _run_functional_work_loop_status_continue();

      void _run_functional_work_loop_status_stop();

    private:

      // System management:
      bool                      _initialized_ = false;           //!< Initialization flag

      // Run management:
      run_stage_type            _run_stage_   = RUN_STAGE_UNDEF; //!< Run stage status
      //! Status of the functional work loop
      run_functional_work_loop_status_type _run_functional_work_loop_status_;
      run_report_type           _run_report_;                    //!< Run report
      std::mutex                _run_stop_request_mutex_;        //!< Mutex for teh stop request flag
      bool                      _run_stop_requested_ = false;    //!< Stop request flag

      // Configuration:
      std::string _role_expression_; //!< Expression describing the role

      // Internal data:
      const session * _mother_session_ = nullptr;                         //!< Handle to the mother session
      boost::posix_time::time_duration _distributable_up_max_duration_;   //!< Maximum duration of the distributable up action
      boost::posix_time::time_duration _distributable_down_max_duration_; //!< Maximum duration of the distributable down action
      boost::posix_time::time_duration _functional_up_max_duration_;      //!< Maximum duration of the functional up action
      boost::posix_time::time_duration _functional_work_min_duration_;    //!< Minimum duration of the functional work action
      boost::posix_time::time_duration _functional_work_max_duration_;    //!< Maximum duration of the functional work action
      boost::posix_time::time_duration _functional_down_max_duration_;    //!< Maximum duration of the functional down action
      std::unique_ptr<const vire::resource::role> _minimal_role_;         //!< Cached minimal role

      friend class session;

      // Factory declaration :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(base_use_case)

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_BASE_USE_CASE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
