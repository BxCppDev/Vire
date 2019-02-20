//! \file  vire/cms/base RPC worker.h
//! \brief Vire CMS base RPC worker
//
// Copyright (c) 2019 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2019 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//
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

// Third party:
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/factory_macros.h>

// This project:
#include <vire/utility/base_payload.h>
#include <vire/utility/exec_report.h>

namespace vire {

  namespace cms {

    /// \brief Base class for all RPC worker classes
    class base_rpc_worker
    {
    public:

      enum rpc_error_type {
        RPC_ERROR_CHECK_FAILURE   = 100,
        RPC_ERROR_MISSING_REQUEST = 101,
        RPC_ERROR_BAD_REQUEST     = 102,
        RPC_ERROR_BAD_ARGUMENT    = 103,
        RPC_ERROR_BAD_CONTEXT     = 104
      };
      
      /// Constructor
      base_rpc_worker();

      /// Destructor
      virtual ~base_rpc_worker();

      datatools::logger::priority get_logging() const;

      void set_logging(const datatools::logger::priority);
      
      void add_supported_payload_type_id(const std::string &);

      const std::set<std::string> & get_supported_payload_type_ids() const;

      bool support_payload_type_id(const std::string &) const;
      
      vire::utility::exec_report work_sync(vire::utility::const_payload_ptr_type & request_,
                                           vire::utility::payload_ptr_type & completion_response_);
      
      vire::utility::exec_report work_async(vire::utility::const_payload_ptr_type & request_,
                                            vire::utility::payload_ptr_type & fast_response_,
                                            vire::utility::payload_ptr_type & completion_response_);

    private:

      /*
      vire::utility::exec_report _work_(vire::utility::const_payload_ptr_type & request_,
                                        vire::utility::payload_ptr_type & fast_response_,
                                        vire::utility::payload_ptr_type & completion_response_);
      */

      /// Perform basic checks
      bool _check_(vire::utility::const_payload_ptr_type & request_,
                   vire::utility::payload_ptr_type & fast_response_);
      
      virtual bool _at_check_(vire::utility::const_payload_ptr_type & request_,
                              vire::utility::payload_ptr_type & fast_response_);
    
      virtual void _at_work_(vire::utility::const_payload_ptr_type & request_,
                             vire::utility::payload_ptr_type & completion_response_) /* = 0 */;
      
    private:

      /// Logging priority threshold
      datatools::logger::priority _logging_ = datatools::logger::PRIO_FATAL;
      
      /// Set of supported payload type IDs
      std::set<std::string> _supported_payload_type_ids_;
      
      // Factory stuff :
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(base_rpc_worker)
     
    };
    
  } // namespace cms

} // namespace vire

#define VIRE_CMS_RPC_WORKER_REGISTRATION_INTERFACE(RPC_WORKER_CLASS_NAME) \
  private:                                                              \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_INTERFACE (::vire::cms::base_rpc_worker,RPC_WORKER_CLASS_NAME) \
  /**/

#define VIRE_CMS_RPC_WORKER_REGISTRATION_IMPLEMENT(RPC_WORKER_CLASS_NAME,RPC_WORKER_CLASS_ID) \
  DATATOOLS_FACTORY_SYSTEM_AUTO_REGISTRATION_IMPLEMENTATION (::vire::cms::base_rpc_worker,RPC_WORKER_CLASS_NAME,RPC_WORKER_CLASS_ID) \
  /**/

#endif // VIRE_CMS_BASE_RPC_WORKER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
