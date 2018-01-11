// vire/time/utils.cc - Implementation of Vire time utilities
//
// Copyright (c) 2015-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/time/utils.h>

// Standard library:
#include <cstddef>
#include <sstream>
#include <memory>

// Third party:
// - Boost:
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
// - Bayeux/datatools:
#include <datatools/units.h>
#include <datatools/clhep_units.h>

namespace vire {

  namespace time {

    // POSIX time point:

    const boost::posix_time::ptime & invalid_time()
    {
      static std::unique_ptr<boost::posix_time::ptime> _it;
      if (_it.get() == nullptr) {
        _it.reset(new boost::posix_time::ptime(boost::posix_time::not_a_date_time));
      }
      return *_it.get();
    }

    bool is_valid(const boost::posix_time::ptime & t_)
    {
      return t_ != invalid_time();
    }

    void invalidate_time(boost::posix_time::ptime & t_)
    {
      t_ = invalid_time();
      return;
    }

    void invalidate(boost::posix_time::ptime & t_)
    {
      invalidate_time(t_);
      return;
    }

    std::string to_string(const boost::posix_time::ptime & t_)
    {
      return boost::posix_time::to_iso_string(t_);
    }

    bool parse_time(const std::string & token_, boost::posix_time::ptime & t_)
    {
      t_ = boost::posix_time::not_a_date_time;
      std::string repr = token_;
      boost::algorithm::trim(repr);
      if (repr == "now") {
        t_ = boost::posix_time::second_clock::local_time();
      } else if (repr == "never" || repr == "not-a-date-time") {
        t_ = boost::posix_time::not_a_date_time;
      } else if (repr == "min" || repr == "min-date-time") {
        t_ = boost::posix_time::min_date_time;
      } else if (repr == "max" || repr == "max-date-time") {
        t_ = boost::posix_time::max_date_time;
      } else if (repr == "inf" || repr == "infinity" || repr == "+infinity") {
        t_ = boost::posix_time::pos_infin;
      } else if (repr == "-inf" || repr == "-infinity") {
        t_ = boost::posix_time::neg_infin;
      } else {
        try {
          t_ = boost::posix_time::time_from_string(repr);
          if (!is_valid(t_)) {
            return false;
          }
        } catch (std::exception &) {
          t_ = boost::posix_time::not_a_date_time;
          return false;
        }
      }
      return true;
    }

    void now(boost::posix_time::ptime & t_)
    {
      t_ = boost::posix_time::microsec_clock::local_time();
      return;
    }

    boost::posix_time::ptime now()
    {
      boost::posix_time::ptime t;
      now(t);
      return t;
    }

