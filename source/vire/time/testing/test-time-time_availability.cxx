//! \file  vire/testing/test-time-time_availability.cxx
//! \brief Vire time availability support
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

// Third party:
// - Boost:
#include <boost/scoped_ptr.hpp>
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
#include <vire/time/time_availability.h>
#include <vire/time/time_interval_cut.h>

void test_time_availability_1(bool draw_);

int main(int argc_, char ** argv_)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::time::time_availability' class."
              << std::endl;
    bool draw = false;

    int iarg = 1;
    while (iarg < argc_) {
      std::string arg = argv_[iarg];
      if (arg == "--draw") {
        draw = true;
      } else if (arg == "--no-draw") {
        draw = false;
      }
      iarg++;
    }

    test_time_availability_1(draw);

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

void test_time_availability_1(bool draw_)
{
  std::clog << "\ntest_time_availability_1: basics" << std::endl;

  boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
  boost::posix_time::ptime near_future = now + boost::posix_time::hours(2);
  boost::posix_time::ptime tomorrow = now + boost::posix_time::hours(24);
  boost::posix_time::ptime far_future = now + boost::posix_time::hours(10000);

  boost::posix_time::time_period session_slot(now, near_future);
  boost::posix_time::time_period shift_slot(now, now + boost::posix_time::hours(24 * 7));

  cuts::accept_cut always_cut;
  always_cut.set_name("Always");
  always_cut.set_description("Time slot always accessible for a given session");
  always_cut.initialize_simple();

  vire::time::time_availability always(always_cut);
  always.tree_dump(std::clog, always.get_name() + " : ");

  if (always.accept_time(now)) {
    std::clog << "Time '" << now << "' is accepted by '" << always.get_name() << "' !" << std::endl;
  }

  if (always.accept_time(tomorrow)) {
    std::clog << "Time '" << tomorrow << "' is accepted by '" << always.get_name() << "' !" << std::endl;
  }

  if (always.accept_time(far_future)) {
    std::clog << "Time '" << far_future << "' is accepted by '" << always.get_name() << "' !" << std::endl;
  }

  if (always.accept_period(session_slot)) {
    std::clog << "Session slot '" << session_slot << "' is accepted by '" << always.get_name() << "' !" << std::endl;
  }

  if (always.accept_period(shift_slot)) {
    std::clog << "Shift slot '" << shift_slot << "' is accepted by '" << always.get_name() << "' !" << std::endl;
  }

  std::clog << std::endl;

  cuts::reject_cut never_cut;
  never_cut.set_name("Never");
  never_cut.set_description("Time slot never accessible for a given session");

  vire::time::time_availability never(never_cut);
  never.tree_dump(std::clog, never.get_name() + " : ");

  if (! never.accept_time(now)) {
    std::clog << "Time '" << now << "' is rejected by '" << never.get_name() << "' !" << std::endl;
  }

  if (! never.accept_time(tomorrow)) {
    std::clog << "Time '" << tomorrow << "' is rejected by '" << never.get_name() << "' !" << std::endl;
  }

  if (! never.accept_time(far_future)) {
    std::clog << "Time '" << far_future << "' is rejected by '" << never.get_name() << "' !" << std::endl;
  }

  if (! never.accept_period(session_slot)) {
    std::clog << "Session slot '" << session_slot << "' is rejected by '" << never.get_name() << "' !" << std::endl;
  }

  if (! never.accept_period(shift_slot)) {
    std::clog << "Shift slot '" << shift_slot << "' is rejected by '" << never.get_name() << "' !" << std::endl;
  }

  std::clog << std::endl;

  vire::time::time_interval_cut soon_cut;
  soon_cut.set_name("Soon");
  soon_cut.set_description("Time slot accessible only quite soon");
  soon_cut.set_interval(now + boost::posix_time::minutes(45),
                        boost::posix_time::hours(5));
  soon_cut.initialize_simple();

  vire::time::time_availability soon(soon_cut);
  soon.tree_dump(std::clog, soon.get_name() + " : ");

  if (soon.accept_period(session_slot)) {
    std::clog << "Session slot '" << session_slot << "' is accepted by '" << soon.get_name() << "' !" << std::endl;
  } else {
    std::clog << "Session slot '" << session_slot << "' is rejected by '" << soon.get_name() << "' !" << std::endl;
  }

  vire::time::time_interval_cut * period1_cut_ptr = new vire::time::time_interval_cut;
  period1_cut_ptr->set_name("Period1");
  period1_cut_ptr->set_description("Period1");
  period1_cut_ptr->set_interval(now - boost::posix_time::hours(40),
                           boost::posix_time::hours(72));
  period1_cut_ptr->initialize_simple();
  cuts::cut_handle_type period1_cut_h(period1_cut_ptr);
  vire::time::time_availability period1(period1_cut_h.grab());

  vire::time::time_interval_cut * period2_cut_ptr = new vire::time::time_interval_cut;
  period2_cut_ptr->set_name("Period2");
  period2_cut_ptr->set_description("Period2");
  period2_cut_ptr->set_interval(now + boost::posix_time::hours(10),
                           boost::posix_time::hours(100));
  period2_cut_ptr->initialize_simple();
  cuts::cut_handle_type period2_cut_h(period2_cut_ptr);
  vire::time::time_availability period2(period2_cut_h.grab());

  cuts::and_cut * period1_x_period2_cut_ptr = new cuts::and_cut;
  period1_x_period2_cut_ptr->set_name("Period1xPeriod2");
  period1_x_period2_cut_ptr->set_description("Period1xPeriod2");
  period1_x_period2_cut_ptr->set_cuts(period1_cut_h, period2_cut_h);
  period1_x_period2_cut_ptr->initialize_simple();
  cuts::cut_handle_type period1_x_period2_cut_h(period1_x_period2_cut_ptr);
  vire::time::time_availability period1_x_period2(period1_x_period2_cut_h.grab());
  period1_x_period2.tree_dump(std::clog, period1_x_period2.get_name() + " : ");

  cuts::not_cut * not_period1_x_period2_cut_ptr = new cuts::not_cut;
  not_period1_x_period2_cut_ptr->set_name("NotPeriod1xPeriod2");
  not_period1_x_period2_cut_ptr->set_description("!Period1xPeriod2");
  not_period1_x_period2_cut_ptr->set_cut(period1_x_period2_cut_h);
  not_period1_x_period2_cut_ptr->initialize_simple();
  cuts::cut_handle_type not_period1_x_period2_cut_h(not_period1_x_period2_cut_ptr);
  vire::time::time_availability not_period1_x_period2(not_period1_x_period2_cut_h.grab());
  not_period1_x_period2.tree_dump(std::clog, not_period1_x_period2.get_name() + " : ");

  // Array of time availabilities:
  std::vector<const vire::time::time_availability *> tas;
  tas.push_back(&always);
  tas.push_back(&never);
  tas.push_back(&soon);
  tas.push_back(&period1);
  tas.push_back(&period2);
  tas.push_back(&period1_x_period2);
  tas.push_back(&not_period1_x_period2);

  datatools::temp_file tmp_file;
  tmp_file.set_remove_at_destroy(true);
  tmp_file.create("/tmp", "test-time-time_availability_");

  boost::posix_time::time_period this_week(now - boost::gregorian::days(3),
                                           now + boost::gregorian::days(4));
  boost::posix_time::time_duration delta_time = boost::posix_time::minutes(1);
  for (int i = 0; i < tas.size(); i++) {
    tas[i]->draw(tmp_file.out(), this_week, delta_time, 0);
  }

  if (draw_) {
#if GEOMTOOLS_WITH_GNUPLOT_DISPLAY == 1
    Gnuplot g1;
    g1.cmd ("set notitle");
    g1.cmd ("set grid");
    g1.cmd ("set key outside width 20");
    // g1.cmd ("set ylabel \"Availability\"");
    g1.cmd ("set yrange [-0.2:+1.2]");
    g1.cmd ("set ytics 0.0, 1.0, 1.0 "); // \"No\" 0.0, \"Yes\" ");
    {
      std::ostringstream plot_cmd;

      plot_cmd << "set multiplot title \"Time availability\" layout " << tas.size() << ",1 ;" << std::endl;
      for (int i = 0; i < tas.size(); i++) {
        if (i == tas.size() - 1) {
          // g1.cmd ("set xlabel \"Time (s)\"");
        }
        plot_cmd << "plot '" << tmp_file.get_filename() << "' index " << i << " title \""
                 << tas[i]->get_name() << "\" with lines ;" << std::endl;
      }
      plot_cmd << "unset multiplot ;" << std::endl;

      g1.cmd(plot_cmd.str());
      g1.showonscreen(); // window output
      geomtools::gnuplot_drawer::wait_for_key();
      usleep(200);
    }
#endif // GEOMTOOLS_WITH_GNUPLOT_DISPLAY
  }

  return;
}
