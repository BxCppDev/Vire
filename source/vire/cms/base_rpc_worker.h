//! \file  vire/cms/base RPC worker.h
//! \brief Vire CMS base RPC worker
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

#ifndef VIRE_CMS_BASE_RPC_WORKER_H
#define VIRE_CMS_BASE_RPC_WORKER_H

// Standard library:
#include <string>
#include <set>

// This project:
#include <vire/utility/base_payload.h>
#include <vire/utility/exec_report.h>

namespace vire {

  namespace cms {

    class base_rpc_worker
    {
    public:

      /// Constructor
      base_rpc_worker();

      /// Destructor
      virtual ~base_rpc_worker();

      void add_supported_payload_type_id(const std::string &);

      const std::set<std::string> & get_supported_payload_type_ids() const;

      vire::utility::exec_report work(vire::utility::const_payload_ptr_type & request_,
                                      vire::utility::payload_ptr_type & response_);
             
    private:

      virtual void _at_work_(vire::utility::const_payload_ptr_type & request_,
                             vire::utility::payload_ptr_type & response_) =  0;
      
    private:

      std::set<std::string> _supported_payload_type_ids_;
      
      
    };
    
  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_BASE_RPC_WORKER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
