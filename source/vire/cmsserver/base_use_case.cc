//! \file vire/cmsserver/base_use_case.cc
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
#include <vire/cmsserver/base_use_case.h>

// Standard Library:
#include <thread>
#include <chrono>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/datatools/units.h>
#include <bayeux/datatools/exception.h>

// This project:
#include <vire/cmsserver/error.h>
#include <vire/cmsserver/session_info.h>
#include <vire/cmsserver/uc_time_constraints.h>

namespace vire {

  namespace cmsserver {

    // Factory system register :
    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_use_case,
                                                     "vire::cmsserver::base_use_case/_system_")

    const parametrized_resource_specifications & base_use_case::get_relative_functional_requirements() const
    {
      static const parametrized_resource_specifications _nospecs;
      return _nospecs;
    }
 
    bool base_use_case::has_relative_functional_requirements() const
    {
      return !get_relative_functional_requirements().is_empty();
    }

    bool base_use_case::has_functional_mount_point(const std::string & name_) const
    {
      return _functional_mount_points_.count(name_);
    }

    bool base_use_case::is_completed_functional_mount_points() const
    {
      if (_functional_mount_points_.size() == get_relative_functional_requirements().size()) {
        return true;
      }
      return false;
    }
    
    void base_use_case::set_functional_device_mount_point(const std::string & name_, const std::string & device_path_)
    {
      DT_THROW_IF(!has_relative_functional_requirements(),
                  std::logic_error,
                  "This use case has no relative functional requirements!");
      const parametrized_resource_specifications & rfr = get_relative_functional_requirements();
      DT_THROW_IF(!rfr.has(name_),
                  std::logic_error,
                  "No relative functional required object with name '" << name_ << "' is defined!");
      DT_THROW_IF(!rfr.is_device(name_),
                  std::logic_error,
                  "Relative functional required object with name '" << name_ << "' is not a device!");
      DT_THROW_IF(has_functional_mount_point(name_),
                  std::logic_error,
                  "Relative functional mount point with name '" << name_ << "' is already set!");
      _functional_mount_points_[name_] = device_path_;
      return;
    }

    void base_use_case::set_functional_resource_mount_point(const std::string & name_, const std::string & resource_path_)
    {
      DT_THROW_IF(!has_relative_functional_requirements(),
                  std::logic_error,
                  "This use case has no relative functional requirements!");
      const parametrized_resource_specifications & rfr = get_relative_functional_requirements();
      DT_THROW_IF(!rfr.has(name_),
                  std::logic_error,
                  "No relative functional required object with name '" << name_ << "' is defined!");
      DT_THROW_IF(!rfr.is_resource(name_),
                  std::logic_error,
                  "Relative functional required object with name '" << name_ << "' is not a resource!");
      DT_THROW_IF(has_functional_mount_point(name_),
                  std::logic_error,
                  "Relative functional mount point with name '" << name_ << "' is already set!");
      _functional_mount_points_[name_] = resource_path_;
      return;
    }
 
    base_use_case::base_use_case(const uint32_t flags_)
    {
      if (flags_ & INIT_RUN) {
        _run_mode_ = true;
      }
      return;
    }

    base_use_case::~base_use_case()
    {
      if (is_initialized()) {
        DT_THROW(std::logic_error, "Use case has not been properly terminated!");
      }
      return;
    }

    bool base_use_case::has_rc() const
    {
      return _rc_.get() != nullptr;
    }
                        
    const running::run_control & base_use_case::get_rc() const
    {
      DT_THROW_IF(!has_rc() , std::logic_error, "No run control!")
      return *_rc_;
    }

    bool base_use_case::is_initialized() const
    {
      return _initialized_;
    }

    bool base_use_case::is_dry_run_mode() const
    {
      return !_run_mode_;
    }

    bool base_use_case::is_run_mode() const
    {
      return _run_mode_;
    }

    bool base_use_case::has_resource_constraints() const
    {
      return _resource_constraints_.get() != nullptr;
    }
    
    const uc_resource_constraints & base_use_case::get_resource_constraints() const
    {
      DT_THROW_IF(!has_resource_constraints(), std::logic_error, "No resource constraints!");
      return *_resource_constraints_.get();
    }

