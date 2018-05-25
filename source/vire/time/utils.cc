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
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
// - Bayeux/datatools:
#include <datatools/units.h>
#include <datatools/clhep_units.h>

// Useful links:
// - https://www.boost.org/doc/libs/1_67_0/doc/html/date_time/posix_time.html 
// - https://stackoverflow.com/questions/4910373/interoperability-between-boostdate-time-and-stdchrono#4918873
// - https://howardhinnant.github.io/date/date.html
// - https://stackoverflow.com/questions/11200763/when-is-it-appropriate-to-use-a-c11-until-timeout-function-instead-of-the-co
// - https://www.informit.com/articles/article.aspx?p=1881386&seqNum=2

/*
namespace {
  
  boost::posix_time::ptime convert_chrono_to_boost(const std::chrono::system_clock::time_point & from_) 
  { 
    typedef std::chrono::system_clock::time_point time_point_t; 
    typedef std::chrono::nanoseconds duration_t; 
    typedef duration_t::rep rep_t; 
    rep_t d = std::chrono::duration_cast<duration_t>(from_.time_since_epoch()).count(); 
    rep_t sec = d / 1000000000; 
    rep_t nsec = d % 1000000000; 
    return boost::posix_time::from_time_t(0)+ 
      boost::posix_time::seconds(static_cast<long>(sec))+ 
#ifdef BOOST_DATE_TIME_HAS_NANOSECONDS 
      boost::posix_time::nanoseconds(nsec); 
#else 
    boost::posix_time::microseconds((nsec+500)/1000); 
#endif 
  } 

  std::chrono::system_clock::time_point convert_boost_to_chrono(const boost::posix_time::ptime & from_) 
  { 
    boost::posix_time::time_duration const time_since_epoch = from_ - boost::posix_time::from_time_t(0); 
    std::chrono::system_clock::time_point t = std::chrono::system_clock::from_time_t(time_since_epoch.total_seconds()); 
    long nsec = time_since_epoch.fractional_seconds() * (1000000000 / time_since_epoch.ticks_per_second()); 
    return t + std::chrono::nanoseconds(nsec); 
  }
  
}
*/

namespace vire {

  namespace time {

    // ----------- time point conversions -----------

    /*
    maybe_std_time_point to_std(const boost::posix_time::ptime & from_)
    {
      maybe_std_time_point otp = boost::none;
      if (is_valid(from_)) {
        if (from_ == boost::posix_time::ptime(boost::posix_time::min_date_time)) {
          otp = std::chrono::system_clock::time_point::min();
        } else if (from_ == boost::posix_time::ptime(boost::posix_time::max_date_time)) {
          otp = std::chrono::system_clock::time_point::max();
        } else if (from_ == boost::posix_time::ptime(boost::posix_time::neg_infin)) {
          otp = std::chrono::system_clock::time_point::min();
        } else if (from_ == boost::posix_time::ptime(boost::posix_time::pos_infin)) {
          otp = std::chrono::system_clock::time_point::max();
        } else {
          otp = convert_boost_to_chrono(from_);
        }
      }
      return otp;
    }
 
    bool from_std(const std::chrono::system_clock::time_point & from_,
                  boost::posix_time::ptime & to_)
    {
      to_ = convert_chrono_to_boost(from_);
      return true;
    }
    */
    
    // ----------- date operations -----------

    const boost::gregorian::date & invalid_date()
    {
      std::unique_ptr<boost::gregorian::date> _invalid_date;
      if (_invalid_date.get() == nullptr) {
        _invalid_date.reset(new boost::gregorian::date(boost::date_time::not_a_date_time));
      }
      return *_invalid_date.get();
    }

    boost::gregorian::date date_today_utc()
    {
      // Methods of interest:
      // - boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
      // - boost::gregorian::date today = now.date();
      // - boost::gregorian::date todayDateInLocalTZ = boost::gregorian::day_clock::local_day();
      boost::gregorian::date todayDateInUTCTZ = boost::gregorian::day_clock::universal_day();
      return todayDateInUTCTZ;
    }
  
    boost::gregorian::date date_epoch()
    {
      return boost::gregorian::date(1970, 1, 1);
    }
    
    // ----------- time point operations -----------
    
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
        t_ = boost::posix_time::second_clock::universal_time();
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
      now_utc(t_);
      return;
    }
    
    void now_utc(boost::posix_time::ptime & t_)
    {
      t_ = boost::posix_time::microsec_clock::universal_time();
      return;
    }

    boost::posix_time::ptime now()
    {
      return now_utc();
    }
 
    boost::posix_time::ptime now_utc()
    {
      boost::posix_time::ptime t;
      now_utc(t);
      return t;
    }

