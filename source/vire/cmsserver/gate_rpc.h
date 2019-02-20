//! \file  vire/cmsserver/gate_rpc.h
//! \brief The CMS server gate RPC support for remote client
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

#ifndef VIRE_CMSSERVER_GATE_RPC_H
#define VIRE_CMSSERVER_GATE_RPC_H

// Standard library:
#include <string>
#include <vector>

// Third party:
// - Bayeux/datatools:
#include <datatools/command_utils.h>


// This project:
#include <vire/cmsserver/session_possibility.h>

namespace vire {

  namespace cmsserver {

    class gate;

    /// \brief Gate RPC support
    class gate_rpc
    {
    public:

      /// Constructor
      gate_rpc(gate & g_);

      /// Return the list of possibilities
      datatools::command::returned_info get_possible(const std::string & username_,
                                                     const std::string & password_,
                                                     const std::vector<std::string> & metadata_,
                                                     std::vector<session_possibility> & possibilities_);


    private:

      gate * _gate_ = nullptr; ///< Gate parent

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_GATE_RPC_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
