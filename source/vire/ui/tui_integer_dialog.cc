//! \file vire/ui/tui_integer_dialog.cc
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
#include <vire/ui/tui_integer_dialog.h>

// Standard library:
#include <iostream>
#include <sstream>
#include <map>
#include <limits>

// This project:
#include <vire/ui/tui_utils.h>

namespace vire {

  namespace ui {

    tui_integer_dialog::tui_integer_dialog()
    {
      return;
    }

    tui_integer_dialog::~tui_integer_dialog()
    {
      return;
    }

    dialog_report tui_integer_dialog::input(int & value_)
    {
      value_ = std::numeric_limits<int>::min();
      std::map<int, std::string> menu;
      bool interactive = false;
      std::string title;
      if (has_title() || has_label()) {
        interactive = true;
      }
      if (has_title()) {
        title = get_title();
      }
      std::string label; // = "Value";
      if (has_label()) {
        label = get_label();
      }
      if (interactive) {
        if (!title.empty()) {
          std::clog << std::endl << title << std::endl;
        }
        if (has_allowed_values()) {
          std::clog << "\nSupported values ";
          if (!label.empty()) {
            std::clog << "for " << label;
          }
          std::clog << ": " << std::endl;
          std::clog << "\n";
          std::size_t counter = 0;
          for (const auto val : get_allowed_values()) {
            std::clog << " " << val << '\t';
            if (((++counter % 10) == 0) || (counter == get_allowed_values().size())) {
              std::clog << std::endl;
            }
          }
          std::clog << "\nEnter your choice ";
        } else {
          if (!label.empty()) {
            std::clog << label << " ";
          }
        }
        if (has_min_value() || has_max_value()) {
          int part = 0;
          std::clog << "(";
          if (has_min_value()) {
            std::clog << "min=" << get_min_value();
            part++;
          }
          if (has_max_value()) {
            if (part>0) std::clog << ',';
            std::clog << "max=" << get_max_value();
          }
          std::clog << ") ";
        }
        if (has_default_value()) {
          std::clog << '[' << get_default_value() << "] ";
        }
        std::clog << " : ";
      }
      std::string input;
      tui::set_stdin_echo(true);
      std::getline(std::cin, input);
      int val = std::numeric_limits<int>::min();
      if (!input.empty()) {
        std::istringstream val_iss(input);
        val_iss >> val;
        if (!val_iss) {
          std::string msg;
          msg += "integer input format error from '";
          msg += input;
          msg += "'!";
          return make_dialog_report(false,msg);
        } else {
          if (has_allowed_values()) {
            if (!has_allowed_value(val)) {
              std::string msg;
              msg += "integer value ";
              msg += std::to_string(val);
              msg += " is not allowed!";
              return make_dialog_report(false,msg);
            }
          }
          if (has_min_value()) {
            if (val < get_min_value()) {
              std::string msg;
              msg += "integer value ";
              msg += std::to_string(val);
              msg += " is lower than minimum value ";
              msg += std::to_string(get_min_value());
              return make_dialog_report(false,msg);
            }
          }
          if (has_max_value()) {
            if (val > get_max_value()) {
              std::string msg;
              msg += "integer value ";
              msg += std::to_string(val);
              msg += " is greater than maximum value ";
              msg += std::to_string(get_max_value());
              return make_dialog_report(false,msg);
            }
          }
          value_ = val;
          return make_dialog_report(true);
        }
      } else {
        if (has_default_value()) {
          value_ = get_default_value();
          return make_dialog_report(true);
        }
      }
      return make_dialog_report(false, "no input valid integer value");
    }

  } // namespace ui

} // namespace vire
