//! \file vire/ui/tui_password_dialog.cc
//
// Copyright (c) 2017-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/ui/tui_password_dialog.h>

// Standard library:
#include <iostream>

// This project:
#include <vire/ui/tui_utils.h>

namespace vire {

  namespace ui {

    tui_password_dialog::tui_password_dialog()
    {
      return;
    }

    tui_password_dialog::~tui_password_dialog()
    {
      return;
    }

    dialog_report tui_password_dialog::input(std::string & password_)
    {
      password_.clear();
      bool interactive = false;
      if (has_label() || has_title()) {
        interactive = true;
      }
      if (interactive) {
        if (has_title()) {
          std::clog << get_title() << ' ' << std::endl;
        }
        if (has_label()) std::clog << get_label() << " : ";
      }
      std::string input;
      tui::set_stdin_echo(false);
      std::getline(std::cin, input);
      if (interactive) {
        std::clog << std::endl;
      }
      tui::set_stdin_echo(true);
      if (std::cin) {
        if (input.empty() && ! is_allow_empty_password()) {
          return make_dialog_report(false, "empty password is not allowed");
        }
        password_ = input;
        input.clear();
        return make_dialog_report(true);
      }
      return make_dialog_report(false, "no input password");
    }

  } // namespace ui

} // namespace vire
