//! \file  vire/testing/test-time-utils.cxx
//! \brief Vire interval time
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

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <random>
#include <chrono>
#include <typeinfo>

// This project:
#include <vire/vire.h>
#include <vire/time/utils.h>

void test_utils_0();
void test_utils_1();
// void test_utils_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::time::utils' utilities."
              << std::endl;
    test_utils_0();
    test_utils_1();
    // test_utils_2();

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return (error_code);
}

void test_utils_0()
{
  std::clog << "\ntest_utils_0: core" << std::endl;

  boost::posix_time::ptime now = vire::time::now_utc();
  std::clog << "UTC now = " << vire::time::to_string(now) << std::endl;
  std::clog << std::endl;
  
  // std::clog << "Infinty = " << std::numeric_limits<long>::infinity() << std::endl;
  // std::clog << "Min = " << std::numeric_limits<long>::min() << std::endl;
  // std::clog << "Max = " << std::numeric_limits<long>::max() << std::endl;
  // std::clog << "NaN = " << std::numeric_limits<long>::quiet_NaN() << std::endl;

  /*
   * std::chrono:
   *
   *     s_epoch                              s_now
   *    ----+-----------------------------------+----------------------------------> t
   *        :                                   :
   *        |-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-| s_from_epoch 
   *        :1      <->                      count
   *        :      period                       :
   *        :                                   :
   *        |---|---|---|---|---|---|---|---|---| s_from_epoch_us
   *        : 1                             count'
   *        :                                   :
   *        :                                   :
   *
   * boost::date_time:
   *
   *           b_epoch                        b_now
   *    ----------+-----------------------------+----------------------------------> t
   *              :                             :
   *              |-|-|-|-|-|-|-|-|-|-|-|-|-|-|-| b_from_epoch 
   *
   *
   */
  
  std::chrono::system_clock::time_point s_epoch;
  std::clog << "std::chrono Epoch From Epoch = " << s_epoch.time_since_epoch().count() << std::endl;
  std::chrono::system_clock::time_point s_now = std::chrono::system_clock::now();
  typedef std::chrono::system_clock::time_point::duration system_type;
   
  system_type s_from_epoch = s_now.time_since_epoch();
  std::clog << "Type system = " << typeid(system_type).name() << std::endl;
  
  std::clog << "std::chrono Now from Epoch = " << s_from_epoch.count() << " ticks" << std::endl;
  std::chrono::microseconds s_from_epoch_us
    = std::chrono::duration_cast<std::chrono::microseconds>(s_from_epoch);
  std::clog << "Type us     = " << typeid(std::chrono::microseconds).name() << std::endl;
  std::uint64_t s_count_us = s_from_epoch_us.count();
  std::clog << "std::chrono Now from Epoch = " << s_count_us << " us" << std::endl;

  std::clog << std::endl;
  boost::posix_time::ptime b_epoch(boost::gregorian::date(1970, boost::date_time::Jan, 1), 
                                   boost::posix_time::time_duration(0,0,0));
  std::clog << "Boost ptime at Epoch   = " << boost::posix_time::to_simple_string(b_epoch) << std::endl;
  boost::posix_time::ptime b_now = vire::time::now_utc();
  std::clog << "Boost ptime at Now     = " << boost::posix_time::to_simple_string(b_now) << std::endl;
  boost::posix_time::time_duration b_from_epoch = b_now - b_epoch;
  std::clog << "Boost Now from Epoch   = " << b_from_epoch.total_seconds() << " s" << std::endl;
  std::uint64_t b_count_us = b_from_epoch.total_microseconds();
  std::clog << "Boost Now from Epoch   = " << b_count_us << " us" << std::endl; 

  std::clog << std::endl;
  std::int64_t bs_calib_us = b_count_us - s_count_us;
  std::clog << "Boost-Chrono calib (us resolution) = " << bs_calib_us << " us" << std::endl; 
  std::int64_t bs_calib_s = bs_calib_us / 1000000;
  std::clog << "Boost-Chrono calib (s resolution)  = " << bs_calib_s << " s" << std::endl; 
  return;
}

