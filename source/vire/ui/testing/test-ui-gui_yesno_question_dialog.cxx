//! \file  vire/user/testing/test-ui-gui_yesno_question_dialog.cxx
//! \brief Vire user graphical user interface yes/no question dialog
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
#include <vire/ui/gui_yesno_question_dialog.h>

void test_1();
void test_2();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::ui::gui_yesno_question_dialog' class."
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
  std::clog << "\ntest_1: basics" << std::endl;

  vire::ui::gui_yesno_question_dialog dialog;
  dialog.set_title("Set a boolean flag");
  dialog.set_label("Debug");
  dialog.set_default_value(false);
  dialog.set_default_value(true);
  bool flag;
  if (dialog.input(flag)) {
    std::clog << "Debug flag is : " << std::boolalpha << flag << std::endl;
  } else {
    std::cerr << "No debug flag was input!" << std::endl;
  }

  return;
}

void test_2()
{
  std::clog << "\ntest_2: question" << std::endl;

  vire::ui::gui_yesno_question_dialog dialog;
  dialog.set_title("Answer a question");
  dialog.set_label("Would you like a cup of tea");
  dialog.set_label_as_question(true);
  dialog.set_default_value(false);
  dialog.set_default_value(true);
  bool answer;
  if (dialog.input(answer)) {
    std::clog << "Answer is : " << std::boolalpha << answer << std::endl;
  } else {
    std::cerr << "No answer was input!" << std::endl;
  }

  return;
}
