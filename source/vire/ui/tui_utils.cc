//! \file vire/ui/tui_utils.cc
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
#include <vire/ui/tui_utils.h>

// Third party:
#include <termios.h>
#include <unistd.h>

namespace vire {

  namespace ui {

    // static
    void tui::set_stdin_echo(bool enable_)
    {
      struct termios tty;
      tcgetattr(STDIN_FILENO, &tty);
      if (! enable_) {
        tty.c_lflag &= ~ECHO;
      } else {
        tty.c_lflag |= ECHO;
      }
      (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    }

  } // namespace ui

} // namespace vire