void test_utils_1()
{
  std::clog << "\ntest_utils_1: basics" << std::endl;
  {
    boost::posix_time::ptime dt0 = vire::time::now_utc();
    boost::posix_time::ptime dt1(boost::posix_time::neg_infin);
    boost::posix_time::ptime dt2(boost::posix_time::pos_infin);
    boost::posix_time::ptime dt3(boost::posix_time::not_a_date_time);
    boost::posix_time::ptime dt4(boost::posix_time::max_date_time);
    boost::posix_time::ptime dt5(boost::posix_time::min_date_time);
    std::clog << "dt0 = " << boost::posix_time::to_simple_string(dt0) << std::endl;
    std::clog << "dt1 = " << boost::posix_time::to_simple_string(dt1) << std::endl;
    std::clog << "dt2 = " << boost::posix_time::to_simple_string(dt2) << std::endl;
    std::clog << "dt3 = " << boost::posix_time::to_simple_string(dt3) << std::endl;
    std::clog << "dt4 = " << boost::posix_time::to_simple_string(dt4) << std::endl;
    std::clog << "dt5 = " << boost::posix_time::to_simple_string(dt5) << std::endl;
    std::clog << std::endl;
  }

  boost::posix_time::ptime t0 = vire::time::invalid_time();
  if (vire::time::is_valid(t0)) {
    std::clog << "t0 is valid." << std::endl;
  } else {
    std::clog << "t0 is not valid." << std::endl;
  }

  boost::posix_time::ptime t1 = boost::posix_time::second_clock::local_time();
  if (vire::time::is_valid(t1)) {
    std::clog << "t1 is valid." << std::endl;
  } else {
    std::clog << "t1 is not valid." << std::endl;
  }

  vire::time::invalidate_time(t1);
  if (vire::time::is_valid(t1)) {
    std::clog << "t1 is still valid." << std::endl;
  } else {
    std::clog << "t1 is not valid anymore." << std::endl;
  }

  std::clog << "\nParse time:" << std::endl;
  if (vire::time::parse_time("2016-05-01 14:30:17.145212", t1)) {
    std::clog << "t1 was successfully parsed: " << boost::posix_time::to_simple_string(t1) << std::endl;
  }
  if (vire::time::parse_time("2016-05-01 00:00", t1)) {
    std::clog << "t1 was successfully parsed: " << boost::posix_time::to_simple_string(t1) << std::endl;
  }
  if (vire::time::parse_time("now", t1)) {
    std::clog << "t1 was successfully parsed: " << boost::posix_time::to_simple_string(t1) << std::endl;
  }
  if (vire::time::parse_time("never", t1)) {
    std::clog << "t1 was successfully parsed: " << boost::posix_time::to_simple_string(t1) << std::endl;
  }
  if (vire::time::parse_time("not-a-date-time", t1)) {
    std::clog << "t1 was successfully parsed: " << boost::posix_time::to_simple_string(t1) << std::endl;
  }

  std::clog << "\nTime durations:" << std::endl;
  boost::posix_time::time_duration d0 = vire::time::invalid_time_duration();
  if (vire::time::is_valid(d0)) {
    std::clog << "d0 is valid." << std::endl;
  } else {
    std::clog << "d0 is not valid." << std::endl;
  }

  std::clog << "\nParse time durations:" << std::endl;

  std::vector<std::string> tokens{{
      "never",
      "forever",
      "23:59:59.000",
      "-12:42:23.023122",
        "0:59.000123",
        "59.000123",
        "0:59.123",
        "foo",
        "12:34",
        "00:00:03.123459 s",
        "2 s",
        "120 minute",
        "forever",
        "3.21 us",
        "123.34 picosecond"
        }};

  for (const std::string & token : tokens) {
    boost::posix_time::time_duration d = vire::time::invalid_time_duration();
    std::clog << "=> Parsing '" << token << "' : " << std::endl;
    if (vire::time::parse_positive_time_duration(token, d)) {
      std::clog << " ==> duration was successfully parsed from '" << token << "' : "
                << boost::posix_time::to_simple_string(d) << std::endl;
    } else {
      std::clog << " ==> duration was not parsed from '" << token << "'!" << std::endl;
    }
  }

  /*
  if (vire::time::parse_time_duration("23:59:59.000", d0)) {
     std::clog << "d0 was successfully parsed: " << boost::posix_time::to_simple_string(d0) << std::endl;
  } else {
     std::clog << "d0 was not parsed!" << std::endl;
  }

  if (vire::time::parse_time_duration("00:00:03.123459 s", d0)) {
     std::clog << "d0 was successfully parsed: " << boost::posix_time::to_simple_string(d0) << std::endl;
  } else {
     std::clog << "d0 was not parsed!" << std::endl;
  }

  if (vire::time::parse_time_duration("2 s", d0)) {
    std::clog << "d0 was successfully parsed: " << boost::posix_time::to_simple_string(d0) << std::endl;
  } else {
     std::clog << "d0 was not parsed!" << std::endl;
  }

  if (vire::time::parse_time_duration("120 minute", d0)) {
    std::clog << "d0 was successfully parsed: " << boost::posix_time::to_simple_string(d0) << std::endl;
  } else {
     std::clog << "d0 was not parsed!" << std::endl;
  }

  if (vire::time::parse_time_duration("forever", d0)) {
    std::clog << "d0 was successfully parsed: " << boost::posix_time::to_simple_string(d0) << std::endl;
  } else {
     std::clog << "d0 was not parsed!" << std::endl;
  }
  */
  

  std::clog << "\nParse time interval:" << std::endl;
  boost::posix_time::time_period p0 = vire::time::invalid_time_interval();

  if (vire::time::is_valid(p0)) {
    std::clog << "p0 is valid." << std::endl;
  } else {
    std::clog << "p0 is not valid." << std::endl;
  }

  if (vire::time::parse_time_interval("[now/2099-05-01 00:00]", p0)) {
    std::clog << "p0 was successfully parsed: " << boost::posix_time::to_simple_string(p0) << std::endl;
  } else {
    std::clog << "cannot parse p0" << std::endl;
  }

  if (vire::time::parse_time_interval("[/2199-01-01 00:01]", p0)) {
    std::clog << "p0 was successfully parsed: " << boost::posix_time::to_simple_string(p0) << std::endl;
  } else {
    std::clog << "cannot parse p0" << std::endl;
  }

  if (vire::time::parse_time_interval("[now/]", p0)) {
    std::clog << "p0 was successfully parsed: " << boost::posix_time::to_simple_string(p0) << std::endl;
  } else {
    std::clog << "cannot parse p0" << std::endl;
  }

  if (vire::time::parse_time_interval("[2016-05-01 12:00:05.425176/2099-05-01 00:00]", p0)) {
    std::clog << "p0 was successfully parsed: " << boost::posix_time::to_simple_string(p0) << std::endl;
  } else {
    std::clog << "cannot parse p0" << std::endl;
  }

  if (vire::time::parse_time_interval("[never/never]", p0)) {
    std::clog << "p0 was successfully parsed: " << boost::posix_time::to_simple_string(p0) << std::endl;
  } else {
    std::clog << "cannot parse p0" << std::endl;
  }

  if (vire::time::parse_time_interval("(now; 2 h)", p0)) {
    std::clog << "p0 was successfully parsed: " << boost::posix_time::to_simple_string(p0) << std::endl;
  } else {
    std::clog << "cannot parse p0" << std::endl;
  }

  if (vire::time::parse_time_interval("(2016-05-01 12:00:00; 4:0)", p0)) {
    std::clog << "p0 was successfully parsed: " << boost::posix_time::to_simple_string(p0) << std::endl;
  } else {
    std::clog << "cannot parse p0" << std::endl;
  }

  if (vire::time::parse_time_interval("(2016-05-01 12:00:00; 6)", p0)) {
    std::clog << "p0 was successfully parsed: " << boost::posix_time::to_simple_string(p0) << std::endl;
  } else {
    std::clog << "cannot parse p0" << std::endl;
  }

  if (vire::time::parse_time_interval("(2016-05-01 12:00:00; 8 min)", p0)) {
    std::clog << "p0 was successfully parsed: " << boost::posix_time::to_simple_string(p0) << std::endl;
  } else {
    std::clog << "cannot parse p0" << std::endl;
  }

  if (vire::time::parse_time_interval("(2016-05-01 12:00:00; 0:5:30)", p0)) {
    std::clog << "p0 was successfully parsed: " << boost::posix_time::to_simple_string(p0) << std::endl;
  } else {
    std::clog << "cannot parse p0" << std::endl;
  }

  return;
}

