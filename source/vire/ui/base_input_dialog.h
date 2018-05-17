//! \file  vire/ui/base_input_dialog.h
//! \brief Base input dialog
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

#ifndef VIRE_UI_BASE_INPUT_DIALOG_H
#define VIRE_UI_BASE_INPUT_DIALOG_H

// Standard library:
#include <string>

// Third party:
#include <boost/optional.hpp>

// This project:
#include <vire/ui/base_dialog.h>

namespace vire {

  namespace ui {

    struct base_input_dialog
      : public base_dialog
    {
    public:

      base_input_dialog();

      virtual ~base_input_dialog();

      bool has_label() const;

      void set_label(const std::string &);

      void reset_label();

      const std::string & get_label() const;

    public:

      boost::optional<std::string> _label_;

    };

  } // namespace ui

} // namespace vire

#endif // VIRE_UI_BASE_INPUT_DIALOG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
