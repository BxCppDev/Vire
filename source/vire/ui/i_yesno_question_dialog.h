//! \file  vire/ui/i_yesno_question_dialog.h
//! \brief Interface for yes/no question dialog
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

#ifndef VIRE_UI_I_YESNO_QUESTION_DIALOG_H
#define VIRE_UI_I_YESNO_QUESTION_DIALOG_H

// Standard library:
#include <string>

// Third party:
#include <boost/optional.hpp>

// This project:
#include <vire/ui/base_input_dialog.h>

namespace vire {

  namespace ui {

    ///! \brief Boolean/yes-no question input dialog
    struct i_yesno_question_dialog
      : public base_input_dialog
    {
      i_yesno_question_dialog();

      virtual ~i_yesno_question_dialog();

      bool has_default_value() const;

      void set_default_value(const bool);

      bool get_default_value() const;

      bool is_label_as_question() const;

      void set_label_as_question(bool);
      
      virtual dialog_report input(bool & value_) = 0;

    public:

      bool _label_as_question_ = false;
      boost::optional<bool> _default_value_;

    };

  } // namespace ui

} // namespace vire

#endif // VIRE_UI_VIRE_UI_I_YESNO_QUESTION_DIALOG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
