//! \file vire/ui/gui_yesno_question_dialog.cc
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
#include <vire/ui/gui_yesno_question_dialog.h>

// Standard library:
#include <iostream>

// Third party:
// - Qt:
#include <QString>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>

// - Bayeux:
#include <bayeux/datatools/qt/interface.h>

// This project:
#include <vire/ui/tui_utils.h>

namespace vire {

  namespace ui {

    gui_yesno_question_dialog::gui_yesno_question_dialog()
    {
      datatools::qt::interface::instance();
      return;
    }

    gui_yesno_question_dialog::~gui_yesno_question_dialog()
    {
      return;
    }

    dialog_report gui_yesno_question_dialog::input(bool & value_)
    {
      value_ = false;
      if (has_default_value()) {
        value_ = get_default_value();
      }
      QString qtitle;
      if (has_title()) {
        qtitle = QString::fromStdString(get_title());
      } else {
        if (is_label_as_question()) {
          qtitle = "Yes/No Question Input Dialog";
        } else {
          qtitle = "Boolean Value Input Dialog";
        }
      }
      QString qlabel = "Answer";
      if (has_label()) {
        qlabel = QString::fromStdString(get_label());
      }
      std::string input;
      if (is_label_as_question()) {
        qlabel.append(" ? ");
        QMessageBox::StandardButtons buttons = QMessageBox::NoButton;
        QMessageBox::StandardButton default_button = QMessageBox::NoButton;
        buttons = QMessageBox::No | QMessageBox::Yes;
        if (has_default_value()) {
          default_button = get_default_value() ? QMessageBox::Yes : QMessageBox::No;
        }
        buttons |= QMessageBox::Cancel;
        QMessageBox::StandardButton choice = QMessageBox::question((QWidget*)nullptr,
                                                                   qtitle,
                                                                   qlabel,
                                                                   buttons,
                                                                   default_button);
        if (choice != QMessageBox::Cancel) {
          if (choice == QMessageBox::Yes) {
            value_ = true;
          } else {
            value_ = false;
          }
          return make_dialog_report(true);
        }
      } else {
        int defaultIndex = 0;
        QStringList texts;
        texts << "false";
        texts << "true";
        if (has_default_value()) {
          if (get_default_value()) {
            defaultIndex = 1;
          }
        }
        bool ok = false;
        QString text = QInputDialog::getItem((QWidget*)nullptr,
                                             qtitle,
                                             qlabel,
                                             texts,
                                             defaultIndex,
                                             false,
                                             &ok);
        if (ok) {
          if (!text.isEmpty()) {
            if (text == "false") {
              value_ = false;
            } else if (text == "true") {
              value_ = true;
            }
            return make_dialog_report(true); 
          }
        }
      }
      return make_dialog_report(false, "no input valid boolean value");
    }

  } // namespace ui

} // namespace vire
