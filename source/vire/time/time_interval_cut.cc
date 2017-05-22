// vire/time/time_interval_cut.cc - Implementation of Vire interval time cut
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Ourselves
#include <vire/time/time_interval_cut.h>

namespace vire {

  namespace time {

    // Registration instantiation macro :
    CUT_REGISTRATION_IMPLEMENT(time_interval_cut, "vire::time::time_interval_cut");

    time_interval_cut::time_interval_cut(datatools::logger::priority logger_priority_)
      : cuts::i_cut(logger_priority_) ,
        _interval_(boost::posix_time::ptime(boost::posix_time::not_a_date_time),
                   boost::posix_time::ptime(boost::posix_time::not_a_date_time))
    {
      return;
    }

    time_interval_cut::~time_interval_cut()
    {
      if (is_initialized()) {
        this->time_interval_cut::reset();
      }
      return;
    }

    int time_interval_cut::_accept()
    {
      int result = cuts::SELECTION_ACCEPTED;
      DT_LOG_TRACE(_logging, "Entering...");
      const boost::posix_time::time_period & candidate_period
        = get_user_data<boost::posix_time::time_period>();
      if (candidate_period.is_null()) {
        result = cuts::SELECTION_REJECTED;
      } else if (_interval_.is_null())  {
        result = cuts::SELECTION_REJECTED;
      } else if (! _interval_.contains(candidate_period)) {
        result = cuts::SELECTION_REJECTED;
      }
      DT_LOG_TRACE(_logging, "Exiting.");
      return result;
    }

    void time_interval_cut::initialize(const datatools::properties & config_,
                                       datatools::service_manager & /* service_manager_ */,
                                       cuts::cut_handle_dict_type & /* cut_dict_ */)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Cut '" << get_name() << "' is already initialized !");

      this->cuts::i_cut::_common_initialize(config_);

      if (! has_valid_interval()) {
        boost::posix_time::ptime start_time(boost::posix_time::not_a_date_time);
        boost::posix_time::ptime stop_time(boost::posix_time::not_a_date_time);

        if (config_.has_key("start_time")) {
          const std::string & start_time_str = config_.fetch_string("start_time");
          start_time = boost::posix_time::time_from_string(start_time_str);
        }

        if (config_.has_key("stop_time")) {
          const std::string & stop_time_str = config_.fetch_string("stop_time");
          stop_time = boost::posix_time::time_from_string(stop_time_str);
        } else if (config_.has_key("duration")) {
          const std::string & duration_str = config_.fetch_string("duration");
          boost::posix_time::time_duration duration
            = boost::posix_time::duration_from_string(duration_str);
          stop_time = start_time + duration;
        } else {
          DT_THROW(std::logic_error,
                   "Cut '" << get_name() << "' : Missing 'stop_time' or 'duration' properties!");
        }
        _interval_ = boost::posix_time::time_period(start_time, stop_time);
      }

      DT_THROW_IF(! has_valid_interval(), std::logic_error,
                  "Cut '" << get_name() << "' : Missing valid time interval!");

      _set_initialized(true);
      return;
    }

    void time_interval_cut::reset()
    {
      _set_initialized(false);
      boost::posix_time::ptime invalid_time(boost::posix_time::not_a_date_time);
      _interval_ = boost::posix_time::time_period(invalid_time, invalid_time);
      this->cuts::i_cut::_reset();
      return;
    }

    bool time_interval_cut::has_valid_interval() const
    {
      return ! _interval_.is_null();
    }

    const boost::posix_time::time_period & time_interval_cut::get_interval() const
    {
      return _interval_;
    }

    void time_interval_cut::set_interval(const boost::posix_time::time_period & interval_)
    {
      _interval_ = interval_;
      return;
    }

    void time_interval_cut::set_interval(const boost::posix_time::ptime & start_,
                                         const boost::posix_time::time_duration & duration_)
    {
      _interval_ = boost::posix_time::time_period(start_, start_ + duration_);
      return;
    }

    void time_interval_cut::tree_dump(std::ostream & out_,
                                      const std::string & title_,
                                      const std::string & indent_,
                                      bool inherit_) const
    {
      this->cuts::i_cut::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Interval : " << _interval_
           << std::endl;

      return;
    }

  } // namespace time

} // namespace vire
