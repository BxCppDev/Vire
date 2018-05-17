//! \file  vire/user/testing/test-ui-tui_file_dialog.cxx
//! \brief Vire user text user interface file dialog
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
#include <vire/ui/tui_file_dialog.h>

void test_1();
void test_2();
void test_3();
void test_4();

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::ui::tui_file_dialog' class."
              << std::endl;

    test_1();
    test_2();
    test_3();
    test_4();

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

  vire::ui::tui_file_dialog dialog;
  dialog.set_title("Input data");
  dialog.set_label("Configuration file");
  dialog.set_default_path("~/.vire/dummy.conf");
  std::string path;
  if (vire::ui::dialog_report report = dialog.input(path)) {
    std::clog << "Path is : '" << path << "'" << std::endl;
  } else {
    std::cerr << "No path was input: " << report.what() << std::endl;
  }

  return;
}

void test_2()
{
  std::clog << "\ntest_2: basics" << std::endl;

  vire::ui::tui_file_dialog dialog;
  dialog.set_title("Input directory");
  dialog.set_select_dirs(true);
  dialog.set_label("Directory");
  dialog.set_read_mode(true);
  dialog.set_default_path("/data");
  std::string path;
  if (vire::ui::dialog_report report = dialog.input(path)) {
    std::clog << "Directory path is : '" << path << "'" << std::endl;
  } else {
    std::cerr << "No directory path was input: " << report.what() << std::endl;
  }

  return;
}

void test_3()
{
  std::clog << "\ntest_3: basics" << std::endl;

  vire::ui::tui_file_dialog dialog;
  dialog.set_title("Output data file");
  dialog.set_label("File");
  dialog.set_create_mode(true);
  dialog.set_default_path("~/.myapprc");
  std::string path;
  if (vire::ui::dialog_report report = dialog.input(path)) {
    std::clog << "File path is : '" << path << "'" << std::endl;
  } else {
    std::cerr << "No file path was input: " << report.what() << std::endl;
  }

  return;
}

void test_4()
{
  std::clog << "\ntest_4: extension filters" << std::endl;

  vire::ui::tui_file_dialog dialog;
  dialog.set_title("Input data file");
  dialog.set_label("File");
  // dialog.set_read_mode(true);
  dialog.set_default_path("/tmp/a1.data");
  dialog.add_extension_filter(".data", "Data");
  dialog.add_extension_filter(".cal", "Calibrated");
  dialog.add_extension_filter(".cal2", "Calibrated");
  dialog.add_extension_filter(".cal3", "Calibrated");
  dialog.add_extension_filter(".raw", "Raw");
  dialog.add_extension_filter(".digi", "Digitized");
  dialog.add_extension_filter(".digi2", "Digitized");
  std::clog << "Extension filters: "
            << std::boolalpha << dialog.has_extension_filters() << std::endl;
  std::string path;
  if (vire::ui::dialog_report report = dialog.input(path)) {
    std::clog << "File path is : '" << path << "'" << std::endl;
  } else {
    std::cerr << "No file path was input: " << report.what() << std::endl;
  }

  return;
}
