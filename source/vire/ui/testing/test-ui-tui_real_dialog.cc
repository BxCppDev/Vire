//! \file  vire/user/testing/test-ui-tui_real_dialog.cxx
//! \brief Vire user text user interface real dialog
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

// Third party:
#include <bayeux/datatools/clhep_units.h>

// This project:
#include <vire/vire.h>
#include <vire/ui/tui_real_dialog.h>

void test_1();
void test_2();
void test_3();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::ui::tui_real_dialog' class."
              << std::endl;

    test_1();
    test_2();
    test_3();

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

void test_1()
{
  std::clog << "\ntest_1: basic" << std::endl;

  vire::ui::tui_real_dialog dialog;
  dialog.set_title("Configuration of the zoo");
  dialog.set_label("Weight of the gnu");
  dialog.set_unit_dimension("mass");
  dialog.set_preferred_unit("kg");
  // dialog.set_min_value(30. * CLHEP::kg);
  // dialog.set_max_value(120. * CLHEP::kg);
  dialog.set_default_value(70. * CLHEP::kg);
  double weight_gnu;
  if (vire::ui::dialog_report report = dialog.input(weight_gnu)) {
    std::clog << "Weight of gnu is : " << weight_gnu / CLHEP::kg << " kg" << std::endl;
  } else {
    std::cerr << "No weight of gnu was input : " << report.what() << std::endl;
  }

  std::clog << std::endl;
  return;
}

void test_2()
{
  std::clog << "\ntest_2: dimensionless" << std::endl;

  vire::ui::tui_real_dialog dialog;
  dialog.set_title("Configuration of the zoo");
  dialog.set_label("Value");
  dialog.set_min_value(30);
  dialog.set_max_value(120);
  // dialog.set_default_value(42.0);
  double value;
  if (vire::ui::dialog_report report = dialog.input(value)) {
    std::clog << "Value is : " << value << std::endl;
  } else {
    std::cerr << "No value was input : " << report.what() << std::endl;
  }

  std::clog << std::endl;
  return;
}

void test_3()
{
  std::clog << "\ntest_3: fraction" << std::endl;

  vire::ui::tui_real_dialog dialog;
  dialog.set_title("Configuration of the zoo");
  dialog.set_label("Ratio");
  dialog.set_unit_dimension("fraction");
  dialog.set_preferred_unit("%");
  dialog.set_min_value(0.1);
  dialog.set_max_value(0.35);
  dialog.set_max_value(1.0);
  dialog.set_default_value(22 * CLHEP::perCent);
  double ratio;
  if (vire::ui::dialog_report report = dialog.input(ratio)) {
    std::clog << "Ratio is : " << ratio << std::endl;
  } else {
    std::cerr << "No ratio was input : " << report.what() << std::endl;
  }

  std::clog << std::endl;
  return;
}
