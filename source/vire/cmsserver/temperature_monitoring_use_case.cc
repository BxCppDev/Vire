//! \file vire/cmsserver/temperature_monitoring_use_case.cc
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
#include <vire/cmsserver/temperature_monitoring_use_case.h>

// Standard library:
#include <chrono>
#include <thread>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/units.h>
#include <bayeux/datatools/exception.h>

// This project:
#include <vire/time/rt_utils.h>

namespace vire {

  namespace cmsserver {

    VIRE_USE_CASE_REGISTRATION_IMPLEMENT(temperature_monitoring_use_case,
                                         "vire::cmsserver::temperature_monitoring_use_case")

    temperature_monitoring_use_case::temperature_monitoring_use_case()
    {
      _set_defaults_();
      return;
    }
 
    temperature_monitoring_use_case::~temperature_monitoring_use_case()
    {
      return;
    }

    void temperature_monitoring_use_case::_set_defaults_()
    {
      _collect_mode_ = vire::monitoring::DATA_COLLECTION_UNDEF;
      vire::time::invalidate(_polling_period_);
      datatools::invalidate(_alarm_min_);
      datatools::invalidate(_alarm_max_);
      // _status_.reset();
      datatools::invalidate(_last_value_);
      return;
    }
 
    void temperature_monitoring_use_case::set_collect_mode(const vire::monitoring::data_collection_mode mode_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Use case is initialized and locked!");
      _collect_mode_ = mode_;
      return;  
    }

    vire::monitoring::data_collection_mode temperature_monitoring_use_case::get_collect_mode() const
    {
      return _collect_mode_;
    }

    bool temperature_monitoring_use_case::is_polling() const
    {
      return _collect_mode_ == vire::monitoring::DATA_COLLECTION_POLLING;
    }

    bool temperature_monitoring_use_case::is_pubsub() const
    {
      return _collect_mode_ == vire::monitoring::DATA_COLLECTION_PUBSUB;
    }

    bool temperature_monitoring_use_case::has_alarm_min() const
    {
      return datatools::is_valid(_alarm_min_);
    }
      
    void temperature_monitoring_use_case::set_alarm_min(const double min_alarm_temperature_)
    {
      DT_THROW_IF(has_alarm_max() && min_alarm_temperature_ >= _alarm_max_,
                  std::logic_error,
                  "Invalid alarm min/max range!");
      _alarm_min_ = min_alarm_temperature_;
      return;
    }

    double temperature_monitoring_use_case::get_alarm_min() const
    {
      return _alarm_min_;
    }

    bool temperature_monitoring_use_case::has_alarm_max() const
    {
      return datatools::is_valid(_alarm_max_);
    }
       
    void temperature_monitoring_use_case::set_alarm_max(const double max_alarm_temperature_)
    {
      DT_THROW_IF(has_alarm_min() && max_alarm_temperature_ <= _alarm_min_,
                  std::logic_error,
                  "Invalid alarm min/max range!");
      _alarm_max_ = max_alarm_temperature_;
      return;
    }

    double temperature_monitoring_use_case::get_alarm_max() const
    {
      return _alarm_max_;
    }

    bool temperature_monitoring_use_case::has_polling_period() const
    {
    }

    void temperature_monitoring_use_case::set_polling_period(const boost::posix_time::time_duration & period_)
    {
      DT_THROW_IF(_collect_mode_ == vire::monitoring::DATA_COLLECTION_PUBSUB,
                  std::logic_error,
                  "Cannot set polling period in Pub/Sub mode!");
      DT_THROW_IF(period_.is_negative() || period_.total_seconds() < 1,
                  std::domain_error,
                  "Invalid polling period time duration!");
      _polling_period_ = period_;
      return;
    }
  
    const boost::posix_time::time_duration & temperature_monitoring_use_case::get_polling_period() const
    {
      return _polling_period_;
    }

    void temperature_monitoring_use_case::
    _build_resource_requirements_(parametrised_resource_specifications & specs_) const
    {
      // A resource returning a temperature value is required:
      specs_.add_resource("T",
                          vire::resource::RESOURCE_CATEGORY_DATAPOINT,
                          vire::resource::ACCESS_READ_ONLY,
                          datatools::introspection::DATA_TYPE_DOUBLE,
                          "temperature");
      specs_.lock(); // And nothing more.
      return;
    }
 
    void temperature_monitoring_use_case::_at_initialize_(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());

      if (_collect_mode_ == vire::monitoring::DATA_COLLECTION_UNDEF) {
        if (config_.has_key("collect.mode")) {
          std::string mode_repr = config_.fetch_string("collect.mode");
          vire::monitoring::data_collection_mode mode = vire::monitoring::data_collection_mode_from_string(mode_repr);
          DT_THROW_IF(mode == vire::monitoring::DATA_COLLECTION_UNDEF,
                      std::logic_error,
                      "Invalid data collection mode '" << mode_repr << "'!");
          set_collect_mode(mode);
        }
      }
      
