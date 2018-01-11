//! \file  vire/cmsserver/dummy_process.cc
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/dummy_process.h>

namespace vire {

  namespace cmsserver {

    VIRE_CMSSERVER_PROCESS_REGISTRATION_IMPLEMENT(dummy_process,
                                                  "vire::cmsserver::dummy_process");


    // virtual
    bool dummy_process::_is_subprocess_allowed() const
    {
      return true;
    }

    dummy_process::dummy_process()
    {
      return;
    }

    dummy_process::~dummy_process()
    {
      return;
    }

  } // namespace cmsserver

} // namespace vire