    boost::posix_time::ptime epoch()
    {
      return boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));
    }

    /** Comparison table:
     *  \code
     *  +-------+----+------+-------+------+
     *  |   \ t1| not| -inf |normal |+inf  |
     *  | t2 \  |    |      |       |      |
     *  +-------+----+------+-------+------+
     *  |   not | NA |  NA  | NA    | NA   |
     *  +-------+----+------+-------+------+
     *  |  -inf | NA |  EQ  | GREAT | GREAT|
     *  +-------+----+------+-------+------+
     *  |normal | NA |  LESS| L/E/G | GREAT|
     *  +-------+----+------+-------+------+
     *  |  +inf | NA |  LESS| LESS  | EQ   |
     *  +-------+----+------+-------+------+
     *  \endcode
     */
    vire::utility::comparison_result compare(const boost::posix_time::ptime & t1_,
                                             const boost::posix_time::ptime & t2_)
    {
      if (t1_.is_not_a_date_time() || t2_.is_not_a_date_time()) {
        return vire::utility::COMPARISON_NOT_APPLICABLE;
      }
      if (t1_.is_neg_infinity() && t2_.is_neg_infinity()) {
        return vire::utility::COMPARISON_EQUAL;
      }
      if (t1_.is_pos_infinity() && t2_.is_pos_infinity()) {
        return vire::utility::COMPARISON_EQUAL;
      }
      if (t1_.is_neg_infinity()) {
        return vire::utility::COMPARISON_LESS;
      }
      if (t2_.is_neg_infinity()) {
        return vire::utility::COMPARISON_GREATER;
      }
      if (t1_.is_pos_infinity()) {
        return vire::utility::COMPARISON_GREATER;
      }
      if (t2_.is_pos_infinity()) {
        return vire::utility::COMPARISON_LESS;
      }
      // Both times are 'normal' :
      if (t1_ < t2_) {
        return vire::utility::COMPARISON_LESS;
      } else if (t1_ > t2_) {
        return vire::utility::COMPARISON_GREATER;
      }
      return vire::utility::COMPARISON_EQUAL;
    }

    // Time duration:

    const boost::posix_time::time_duration & invalid_time_duration()
    {
      static std::unique_ptr<boost::posix_time::time_duration> _itd;
      if (_itd.get() == nullptr) {
        _itd.reset(new boost::posix_time::time_duration(boost::posix_time::not_a_date_time));
      }
      return *_itd.get();
    }

    bool is_valid(const boost::posix_time::time_duration & td_)
    {
      return !td_.is_not_a_date_time() && !td_.is_negative();
    }

    void invalidate_time_duration(boost::posix_time::time_duration & td_)
    {
      td_ = invalid_time_duration();
      return;
    }

    void invalidate(boost::posix_time::time_duration & td_)
    {
      invalidate_time_duration(td_);
      return;
    }

    std::string to_string(const boost::posix_time::time_duration & td_)
    {
      return boost::posix_time::to_iso_string(td_);
    }

    bool parse_time_duration(const std::string & token_, boost::posix_time::time_duration & td_)
    {
      td_ = boost::posix_time::not_a_date_time;
      std::string repr = token_;
      boost::algorithm::trim(repr);
      std::istringstream iss(token_);
      if (repr == "forever" || repr == "+infinity") {
        td_ = boost::posix_time::pos_infin;
        return true;
      } else {
        try {
          td_ = boost::posix_time::duration_from_string(repr);
          if (td_.is_negative()) {
            td_ = boost::posix_time::not_a_date_time;
            return false;
          }
        } catch (std::exception &) {
          td_ = boost::posix_time::not_a_date_time;
        }
        if (td_.is_not_a_date_time()) {
          double time_duration;
          std::string time_unit_label;
          bool ok = datatools::units::find_value_with_unit(repr, time_duration, time_unit_label);
          if(!ok) {
            // Invalid time duration format:
            return false;
          }
          if (time_unit_label != "time") {
            // Invalid unit , time unit is expected:
            return false;
          }
          if (time_duration < 0.0) {
            // Invalid negative time duration :
            return false;
          }
          // if (time_duration < 1.0000001 * CLHEP::second) {
          //   // Time duration is too short:
          //   return false;
          // }
          td_ = boost::posix_time::seconds((int) (time_duration / CLHEP::second));
        }
      }
      if (td_.total_seconds() < 1) {
        td_ = boost::posix_time::not_a_date_time;
        // Time duration is too short :
        return false;
      }
      return true;
    }

    // Time period:

    const boost::posix_time::time_period & invalid_time_interval()
    {
      static std::unique_ptr<boost::posix_time::time_period> _iti;
      if (_iti.get() == nullptr) {
        boost::gregorian::date d1(1970, 1, 31);
        boost::gregorian::date d2(1970, 1, 30);
        boost::posix_time::ptime t1(d1);
        boost::posix_time::ptime t2(d2);
        _iti.reset(new boost::posix_time::time_period(t1, t2));
      }
      return *_iti.get();
    }

    bool is_valid(const boost::posix_time::time_period & ti_)
    {
      return !ti_.is_null();
    }

    void invalidate_time_interval(boost::posix_time::time_period & ti_)
    {
      ti_ = invalid_time_interval();
      return;
    }

    void invalidate(boost::posix_time::time_period & ti_)
    {
      invalidate_time_interval(ti_);
      return;
    }

    // [ start / stop ]
    // ( start / duration )
    bool parse_time_interval(const std::string & token_, boost::posix_time::time_period & ti_)
    {
      ti_ = invalid_time_interval();
      boost::posix_time::ptime start_time(boost::posix_time::not_a_date_time);
      boost::posix_time::ptime stop_time(boost::posix_time::not_a_date_time);
      boost::posix_time::time_duration duration(boost::posix_time::not_a_date_time);
      std::string repr = token_;
      boost::algorithm::trim(repr);
      if (repr.length() < 3) {
        return false;
      }
      char open = repr[0];
      char close = repr[repr.length()-1];

      bool parse_start_stop = false;
      bool parse_start_duration = false;

      if (open == '[' && close == ']') {
        parse_start_stop = true;
      }
      if (open == '(' && close == ')') {
        parse_start_duration = true;
      }
      if (! parse_start_stop && !parse_start_duration) {
        return false;
      }
      std::string inner = repr.substr(1, repr.length() - 2);

      //std::cerr << "DEVEL: parse_time_interval: inner='" << inner << "'" << std::endl;

      std::vector<std::string> inner_tokens;
      if (parse_start_stop) {
        boost::algorithm::split(inner_tokens, inner, boost::is_any_of("/"));
      } else {
        boost::algorithm::split(inner_tokens, inner, boost::is_any_of(";"));
      }
      if (inner_tokens.size() == 2) {
        std::string tok0 = inner_tokens[0];
        boost::algorithm::trim(tok0);
        if (tok0.length() == 0) {
          tok0 = "-infinity";
        }
        //std::cerr << "DEVEL: parse_time_interval: tok0='" << tok0 << "'" << std::endl;
        if (!parse_time(tok0, start_time)) {
          return false;
        }
        std::string tok1 = inner_tokens[1];
        boost::algorithm::trim(tok1);
        if (parse_start_stop) {
          if (tok1.length() == 0) {
            tok1 = "+infinity";
          }
          //std::cerr << "DEVEL: parse_time_interval: tok1='" << tok1 << "'" << std::endl;
          if (!parse_time(tok1, stop_time)) {
            return false;
          }
          //std::cerr << "DEVEL: parse_time_interval: start_time = " << boost::posix_time::to_simple_string(start_time) << std::endl;
          //std::cerr << "DEVEL: parse_time_interval: stop_time  = " << boost::posix_time::to_simple_string(stop_time) << std::endl;
          ti_ = boost::posix_time::time_period(start_time, stop_time);
          if (!is_valid(start_time) && !is_valid(stop_time)) {
            return true;
          }
        } else {
          boost::posix_time::time_duration period_duration;
          if (tok1.length() == 0) {
            tok1 = "+infinity";
          }
          // std::cerr << "DEVEL: parse_time_interval: duration tok1='" << tok1 << "'" << std::endl;
          if (!parse_time_duration(tok1, period_duration)) {
            return false;
          }
          // std::cerr << "DEVEL: parse_time_interval: start_time = " << boost::posix_time::to_simple_string(start_time) << std::endl;
          // std::cerr << "DEVEL: parse_time_interval: period_duration = " << boost::posix_time::to_simple_string(period_duration) << std::endl;
          ti_ = boost::posix_time::time_period(start_time, period_duration);
          // std::cerr << "DEVEL: parse_time_interval: ti_ = " << boost::posix_time::to_simple_string(ti_) << std::endl;
        }
      }
      if (!is_valid(ti_)) {
        return false;
      }
      return true;
    }

    std::string to_string(const boost::posix_time::time_period & tp_)
    {
      std::ostringstream out;
      out << '[' << to_string(tp_.begin()) << "/" << to_string(tp_.end()) << "]";
      return out.str();
    }

    void initialize(const datatools::properties & config_, boost::posix_time::time_period  & ti_)
    {
      boost::posix_time::ptime start_time(boost::posix_time::not_a_date_time);
      boost::posix_time::ptime stop_time(boost::posix_time::not_a_date_time);
      boost::posix_time::time_duration duration(boost::posix_time::not_a_date_time);

      if (config_.has_key("start")) {
        std::string start_str = config_.fetch_string("start");
        DT_THROW_IF(!parse_time(start_str, start_time), std::logic_error, "Invalid start time format!");
      } else {
        // Default value:
        start_time = boost::posix_time::second_clock::local_time();
      }

      if (config_.has_key("stop")) {
        std::string stop_str = config_.fetch_string("stop");
        DT_THROW_IF(!parse_time(stop_str, stop_time), std::logic_error, "Invalid stop time format!");
        ti_ = boost::posix_time::time_period(start_time, stop_time);
      } else {
        if (config_.has_key("duration")) {
          std::string duration_str = config_.fetch_string("duration");
          DT_THROW_IF(!parse_time_duration(duration_str, duration), std::logic_error, "Invalid time duration format!");
          ti_ = boost::posix_time::time_period(start_time, duration);
        }
      }
      DT_THROW_IF(!is_valid(ti_), std::logic_error, "Invalid time period!");
      return;
    }

    // vire::utility::comparison_result
    // compare(const boost::posix_time::time_period & i1_,
    //      const boost::posix_time::time_period & i2_)
    // {
    //   int comp = vire::utility::COMPARISON_NO_APPLICABLE;
    //   if (!is_valid(i1_) || !is_valid(i2)) {
    //  return vire::utility::COMPARISON_NO_APPLICABLE;
    //   }
    //   return comp;
    // }

  } // namespace time

} // namespace vire
