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

  vire::cmsserver::session_reservation sres1;
  {
    vire::cmsserver::session_info sinfo;
    sinfo.set_key("Dummy");
    sinfo.set_description("A dummy session info record");
    sinfo.set_role_identifier("foo");
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_period when(now, now + boost::posix_time::hours(2));
    sinfo.set_when(when);
    sinfo.set_use_case_type_id("basic_shift");
    for (int resource_id = 1000; resource_id < 1004; resource_id++) {
      sinfo.set_special_functional_limited(resource_id, 2);
    }
    for (int resource_id = 1010; resource_id < 1020; resource_id++) {
      sinfo.set_special_distributable_limited(resource_id, 4);
    }
    sinfo.initialize_simple();

    sres1.set_sinfo(sinfo);
    sres1.set_start_macro("daq_start.macro");
    sres1.set_stop_macro("daq_stop.macro");
  }
  sres1.tree_dump(std::clog, "Session reservation 1: ", "[info] ");
  std::clog << std::endl;

  vire::cmsserver::session_reservation sres2;
  {

    vire::cmsserver::session_info sinfo;
    sinfo.set_key("Survey");
    sinfo.set_description("A survey session info record");
    sinfo.set_role_identifier("toto");
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_period when(now, // - boost::posix_time::minutes(4),
                                        now + boost::posix_time::hours(1));
    sinfo.set_when(when);
    sinfo.set_use_case_type_id("basic_survey");
    for (int resource_id = 1000; resource_id < 1004; resource_id++) {
      sinfo.set_special_functional_limited(resource_id, 2);
    }
    for (int resource_id = 1010; resource_id < 1020; resource_id++) {
      sinfo.set_special_distributable_limited(resource_id, 4);
    }
    sinfo.initialize_simple();

    sres2.set_sinfo(sinfo);
    sres2.set_start_macro("survey_start.macro");
    sres2.set_stop_macro("survey_stop.macro");
  }
  sres2.tree_dump(std::clog, "Session reservation 2: ", "[info] ");
  std::clog << std::endl;

  if (sres1 < sres2) {
    std::clog << "[info] reservation 1 < reservation 2" << std::endl;
  } else {
    std::clog << "[info] reservation 1 not < reservation 2" << std::endl;
  }
  std::clog << std::endl;

  std::clog << "[info] serialization..." << std::endl;
  {
    datatools::data_writer writer("test-cmsserver-session_reservation.xml",
                                  datatools::using_multi_archives);
    writer.store(sres1);
    writer.store(sres2);
  }
  std::clog << std::endl;

  return;
}
