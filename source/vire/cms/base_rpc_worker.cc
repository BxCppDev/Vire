//! \file vire/cms/base_rpc_worker.cc
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
#include <vire/cms/base_rpc_worker.h>

// This project:
#include <vire/utility/invalid_context_error.h>

namespace vire {

  namespace cms {

    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_rpc_worker,
                                                     "vire::cms::base_rpc_worker/__system__")
   
    base_rpc_worker::base_rpc_worker()
    {
      return;
    }
    
    base_rpc_worker::~base_rpc_worker()
    {
      return;
    }

    datatools::logger::priority base_rpc_worker::get_logging() const
    {
      return _logging_;
    }

    void base_rpc_worker::set_logging(const datatools::logger::priority logging_)
    {
      _logging_ = logging_;
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

    bool base_rpc_worker::support_payload_type_id(const std::string & payload_type_id_) const
    {
      return _supported_payload_type_ids_.count(payload_type_id_) > 0;
    }

    const std::set<std::string> &
    base_rpc_worker::get_supported_payload_type_ids() const
    {
      return _supported_payload_type_ids_;
    }

    // virtual
    bool base_rpc_worker::_at_check_(vire::utility::const_payload_ptr_type & request_,
                                     vire::utility::payload_ptr_type & fast_response_)
    {
      fast_response_.reset();
      return true;
    }

    bool
    base_rpc_worker::_check_(vire::utility::const_payload_ptr_type & request_,
                             vire::utility::payload_ptr_type & fast_response_)
    {
      fast_response_.reset();
      // Check for missing request:
      if (request_.get() == nullptr) {
        fast_response_ = std::make_shared<vire::utility::invalid_context_error>("Missing RPC request payload");
        return false;
      }
      // Check for supported types of request:
      if (_supported_payload_type_ids_.size()) {
        const vire::utility::base_payload & request_payload = *request_.get();
        const std::type_info & tinfo = typeid(request_payload);
        std::string request_payload_type_id;
        vire::utility::base_payload::get_system_factory_register().fetch_type_id(tinfo,
                                                                                 request_payload_type_id);
        if (!support_payload_type_id(request_payload_type_id)) {
          std::ostringstream errmsg;
          errmsg << "Unsupported payload type '" << request_payload_type_id << "'";
          fast_response_ = std::make_shared<vire::utility::invalid_context_error>(errmsg.str());
          return false;
        }
      }
      // Additional checks:
      try {
        bool more_check = _at_check_(request_, fast_response_);
        if (!more_check) {
          return false;
        }
      } catch (std::exception & err) {
        std::ostringstream errmsg;
        errmsg << "RPC check error: " << err.what();
        fast_response_ = std::make_shared<vire::utility::invalid_context_error>(errmsg.str());
        return false;
      } catch (...) {
        std::ostringstream errmsg;
        errmsg << "Unexpected RPC check error";
        fast_response_ = std::make_shared<vire::utility::invalid_context_error>(errmsg.str());
        return false;
      }
      return true;
    }

     
    vire::utility::exec_report
    base_rpc_worker::work_sync(vire::utility::const_payload_ptr_type & request_,
                               vire::utility::payload_ptr_type & completion_response_)
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      vire::utility::exec_report execReport;
      execReport.set_code(vire::utility::error::CODE_SUCCESS);
      completion_response_.reset();
      // Perform checks:
      vire::utility::payload_ptr_type fast_response;
      if (!this->_check_(request_, fast_response)) {
        completion_response_ = fast_response;
        execReport.set_code(RPC_ERROR_CHECK_FAILURE);
        execReport.set_message("Missing request payload!");
        return execReport;
      }
      // Apply the worker's task synchronously:
      vire::utility::payload_ptr_type completion_response;
      try {
        this->_at_work_(request_, completion_response_);
      } catch (std::exception & x) {
        execReport.set_code(vire::utility::error::CODE_FAILURE);
        execReport.set_message(x.what());
      }
      return execReport;
    }  
    
    vire::utility::exec_report 
    base_rpc_worker::work_async(vire::utility::const_payload_ptr_type & request_,
                                vire::utility::payload_ptr_type & fast_response_,
                                vire::utility::payload_ptr_type & completion_response_)
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      vire::utility::exec_report execReport;
      execReport.set_code(vire::utility::error::CODE_SUCCESS);
      fast_response_.reset();
      completion_response_.reset();
      // Perform checks:
      vire::utility::payload_ptr_type fast_response;
      if (!this->_check_(request_, fast_response)) {
        fast_response_ = fast_response;
        execReport.set_code(RPC_ERROR_CHECK_FAILURE);
        execReport.set_message("Missing request payload!");
        return execReport;
      }

      return execReport;
    }

    /// Fondamental synchronous work method
    void base_rpc_worker::_at_work_(vire::utility::const_payload_ptr_type & request_,
                                    vire::utility::payload_ptr_type & response_)
    {
      // Default : echo the request 
      // response_ = request_->clone(); // XXX
      // Default : null the response 
      response_.reset();
      return;
    }

    /*
    vire::utility::exec_report
    base_rpc_worker::work(vire::utility::const_payload_ptr_type & request_,
                          vire::utility::payload_ptr_type & response_)
    {
      DT_LOG_TRACE_ENTERING(_logging_);
      vire::utility::exec_report execReport;
      execReport.set_code(vire::utility::error::CODE_SUCCESS);
      if (request_.get() == nullptr) {
        execReport.set_code(RPC_ERROR_MISSING_REQUEST);
        execReport.set_message("Missing request payload!");
        return execReport;
      }
      if (_supported_payload_type_ids_.size()) {
        const vire::utility::base_payload & request_payload = *request_.get();
        const std::type_info & tinfo = typeid(request_payload);
        std::string request_payload_type_id;
        vire::utility::base_payload::get_system_factory_register().fetch_type_id(tinfo,
                                                                                 request_payload_type_id);
        if (!support_payload_type_id(request_payload_type_id)) {
          execReport.set_code(RPC_ERROR_BAD_REQUEST);
          execReport.set_message("Unsupported request payload!");
          return execReport;
        }
      }
      response_.reset();
      try {
        // Callback:
        DT_LOG_DEBUG(_logging_, "Invoking callback method...");
        _at_work_(request_, response_); 
        DT_LOG_DEBUG(_logging_, "Run is done.");
      } catch (std::exception & error) {
        response_.reset();
        execReport.set_code(vire::utility::error::CODE_FAILURE);
        execReport.set_message(error.what());
      } catch (...) {
        response_.reset();
        execReport.set_code(vire::utility::error::CODE_FAILURE);
        execReport.set_message("Unexpected error!");
      }
      DT_LOG_TRACE_EXITING(_logging_);
      return execReport;
    }
*/
    
  } // namespace cms

} // namespace vire
