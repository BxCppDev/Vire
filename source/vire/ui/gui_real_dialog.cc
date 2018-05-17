//! \file vire/ui/gui_real_dialog.cc
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
#include <vire/ui/gui_real_dialog.h>

// Standard library:
#include <iostream>
#include <sstream>

// Third party:
// - Qt:
#include <QString>
#include <QApplication>
#include <QInputDialog>
#include <QDialog>
// - Bayeux:
#include <bayeux/datatools/qt/interface.h>
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/units.h>

namespace vire {

  namespace ui {

    gui_real_dialog::gui_real_dialog()
    {
      datatools::qt::interface::instance();
      return;
    }

    gui_real_dialog::~gui_real_dialog()
    {
      return;
    }

    dialog_report gui_real_dialog::input(double & value_)
    {
      value_ = std::numeric_limits<int>::min();

      // Unit management:
      std::string display_unit_symbol;
      std::string unit_dimension;
      double display_unit_value = 1.0;
      if (has_preferred_unit()) {
        display_unit_symbol = get_preferred_unit();
        std::string test_unit_dimension;
        if (!datatools::units::find_unit(display_unit_symbol,
                                         display_unit_value,
                                         test_unit_dimension)) {
          DT_THROW(std::logic_error,
                   "Cannot find unit with symbol '" << display_unit_symbol << "'!");
        }
      } else {
        if (has_unit_dimension()) {
          display_unit_symbol = datatools::units::get_default_unit_symbol_from_label(get_unit_dimension());
          if (!datatools::units::find_unit(display_unit_symbol,
                                           display_unit_value,
                                           unit_dimension)) {
            DT_THROW(std::logic_error,
                     "Cannot find unit with symbol '" << display_unit_symbol << "'!");
          }
        }
      }

      QString qtitle;
      if (has_title()) {
        qtitle = QString::fromStdString(get_title());
      } else {
        qtitle = "Real Input Dialog";
      }
      QString qlabel = "Value";
      if (has_label()) {
        qlabel = QString::fromStdString(get_label());
      }
      if (!display_unit_symbol.empty()) {
        std::ostringstream more_ss;
        more_ss << " [" << display_unit_symbol << "]";
        qlabel.append(more_ss.str().c_str());
      }
      double min_val = std::numeric_limits<double>::min();
      if (has_min_value()) {
        min_val = get_min_value();
      }
      double max_val = std::numeric_limits<double>::max();
      if (has_max_value()) {
        max_val = get_max_value();
      }
      bool ok = false;
      double init_val=std::max<double>(min_val, 0.0);
      if (has_default_value()) {
        init_val = get_default_value();
      }
      double rmin = min_val / display_unit_value;
      double rmax = max_val / display_unit_value;
      double rinit_val = init_val / display_unit_value;
      double input = QInputDialog::getDouble(nullptr,
                                             qtitle,
                                             qlabel,
                                             rinit_val,
                                             rmin,
                                             rmax,
                                             1,
                                             &ok);
      if (ok) {
        value_ = input * display_unit_value;
        return make_dialog_report(true);
      }

      return make_dialog_report(false, "no input valid real value");
    }

  } // namespace ui

} // namespace vire