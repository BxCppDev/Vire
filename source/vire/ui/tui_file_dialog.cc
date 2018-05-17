//! \file vire/ui/tui_file_dialog.cc
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

// Ourselves:
#include <vire/ui/tui_file_dialog.h>

// Standard library:
#include <iostream>
#include <sstream>
#include <map>

// Third party:
#include <boost/filesystem.hpp>
#include <datatools/utils.h>

// This project:
#include <vire/ui/tui_utils.h>

namespace vire {

  namespace ui {

    tui_file_dialog::tui_file_dialog()
    {
    }

    tui_file_dialog::~tui_file_dialog()
    {
    }

    dialog_report tui_file_dialog::input(std::string & path_)
    {
      path_.clear();
      bool interactive = false;
      if (has_label() || has_title()) {
        interactive = true;
      }
      std::string title;
      if (has_title()) {
        title = get_title();
      }
      std::string label;
      if (has_label()) {
        label = get_label();
      }
      if (interactive) {
        if (!title.empty()) {
          std::clog << std::endl << title << std::endl;
        }
        if (!label.empty()) {
          std::clog << std::endl << label << " ";
          if (has_default_path()) {
            std::clog << '[' << get_default_path() << "] ";
          }
          std::clog << ": ";
        }
      }
      std::string the_path;
      std::string input;
      tui::set_stdin_echo(true);
      std::getline(std::cin, input);
      if (!input.empty()) {
        the_path = input;
      } else {
        if (has_default_path()) {
          the_path = get_default_path();
        }
      }
      if (!the_path.empty()) {
        try {
          std::string test_path = the_path;
          if (is_resolve() && !datatools::fetch_path_with_env(test_path)) {
            return make_dialog_report(false,
                                      "cannot resolve path '" + the_path + "'");
          }
          boost::filesystem::path p(test_path);
          if (is_read_mode() && !boost::filesystem::exists(p)) {
            return make_dialog_report(false,
                                      "path '" + test_path + "' does not exist");
          }
          if (is_create_mode() && boost::filesystem::exists(p)) {
            return make_dialog_report(false,
                                      "path '" + test_path + "' already exists");
          }
          if (is_select_dirs() && !boost::filesystem::is_directory(p)) {
            return make_dialog_report(false,
                                      "path '" + test_path + "' is not a directory");
          }
          std::string path_ext = boost::filesystem::extension(p);
          if (has_extension_filters()) {
            if (!is_allowed_extension(path_ext)) {
              std::string msg;
              msg += "path '";
              msg += test_path;
              msg += "' has no valid extension";
              return make_dialog_report(false, msg);
            }
          }
          path_ = the_path;
          return make_dialog_report(true);
        } catch (std::exception & e) {
          std::string msg("could not input a valid path : ");
          msg += e.what();
          return make_dialog_report(false, msg);
        }
      }
      return make_dialog_report(false, "no valid path was input");
    }

  } // namespace ui

} // namespace vire
