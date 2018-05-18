//! \file  vire/ui/gui_yesno_question_dialog.h
//! \brief Test user interface string dialog
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

#ifndef VIRE_UI_GUI_YESNO_QUESTION_DIALOG_H
#define VIRE_UI_GUI_YESNO_QUESTION_DIALOG_H

// Standard library:
#include <string>

// This project:
#include <vire/ui/i_yesno_question_dialog.h>

namespace vire {

  namespace ui {

    /// \brief Graphical user interface yes/no question (boolean) dialog
    class gui_yesno_question_dialog
      : public i_yesno_question_dialog
    {
    public:

      // Constructor:
      gui_yesno_question_dialog();

      virtual ~gui_yesno_question_dialog();

      virtual dialog_report input(bool & value_) final;

    };

  } // namespace ui

} // namespace vire

#endif // VIRE_UI_GUI_YESNO_QUESTION_DIALOG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
