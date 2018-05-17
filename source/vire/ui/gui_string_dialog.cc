//! \file vire/ui/gui_string_dialog.cc
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
#include <vire/ui/gui_string_dialog.h>

// Standard library:
#include <iostream>

// Third party:
// - Qt:
#include <QString>
#include <QApplication>
#include <QInputDialog>
#include <QComboBox>
#include <QStringList>
// - Bayeux:
#include <bayeux/datatools/qt/interface.h>

// This project:
#include <vire/ui/tui_utils.h>

namespace vire {

  namespace ui {

    gui_string_dialog::gui_string_dialog()
    {
      datatools::qt::interface::instance();
      return;
    }

    gui_string_dialog::~gui_string_dialog()
    {
      return;
    }

    dialog_report gui_string_dialog::input(std::string & value_)
    {
      value_.clear();
      QString qtitle;
      if (has_title()) {
        qtitle = QString::fromStdString(get_title());
      } else {
        qtitle = "String Input Dialog";
      }
      QString qlabel = "Value";
      if (has_label()) {
        qlabel = QString::fromStdString(get_label());
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
      std::string input;
      if (menu.size() == 0) {
        bool ok = false;
        QString defval = QString::fromLocal8Bit("");
        if (has_default_value()) {
          defval = QString::fromLocal8Bit(get_default_value().c_str());
        }
        QString text = QInputDialog::getText(nullptr,
                                             qtitle,
                                             qlabel,
                                             QLineEdit::Normal,
                                             defval,
                                             &ok);
        if (ok) {
          if (!text.isEmpty()) {
            input = text.toStdString();
          }
        }
      } else {
        int defaultIndex = 0;
        QStringList texts;
        int counter = 0;
        for (const auto entry : menu) {
          texts << entry.second.c_str();
          if (has_default_value()) {
            if (entry.second == get_default_value()) {
              defaultIndex = counter;
            }
          }
          counter++;
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
            input = text.toStdString();
          }
        }
      }

      if (!input.empty()) {
        value_ = input;
        return make_dialog_report(true);
      }
      return make_dialog_report(false, "no input valid string value");
    }

  } // namespace ui

} // namespace vire
