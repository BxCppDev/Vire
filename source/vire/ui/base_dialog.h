//! \file  vire/ui/base_dialog.h
//! \brief Base dialog
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

#ifndef VIRE_UI_BASE_DIALOG_H
#define VIRE_UI_BASE_DIALOG_H

// Standard library:
#include <string>

// Third party:
#include <boost/optional.hpp>

namespace vire {

  namespace ui {

    struct dialog_report
    {
      bool success = true;
      std::string message;
      operator bool() const;
      const std::string & what() const;

    };

    dialog_report make_dialog_report(const bool = true,
                                     const std::string & = "");

    struct base_dialog
    {
    public:

      base_dialog();

      virtual ~base_dialog();

      bool has_title() const;

      void set_title(const std::string &);

      void reset_title();

      const std::string & get_title() const;

    public:

      boost::optional<std::string> _title_;

    };

  } // namespace ui

} // namespace vire

#endif // VIRE_UI_BASE_DIALOG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