    bool base_use_case::has_time_constraints() const
    {
      return _time_constraints_.get() != nullptr;
    }

    const uc_time_constraints & base_use_case::get_time_constraints() const
    {
      DT_THROW_IF(!has_time_constraints(), std::logic_error, "No time constraints!");
      return *_time_constraints_.get();
    }

    // virtual
    std::shared_ptr<uc_resource_constraints> base_use_case::_build_resource_constraints()
    {
      return std::make_shared<uc_resource_constraints>();
    }

    // virtual
    std::shared_ptr<uc_time_constraints> base_use_case::_build_time_constraints()
    {
      return std::make_shared<uc_time_constraints>();
    }

    void base_use_case::initialize_simple()
    {
      datatools::properties dummy_config;
      initialize(dummy_config);
      return;
    }

    void base_use_case::initialize(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Use case is already initialized!");

      base_use_case::_basic_initialize_(config_);

      _at_initialize_(config_);

      if (! is_dry_run_mode()) {
        // Install a run control structure:
        _rc_.reset(new running::run_control);
        _rc_->run_stage = running::RUN_STAGE_READY;
      }
      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void base_use_case::finalize()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Use case is not initialized!");
      /*
        if (!is_dry_run_mode()) {
        if (has_rc()) {
        DT_THROW_IF(_rc_->run_stage != running::RUN_STAGE_TERMINATED, std::logic_error,
        "Use case run is not terminated!");
        }
        }
      */
      
      _initialized_ = false;

      _at_finalize_();

      base_use_case::_basic_finalize_();

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    /*
    void base_use_case::_basic_time_calibration_(const datatools::properties & config_)
    {
      _compute_run_distributable_times_();

      // User can force the use case's timing:
      if (config_.has_key("distributable_up_max_duration")) {
        double d = config_.fetch_real_with_explicit_dimension("distributable_up_max_duration", "time");
        DT_THROW_IF(d <= 0.0, std::logic_error, "Invalid distributable up max duration!");
        std::size_t nb_sec = (std::size_t) (d / CLHEP::second);
        set_distributable_up_max_duration(boost::posix_time::seconds(nb_sec));
      }

      if (config_.has_key("distributable_down_max_duration")) {
        double d = config_.fetch_real_with_explicit_dimension("distributable_down_max_duration", "time");
        DT_THROW_IF(d <= 0.0, std::logic_error, "Invalid distributable down max duration!");
        std::size_t nb_sec = (std::size_t) (d / CLHEP::second);
        set_distributable_down_max_duration(boost::posix_time::seconds(nb_sec));
      }

      if (config_.has_key("functional_down_max_duration")) {
        double d = config_.fetch_real_with_explicit_dimension("functional_down_max_duration", "time");
        DT_THROW_IF(d <= 0.0, std::logic_error, "Invalid functional down max duration!");
        std::size_t nb_sec = (std::size_t) (d / CLHEP::second);
        set_functional_down_max_duration(boost::posix_time::seconds(nb_sec));
      }

      if (config_.has_key("functional_work_min_duration")) {
        double d = config_.fetch_real_with_explicit_dimension("functional_work_min_duration", "time");
        DT_THROW_IF(d <= 0.0, std::logic_error, "Invalid functional work min duration!");
        std::size_t nb_sec = (std::size_t) (d / CLHEP::second);
        set_functional_work_min_duration(boost::posix_time::seconds(nb_sec));
      }

      if (config_.has_key("functional_work_max_duration")) {
        double d = config_.fetch_real_with_explicit_dimension("functional_work_max_duration", "time");
        DT_THROW_IF(d <= 0.0, std::logic_error, "Invalid functional work max duration!");
        std::size_t nb_sec = (std::size_t) (d / CLHEP::second);
        set_functional_work_max_duration(boost::posix_time::seconds(nb_sec));
      }

      if (config_.has_key("functional_down_max_duration")) {
        double d = config_.fetch_real_with_explicit_dimension("functional_down_max_duration", "time");
        DT_THROW_IF(d <= 0.0, std::logic_error, "Invalid functional down max duration!");
        std::size_t nb_sec = (std::size_t) (d / CLHEP::second);
        set_functional_down_max_duration(boost::posix_time::seconds(nb_sec));
      }

      return;
    }
    */
    
    void base_use_case::_basic_initialize_(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (!is_dry_run_mode()) {
        // DT_THROW_IF(_run_session_ == nullptr, std::logic_error,
        //             "No mother session is set!");
      }
      
      this->datatools::enriched_base::initialize(config_, false);

      // Handle mount points
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void base_use_case::_basic_finalize_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (!is_dry_run_mode()) {
        _run_session_ = nullptr;
        _rc_.reset();
      }
      _resource_constraints_.reset();
      _time_constraints_.reset();
      _composition_.reset();
      _functional_mount_points_.clear();
      // this->datatools::enriched_base::reset();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void base_use_case::_at_initialize_(const datatools::properties & config_)
    {
      return;
    }

    void base_use_case::_at_finalize_()
    {
      return;
    }

    /* Actions */

    running::run_stage_completion base_use_case::run_prepare()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      running::run_stage_completion completion;
      try {
        DT_THROW_IF(!is_initialized(), std::logic_error,
                    "Use case is not initialized!");
        DT_THROW_IF(_rc_->run_stage != running::RUN_STAGE_READY,
                    std::logic_error,
                    "Use case cannot prepare for running!");
        _rc_->run_stage = running::RUN_STAGE_PREPARING;
        running::run_preparation_status_type _at_run_prepare_();
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now_utc();
        completion.run_stage = _rc_->run_stage;
        _rc_->run_stage = running::RUN_STAGE_PREPARED;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }

    running::run_stage_completion base_use_case::run_distributable_up()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      running::run_stage_completion completion;
      try {
        DT_THROW_IF(_rc_->run_stage != running::RUN_STAGE_PREPARED,
                    std::logic_error,
                    "Use case is not prepared!");
        _rc_->run_stage = running::RUN_STAGE_DISTRIBUTABLE_UP_RUNNING;
        _at_run_distributable_up_();
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now_utc();
        completion.run_stage = _rc_->run_stage;
        _rc_->run_stage = running::RUN_STAGE_DISTRIBUTABLE_UP_DONE;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }

    running::run_stage_completion base_use_case::run_functional_up()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      running::run_stage_completion completion;
      try {
        DT_THROW_IF(_rc_->run_stage != running::RUN_STAGE_DISTRIBUTABLE_UP_DONE,
                    std::logic_error,
                    "Distributable up is not done!");
        _rc_->run_stage = running::RUN_STAGE_FUNCTIONAL_UP_RUNNING;
        _at_run_functional_up_();
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now_utc();
        _rc_->run_stage = running::RUN_STAGE_FUNCTIONAL_UP_DONE;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }

    // void base_use_case::_run_work_init_()
    // {
    //   return;
    // }

    // void base_use_case::_run_work_begin_()
    // {
    //   // run_report_record * record = nullptr;
    //   // run_report_record_dict_type::iterator found = _run_reports_.find(_stage_);
    //   // if (found == _run_reports_.end()) {
    //   //   run_report_record new_record = run_report_record;
    //   //   new_record.start_stop = boost::posix_time::time_period(vire::time::now_utc(),
    //   //                                                                 vire::time::invalid_time());
    //   //   _run_reports_[_stage_] = new_record;
    //   // } else {
    //   //   record = &found->second;
    //   // }
    //   return;
    // }

    // void base_use_case::_run_work_end_()
    // {
    //   return;
    // }

    // void base_use_case::_run_work_terminate_()
    // {
    //   return;
    // }

    running::run_stage_completion base_use_case::run_functional_work()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      running::run_stage_completion completion;
      try {
        DT_THROW_IF(_rc_->run_stage != running::RUN_STAGE_FUNCTIONAL_UP_DONE,
                    std::logic_error,
                    "Functional up is not done!");
        _rc_->run_stage = running::RUN_STAGE_FUNCTIONAL_WORK_RUNNING;
        // LOOP STAT INIT/RESET
        DT_LOG_TRACE(get_logging_priority(), "Starting loop...");
        running::run_functional_work_loop_status_type continue_flag = running::RUN_FUNCTIONAL_WORK_LOOP_CONTINUE;
        while (continue_flag == running::RUN_FUNCTIONAL_WORK_LOOP_CONTINUE
               && !_rc_->check_run_stop_requested()) {
          DT_LOG_TRACE(get_logging_priority(), "Loop...");
          _rc_->run_functional_work_loop_counter++;

          // LOOP STAT LAST BEGIN
          //_run_work_begin_(),

          continue_flag = _at_run_functional_work_loop_iteration_();
          
          //_run_work_end_(),
          // LOOP STAT LAST END
          // LOOP STAT UPDATE
        }
        // (LOOP STAT FINI)
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now_utc();
        _rc_->run_stage = running::RUN_STAGE_FUNCTIONAL_WORK_DONE;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }

    running::run_stage_completion base_use_case::run_functional_down()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      running::run_stage_completion completion;
      try {
        DT_THROW_IF(_rc_->run_stage != running::RUN_STAGE_FUNCTIONAL_WORK_DONE,
                    std::logic_error,
                    "Functional work is not done!");
         _rc_->run_stage = running::RUN_STAGE_FUNCTIONAL_DOWN_RUNNING;
        _at_run_functional_down_();
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now_utc();
        _rc_->run_stage = running::RUN_STAGE_FUNCTIONAL_DOWN_DONE;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }

    running::run_stage_completion base_use_case::run_distributable_down()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      running::run_stage_completion completion;
      try {
        DT_THROW_IF(_rc_->run_stage != running::RUN_STAGE_FUNCTIONAL_DOWN_DONE,
                    std::logic_error,
                    "Functional down is not done!");
        _rc_->run_stage = running::RUN_STAGE_DISTRIBUTABLE_DOWN_RUNNING;
        _at_run_distributable_down_();
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now_utc();
        _rc_->run_stage = running::RUN_STAGE_DISTRIBUTABLE_DOWN_DONE;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }

    running::run_stage_completion base_use_case::run_terminate()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      // DT_THROW_IF(!_run_interrupt_
      //             && _rc_->run_stage != running::RUN_STAGE_DISTRIBUTABLE_DOWN_DONE,
      //             std::logic_error,
      //             "Distributable down is not done while run !");
      // SOME CHECK HERE!!!
      running::run_stage_completion completion;
      try {
        _rc_->run_stage = running::RUN_STAGE_TERMINATING;
        _at_run_terminate_();
        completion.run_termination = running::RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now_utc();
        _rc_->run_stage = running::RUN_STAGE_TERMINATED;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now_utc();
        completion.run_termination = running::RUN_TERMINATION_ERROR;
        completion.run_stage = _rc_->run_stage;
        completion.error_class_id = "unsupported exception";
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return completion;
    }

    running::run_preparation_status_type base_use_case::_at_run_prepare_()
    {
      running::run_preparation_status_type ret = running::RUN_PREPARE_STATUS_OK;
      return ret;
    }

    running::run_preparation_status_type base_use_case::_at_run_terminate_()
    {
      running::run_preparation_status_type ret = running::RUN_PREPARE_STATUS_OK;
      return ret;
    }

    void base_use_case::_at_run_distributable_up_()
    {
      return;
    }

    void base_use_case::_at_run_distributable_down_()
    {
      return;
    }

    void base_use_case::_at_run_functional_up_()
    {
      return;
    }

    void base_use_case::_at_run_functional_down_()
    {
      return;
    }

    running::run_functional_work_loop_status_type
    base_use_case::_at_run_functional_work_loop_iteration_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      running::run_functional_work_loop_status_type ret = running::RUN_FUNCTIONAL_WORK_LOOP_STOP;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return ret;
    }

    void base_use_case::dry_run_generate(session_info & sinfo_) const
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Use case is not initialized!");
      DT_THROW_IF(!is_dry_run_mode(), std::logic_error,
                  "Use case is not in 'dry-run' mode!");
      _at_dry_run_generate_(sinfo_);
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
 
    void base_use_case::_at_dry_run_generate_(session_info & sinfo_) const
    {
      sinfo_.reset();      
      return;
    }

    // virtual
    void base_use_case::print_tree(std::ostream & out_,
                                   const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      datatools::enriched_base::tree_dump(out_, popts.title, popts.indent, true);

      if (has_relative_functional_requirements()) {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Relative functional requirements  : ";
        out_ << std::endl;
        {
          boost::property_tree::ptree popts2;
          popts2.put(datatools::i_tree_dumpable::base_print_options::indent_key(),
                     popts.indent + datatools::i_tree_dumpable::tags::skip_item());
          get_relative_functional_requirements().print_tree(out_, popts2);
        }

        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Functional mount points : ";
        out_ << std::endl;
        size_t count = 0;
        for (const auto fmp : _functional_mount_points_) {
          out_ << popts.indent << datatools::i_tree_dumpable::skip_tag;
          if (++count == _functional_mount_points_.size()) {
            out_ << datatools::i_tree_dumpable::last_tag; 
          } else {
            out_ << datatools::i_tree_dumpable::tag; 
          }
          out_ << "Mount point '" << fmp.first << "'"
               << " with path '" << fmp.second << "'"
               << std::endl;
        }
        
      }
        
      // out_ << popts.indent << datatools::i_tree_dumpable::tag
      //      << "Role expression  : ";
      // out_ << "'" << _role_expression_ << "'";
      // out_ << std::endl;

      /*
      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Distributable up max duration   : ";
      if (has_distributable_up_max_duration()) {
        out_ << vire::time::to_string(_distributable_up_max_duration_);
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Distributable down max duration   : ";
      if (has_distributable_down_max_duration()) {
        out_ << vire::time::to_string(_distributable_down_max_duration_);
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Functional up max duration : ";
      if (has_functional_up_max_duration()) {
        out_ << vire::time::to_string(_functional_up_max_duration_);
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Functional work min duration : ";
      if (has_functional_work_min_duration()) {
        out_ << vire::time::to_string(_functional_work_min_duration_);
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Functional work max duration : ";
      if (has_functional_work_max_duration()) {
        out_ << vire::time::to_string(_functional_work_max_duration_);
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Functional down max duration : ";
      if (has_functional_down_max_duration()) {
        out_ << vire::time::to_string(_functional_down_max_duration_);
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      // out_ << popts.indent << datatools::i_tree_dumpable::tag
      //      << "Total min duration : " << vire::time::to_string(get_total_min_duration())
      //      << std::endl;

      // out_ << popts.indent << datatools::i_tree_dumpable::tag
      //      << "Total max duration : " << vire::time::to_string(get_total_max_duration())
      //      << std::endl;
      */
      
      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Initialized : " << std::boolalpha << is_initialized()
           << std::endl;
      
      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Dry-run : " << std::boolalpha << is_dry_run_mode()
           << std::endl;

      if (is_run_mode()) {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Mother session : ";
        if (_run_session_ != nullptr) {
          out_ << _run_session_;
        } else {
          out_ << "<none>";
        }
        out_ << std::endl;
      }

      if (has_rc() ) {
        out_ << popts.indent << datatools::i_tree_dumpable::tag
             << "Run stage : '" << running::run_stage_label(_rc_->run_stage) << "'"
             << std::endl;
      }

      if (! popts.inherit) {
        out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
             << "End.";
        out_ << std::endl;
      }

      return;
    }

    /*
    // static
    use_case_ptr_type base_use_case::_create_use_case_(const std::string & use_case_type_id_,
                                                       session * run_session_)
    {
      use_case_ptr_type new_use_case;
      const factory_register_type & the_factory_register
        = DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(vire::cmsserver::base_use_case);
      
      DT_THROW_IF(!the_factory_register.has(use_case_type_id_),
                  std::logic_error,
                  "Use case factory has no type with identifier '" << use_case_type_id_ << "'!");
      const factory_register_type::factory_type & the_factory = the_factory_register.get(use_case_type_id_);
      new_use_case.reset(the_factory());
      new_use_case->_run_session_ = run_session_;
      return new_use_case;
    }
    */
    
  } // namespace cmsserver

} // namespace vire
