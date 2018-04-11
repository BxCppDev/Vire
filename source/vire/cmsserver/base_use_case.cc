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

namespace vire {

  namespace cmsserver {

    // Factory system register :
    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_use_case,
                                                     "vire::cmsserver::base_use_case/_system_")

    std::string base_use_case::run_stage_label(const run_stage_type stage_)
    {
      switch (stage_) {
        case RUN_STAGE_UNDEF: return std::string("undefined");
        case RUN_STAGE_READY: return std::string("ready");
        case RUN_STAGE_PREPARING: return std::string("preparing");
        case RUN_STAGE_PREPARED: return std::string("prepared");
        case RUN_STAGE_DISTRIBUTABLE_UP_RUNNING: return std::string("distributable-up-running");
        case RUN_STAGE_DISTRIBUTABLE_UP_DONE: return std::string("distributable-up-done");
        case RUN_STAGE_FUNCTIONAL_UP_RUNNING: return std::string("functional-up-running");
        case RUN_STAGE_FUNCTIONAL_UP_DONE: return std::string("functional-up-done");
        case RUN_STAGE_FUNCTIONAL_WORK_RUNNING: return std::string("functional-work-running");
        case RUN_STAGE_FUNCTIONAL_WORK_DONE: return std::string("functional-work-done");
        case RUN_STAGE_FUNCTIONAL_DOWN_RUNNING: return std::string("functional-down-running");
        case RUN_STAGE_FUNCTIONAL_DOWN_DONE: return std::string("functional-down-done");
        case RUN_STAGE_DISTRIBUTABLE_DOWN_RUNNING: return std::string("distributable-down-running");
        case RUN_STAGE_DISTRIBUTABLE_DOWN_DONE: return std::string("distributable-down-done");
        case RUN_STAGE_TERMINATING: return std::string("terminating");
        case RUN_STAGE_TERMINATED: return std::string("terminated");
      }
    }

    bool base_use_case::stage_completion::is_error() const
    {
      return run_termination == RUN_TERMINATION_ERROR;
    }

    bool base_use_case::stage_completion::is_normal_termination() const
    {
      return run_termination == RUN_TERMINATION_NORMAL;
    }

    base_use_case::stage_time_statistics::stage_time_statistics()
      : loop_counter(0)
      , start_stop(vire::time::invalid_time_interval())
      , last_run_start_stop(vire::time::invalid_time_interval())
    {
      return;
    }

    base_use_case::base_use_case()
    {
      return;
    }

    base_use_case::~base_use_case()
    {
      if (is_initialized()) {
        DT_THROW(std::logic_error, "Use case is still initialized!");
      }
      return;
    }

    const ::vire::resource::role & base_use_case::get_minimal_role() const
    {
      if (_minimal_role_ == nullptr) {
        base_use_case * mutable_this = const_cast<base_use_case*>(this);
        mutable_this->_minimal_role_.reset(mutable_this->_create_minimal_role_());
      }
      return *(_minimal_role_.get());
    }

    bool base_use_case::has_role_expression() const
    {
      return !_role_expression_.empty();
    }

    void base_use_case::set_role_expression(const std::string & role_expression_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Use case is already initialized!");
      _role_expression_ = role_expression_;
      return;
    }

    const std::string & base_use_case::get_role_expression() const
    {
      return _role_expression_;
    }

    bool base_use_case::has_mother_session() const
    {
      return _mother_session_ != nullptr;
    }

    void base_use_case::set_mother_session(const session & s_)
    {
      _mother_session_ = &s_;
      return;
    }

    const session & base_use_case::get_mother_session() const
    {
      DT_THROW_IF(!has_mother_session(), std::logic_error,
                  "No mother session is set in use case '" << get_name() << "'!");
      return *_mother_session_;
    }

    /* Distributable */

    bool base_use_case::has_distributable_up_max_duration() const
    {
      return vire::time::is_valid(_distributable_up_max_duration_);
    }

    void base_use_case::set_distributable_up_max_duration(const boost::posix_time::time_duration & d_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already intialized!");
      _distributable_up_max_duration_ = d_;
      return;
    }

