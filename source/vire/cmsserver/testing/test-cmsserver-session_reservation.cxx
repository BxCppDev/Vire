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
    vire::cmsserver::process_info pinfo;
    pinfo.set_name("DAQ");
    pinfo.set_description("Data acquisition process");
    pinfo.set_model_id("snemo::demonstrator::cms::daq_process");
    pinfo.set_config_urn("snemo:demonstrator:daq:config:dbd-1.2");
    std::clog << std::endl;

    vire::cmsserver::session_info sinfo;
    sinfo.set_id(1000);
    sinfo.set_key("Dummy");
    sinfo.set_description("A dummy session info record");
    sinfo.set_role_id(2014);
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_period when(now, now + boost::posix_time::hours(2));
    sinfo.set_when(when);
    sinfo.set_usecase_id("basic_shift");
    for (int resource_id = 1000; resource_id < 1004; resource_id++) {
      sinfo.set_special_functional_limited(resource_id, 2);
    }
    for (int resource_id = 1010; resource_id < 1020; resource_id++) {
      sinfo.set_special_distributable_limited(resource_id, 4);
    }
    sinfo.initialize_simple();

    sres1.set_sinfo(sinfo);
    sres1.set_pinfo(pinfo);
    sres1.set_start_macro("daq_start.macro");
  }
  sres1.tree_dump(std::clog, "Session reservation 1: ", "[info] ");

  vire::cmsserver::session_reservation sres2;
  {
    vire::cmsserver::process_info pinfo;
    pinfo.set_name("Survey");
    pinfo.set_description("Data quality survey process");
    pinfo.set_model_id("snemo::demonstrator::cms::qsurvey_process");
    pinfo.set_config_urn("snemo:demonstrator:qsurvey-3.7");
    std::clog << std::endl;

    vire::cmsserver::session_info sinfo;
    sinfo.set_id(1001);
    sinfo.set_key("Survey");
    sinfo.set_description("A survey session info record");
    sinfo.set_role_id(2014);
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_period when(now, // - boost::posix_time::minutes(4),
                                        now + boost::posix_time::hours(1));
    sinfo.set_when(when);
    sinfo.set_usecase_id("basic_survey");
    for (int resource_id = 1000; resource_id < 1004; resource_id++) {
      sinfo.set_special_functional_limited(resource_id, 2);
    }
    for (int resource_id = 1010; resource_id < 1020; resource_id++) {
      sinfo.set_special_distributable_limited(resource_id, 4);
    }
    sinfo.initialize_simple();

    sres2.set_sinfo(sinfo);
    sres2.set_pinfo(pinfo);
    sres2.set_start_macro("survey_start.macro");
    sres2.set_stop_macro("survey_stop.macro");
  }
  sres2.tree_dump(std::clog, "Session reservation 2: ", "[info] ");

  if (sres1 < sres2) {
    std::clog << "[info] reservation 1 < reservation 2" << std::endl;
  } else {
    std::clog << "[info] reservation 1 not < reservation 2" << std::endl;
  }

  return;
}
