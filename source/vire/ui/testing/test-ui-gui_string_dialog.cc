//! \file  vire/user/testing/test-ui-gui_string_dialog.cxx
//! \brief Vire user graphical user interface string dialog
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
#include <vire/ui/gui_string_dialog.h>

void test_1();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::ui::gui_string_dialog' class."
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

  vire::ui::gui_string_dialog dialog;
  dialog.set_title("Configuration of the zoo");
  dialog.set_label("Duck's name");
  dialog.add_allowed_value("riri");
  dialog.add_allowed_value("fifi");
  dialog.add_allowed_value("loulou");
  dialog.add_allowed_value("picsou");
  dialog.add_allowed_value("donald");
  dialog.add_allowed_value("daffy");
  dialog.set_default_value("picsou");
  std::string word;
  if (dialog.input(word)) {
    std::clog << "String is : '" << word << "'" << std::endl;
  } else {
    std::cerr << "No string was input!" << std::endl;
  }

  return;
}