    boost::posix_time::ptime epoch()
    {
      return boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));
    }

    void make(boost::posix_time::ptime & time_point_,
              const unsigned int year_,
              const unsigned int month_,
              const unsigned int day_,
              const unsigned int hours_,
              const unsigned int minutes_,
              const unsigned int seconds_,
              const unsigned int microseconds_)
    {
      boost::gregorian::date d(year_, month_, day_);
      boost::posix_time::time_duration t
        = boost::posix_time::hours(hours_)
        + boost::posix_time::minutes(minutes_)
        + boost::posix_time::seconds(seconds_)
        + boost::posix_time::microseconds(microseconds_);
      time_point_ = boost::posix_time::ptime(d, t);
      return;
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
   
    // ----------- time duration operations -----------

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
      return boost::posix_time::to_simple_string(td_);
      //return boost::posix_time::to_iso_extended_string(td_);
    }

    boost::posix_time::time_duration make_duration(const unsigned int hours_,
                                                   const unsigned int minutes_,
                                                   const unsigned int seconds_,
                                                   const unsigned int microseconds_)
    {
      unsigned int fractional_seconds = microseconds_;
      unsigned int microseconds_count = microseconds_;
      unsigned int fractional_count
        = microseconds_count * (boost::posix_time::time_duration::ticks_per_second()/1000000);
      boost::posix_time::time_duration td(hours_,
                                          minutes_,
                                          seconds_,
                                          fractional_count);
      return td;
      return boost::posix_time::time_duration(boost::date_time::not_a_date_time);
    }

    bool parse_positive_time_duration(const std::string & token_, boost::posix_time::time_duration & td_)
    {
      bool debug = false;
      if (debug) std::clog << "[debug] parse_time_duration: token_ = '" << token_ << "'" << std::endl;
      td_ = boost::posix_time::not_a_date_time;
      std::string repr = token_;
      boost::algorithm::trim(repr);
      if (debug) std::clog << "[debug] repr = '" << repr << "'" << std::endl;
      if (repr == "never"
          || repr == "not-a-time-duration"
          || repr == "not-a-date-time"
          || repr == "none"
          || repr == "invalid") {
        return true;
      }
      
      if (repr == "forever" || repr == "+infinity") {
        td_ = boost::posix_time::pos_infin;
        if (debug) std::clog << "[debug] Success: +infinity" << std::endl;
        return true;
      } 
      
      {
        std::istringstream iss(repr);
        std::string token;
        iss >> token >> std::ws;
        if (debug) std::clog << "[debug] token = '" << token << "'" << std::endl;
        try {
          boost::posix_time::time_duration attempt(boost::posix_time::duration_from_string(token));
          std::string more;
          iss >> more >> std::ws;
          if (debug) std::clog << "[debug] more = '" << more << "'" << std::endl;
          if (more.empty()) {
            // No trailing chars:
            if (attempt >= boost::posix_time::time_duration(0, 0, 0)) {
              // Why not use: !td_.is_negative()
              // Only positive or null time duration:
              td_ = attempt;
              if (debug) std::clog << "[debug] Success..." << std::endl;
              return true;
            }
          }
        } catch (std::exception & error) {
          if (debug) std::clog << "[debug] Failure..." << std::endl;
        } catch (...) {
          if (debug) std::clog << "[debug] Failure..." << std::endl;
        }
      }
     
      if (debug) std::clog << "[debug] Trying more formats..." << std::endl;
      {
        if (td_.is_not_a_date_time()) {
          double time_duration;
          std::string time_unit_label;
          bool ok = datatools::units::find_value_with_unit(repr, time_duration, time_unit_label);
          if(!ok) {
            // Invalid time duration format:
            return false;
          }
          if (time_unit_label != "time") {
            // Invalid unit, time unit is expected:
            return false;
          }
          if (time_duration < 0.0 * CLHEP::microsecond) {
            // Invalid negative time duration :
            return false;
          }
          double time_unit_usec = time_duration / CLHEP::microsecond;
          td_ = boost::posix_time::microseconds((long) time_unit_usec);
          return true;
        }
      }
      // if (td_.total_microseconds() < 1) {
      //   td_ = boost::posix_time::not_a_date_time;
      //   // Time duration is too short :
      //   return false;
      // }
      return false;
    }

    // ----------- time period operations -----------

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
          if (!parse_positive_time_duration(tok1, period_duration)) {
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
          DT_THROW_IF(!parse_positive_time_duration(duration_str, duration), std::logic_error, "Invalid time duration format!");
          ti_ = boost::posix_time::time_period(start_time, duration);
        }
      }
      DT_THROW_IF(!is_valid(ti_), std::logic_error, "Invalid time period!");
      return;
    }

  } // namespace time

} // namespace vire
