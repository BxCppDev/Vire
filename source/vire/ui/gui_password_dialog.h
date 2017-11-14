//! \file  vire/ui/gui_password_dialog.h
//! \brief Test user interface password dialog
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_UI_GUI_PASSWORD_DIALOG_H
#define VIRE_UI_GUI_PASSWORD_DIALOG_H

// Standard library:
#include <string>

namespace vire {

  namespace ui {

    /// \brief Text user interface password dialog
    class gui_password_dialog
    {
    public:

      // Constructor:
      gui_password_dialog(const std::size_t max_attempts_ = 3,
			  bool show_stars_ = true);

      virtual bool input_password(const std::string & prompt_,
				  std::string & password_);

    private:

      std::size_t _max_attempts_;
      bool _show_stars_ = true;

    };

  } // namespace ui

} // namespace vire

#endif // VIRE_UI_GUI_PASSWORD_DIALOG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
