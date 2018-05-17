//! \file  vire/ui/i_real_dialog.h
//! \brief Interface for real dialog
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

#ifndef VIRE_UI_I_REAL_DIALOG_H
#define VIRE_UI_I_REAL_DIALOG_H

// Standard library:
#include <string>
#include <set>

// Third party:
#include <boost/optional.hpp>

// This project:
#include <vire/ui/base_input_dialog.h>

namespace vire {

  namespace ui {

    struct i_real_dialog
      : public base_input_dialog
    {
      i_real_dialog();

      virtual ~i_real_dialog();

      bool has_unit_dimension() const;

      void set_unit_dimension(const std::string &);

      const std::string & get_unit_dimension() const;

      bool has_preferred_unit() const;

      void set_preferred_unit(const std::string &);

      const std::string & get_preferred_unit() const;

      bool has_default_value() const;

      void set_default_value(const double);

      double get_default_value() const;

      bool has_min_value() const;

      void set_min_value(const double);

      double get_min_value() const;

      bool has_max_value() const;

      void set_max_value(const double);

      double get_max_value() const;

      bool has_precision() const;

      void set_precision(const double);

      double get_precision() const;

      virtual dialog_report input(double & value_) = 0;

    public:

      std::string _unit_dimension_;
      std::string _preferred_unit_;
      boost::optional<double> _min_value_;
      boost::optional<double> _max_value_;
      boost::optional<double> _default_value_;
      boost::optional<double> _precision_;     ///< Absolute precision

    };

  } // namespace ui

} // namespace vire

#endif // VIRE_UI_I_INTEGER_DIALOG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
