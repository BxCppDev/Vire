//! \file vire/ui/tui_yesno_question_dialog.cc
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
#include <vire/ui/tui_yesno_question_dialog.h>

// Standard library:
#include <iostream>
#include <sstream>

// Third Party:
#include <boost/algorithm/string.hpp>

// This project:
#include <vire/ui/tui_utils.h>

namespace vire {

  namespace ui {

    tui_yesno_question_dialog::tui_yesno_question_dialog()
    {
      return;
    }

    tui_yesno_question_dialog::~tui_yesno_question_dialog()
    {
      return;
    }

    dialog_report tui_yesno_question_dialog::input(bool & value_)
    {
      value_ = false;
      bool interactive = false;
      if (has_label() || has_title()) {
        interactive = true;
      }
      if (interactive) {
        std::string title;
        if (has_title()) {
          title = get_title();
        }
        std::string label;
        if (has_label()) {
          label = get_label();
        }
        if (!title.empty()) {
          std::clog << std::endl << title << std::endl;
        }
        if (!label.empty()) {
          std::clog << label << ' ';
        }
        if (is_label_as_question()) {
          char chyes = 'y';
          char chno = 'n';
          if (has_default_value()) {
            if (get_default_value()) {
              chyes = 'Y';
            } else {
              chno = 'N';
            }
            std::clog << '(' << chno << '/' << chyes << ") ";
          }
        } else {
          std::clog << '[' << std::boolalpha << false << ',' << std::boolalpha << true << "] ";
          if (has_default_value()) {
            std::clog << '(' << std::boolalpha << get_default_value() << ") ";
          }
        }
        if (is_label_as_question()) {
          std::clog << "? ";
        } else {
          std::clog << ": ";
        }
      }
      std::string input;
      tui::set_stdin_echo(true);
      std::getline(std::cin, input);
      if (!input.empty()) {
        if (input[0] == 'y' ||
            input[0] == 'Y' ||
            boost::algorithm::to_lower_copy(input) == "yes" ||
            boost::algorithm::to_lower_copy(input) == "true" ||
            input[0] == '1') {
          value_ = true;
          return make_dialog_report(true);
        } else if (input[0] == 'n' ||
                   input[0] == 'N'||
                   boost::algorithm::to_lower_copy(input) == "no" ||
                   boost::algorithm::to_lower_copy(input) == "false" ||
                   input[0] == '0') {
          value_ = false;
          return make_dialog_report(true);
        } else {
          std::string msg;
          msg += "invalid answer";
          return make_dialog_report(false, msg);
        }
      } else {
        if (has_default_value()) {
          value_ = get_default_value();
          return make_dialog_report(true);
        }
      }
      return make_dialog_report(false, "no input valid answer value");
    }
    
  } // namespace ui

} // namespace vire