/*
void test_utils_2()
{
  std::clog << "\ntest_utils_2: basics" << std::endl;
  {
    boost::posix_time::ptime dt1(boost::posix_time::neg_infin);
    boost::optional<std::chrono::system_clock::time_point> maybe
      = vire::time::to_std(dt1);
    if (maybe) {
      std::clog << " Maybe = " << maybe.value().time_since_epoch().count() << std::endl;
    } else {
      std::clog << " Maybe is invalid." << std::endl;      
    }
  }
  {
    boost::posix_time::ptime dt1(boost::posix_time::pos_infin);
    boost::optional<std::chrono::system_clock::time_point> maybe
      = vire::time::to_std(dt1);
    if (maybe) {
      std::clog << " Maybe = " << maybe.value().time_since_epoch().count() << std::endl;
    } else {
      std::clog << " Maybe is invalid." << std::endl;      
    }
  }
  {
    boost::posix_time::ptime dt1(boost::posix_time::not_a_date_time);
    boost::optional<std::chrono::system_clock::time_point> maybe
      = vire::time::to_std(dt1);
    if (maybe) {
      std::clog << " Maybe = " << maybe.value().time_since_epoch().count() << std::endl;
    } else {
      std::clog << " Maybe is invalid." << std::endl;      
    }
  }
  {
    boost::posix_time::ptime dt1(boost::posix_time::max_date_time);
    boost::optional<std::chrono::system_clock::time_point> maybe
      = vire::time::to_std(dt1);
    if (maybe) {
      std::clog << " Maybe = " << maybe.value().time_since_epoch().count() << std::endl;
    } else {
      std::clog << " Maybe is invalid." << std::endl;      
    }
  }
  {
    boost::posix_time::ptime dt1(boost::posix_time::min_date_time);
    boost::optional<std::chrono::system_clock::time_point> maybe
      = vire::time::to_std(dt1);
    if (maybe) {
      std::clog << " Maybe = " << maybe.value().time_since_epoch().count() << std::endl;
    } else {
      std::clog << " Maybe is invalid." << std::endl;      
    }
  }
  return;
}
*/
