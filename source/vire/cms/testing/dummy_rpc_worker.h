#ifndef VIRE_CMS_TESTING_DUMMY_RPC_WORKER_H
#define VIRE_CMS_TESTING_DUMMY_RPC_WORKER_H

// Standard library:
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

// This project:
#include <vire/cms/base_rpc_worker.h>
#include <vire/cms/resource_fetch_status.h>
#include <vire/cms/resource_fetch_status_success.h>
#include <vire/cms/resource_fetch_status_failure.h>
#include <vire/cms/resource_status_record.h>

namespace vire {
  namespace cms {
    namespace testing {

      /// \brief Dummy RPC worker
      class dummy_rpc_worker
        : public base_rpc_worker
      {
      public:
        
        /// Constructor
        dummy_rpc_worker()
        {
          return;
        }

        /// Destructor
        virtual ~dummy_rpc_worker()
        {
          return;
        }
                
      private:

        bool _at_check_(vire::utility::const_payload_ptr_type & request_,
                        vire::utility::payload_ptr_type & fast_response_) override
        {
          auto resFetchStatusPtr = std::dynamic_pointer_cast<const vire::cms::resource_fetch_status>(request_);
          if (resFetchStatusPtr == nullptr) {
            
            return false;
          }
          return true;
        }
    
        void _at_work_(vire::utility::const_payload_ptr_type & request_,
                       vire::utility::payload_ptr_type & response_) override
        {
          std::clog << "Dummy RPC worker is running..." << std::endl;
          std::this_thread::sleep_for(std::chrono::seconds(2));
          auto resFetchStatusPtr = std::dynamic_pointer_cast<const vire::cms::resource_fetch_status>(request_);
          if (resFetchStatusPtr) {
            const std::string & resPath = resFetchStatusPtr->get_path(); 
            vire::cms::resource_status_record resStatRec;
            resStatRec.set_path(resPath);
            resStatRec.set_timestamp(vire::time::now_utc());
            resStatRec.set_disabled();
            response_ = std::make_shared<resource_fetch_status_success>(resStatRec);
          } else {
            std::ostringstream mess;
            mess << "Unsupported payload type '" << request_->get_serial_tag() << "'";
            auto error_ptr = std::make_shared<vire::utility::invalid_context_error>(mess.str());
            response_ = error_ptr;
          }

          return;
        }
        
        VIRE_CMS_RPC_WORKER_REGISTRATION_INTERFACE(dummy_rpc_worker);
        
      };

    } // namespace testing
  } // namespace cms
} // namespace vire

VIRE_CMS_RPC_WORKER_REGISTRATION_IMPLEMENT(vire::cms::testing::dummy_rpc_worker,
                                           "vire::cms::testing::dummy_rpc_worker");

#endif // VIRE_CMS_TESTING_DUMMY_RPC_WORKER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --


