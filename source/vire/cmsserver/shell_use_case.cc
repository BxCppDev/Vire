//! \file  vire/cmsserver/shell_use_case.cc
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

// Ourselves:
#include <vire/cmsserver/shell_use_case.h>

namespace vire {

  namespace cmsserver {

    VIRE_CMSSERVER_USE_CASE_REGISTRATION_IMPLEMENT(shell_use_case,
                                                   "vire::cmsserver::shell_use_case");

    shell_use_case::shell_use_case()
    {
      return;
    }

    shell_use_case::~shell_use_case()
    {
      return;
    }

    void shell_use_case::_at_initialized_(const datatools::properties & config_)
    {
      return;
    }

    void shell_use_case::_at_reset_()
    {
      return;
    }

    void shell_use_case::_at_up_()
    {
      return;
    }

    void shell_use_case::_at_work_()
    {
      // TO BE IMPLEMENTED
      // while (true) {
      // }
      return;
    }

    void shell_use_case::_at_down_()
    {
      return;
    }

  } // namespace cmsserver

} // namespace vire
