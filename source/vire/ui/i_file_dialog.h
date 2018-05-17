//! \file  vire/ui/i_file_dialog.h
//! \brief Interface for file dialog
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

#ifndef VIRE_UI_I_FILE_DIALOG_H
#define VIRE_UI_I_FILE_DIALOG_H

// Standard library:
#include <string>
#include <map>
#include <set>

// Third party:
#include <boost/optional.hpp>

// This project:
#include <vire/ui/base_input_dialog.h>

namespace vire {

  namespace ui {

    struct i_file_dialog
      : public base_input_dialog
    {
      i_file_dialog();

      virtual ~i_file_dialog();

      bool has_default_path() const;

      void set_default_path(const std::string &);

      const std::string & get_default_path() const;

      void reset_default_path();

      bool has_base_dir_path() const;

      void set_base_dir_path(const std::string & path_);

      void reset_base_dir_path();

      const std::string & get_base_dir_path() const;

      bool is_select_dirs() const;

      void set_select_dirs(bool);

      bool is_read_mode() const;

      void set_read_mode(bool);

      bool is_create_mode() const;

      void set_create_mode(bool);

      bool is_resolve() const;

      void set_resolve(bool);

      bool has_extension_filters() const;

      void add_extension_filter(const std::string & ext_,
                                const std::string & category_ = "");

      const std::map<std::string, std::set<std::string>> & get_allowed_extensions() const;

      void build_allowed_extension(std::set<std::string> & exts_,
                                   const std::string & category_ = "") const;

      bool is_allowed_extension(const std::string & ext_) const;

      bool is_allowed_extension_in_category(const std::string & ext_,
                                            const std::string & category_ = "") const;

      virtual dialog_report input(std::string & value_) = 0;

    public:

      boost::optional<std::string> _base_dir_path_;
      boost::optional<std::string> _default_path_;
      bool _no_resolve_ = false;
      bool _select_dirs_ = false;
      bool _read_mode_ = false;
      bool _create_mode_ = false;
      std::map<std::string, std::set<std::string>> _allowed_extensions_;

    };

  } // namespace ui

} // namespace vire

#endif // VIRE_UI_I_FILE_DIALOG_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