    const boost::posix_time::time_duration & base_use_case::get_distributable_up_max_duration() const
    {
      return _distributable_up_max_duration_;
    }

    bool base_use_case::has_distributable_down_max_duration() const
    {
      return vire::time::is_valid(_distributable_down_max_duration_);
    }

    void base_use_case::set_distributable_down_max_duration(const boost::posix_time::time_duration & d_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already intialized!");
      _distributable_down_max_duration_ = d_;
      return;
    }

    const boost::posix_time::time_duration & base_use_case::get_distributable_down_max_duration() const
    {
      return _distributable_down_max_duration_;
    }

    /* Functional */

    bool base_use_case::has_functional_up_max_duration() const
    {
      return vire::time::is_valid(_functional_up_max_duration_);
    }

    void base_use_case::set_functional_up_max_duration(const boost::posix_time::time_duration & d_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already intialized!");
      _functional_up_max_duration_ = d_;
      return;
    }

    const boost::posix_time::time_duration & base_use_case::get_functional_up_max_duration() const
    {
      return _functional_up_max_duration_;
    }

    bool base_use_case::has_functional_down_max_duration() const
    {
      return vire::time::is_valid(_functional_down_max_duration_);
    }

    void base_use_case::set_functional_down_max_duration(const boost::posix_time::time_duration & d_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already intialized!");
      _functional_down_max_duration_ = d_;
      return;
    }

    const boost::posix_time::time_duration & base_use_case::get_functional_down_max_duration() const
    {
      return _functional_down_max_duration_;
    }

    bool base_use_case::has_functional_work_min_duration() const
    {
      return vire::time::is_valid(_functional_work_min_duration_);
    }

    void base_use_case::set_functional_work_min_duration(const boost::posix_time::time_duration & d_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already intialized!");
      if (has_functional_work_max_duration() && vire::time::is_valid(d_)) {
        DT_THROW_IF(_functional_work_max_duration_ < d_,
                    std::range_error, "Invalid functional work duration range!");
      }
      _functional_work_min_duration_ = d_;
      return;
    }

    const boost::posix_time::time_duration & base_use_case::get_functional_work_min_duration() const
    {
      return _functional_work_min_duration_;
    }

    bool base_use_case::has_functional_work_max_duration() const
    {
      return vire::time::is_valid(_functional_work_max_duration_);
    }

    void base_use_case::set_functional_work_max_duration(const boost::posix_time::time_duration & d_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Use case is already intialized!");
      if (has_functional_work_min_duration() && vire::time::is_valid(d_)) {
        DT_THROW_IF(d_ < _functional_work_min_duration_,
                    std::range_error, "Invalid functional work duration range!");
      }
      _functional_work_max_duration_ = d_;
      return;
    }

    const boost::posix_time::time_duration & base_use_case::get_functional_work_max_duration() const
    {
      return _functional_work_max_duration_;
    }

    boost::posix_time::time_duration base_use_case::get_total_min_duration() const
    {
      boost::posix_time::time_duration d = boost::posix_time::seconds(0);
      if (has_distributable_up_max_duration()) {
        d += _distributable_up_max_duration_;
      }
      if (has_functional_up_max_duration()) {
        d += _functional_up_max_duration_;
      }
      if (has_functional_work_min_duration()) {
        d += _functional_work_min_duration_;
      }
      if (has_functional_down_max_duration()) {
        d += _functional_down_max_duration_;
      }
      if (has_distributable_down_max_duration()) {
        d += _distributable_down_max_duration_;
      }
      return d;
    }

    boost::posix_time::time_duration base_use_case::get_total_max_duration() const
    {
      boost::posix_time::time_duration d = boost::posix_time::seconds(0);
      if (has_distributable_up_max_duration()) {
        d += _distributable_up_max_duration_;
      }
      if (has_functional_up_max_duration()) {
        d += _functional_up_max_duration_;
      }
      if (has_functional_work_max_duration()) {
        d += _functional_work_max_duration_;
      }
      if (has_functional_down_max_duration()) {
        d += _functional_down_max_duration_;
      }
      if (has_distributable_down_max_duration()) {
        d += _distributable_down_max_duration_;
      }
      return d;
    }

