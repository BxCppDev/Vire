//! \file vire/ui/gui_password_dialog.cc
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
#include <vire/ui/gui_password_dialog.h>

// Standard library:

// Third party:
// - Qt:
#include <QString>
#include <QApplication>
#include <QInputDialog>
// - Bayeux:
#include <bayeux/datatools/qt/interface.h>

// This project:
#include <vire/ui/tui_utils.h>

namespace vire {

  namespace ui {

    gui_password_dialog::gui_password_dialog(const std::size_t max_attempts_,
                                             bool show_stars_)
      : _max_attempts_(max_attempts_ > 0 ? max_attempts_ : 1)
      , _show_stars_(show_stars_)
    {
      return;
    }

    bool gui_password_dialog::input_password(const std::string & prompt_,
                                             std::string & password_)
    {
      password_.clear();
      std::size_t attempt_counter = 0;
      // datatools::qt::;
      while (attempt_counter < _max_attempts_) {
        attempt_counter++;
        std::string title;
        if (!prompt_.empty()) {
          title = prompt_;
        } else {
          title = "Input password";
        }

        std::string input;
        {
          bool ok = false;
          QString text = QInputDialog::getText(0,
                                               QString::fromStdString(title),
                                               QString::fromLocal8Bit("Password: "),
                                               (_show_stars_ ? QLineEdit::Password : QLineEdit::NoEcho),
                                               QString::fromLocal8Bit(""),
                                               &ok);
          //,
          //                                   Qt::Popup);
          if (ok && !text.isEmpty()) {
            input = text.toStdString();
          }
        }
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
