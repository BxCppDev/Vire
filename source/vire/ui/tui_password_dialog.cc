//! \file vire/ui/tui_password_dialog.cc
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

// Ourselves:
#include <vire/ui/tui_password_dialog.h>

// Standard library:
#include <iostream>

// This project:
#include <vire/ui/tui_utils.h>

namespace vire {

  namespace ui {

    tui_password_dialog::tui_password_dialog(const std::size_t max_attempts_)
      : _max_attempts_(max_attempts_ > 0 ? max_attempts_ : 1)
    {
      return;
    }

    bool tui_password_dialog::input_password(const std::string & prompt_,
                                             std::string & password_)
    {
      password_.clear();
      std::size_t attempt_counter = 0;
      while (attempt_counter < _max_attempts_) {
        if (!prompt_.empty()) std::clog << prompt_;
        attempt_counter++;
        std::string input;
        tui::set_stdin_echo(false);
        std::getline(std::cin, input);
        //if (input.empty()) {
          std::clog << std::endl;
          //}
        tui::set_stdin_echo(true);
        if (!input.empty()) {
          password_ = input;
          input.clear();
          return true;
        }
      }
      return false;
    }

  } // namespace ui

} // namespace vire
