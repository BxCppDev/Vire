//! \file vire/ui/tui_string_dialog.cc
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
#include <vire/ui/tui_string_dialog.h>

// Standard library:
#include <iostream>
#include <sstream>
#include <map>

// This project:
#include <vire/ui/tui_utils.h>

namespace vire {

  namespace ui {

    tui_string_dialog::tui_string_dialog()
    {
      return;
    }

    tui_string_dialog::~tui_string_dialog()
    {
      return;
    }

    dialog_report tui_string_dialog::input(std::string & value_)
    {
      value_.clear();
      bool interactive = false;
      std::string title;
      if (has_label() || has_title()) {
        interactive = true;
      }
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
      }
      std::map<int, std::string> menu;
      int default_menu = 0;
      if (has_allowed_values()) {
        int value_counter = 0;
        for (const auto value : get_allowed_values()) {
          value_counter++;
          menu[value_counter] = value;
          if (has_default_value()) {
            if (value == get_default_value()) {
              default_menu = value_counter;
            }
          }
        }
      }
      if (interactive) {
        if (menu.size() > 0) {
          std::clog << "\nSupported values ";
          if (!label.empty()) {
            std::clog << "for " << label;
          }
          std::clog << ": " << std::endl;
          std::clog << "\n";
          for (const auto entry : menu) {
            std::clog << " " << entry.first << ") " << entry.second << " " << std::endl;
          }
          std::clog << "\nEnter your choice ";
        } else {
          if (!label.empty()) {
            std::clog << label << " ";
          }
        }
        if (has_default_value()) {
          std::clog << '[' << get_default_value() << "]";
        }
        std::clog << ": ";
      }
      std::string input;
      tui::set_stdin_echo(true);
      std::getline(std::cin, input);
      if (!input.empty()) {
        if (menu.size() > 0) {
          std::istringstream menu_iss(input);
          int imenu = 0;
          menu_iss >> imenu;
          if (menu_iss) {
            if (imenu > menu.size() || imenu <= 0) {
              value_.clear();
              std::string msg;
              msg += "invalid menu item";
              return make_dialog_report(false,msg);
            } else {
              value_ = menu.find(imenu)->second;
              return make_dialog_report(true);
            }
          } else {
            if (has_allowed_value(input)) {
              value_ = input;
              return make_dialog_report(true);
            } else {
              std::string msg;
              msg += "unsupported menu item";
              return make_dialog_report(false,msg);
            }
          }
        } else {
          value_ = input;
          return make_dialog_report(true);
        }
      } else {
        if (has_default_value()) {
          value_ = get_default_value();
          return make_dialog_report(true);
        }
      }
      return make_dialog_report(false, "no input valid string value");
    }

  } // namespace ui

} // namespace vire