    // bool base_use_case::has_run_functional_work_loop_tick() const
    // {
    //   return vire::time::is_valid(_run_functional_work_loop_tick_);
    // }

    // void base_use_case::set_run_functional_work_loop_tick(const boost::posix_time::time_duration & d_)
    // {
    //   _run_functional_work_loop_tick_ = d_;
    //   return;
    // }

    // const boost::posix_time::time_duration & base_use_case::get_run_functional_work_loop_tick() const
    // {
    //   return _run_functional_work_loop_tick_;
    // }


    bool base_use_case::is_initialized() const
    {
      return _initialized_;
    }

    void base_use_case::initialize_simple()
    {
      datatools::properties dummy_config;
      initialize(dummy_config);
      return;
    }

    void base_use_case::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Use case is already initialized!");
      // DT_THROW_IF(_run_stage_ != RUN_STAGE_UNDEF, std::logic_error,
      //             "Use case run stage is already set!");

      base_use_case::_basic_initialize_(config_);

      _at_initialize_(config_);

      base_use_case::_basic_time_calibration_(config_);

      _initialized_ = true;
      _run_stage_ = RUN_STAGE_READY;
      return;
    }

    void base_use_case::finalize()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Use case is not initialized!");
      DT_THROW_IF(_run_stage_ != RUN_STAGE_TERMINATED, std::logic_error,
                  "Use case run is not terminated!");
      _initialized_ = false;

      _at_finalize_();

      base_use_case::_basic_finalize_();

      return;
    }

    void base_use_case::run_stop_request()
    {
      std::lock_guard<std::mutex> lck(_run_stop_request_mutex_);
      _run_stop_requested_ = true;
      return;
    }

    bool base_use_case::_check_run_stop_requested() const
    {
      base_use_case * mutable_this = const_cast<base_use_case *>(this);
      std::lock_guard<std::mutex> lck(mutable_this->_run_stop_request_mutex_);
      return _run_stop_requested_;
    }

    void base_use_case::_compute_run_distributable_times_()
    {
      return;
    }

    void base_use_case::_compute_run_functional_times_()
    {
      return;
    }

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

    void base_use_case::_basic_initialize_(const datatools::properties & config_)
    {
      DT_THROW_IF(_mother_session_ == nullptr, std::logic_error,
                  "No mother session is set!");

      this->datatools::enriched_base::initialize(config_, false);

      return;
    }

    void base_use_case::_basic_finalize_()
    {
      _run_stage_ = RUN_STAGE_TERMINATED;
      vire::time::invalidate_time_duration(_distributable_up_max_duration_);
      vire::time::invalidate_time_duration(_distributable_down_max_duration_);
      vire::time::invalidate_time_duration(_functional_up_max_duration_);
      vire::time::invalidate_time_duration(_functional_work_min_duration_);
      vire::time::invalidate_time_duration(_functional_work_max_duration_);
      vire::time::invalidate_time_duration(_functional_down_max_duration_);
      _minimal_role_.reset();
      _mother_session_ = nullptr;

      this->datatools::enriched_base::reset();
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

    bool base_use_case::is_run_ready() const
    {
      return _run_stage_ == RUN_STAGE_READY;
    }

    bool base_use_case::is_run_terminated() const
    {
      return _run_stage_ == RUN_STAGE_TERMINATED;
    }

    // bool base_use_case::is_run_broken() const
    // {
    //   return _run_broken_;
    // }

    // void base_use_case::set_run_broken(const bool b_)
    // {
    //   _run_broken_ = b_;
    //   return;
    // }

    /* Actions */

    base_use_case::stage_completion base_use_case::run_prepare()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Use case is not initialized!");
      DT_THROW_IF(_run_stage_ != RUN_STAGE_READY, std::logic_error,
                  "Use case cannot prepare for running!");
      stage_completion completion;
      try {
        _run_stage_ = RUN_STAGE_PREPARING;
        _at_run_prepare_();
        completion.run_termination = RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now();
        completion.run_stage = _run_stage_;
        _run_stage_ = RUN_STAGE_PREPARED;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "unsupported exception";
      }
      return completion;
    }

    base_use_case::stage_completion base_use_case::run_distributable_up()
    {
      DT_THROW_IF(_run_stage_ != RUN_STAGE_PREPARED, std::logic_error,
                  "Use case is not prepared!");
      stage_completion completion;
      try {
        _run_stage_ = RUN_STAGE_DISTRIBUTABLE_UP_RUNNING;
        _at_run_distributable_up_();
        completion.run_termination = RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now();
        completion.run_stage = _run_stage_;
        _run_stage_ = RUN_STAGE_DISTRIBUTABLE_UP_DONE;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "unsupported exception";
      }
      return completion;
    }

    base_use_case::stage_completion base_use_case::run_functional_up()
    {
      DT_THROW_IF(_run_stage_ != RUN_STAGE_DISTRIBUTABLE_UP_DONE, std::logic_error,
                  "Distributable up is not done!");
      stage_completion completion;
      try {
        _run_stage_ = RUN_STAGE_FUNCTIONAL_UP_RUNNING;
        _at_run_functional_up_();
        completion.run_termination = RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now();
        _run_stage_ = RUN_STAGE_FUNCTIONAL_UP_DONE;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "unsupported exception";
      }
      return completion;
    }

    void base_use_case::_run_functional_work_loop_status_continue()
    {
      _run_functional_work_loop_status_ = RUN_FUNCTIONAL_WORK_LOOP_ITERATE;
      return;
    }

    void base_use_case::_run_functional_work_loop_status_stop()
    {
      _run_functional_work_loop_status_ = RUN_FUNCTIONAL_WORK_LOOP_STOP;
      return;
    }

    void base_use_case::_run_work_init_()
    {
      return;
    }

    void base_use_case::_run_work_begin_()
    {
      // run_report_record * record = nullptr;
      // run_report_record_dict_type::iterator found = _run_reports_.find(_stage_);
      // if (found == _run_reports_.end()) {
      //   run_report_record new_record = run_report_record;
      //   new_record.start_stop = boost::posix_time::time_period(vire::time::now(),
      //                                                                 vire::time::invalid_time());
      //   _run_reports_[_stage_] = new_record;
      // } else {
      //   record = &found->second;
      // }
      return;
    }

    void base_use_case::_run_work_end_()
    {
      return;
    }

    void base_use_case::_run_work_terminate_()
    {
      return;
    }

    base_use_case::stage_completion base_use_case::run_functional_work()
    {
      DT_THROW_IF(_run_stage_ != RUN_STAGE_FUNCTIONAL_UP_DONE, std::logic_error,
                  "Functional up is not done!");
      _run_functional_work_loop_status_continue();
      stage_completion completion;
      try {
        _run_stage_ = RUN_STAGE_FUNCTIONAL_WORK_RUNNING;
        // LOOP STAT INIT/RESET
        while (_run_functional_work_loop_status_ == RUN_FUNCTIONAL_WORK_LOOP_ITERATE) {
          _run_functional_work_loop_status_stop();
          // LOOP STAT LAST BEGIN
          //_run_work_begin_(),
          _at_run_functional_work_loop_iteration_();
          //_run_work_end_(),
          // LOOP STAT LAST END
          // LOOP STAT UPDATE
          // STAT_run_functional_work_loop_stat_.loop_count++;
        }
        // (LOOP STAT FINI)
        completion.run_termination = RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now();
        _run_stage_ = RUN_STAGE_FUNCTIONAL_WORK_DONE;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "unsupported exception";
      }
      return completion;
    }

    base_use_case::stage_completion base_use_case::run_functional_down()
    {
      DT_THROW_IF(_run_stage_ != RUN_STAGE_FUNCTIONAL_WORK_DONE, std::logic_error,
                  "Functional work is not done!");
      stage_completion completion;
      try {
        _run_stage_ = RUN_STAGE_FUNCTIONAL_DOWN_RUNNING;
        _at_run_functional_down_();
        completion.run_termination = RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now();
        _run_stage_ = RUN_STAGE_FUNCTIONAL_DOWN_DONE;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "unsupported exception";
      }
      return completion;
    }

    base_use_case::stage_completion base_use_case::run_distributable_down()
    {
      DT_THROW_IF(_run_stage_ != RUN_STAGE_FUNCTIONAL_DOWN_DONE, std::logic_error,
                  "Functional down is not done!");
      stage_completion completion;
      try {
        _run_stage_ = RUN_STAGE_DISTRIBUTABLE_DOWN_RUNNING;
        _at_run_distributable_down_();
        completion.run_termination = RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now();
        _run_stage_ = RUN_STAGE_DISTRIBUTABLE_DOWN_DONE;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "unsupported exception";
      }
      return completion;
    }

    base_use_case::stage_completion base_use_case::run_terminate()
    {
      // DT_THROW_IF(!_run_interrupt_
      //             && _run_stage_ != RUN_STAGE_DISTRIBUTABLE_DOWN_DONE,
      //             std::logic_error,
      //             "Distributable down is not done while run !");
      // SOME CHECK HERE!!!
      stage_completion completion;
      try {
        _run_stage_ = RUN_STAGE_TERMINATING;
        _at_run_terminate_();
        completion.run_termination = RUN_TERMINATION_NORMAL;
        completion.timestamp = vire::time::now();
        _run_stage_ = RUN_STAGE_TERMINATED;
      } catch (base_exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.error_class_id = "vire::cmsserver::base_use_case::exception";
        x.export_error_data(completion.error_data);
      } catch (std::exception & x) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "std::exception";
        completion.error_data.put("what", x.what());
      } catch (...) {
        completion.timestamp = vire::time::now();
        completion.run_termination = RUN_TERMINATION_ERROR;
        completion.run_stage = _run_stage_;
        completion.error_class_id = "unsupported exception";
      }
      return completion;
    }

    void base_use_case::_at_run_prepare_()
    {
      return;
    }

    void base_use_case::_at_run_distributable_up_()
    {
      return;
    }

    void base_use_case::_at_run_functional_up_()
    {
      return;
    }

    void base_use_case::_at_run_functional_work_loop_iteration_()
    {
      // if (has_functional_work_min_duration()) {
      //   boost::posix_time::time_duration d = get_functional_work_min_duration();
      //   for (long tick = 1; tick <= d.seconds(); tick++) {
      //     std::this_thread::sleep_for(std::chrono::seconds(1));
      //     DT_LOG_DEBUG(get_logging_priority(),
      //                  "Elapsed = " << tick << '/'<< _functional_work_time_sec_ << " sec");
      //   }
      // }
      // _run_functional_work_loop_status_continue();
      return;
    }

    void base_use_case::_at_run_functional_down_()
    {
      return;
    }

    void base_use_case::_at_run_distributable_down_()
    {
      return;
    }

    void base_use_case::_at_run_terminate_()
    {
      return;
    }

    // virtual
    void base_use_case::print_tree(std::ostream & out_,
                                   const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      datatools::enriched_base::tree_dump(out_, popts.title, popts.indent, true);

      // if (!popts.title.empty()) {
      //   out_ << popts.indent << popts.title << std::endl;
      // }

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Role expression  : ";
      out_ << "'" << _role_expression_ << "'";
      out_ << std::endl;

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

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Initialized : " << std::boolalpha << is_initialized()
           << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Run stage : '" << run_stage_label(_run_stage_) << "'"
           << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::tag
           << "Mother session : ";
      if (has_mother_session()) {
        out_ << _mother_session_;
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
           << "Run stage : ";
      out_ << _run_stage_;
      out_ << std::endl;

      return;
    }

  } // namespace cmsserver

} // namespace vire
