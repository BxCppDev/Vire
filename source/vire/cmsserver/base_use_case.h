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

// Third party:
// - Boost:
#include <boost/noncopyable.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/enriched_base.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/factory_macros.h>

// This project:
#include <vire/time/utils.h>
#include <vire/cmsserver/task_utils.h>
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

      enum run_stage_type {
        RUN_STAGE_UNDEF = 0,
        RUN_STAGE_PREPARING = 1,
        RUN_STAGE_READY = 2,
        RUN_STAGE_DISTRIBUTABLE_UP = 3,
        RUN_STAGE_DISTRIBUTABLE_UP_DONE = 4,
        RUN_STAGE_FUNCTIONAL_UP = 5,
        RUN_STAGE_FUNCTIONAL_UP_DONE = 6,
        RUN_STAGE_FUNCTIONAL_WORK = 7,
        RUN_STAGE_FUNCTIONAL_WORK_DONE = 8,
        RUN_STAGE_FUNCTIONAL_DOWN = 9,
        RUN_STAGE_FUNCTIONAL_DOWN_DONE = 10,
        RUN_STAGE_DISTRIBUTABLE_DOWN = 11,
        RUN_STAGE_DISTRIBUTABLE_DOWN_DONE = 12,
        RUN_STAGE_TERMINATING = 100,
        RUN_STAGE_TERMINATED = 200
      };

      struct base_signal {};
      struct termination_signal : public base_signal {};
      struct kill_signal : public base_signal {};
      struct timeout_signal : public base_signal {};
      // struct user1_signal : public base_signal {};
      // struct user2_signal : public base_signal {};

      enum run_termination_type {
        RUN_TERMINATION_UNDEF   = 0,
        RUN_TERMINATION_NORMAL  = 1, ///< Normal termination by the usecase itself
        RUN_TERMINATION_ERROR   = 2, ///< Abnormal termination by the usecase itself because an excpetion was thrown
        RUN_TERMINATION_SESSION = 3, ///< Normal anticipated termination requested by the session (through a termination_signal)
        RUN_TERMINATION_KILLED  = 4, ///< Forced anticipated termination requested by the session (through a kill_signal)
        RUN_TERMINATION_TIMEOUT = 5  ///< Forced termination requested by the session because of the timeout (through a timeout_signal)
      };

      struct base_exception : public std::exception
      {
        virtual void export(boost::property_tree::ptree & error_data_) const = 0;
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

      struct run_report_type
      {
        run_termination_type        run_termination = RUN_TERMINATION_UNDEF;
        boost::posix_time::ptime    timestamp;
        run_stage_type              run_stage  = RUN_STAGE_UNDEF;
        std::string                 error_class_id;
        boost::property_tree::ptree error_data;

        bool is_error() const;
        bool is_normal_termination() const;
      };

      typedef std::future<run_report_type> run_future_type;

      //! Default constructor
      base_use_case();

      //! Destructor
      virtual ~base_use_case();

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

      // Instance initialization

      bool is_initialized() const;

      void initialize_simple();

      void initialize(const datatools::properties & config_);

      void terminate();

      // Business running:
      run_stage_type get_run_stage() const;

      bool is_run_ready() const;

      bool is_run_done() const;

      bool is_run_broken() const;

      void set_run_broken(const bool b_ = true);

      run_report_type run_prepare();

      run_report_type run_distributable_up();

      run_report_type run_functional_up();

      run_report_type run_functional_work();

      run_report_type run_functional_down();

      run_report_type run_distributable_down();

      run_report_type run_terminate();

      // Signal handler based on the few signals and possible overidden handler methods in the UC

      //! Smart print
      virtual void print_tree(std::ostream & out_ = std::clog,
                              const boost::property_tree::ptree & options_ = datatools::i_tree_dumpable::empty_options()) const;

      const ::vire::resource::role & get_minimal_role() const;

    private:

      // Initialization/termination
      void _basic_initialize_(const datatools::properties & config_);

      void _basic_time_calibration_(const datatools::properties & config_);

      void _basic_terminate_();

      virtual const ::vire::resource::role * _create_minimal_role_() = 0;

      virtual void _compute_run_distributable_times_();

      virtual void _compute_run_functional_times_();

      virtual void _at_initialize_(const datatools::properties & config_) = 0;

      virtual void _at_terminate_() = 0;

      // Running:

      virtual void _at_run_prepare_();

      virtual void _at_run_distributable_up_() = 0;

      virtual void _at_run_functional_up_() = 0;

      virtual void _at_run_functional_work_() = 0;

      virtual void _at_run_functional_down_() = 0;

      virtual void _at_run_distributable_down_() = 0;

      virtual void _at_run_terminate_();

      // In case of broken run at some given stage, we should be able
      // to build a recursive snapshot of the use case (and its daughter
      // use cases) and report.

    private:

      // Management:
      bool           _initialized_ = false;           //!< Initialization flag
      run_stage_type _run_stage_   = RUN_STAGE_UNDEF; //!< Run stage status
      bool           _run_broken_  = false;           //!< Flag for broken stage


      // Configuration:
      std::string _role_expr_; //!< Expression describing the role

      // Internal data:
      const session * _mother_session_ = nullptr;                         //!< Handle to the mother session
      boost::posix_time::time_duration _distributable_up_max_duration_;   //!< Maximum duration of the distributable up action
      boost::posix_time::time_duration _distributable_down_max_duration_; //!< Maximum duration of the distributable down action
      boost::posix_time::time_duration _functional_up_max_duration_;      //!< Maximum duration of the functional up action
      boost::posix_time::time_duration _functional_work_min_duration_;    //!< Minimum duration of the functional work action
      boost::posix_time::time_duration _functional_work_max_duration_;    //!< Maximum duration of the functional work action
      boost::posix_time::time_duration _functional_down_max_duration_;    //!< Maximum duration of the functional down action
      std::unique_ptr<vire::resource::role *> _minimal_role_;             //!< Cached minimal role

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
