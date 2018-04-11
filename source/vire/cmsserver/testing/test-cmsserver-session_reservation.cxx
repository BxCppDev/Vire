// test-cmsserver-session_reservation.cxx

// Ourselves:
#include <vire/cmsserver/session_reservation.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// Third party:
// - Boost:
#include <boost/filesystem.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/io_factory.h>

// This project:
#include <vire/vire.h>

void test_session_reservation_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::session_reservation' class."
              << std::endl;

    test_session_reservation_1();

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

void test_session_reservation_1()
{
  std::clog << "\ntest_session_reservation_1: basics" << std::endl;

  vire::cmsserver::session_reservation resa1;
  resa1.set_id(1);
  resa1.set_booked_by("nemoprod");
  resa1.set_last_update(boost::posix_time::second_clock::local_time());
  resa1.set_resource_scope("stdshift");
  boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
  boost::posix_time::time_period when(now, now + boost::posix_time::hours(2));
  resa1.set_when(when);
  {
    vire::cmsserver::use_case_info ucinfo;
    ucinfo.set_name("Dummy");
    ucinfo.set_description("A dummy session info record");
    ucinfo.set_role_definition("foo");
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_period when(now, now + boost::posix_time::hours(2));
    ucinfo.set_use_case_type_id("snemo::demonstrator::cms::basic_shift_use_case");
    // for (int resource_id = 1000; resource_id < 1004; resource_id++) {
    //   ucinfo.set_special_functional_limited(resource_id, 2);
    // }
    // for (int resource_id = 1010; resource_id < 1020; resource_id++) {
    //   ucinfo.set_special_distributable_limited(resource_id, 4);
    // }
    ucinfo.initialize_simple();

    resa1.set_use_case_info(ucinfo);
  }
  resa1.set_start_macro("daq_start.macro");
  resa1.set_stop_macro("daq_stop.macro");
  resa1.tree_dump(std::clog, "Session reservation 1: ", "[info] ");
  std::clog << std::endl;

  std::clog << "[info] serialization..." << std::endl;
  {
    datatools::data_writer writer("test-cmsserver-session_reservation.xml",
                                  datatools::using_multi_archives);
    writer.store(resa1);
  }
  std::clog << std::endl;

  return;
}
