//! \file    vire/plugin.h
//! \brief   Utilities for accessing Vire plugin apps and libraries
//! \details The Vire library accepts several plugin apps and libraries
//!
//!          As these files are not compiled into the Vire library,
//!          Vire provides a simple API to get a path to known
//!          plugin DLL.
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_PLUGIN_H
#define VIRE_PLUGIN_H

// Standard Library:
#include <string>

namespace vire {

  //! Return URL, i.e. a path, to the  base directory where plugin DLL files are installed
  //! \param overriden_env If set this flag trigger the search for the
  //!        environment variable VIRE_PLUGIN_LIB_DIR as a directory path on the filesystem
  //!        as an alternative base directory for resources in place of the
  //!        standard installation path
  std::string get_plugin_dir(bool overriden_env = false);

  //! Return URL, i.e. a path, to named resource
  //! By default the encoded resource root, determined at compile time
  //! is used to search for the resource. This can be overriden by setting
  //! the VIRE_PLUGIN_LIB_DIR environment variable to a path holding custom
  //! resources.
  //! \param rname name of resource, given as a path relative to
  //!        resource root.
  //! \param overriden_env flag to allow path overriding by the VIRE_PLUGIN_LIB_DIR environment variable.
  std::string get_plugin(const std::string& rname, bool overriden_env = false);

} // namespace vire

#endif // VIRE_PLUGIN_H

/*
** Local Variables: --
** mode: c++ --
** c-file-style: "gnu" --
** tab-width: 2 --
** End: --
*/
