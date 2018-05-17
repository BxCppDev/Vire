//! \file  vire/user/testing/test-ui-gui_integer_dialog.cxx
//! \brief Vire user graphical user interface integer dialog
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
#include <vire/ui/gui_integer_dialog.h>

void test_1();
void test_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::ui::gui_integer_dialog' class."
              << std::endl;

    test_1();
    test_2();

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
  std::clog << "\ntest_1: enumeration" << std::endl;

  vire::ui::gui_integer_dialog dialog;
  dialog.set_title("Configuration of the zoo");
  dialog.set_label("Number of gnus");
  dialog.add_allowed_value(1);
  dialog.add_allowed_value(2);
  dialog.add_allowed_value(4);
  dialog.add_allowed_value(40);
  dialog.add_allowed_value(42);
  dialog.add_allowed_value(64);
  dialog.add_allowed_value(101);
  dialog.add_allowed_value(200);
  dialog.add_allowed_value(233);
  dialog.add_allowed_value(666);
  dialog.add_allowed_value(1200);
  dialog.set_default_value(2);
  int nb_gnus;
  if (vire::ui::dialog_report report = dialog.input(nb_gnus)) {
    std::clog << "Number of gnus is : " << nb_gnus << std::endl;
  } else {
    std::cerr << "No number of gnus was input : " << report.what() << std::endl;
  }

  std::clog << std::endl;
  return;
}

void test_2()
{
  std::clog << "\ntest_2: range" << std::endl;

  vire::ui::gui_integer_dialog dialog;
  dialog.set_title("Configuration of the zoo");
  dialog.set_label("Number of gnus");
  dialog.set_min_value(12);
  dialog.set_max_value(42);
  dialog.set_default_value(20);
  int nb_gnus;
  if (vire::ui::dialog_report report = dialog.input(nb_gnus)) {
    std::clog << "Number of gnus is : " << nb_gnus << std::endl;
  } else {
    std::cerr << "No number of gnus was input : " << report.what() << std::endl;
  }

  std::clog << std::endl;
  return;
}
