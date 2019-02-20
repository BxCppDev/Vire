#ifndef VIRE_CMSSERVER_TESTING_SC_DRIVER_RUNNER_H
#define VIRE_CMSSERVER_TESTING_SC_DRIVER_RUNNER_H

// Standard library:
#include <chrono>
#include <thread>
#include <random>
#include <iostream>

// This project:
#include <vire/cmsserver/sc_driver.h>

namespace vire {
  namespace cmsserver {
    namespace testing {

      struct sc_driver_runner
      {
        sc_driver_runner(vire::cmsserver::sc_driver & sc_driver_)
          : _sc_driver_(sc_driver_)
        {
          return;
        }
    
        void operator()()
        {
          long seed = 314159;
          std::default_random_engine generator(seed);
          std::uniform_real_distribution<double> distribution(0.0, 1.0);
          int nloops = 12;
          for (int i = 0; i < nloops; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            double r = distribution(generator);
            if (r < 0.25) {
              _sc_driver_.set_connected(true);
            } else {
              _sc_driver_.set_connected(false);
            }
            _sc_driver_.print_tree(std::cerr, datatools::i_tree_dumpable::make_base_print_options("Subcontractor info (updated):"));
            std::cerr << std::endl;
          }
          std::cerr << "sc_runner run stopped.\n";
          return;
        }
 
      private:
  
        vire::cmsserver::sc_driver & _sc_driver_;
  
      };

    } // end namespace testing 
  } // end namespace cmsserver 
} // end namespace cms 

#endif // VIRE_CMSSERVER_TESTING_SC_DRIVER_RUNNER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
