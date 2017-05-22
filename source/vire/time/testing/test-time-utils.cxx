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

// This project:
#include <vire/vire.h>
#include <vire/time/utils.h>

void test_utils_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::time::time_interval_cut' class."
              << std::endl;

    test_utils_1();

    std::clog << "The end." << std::endl;
  }
  catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  }
  catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return (error_code);
}

void test_utils_1()
{
  std::clog << "\ntest_utils_1: basics" << std::endl;
  {
    boost::posix_time::ptime dt1(boost::posix_time::neg_infin);
    boost::posix_time::ptime dt2(boost::posix_time::pos_infin);
    boost::posix_time::ptime dt3(boost::posix_time::not_a_date_time);
    boost::posix_time::ptime dt4(boost::posix_time::max_date_time);
    boost::posix_time::ptime dt5(boost::posix_time::min_date_time);
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

  boost::posix_time::time_duration d0 = vire::time::invalid_time_duration();
  if (vire::time::is_valid(d0)) {
    std::clog << "d0 is valid." << std::endl;
  } else {
    std::clog << "d0 is not valid." << std::endl;
  }

  if (vire::time::parse_time_duration("2 s", d0)) {
    std::clog << "d0 was successfully parsed: " << boost::posix_time::to_simple_string(d0) << std::endl;
  }

  if (vire::time::parse_time_duration("120 minute", d0)) {
    std::clog << "d0 was successfully parsed: " << boost::posix_time::to_simple_string(d0) << std::endl;
  }

  if (vire::time::parse_time_duration("forever", d0)) {
    std::clog << "d0 was successfully parsed: " << boost::posix_time::to_simple_string(d0) << std::endl;
  }

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
