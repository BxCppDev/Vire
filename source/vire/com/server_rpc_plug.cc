//! \file vire/com/server_rpc_plug.cc
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
#include <vire/com/server_rpc_plug.h>

namespace vire {

  namespace com {

    server_rpc_plug::server_rpc_plug(const manager & mgr_)
      : base_plug(mgr_)
    {
      return;
    }

    server_rpc_plug::~server_rpc_plug()
    {
      return;
    }

  } // namespace com

} // namespace vire
