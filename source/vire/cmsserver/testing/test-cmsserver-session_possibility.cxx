// test-cmsserver-session_possibility.cxx

// Ourselves:
#include <vire/cmsserver/session_possibility.h>

// Standard library:
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <vector>

// This project:
#include <vire/vire.h>

void test_session_possibility_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::cmsserver::session_possibility' class."
              << std::endl;

    test_session_possibility_1();

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

void test_session_possibility_1()
{
  std::clog << "\ntest_session_possibility_1: basics" << std::endl;

  {
    vire::cmsserver::session_possibility sp;
    boost::posix_time::ptime end = vire::time::now() + boost::posix_time::hours(1);
    sp.make_enter_session("shifter", "stdrun", end);
    sp.tree_dump(std::clog, "Session possibility:");
    std::clog << std::endl;
  }

  // {
  //   vire::cmsserver::session_possibility sp;
  //   boost::posix_time::ptime begin = vire::time::now() + boost::posix_time::hours(1);
  //   boost::posix_time::ptime end = vire::time::now() + boost::posix_time::hours(3);
  //   const boost::posix_time::time_period when(begin, end);
  //   sp.make_create_session("comm", "runexpert", when);
  //   sp.tree_dump(std::clog, "Session possibility:");
  //   std::clog << std::endl;
  // }

  return;
}
