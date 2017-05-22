//! \file  vire/testing/test-time-time_interval_cut.cxx
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

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
// - Bayeux/datatools:
#include <datatools/io_factory.h>
#include <datatools/temporary_files.h>
#include <datatools/utils.h>
// - Bayeux/cuts:
#include <cuts/accept_cut.h>
#include <cuts/reject_cut.h>
#include <cuts/and_cut.h>
#include <cuts/not_cut.h>
// - Bayeux/geomtools:
#include <geomtools/geomtools_config.h>
#include <geomtools/box.h>
#include <geomtools/gnuplot_draw.h>
#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
#include <geomtools/gnuplot_i.h>
#include <geomtools/gnuplot_drawer.h>
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY

// This project:
#include <vire/vire.h>
#include <vire/time/time_interval_cut.h>

void test_time_interval_cut_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::time::time_interval_cut' class."
              << std::endl;

    test_time_interval_cut_1();

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

void test_time_interval_cut_1()
{
  std::clog << "\ntest_time_interval_cut_1: basics" << std::endl;

  boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
  boost::posix_time::ptime near_future = now + boost::posix_time::hours(2);
  boost::posix_time::ptime tomorrow = now + boost::posix_time::hours(24);
  boost::posix_time::ptime far_future = now + boost::posix_time::hours(10000);

  boost::posix_time::time_period session_slot(now, near_future);
  boost::posix_time::time_period shift_slot(now, now + boost::posix_time::hours(24 * 7));

  /*
   *
   *             now              now              now               now
   *     now    + 2 h           + 24 h           + 7x24 h          + 10000 h
   *  ----+-------+----------------+-----------------+----------------+-------> Time
   *      :<----->:                                  :
   *      :session                                   :
   *      : slot                                     :
   *      :<---------------------------------------->:
   *                    shift slot
   */
  // A cut that always accepts:
  cuts::accept_cut always_cut;
  always_cut.set_name("Always");
  always_cut.set_description("Time slot always accessible for a given session");
  always_cut.initialize_simple();
  always_cut.tree_dump(std::clog, always_cut.get_name() + " : ");

  // A cut that always rejects:
  cuts::reject_cut never_cut;
  never_cut.set_name("Never");
  never_cut.set_description("Time slot never accessible for a given session");
  never_cut.initialize_simple();
  never_cut.tree_dump(std::clog, never_cut.get_name() + " : ");

  // A cut that rejects time slot out of the shift time interval:
  vire::time::time_interval_cut shift_cut;
  shift_cut.set_name("Shift");
  shift_cut.set_description("Time slot accessible only during the shift");
  shift_cut.set_interval(shift_slot);
  shift_cut.initialize_simple();
  shift_cut.tree_dump(std::clog, shift_cut.get_name() + " : ");

  // A cut that always rejects time slot out of a specific time interval:
  vire::time::time_interval_cut soon_cut;
  soon_cut.set_name("Soon");
  soon_cut.set_description("Time slot accessible only quite soon");
  soon_cut.set_interval(now + boost::posix_time::minutes(45),
                        boost::posix_time::hours(5));
  soon_cut.initialize_simple();
  soon_cut.tree_dump(std::clog, soon_cut.get_name() + " : ");

  int seed = 314159;
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(0, 200);
  auto dice200 = std::bind( distribution, generator );

  // Randomize a time in the future (within 200 h):
  boost::posix_time::ptime when = now + boost::posix_time::hours(dice200());
  boost::posix_time::time_period when_slot(when, when + boost::posix_time::seconds(1));

  std::clog << "now  = " << now << std::endl;
  std::clog << "when = " << when << std::endl;

  always_cut.set_user_data(when_slot);
  never_cut.set_user_data(when_slot);
  shift_cut.set_user_data(when_slot);
  soon_cut.set_user_data(when_slot);

  if (always_cut.process() == cuts::SELECTION_ACCEPTED) {
    std::clog << "Time slot '" << when_slot << "' is accepted by '" << always_cut.get_name() << "' !" << std::endl;
  }else {
    std::clog << "Time slot '" << when_slot << "' is not accepted by '" << always_cut.get_name() << "' !" << std::endl;
  }

  if (never_cut.process() == cuts::SELECTION_ACCEPTED) {
    std::clog << "Time slot '" << when_slot << "' is accepted by '" << never_cut.get_name() << "' !" << std::endl;
  } else {
    std::clog << "Time slot '" << when_slot << "' is not accepted by '" << never_cut.get_name() << "' !" << std::endl;
  }

  if (shift_cut.process() == cuts::SELECTION_ACCEPTED) {
    std::clog << "Time slot '" << when_slot << "' is accepted by '" << shift_cut.get_name() << "' !" << std::endl;
  } else {
    std::clog << "Time slot '" << when_slot << "' is not accepted by '" << shift_cut.get_name() << "' !" << std::endl;
  }

  if (soon_cut.process() == cuts::SELECTION_ACCEPTED) {
    std::clog << "Time slot '" << when_slot << "' is accepted by '" << soon_cut.get_name() << "' !" << std::endl;
  } else {
    std::clog << "Time slot '" << when_slot << "' is not accepted by '" << soon_cut.get_name() << "' !" << std::endl;
  }

  return;
}
