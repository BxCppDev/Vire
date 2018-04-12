//! \file vire/auth/password_entry.cc
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
#include <vire/auth/password_entry.h>

// Standard libraries:
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <cstring>

namespace vire {

  namespace auth {

    password_entry::password_entry()
    {
      _password_[0] = 0;
      return;
    }

    password_entry::~password_entry()
    {
      for (int i = 0; i < BUFFER_SIZE; i++) {
        _password_[i]= 0;
      }
      return;
    }

    std::string password_entry::operator()(const std::string & message_)
    {
      // Source : https://stackoverflow.com/questions/1786532/c-command-line-password-input
      bool out = false;
      if (!message_.empty()) {
        out = true;
      }
      if (out) {
        std::fputs(message_.c_str(), stderr);
      }
      static struct termios old_terminal;
      static struct termios new_terminal;

      // Get settings of the actual terminal
      tcgetattr(STDIN_FILENO, &old_terminal);

      // Do not echo the characters
      new_terminal = old_terminal;
      new_terminal.c_lflag &= ~(ECHO);

      // Set this as the new terminal options
      tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);

      // Get the password:
      // the user can add chars and delete if he puts it wrong
      // the input process is done when he hits the enter
      // the \n is stored, we replace it with \0
      if (fgets(_password_, BUFFER_SIZE, stdin) == NULL) {
        _password_[0] = '\0';
      } else {
        _password_[strlen(_password_)-1] = '\0';
      }
      if (out) {
        std::fputs("\n", stderr);
      }
      // Go back to the old settings
      tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
      return std::string(_password_);
    }

  } // namespace auth

} // namespace vire
