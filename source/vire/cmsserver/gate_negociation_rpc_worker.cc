//! \file vire/cmsserver/gate_negociation_rpc_worker.cc
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

// Ourselves:
#include <vire/cmsserver/gate_negociation_rpc_worker.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/cmsserver/gate.h>
#include <vire/time/utils.h>

namespace vire {

  namespace cmsserver {

    gate_negociation_rpc_worker::gate_negociation_rpc_worker()
    {
      return;
    }

    gate_negociation_rpc_worker::gate_negociation_rpc_worker(gate & gate_)
      : _gate_(&gate_)
    {
      return;
    }

    void gate_negociation_rpc_worker::set_gate(gate & gate_)
    {
      _gate_ = &gate_;
      return;
    }

    // datatools::command::returned_info
    // gate_rpc::get_possible(const std::string & username_,
    //                        const std::string & password_,
    //                        const std::vector<std::string> & metadata_,
    //                        std::vector<session_possibility> & possibilities_)
    // {
    //   datatools::command::returned_info cri;
    //   possibilities_.clear();
    //   return cri;
    // }

  } // namespace cmsserver

} // namespace vire
