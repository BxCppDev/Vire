//! \file vire/cms/application.cc
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
#include <vire/cms/application.h>

namespace vire {

  namespace cms {

    
    base_rpc_worker::base_rpc_worker()
    {
      return;
    }

    
    base_rpc_worker::~base_rpc_worker()
    {
      return;
    }

    void base_rpc_worker::add_supported_payload_type_id(const std::string & id_)
    {
      DT_THROW_IF(!vire::utility::base_payload::get_system_factory_register().has(id_),
                  std::logic_error,
                  "No payload with type ID '" << id_
                  << "' is registered in the system factory!");
      _supported_payload_type_ids_.insert(id_);
      return;
    }

    const std::set<std::string> &
    base_rpc_worker::get_supported_payload_type_ids() const
    {
      return _supported_payload_type_ids_;
    }

    vire::utility::exec_report
    base_rpc_worker::work(vire::utility::const_payload_ptr_type & request_,
                          vire::utility::payload_ptr_type & response_)
    {
      vire::utility::exec_report execReport;
      if (request_.get() == nullptr) {
        execReport.set_code(vire::utility::error::CODE_FAILURE);
        execReport.set_message("Missing request payload!");
        return execReport;
      }
      if (_supported_payload_type_ids_.size()) {
        const vire::utility::base_payload & request_payload = *request_.get();
        const std::type_info & tinfo = typeid(request_payload);
        std::string request_payload_type_id;
        vire::utility::base_payload::get_system_factory_register().fetch_type_id(tinfo,
                                                                                 request_payload_type_id);
        if (_supported_payload_type_ids_.count(request_payload_type_id)) {
          execReport.set_code(vire::utility::error::CODE_FAILURE);
          execReport.set_message("Unsupported request payload!");
          return execReport;
        }
      }
      response_.reset();
      try {
        // Callback
        _at_work_(request_, response_);
        
      } catch (std::exception & error) {
        response_.reset();
        execReport.set_code(vire::utility::error::CODE_FAILURE);
        execReport.set_message(error.what());
      } catch (...) {
        response_.reset();
        execReport.set_code(vire::utility::error::CODE_FAILURE);
        execReport.set_message("Unexpected error!");
      }
      return execReport;
    }

  } // namespace cms

} // namespace vire
