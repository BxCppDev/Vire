//! \file vire/ui/gui_password_dialog.cc
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

    gui_password_dialog::gui_password_dialog()
    {
      datatools::qt::interface::instance();
      return;
    }

    gui_password_dialog::~gui_password_dialog()
    {
      return;
    }

    dialog_report gui_password_dialog::input(std::string & password_)
    {
      password_.clear();
      QString qtitle;
      if (has_title()) {
        qtitle = QString::fromStdString(get_title());
      } else {
        qtitle = "Password Input Dialog";
      }
      QString qlabel = "Password";
      if (has_label()) {
        qlabel = QString::fromStdString(get_label());
      }
      std::string input;
      bool ok = false;
      {
        QString text = QInputDialog::getText(0,
                                             qtitle,
                                             qlabel,
                                             (is_show_stars()? QLineEdit::Password : QLineEdit::NoEcho),
                                             QString::fromLocal8Bit(""),
                                             &ok);
        if (ok) {
          if (text.isEmpty() && ! is_allow_empty_password()) {
            return make_dialog_report(false, "empty password is not allowed");
          }
          input = text.toStdString();
        }
      }
      if (ok) {
        password_ = input;
        input.clear();
        return make_dialog_report(true);
      }
      return make_dialog_report(false, "no input password");
    }

  } // namespace ui

} // namespace vire
