//! \file  vire/testing/test-time-time_service.cxx
//! \brief Vire time service
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

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>

// This project:
#include <vire/vire.h>
#include <vire/time/time_service.h>
#include <vire/time/utils.h>

void test_0();
void test_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::time::time_service' class."
              << std::endl;
    test_0();
    test_1();

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

void test_0()
{

  {
    std::clog << "test_0: UTC -> Local" << std::endl;
    boost::posix_time::ptime utc_pt(boost::gregorian::date(2004,11,5), 
                                    boost::posix_time::hours(15));
    boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("MST-07"));
    std::clog << "Time zone   = " << zone->to_posix_string() << std::endl;
    boost::local_time::local_date_time az(utc_pt, zone);
    std::clog << "UTC time   = " << az.utc_time() << std::endl;   // 15am 2004-Nov-5
    std::clog << "Local time = " << az.local_time() << std::endl; // 8am 2004-Nov-5
    std::clog << std::endl;
  }
  
  {
    std::clog << "test_0: Local -> UTC" << std::endl;
    boost::posix_time::ptime local_pt(boost::gregorian::date(2004,11,5), 
                                      boost::posix_time::hours(8)); 
    std::clog << "Local time = " << local_pt << std::endl; // 8am 2004-Nov-5
    boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("MST-07"));
    //boost::local_time::time_zone_ptr zone(new boost::local_time::posix_time_zone("GMT+01"));
    std::clog << "Time zone   = " << zone->to_posix_string() << std::endl;
    
    std::clog << std::endl;
  }
  
  return;
}

void test_1()
{
  std::clog << "\ntest_1: Time service" << std::endl;
  vire::time::time_service timeServ;
  
  timeServ.set_name("Time");
  timeServ.set_display_name("Time service");
  timeServ.set_terse_description("Time service associated to a given location");
  timeServ.set_time_zone_id("America/Chicago");
  timeServ.set_time_zone_id("Africa/Abidjan");
  timeServ.initialize_simple();

  std::clog << "Time service TZ : " << std::endl;
  std::clog << "  POSIX repr.         : " << timeServ.get_time_zone()->to_posix_string() << std::endl;
  std::clog << "  Time zone           : " << timeServ.get_time_zone()->std_zone_name() << std::endl;
  std::clog << "  Time zone (abbrev.) : " << timeServ.get_time_zone()->std_zone_abbrev() << std::endl;
  std::clog << "  Offset from UTC     : " << timeServ.get_time_zone()->base_utc_offset() << std::endl;
  std::clog << "  Has DST             : " << std::boolalpha << timeServ.get_time_zone()->has_dst() << std::endl;
  if (timeServ.get_time_zone()->has_dst()) {
    std::clog << "  DST offset          : " << timeServ.get_time_zone()->dst_offset() << std::endl;
    std::clog << "  DST time zone name  : " << timeServ.get_time_zone()->dst_zone_name() << std::endl;
  }
  std::clog << std::endl;

  boost::posix_time::ptime local_time;
  vire::time::make(local_time, 2018, 5, 24, 11, 25, 23, 123456);
  std::clog << "Local time  : " << local_time << std::endl;

  boost::posix_time::ptime utc_time = timeServ.local_to_utc(local_time, true);
  std::clog << "UTC time    : " << utc_time << std::endl;
  std::clog << std::endl;

  
  timeServ.tree_dump(std::clog, "Time service: ");
  
  timeServ.reset();
  return;
}
