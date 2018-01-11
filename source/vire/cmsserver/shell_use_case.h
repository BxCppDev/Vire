//! \file  vire/cmsserver/shell_use_case.h
//! \brief Vire CMS shell use case
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_SHELL_USE_CASE_H
#define VIRE_CMSSERVER_SHELL_USE_CASE_H

// This project:
#include <vire/cmsserver/detail/use_case_macros.h>
#include <vire/cmsserver/base_use_case.h>

namespace vire {

  namespace cmsserver {

    /// \brief Vire CMS shell use case
    class shell_use_case
      : public base_use_case
    {
    public:

      //! Default constructor
      shell_use_case();

      //! Destructor
      virtual ~shell_use_case();

    private:

      virtual void _at_initialized_(const datatools::properties & config_);

      virtual void _at_reset_();

      virtual void _at_up_();

      virtual void _at_work_();

      virtual void _at_down_();

    private:

      // Class registration:
      VIRE_CMSSERVER_USE_CASE_REGISTRATION_INTERFACE(shell_use_case);

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_SHELL_USE_CASE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
