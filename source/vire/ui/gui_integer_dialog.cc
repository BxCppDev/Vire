//! \file vire/ui/gui_integer_dialog.cc
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
#include <vire/ui/gui_integer_dialog.h>

// Standard library:
#include <iostream>
#include <sstream>

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

    gui_integer_dialog::gui_integer_dialog()
    {
      datatools::qt::interface::instance();
      return;
    }

    gui_integer_dialog::~gui_integer_dialog()
    {
      return;
    }

    dialog_report gui_integer_dialog::input(int & value_)
    {
      value_ = std::numeric_limits<int>::min();
      QString qtitle;
      if (has_title()) {
        qtitle = QString::fromStdString(get_title());
      } else {
        qtitle = "Integer Input Dialog";
      }
      QString qlabel = "Value";
      if (has_label()) {
        qlabel = QString::fromStdString(get_label());
      }
      if (has_allowed_values()) {
        int defaultIndex = 0;
        QStringList texts;
        int counter = 0;
        for (const auto value : get_allowed_values()) {
          texts << std::to_string(value).c_str();
          if (has_default_value()) {
            if (value == get_default_value()) {
              defaultIndex = counter;
            }
          }
          counter++;
        }
        bool ok = false;
        QString input = QInputDialog::getItem((QWidget*)nullptr,
                                              qtitle,
                                              qlabel,
                                              texts,
                                              defaultIndex,
                                              false,
                                              &ok);
        if (ok) {
          std::string inputRepr = input.toStdString();
          std::istringstream input_iss(inputRepr);
          int val;
          input_iss >> val;
          if (input_iss) {
            value_ = val;
            return make_dialog_report(true);
          }
        }
      } else {
        int min = std::numeric_limits<int>::min();
        if (has_min_value()) {
          min = get_min_value();
        }
        int max = std::numeric_limits<int>::max();
        if (has_max_value()) {
          max = get_max_value();
        }
        bool ok = false;
        int init_val=std::max(min,0);
        if (has_default_value()) {
          init_val = get_default_value();
        }
        int input = QInputDialog::getInt(nullptr,
                                         qtitle,
                                         qlabel,
                                         init_val,
                                         min,
                                         max,
                                         1,
                                         &ok);
        if (ok) {
          value_ = input;
          return make_dialog_report(true);
        }
      }
      return make_dialog_report(false, "no input valid integer value");
    }

  } // namespace ui

} // namespace vire
