//! \file  vire/cmsserver/temperature_monitoring_use_case.h
//! \brief Vire CMS temperature monitoring use case
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

#ifndef VIRE_CMSSERVER_TEMPERATURE_MONITORING_USE_CASE_H
#define VIRE_CMSSERVER_TEMPERATURE_MONITORING_USE_CASE_H


// This project:
#include <vire/cmsserver/base_use_case.h>
#include <vire/monitoring/utils.h>
#include <vire/time/utils.h>
#include <vire/cms/resource_status_record.h>

namespace vire {

  namespace cmsserver {
    
    /// \brief Temperature monitor (leaf) use case with alarm
    class temperature_monitoring_use_case
      : public base_use_case
    {
    public:
    
      //! Default constructor
      temperature_monitoring_use_case();

      //! Destructor
      virtual ~temperature_monitoring_use_case();

      void set_collect_mode(const vire::monitoring::data_collection_mode);

      vire::monitoring::data_collection_mode get_collect_mode() const;

      bool is_polling() const;

      bool is_pubsub() const;
      
      bool has_alarm_min() const;
      
      void set_alarm_min(const double min_alarm_temperature_);

      double get_alarm_min() const;

      bool has_alarm_max() const;
        
      void set_alarm_max(const double max_alarm_temperature_);
  
      double get_alarm_max() const;

      bool has_polling_period() const;
 
      void set_polling_period(const boost::posix_time::time_duration & period_);

      const boost::posix_time::time_duration & get_polling_period() const;
   
      //! Smart print
      void print_tree(std::ostream & out_ = std::clog,
                      const boost::property_tree::ptree & options_
                      = datatools::i_tree_dumpable::empty_options()) const override;
     
    private:

      // Construction:
      
      void _set_defaults_();

      void _build_resource_requirements_(parametrised_resource_specifications & specs_) const override;
 
      void _at_initialize_(const datatools::properties & config_) override;

      void _at_finalize_() override;

      // Running:
      
      void _at_run_up_() override;
      
      running::run_work_loop_status_type
      _at_run_work_loop_iteration_() override;

      void _poll_value_();
      
      void _on_value_change_(const double new_value_);

      void _send_alarm_();

    private:

      // Configuration:
      vire::monitoring::data_collection_mode _collect_mode_ = vire::monitoring::DATA_COLLECTION_UNDEF;
      boost::posix_time::time_duration _polling_period_;
      double _alarm_min_;
      double _alarm_max_;

      // Working data:
      vire::cms::resource_status_record _status_;
      double _last_value_;

      VIRE_USE_CASE_REGISTRATION_INTERFACE(temperature_monitoring_use_case)
      
    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_TEMPERATURE_MONITORING_USE_CASE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
