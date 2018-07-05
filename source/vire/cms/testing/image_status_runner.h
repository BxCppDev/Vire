#ifndef VIRE_CMS_TESTING_IMAGE_STATUS_RUNNER_H
#define VIRE_CMS_TESTING_IMAGE_STATUS_RUNNER_H

// Standard library:
#include <chrono>
#include <thread>
#include <random>
#include <iostream>

// This project:
#include <vire/cms/image_status.h>
#include <vire/time/utils.h>

namespace vire {
  namespace cms {
    namespace testing {
      
      struct image_status_runner
      {
        image_status_runner(vire::cms::image_status & status_, const long seed_ = 314159)
          : _status_(status_)
          , _seed_(seed_)
        {
          return;
        }
  
        void operator()()
        {
         std::default_random_engine generator(_seed_);
          std::uniform_real_distribution<double> distribution(0.0, 1.0);
          int nloops = 25;
          for (int i = 0; i < nloops; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            _status_.set_timestamp(vire::time::now());
      
            double r = distribution(generator);
            if (r < 0.15) {
              if (debug) std::cerr << "reset_missing\n";
              _status_.reset_missing();
            } else if (r < 0.5) {
              if (debug) std::cerr << "set_missing\n";
              _status_.set_missing(true);
            } else {
              if (debug) std::cerr << "unset_missing\n";
              _status_.set_missing(false);
            }
      
            r = distribution(generator);
            if (r < 0.10) {
              if (debug) std::cerr << "reset_disabled\n";
              _status_.reset_disabled();
            } else if (r < 0.2) {
              if (debug) std::cerr << "set_disabled\n";
              _status_.set_disabled(true);
            } else {
              if (debug) std::cerr << "unset_disabled\n";
              _status_.set_disabled(false);
            }
      
            r = distribution(generator);
            if (r < 0.05) {
              if (debug) std::cerr << "reset_pending\n";
              _status_.reset_pending();
            } else if (r < 0.15) {
              if (debug) std::cerr << "set_pending\n";
              _status_.set_pending(true);
            } else {
              if (debug) std::cerr << "unset_pending\n";
              _status_.set_pending(false);
            }
      
            r = distribution(generator);
            if (r < 0.05) {
              if (debug) std::cerr << "reset_failed\n";
              _status_.reset_failed();
            } else if (r < 0.15) {
              if (debug) std::cerr << "set_failed\n";
              _status_.set_failed(true);
            } else {
              if (debug) std::cerr << "unset_failed\n";
              _status_.set_failed(false);
            }
            if (debug) {
              _status_.tree_dump(std::cerr, "Updated image status : ");
            }
            if (debug) std::cerr << std::endl;
          }
          if (debug) std::cerr << "status_runner run stopped.\n";
          return;
        }

      public:
        
        bool debug = false;

      private:
        
        vire::cms::image_status & _status_;
        long _seed_;
  
      };
      
    } // end namespace testing 
  } // end namespace cms 
} // end namespace vire 

#endif // VIRE_CMS_TESTING_IMAGE_STATUS_RUNNER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
