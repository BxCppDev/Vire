//! \file vire/logging/testing/test-logging-simple_logging_service.cxx
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

// Ourselves:
#include <vire/logging/simple_logging_service.h>

// Standard library:
#include <iostream>

// Third party:
// - Boost:
#define BOOST_LOG_DYN_LINK
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sources/severity_logger.hpp>

// // This project:
// #include <vire/logging/ui/log_command_interface.h>

void test0();

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    test0();

  }
  catch (std::exception & error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 error.what());
    error_code = EXIT_FAILURE;
  }
  catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}

void test0()
{
  std::clog << "\nTest 0: Entering..." << std::endl;

  boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg(boost::log::keywords::severity = boost::log::trivial::warning);
  BOOST_LOG(lg) << "Default logging: Hello, world!";
  BOOST_LOG_SEV(lg, boost::log::trivial::info) << "Default logging: Welcome word...";

  // Simple global logging:
  vire::logging::simple_logging_service log_service;
  log_service.set_name("Log");
  log_service.set_display_name("Logging service");
  log_service.set_terse_description("Application global logging engine");
  log_service.set_logging_priority(datatools::logger::PRIO_TRACE);
  log_service.tree_dump(std::clog, "Simple global logging: ");

  log_service.set_use_logconsole(true);
  log_service.set_use_logfile(true);
  log_service.set_logfile_dir(".");
  log_service.set_logfile_pattern("test-logging-simple_global_logging-%Y-%m-%d_%N.log");
  log_service.set_severity_label("info");
  log_service.set_logfile_rotate_size(4 * 128);
  log_service.initialize_simple();
  log_service.tree_dump(std::clog, "Simple global logging (initialized): ");
  std::clog << std::endl;

  /*
  vire::logging::ui::log_command_interface logCmdInter(log_service,
                                                       "Log",
                                                       "Logging Service");
  logCmdInter.initialize_simple();
  logCmdInter.tree_dump(std::clog, "Logger Command Interface: ");
  std::clog << std::endl;

  std::clog << "Calling Logger Command Interface : " << std::endl;
  std::vector<std::string> argv = { "print" };
  datatools::command::returned_info cri;
  logCmdInter.call(argv, cri, 0);
  if (cri.is_failure()) {
    std::cerr<< "error: Call failed!" << std::endl;
  }
  std::clog << std::endl;
  */

  /*
  boost::log::sources::severity_logger< boost::log::trivial::severity_level > slg(boost::log::keywords::severity = boost::log::trivial::info);

  boost::log::record rec = slg.open_record(boost::log::keywords::severity = boost::log::trivial::info);
  if (rec) {
    boost::log::record_ostream strm(rec);
    strm << "An informational severity message";
    strm.flush();
    slg.push_record(boost::move(rec));
  }
  */

  // BOOST_LOG_NAMED_SCOPE("simple_global_logging");

  BOOST_LOG_SEV(lg, boost::log::trivial::trace) << "A trace severity message";
  BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "A debug severity message";
  BOOST_LOG_SEV(lg, boost::log::trivial::info) << "An informational severity message";
  BOOST_LOG_SEV(lg, boost::log::trivial::warning) << "A warning severity message";
  BOOST_LOG_SEV(lg, boost::log::trivial::error) << "An error severity message";
  BOOST_LOG_SEV(lg, boost::log::trivial::fatal) << "A fatal severity message";

  for (int i = 1; i < 10; i++) {
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "Another informational severity message";
  }
  BOOST_LOG_SEV(lg, boost::log::trivial::error) << "An error severity message";
  // BOOST_LOG_TRIVIAL(boost::log::trivial::info) << "*****************************";

  if (log_service.is_initialized()) {
    log_service.set_severity_label("trace");
    log_service.reset();
  }

  BOOST_LOG(lg) << "Back to default logging: Bye bye!";
  std::clog << "\nTest 0: Exiting." << std::endl;
  return;
}
