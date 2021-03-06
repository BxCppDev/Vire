//! \file  vire/user/testing/test-ui-gui_password_dialog.cxx
//! \brief Vire user graphical user interface password dialog
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// - Bayeux:
#include <bayeux/datatools/qt/interface.h>

// This project:
#include <vire/vire.h>
#include <vire/ui/gui_password_dialog.h>

void test_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::ui::gui_password_dialog' class."
              << std::endl;

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

void test_1()
{
  std::clog << "\ntest_1: basics" << std::endl;

  datatools::qt::interface::instance();

  vire::ui::gui_password_dialog dialog;
  dialog.set_title("Connection");
  dialog.set_label("Password");
  dialog.set_show_stars(true);
  std::string password;
  if (vire::ui::dialog_report report = dialog.input(password)) {
    std::clog << "Password is : '" << password << "'" << std::endl;
  } else {
    std::cerr << "No password was input : " << report.what() << std::endl;
  }

  return;
}
