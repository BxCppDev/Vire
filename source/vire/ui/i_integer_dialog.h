//! \file  vire/ui/i_integer_dialog.h
//! \brief Interface for integer dialog
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

#ifndef VIRE_UI_I_INTEGER_DIALOG_H
#define VIRE_UI_I_INTEGER_DIALOG_H

// Standard library:
#include <string>
#include <set>

// Third party:
#include <boost/optional.hpp>

// This project:
#include <vire/ui/base_input_dialog.h>

namespace vire {

  namespace ui {

    struct i_integer_dialog
      : public base_input_dialog
    {
      i_integer_dialog();

      virtual ~i_integer_dialog();

      bool has_default_value() const;

      void set_default_value(const int);

      int get_default_value() const;

      bool has_min_value() const;

      void set_min_value(const int);

      int get_min_value() const;

      bool has_max_value() const;

      void set_max_value(const int);

      int get_max_value() const;

      bool has_allowed_values() const;

      const std::set<int> & get_allowed_values() const;

      void add_allowed_value(const int value_);

      bool has_allowed_value(const int value_) const;

      virtual dialog_report input(int & value_) = 0;

    public:

      boost::optional<int> _min_value_;
      boost::optional<int> _max_value_;
      std::set<int> _allowed_values_;
      boost::optional<int> _default_value_;

    };

  } // namespace ui

} // namespace vire

#endif // VIRE_UI_I_INTEGER_DIALOG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
