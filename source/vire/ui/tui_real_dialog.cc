//! \file vire/ui/tui_real_dialog.cc
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
#include <vire/ui/tui_real_dialog.h>

// Standard library:
#include <iostream>
#include <sstream>
#include <map>
#include <limits>

// Third party:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/units.h>
#include <bayeux/datatools/utils.h>

// This project:
#include <vire/ui/tui_utils.h>

namespace vire {

  namespace ui {

    tui_real_dialog::tui_real_dialog()
    {
      return;
    }

    tui_real_dialog::~tui_real_dialog()
    {
      return;
    }

    dialog_report tui_real_dialog::input(double & value_)
    {
      value_ = datatools::invalid_real();
      bool interactive = false;
      std::string title;
      if (has_title() || has_label()) {
        interactive = true;
      }
      if (has_title()) {
        title = get_title();
      }
      std::string label;
      if (has_label()) {
        label = get_label();
      }

      // Unit management:
      std::string display_unit_symbol;
      std::string unit_dimension;
      double display_unit = 1.0;
      if (has_preferred_unit()) {
        display_unit_symbol = get_preferred_unit();
        if (!datatools::units::find_unit(display_unit_symbol,
                                         display_unit,
                                         unit_dimension)) {
          DT_THROW(std::logic_error,
                   "Cannot find unit with symbol '" << display_unit_symbol << "'!");
        }
      } else {
        if (has_unit_dimension()) {
          display_unit_symbol = datatools::units::get_default_unit_symbol_from_label(get_unit_dimension());
          if (!datatools::units::find_unit(display_unit_symbol,
                                           display_unit,
                                           unit_dimension)) {
            DT_THROW(std::logic_error,
                     "Cannot find unit with symbol '" << display_unit_symbol << "'!");
          }
        }
      }

      if (interactive) {
        if (!title.empty()) {
          std::clog << std::endl << title << std::endl;
        }
        if (!label.empty()) {
            std::clog << label << " ";
        }
        if (has_min_value() || has_max_value()) {
          int part = 0;
          std::clog << "(";
          if (has_min_value()) {
            std::clog << "min=" << get_min_value() / display_unit;
            if (! display_unit_symbol.empty()) {
              std::clog << " " << display_unit_symbol;
            }
            part++;
          }
          if (has_max_value()) {
            if (part>0) std::clog << ',';
            std::clog << "max=" << get_max_value() / display_unit;
            if (! display_unit_symbol.empty()) {
              std::clog << " " << display_unit_symbol;
            }
         }
          std::clog << ") ";
        }
        if (has_default_value()) {
          std::clog << '[' << get_default_value() /  display_unit;
          if (! display_unit_symbol.empty()) {
            std::clog << " " << display_unit_symbol;
          }
          std::clog << "] ";
        }
        std::clog << " : ";
      } // interactive

      // Parsing:
      std::string input;
      tui::set_stdin_echo(true);
      std::getline(std::cin, input);
      double val = datatools::invalid_real();
      if (!input.empty()) {
        std::string unit_symbol;
        std::string unit_label;
        if (datatools::units::parse_value_with_unit(input, val, unit_symbol, unit_label)) {
          if (!unit_dimension.empty() || !unit_label.empty()) {
            if (unit_dimension != unit_label) {
              std::ostringstream msg;
              msg << "unit dimension '" << unit_label << "' from '"<< input
                  << "' does not match requested '"
                  << unit_dimension << "'!";
              return make_dialog_report(false,msg.str());
            }
          }
        } else {
          std::string msg;
          msg += "real input format error from '";
          msg += input;
          msg += "'!";
          return make_dialog_report(false,msg);
        }
        if (has_min_value()) {
          if (val < get_min_value()) {
            std::string msg;
            msg += "real value ";
            msg += std::to_string(val / display_unit);
            if (! display_unit_symbol.empty()) {
              msg += " ";
              msg += display_unit_symbol;
            }
            msg += " is lower than minimum value ";
            msg += std::to_string(get_min_value() / display_unit);
            if (! display_unit_symbol.empty()) {
              msg += " ";
              msg += display_unit_symbol;
            }
            return make_dialog_report(false,msg);
          }
        }
        if (has_max_value()) {
          if (val > get_max_value()) {
            std::string msg;
            msg += "real value ";
            msg += std::to_string(val / display_unit);
            if (! display_unit_symbol.empty()) {
              msg += " ";
              msg += display_unit_symbol;
            }
            msg += " is greater than maximum value ";
            msg += std::to_string(get_max_value() / display_unit);
            if (! display_unit_symbol.empty()) {
              msg += " ";
              msg += display_unit_symbol;
            }
            return make_dialog_report(false,msg);
          }
        }
        value_ = val;
        return make_dialog_report(true);
      } else {
        if (has_default_value()) {
          value_ = get_default_value();
          return make_dialog_report(true);
        }
      }
      return make_dialog_report(false, "no input valid real value");
    }

  } // namespace ui

} // namespace vire
