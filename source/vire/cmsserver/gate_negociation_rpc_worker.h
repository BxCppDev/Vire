//! \file  vire/cmsserver/gate_negociation_rpc_worker.h
//! \brief The CMS server gate RPC support for remote client negociation
//
// Copyright (c) 2019 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMSSERVER_GATE_NEGOCIATION_RPC_WORKER_H
#define VIRE_CMSSERVER_GATE_NEGOCIATION_RPC_WORKER_H

// Standard library:
// #include <string>

// Third party:
// - Bayeux/datatools:
#include <datatools/command_utils.h>

// This project:
#include <vire/cmsserver/session_possibility.h>
#include <vire/cms/base_rpc_worker.h>

namespace vire {

  namespace cmsserver {

    class gate;

    /// \brief Gate negociation RPC worker
    class gate_negociation_rpc_worker
      : public vire::cms::base_rpc_worker
    {
    public:

      /// Constructor
      gate_negociation_rpc_worker();

      /// Constructor
      gate_negociation_rpc_worker(gate & parent_);

      /// Set the parent gate
      void set_gate(gate & parent_);
      
    private:

      gate * _gate_ = nullptr; ///< Gate parent

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_GATE_NEGOCIATION_RPC_WORKER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
