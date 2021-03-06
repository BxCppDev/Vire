//! \file  vire/ui/i_password_dialog.h
//! \brief Interface for password dialog
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

#ifndef VIRE_UI_I_PASSWORD_DIALOG_H
#define VIRE_UI_I_PASSWORD_DIALOG_H

// Standard library:
#include <string>

// Third party:
#include <boost/optional.hpp>

// This project:
#include <vire/ui/base_input_dialog.h>

namespace vire {

  namespace ui {

    struct i_password_dialog
      : public base_input_dialog
    {
      i_password_dialog();

      virtual ~i_password_dialog();

      bool is_show_stars() const;

      void set_show_stars(bool ss_);

      bool is_allow_empty_password() const;

      void set_allow_empty_password(bool aep_);

      virtual dialog_report input(std::string & password_) = 0;

    private:

      bool _show_stars_ = false;
      bool _allow_empty_password_ = false;

    };

  } // namespace ui

} // namespace vire

#endif // VIRE_UI_I_PASSWORD_DIALOG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