      if (_collect_mode_ == vire::monitoring::DATA_COLLECTION_UNDEF) {
        // Default data collection mode:
        set_collect_mode(vire::monitoring::DATA_COLLECTION_POLLING);
      }
      
      if (!has_alarm_min()) {
        if (config_.has_key("alarm.min")) {
          double t_min = config_.fetch_real_with_explicit_dimension("alarm.min", "temperature");
          DT_THROW_IF(!datatools::is_valid(t_min) || t_min <= 0.0,
                      std::logic_error,
                      "Invalid alarm minimum temperature!");
          set_alarm_min(t_min);
        }
      }

      if (!has_alarm_max()) {
        if (config_.has_key("alarm.max")) {
          double t_max = config_.fetch_real_with_explicit_dimension("alarm.max", "temperature");
          DT_THROW_IF(!datatools::is_valid(t_max) || t_max <= 0.0,
                      std::logic_error,
                      "Invalid alarm maximum temperature!");
          set_alarm_max(t_max);
        }
      }

      if (is_polling() && !has_polling_period()) {
        if (config_.has_key("polling.period")) {
          double period_value = config_.fetch_real_with_explicit_dimension("polling.period", "time");
          DT_THROW_IF(!datatools::is_valid(period_value) || period_value <= 1.0 * CLHEP::second,
                      std::logic_error,
                      "Invalid polling period!");
          int nmilliseconds = (int) (period_value * 1000);
          boost::posix_time::time_duration period = boost::posix_time::milliseconds(nmilliseconds);
          set_polling_period(period);
        }
      }
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
  
    void temperature_monitoring_use_case::_at_finalize_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void temperature_monitoring_use_case::_poll_value_()
    {
      // Fake read of the value
      double new_value = 300.0 * CLHEP::kelvin;
      if (datatools::is_valid(_last_value_) && _last_value_ != new_value) {
        _on_value_change_(new_value);
      }
      
      return;
    }
    
    void temperature_monitoring_use_case::_on_value_change_(const double new_value_)
    {
      if (has_alarm_min() && new_value_ < _alarm_min_) {
        _send_alarm_();
      }
      if (has_alarm_max() && new_value_ > _alarm_max_) {
        _send_alarm_();
      }
      
      _last_value_ = new_value_;
      return;
    }
  
    void temperature_monitoring_use_case::_send_alarm_()
    {
      std::cerr << "[alarm] temperature = " << _last_value_ / CLHEP::kelvin << " " << std::endl;
      return;
    }

    void temperature_monitoring_use_case::_at_run_up_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      _status_.set_path("/blah/blah/blah/temperature");
      _status_.set_missing();
      _status_.set_timestamp(vire::time::now_utc());
      _poll_value_();
      DT_LOG_TRACE_EXITING(get_logging_priority());
    }
 
    running::run_work_loop_status_type
    temperature_monitoring_use_case::_at_run_work_loop_iteration_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      running::run_work_loop_status_type ret = running::RUN_WORK_LOOP_STOP;

      vire::time::system_time_point rt_now = std::chrono::system_clock::now();
      unsigned int usecs = _polling_period_.total_microseconds();
      // Loop sleep duration
      vire::time::system_duration rt_sleep_time
        = std::chrono::microseconds(usecs);
      vire::time::system_time_point rt_next_poll = rt_now + rt_sleep_time;
  
      if (is_polling()) {
        _poll_value_();
        // Loop sleep duration
        std::this_thread::sleep_until(rt_next_poll);
      }

      // if (is_pubsub()) {
        
      // }
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return ret;
    }

      
    // virtual
    void temperature_monitoring_use_case::print_tree(std::ostream & out_,
                                                     const boost::property_tree::ptree & options_) const
    {
      {
        boost::property_tree::ptree popts
          = base_print_options::force_inheritance(options_);
        this->base_use_case::print_tree(out_, popts);
      }
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);
     
      out_ << popts.indent << tag
           << "Collect mode : "
           << vire::monitoring::data_collection_mode_to_string(_collect_mode_)
           << std::endl;

      if (is_polling()) {
        out_ << popts.indent << tag
             << "Polling period : "
             << _polling_period_
             << std::endl;
      }
      
      if (has_alarm_min()) {
        out_ << popts.indent << tag
             << "Alarm minimum temperature : "
             << _alarm_min_ / CLHEP::kelvin << " K"
             << std::endl;
      }
      
      out_ << popts.indent << inherit_tag(popts.inherit)
           << "Alarm maximum temperature : ";
      if (has_alarm_max()) {
        out_ << _alarm_max_ / CLHEP::kelvin << " K";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
 
      return;
    }

  } // namespace cmsserver

} // namespace vire
