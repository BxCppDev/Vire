// vire/time/rt_utils.cc - Implementation of Vire RT time utilities
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
#include <vire/time/rt_utils.h>

// Standard library:
#include <memory>
#include <thread>

// Third party:
// - Bayeux:
#include <bayeux/datatools/clhep_units.h>

namespace vire {

  namespace time {

    const system_time_point & system_epoch()
    {
      static const system_time_point _sysepoch;
      return _sysepoch;
    }

    const system_duration & system_latency()
    {
      static system_duration tick = std::chrono::microseconds(100);
      // std::cerr << "Tick  = " << tick.count() << std::endl;
      static std::unique_ptr<system_duration> _sysl;
      if (_sysl.get() == nullptr) {
        _sysl.reset(new system_duration);
        std::size_t n = 0;
        system_duration sum = std::chrono::seconds(0);
        for (int i = 0; i < 100; i++) {
          system_time_point t1 = std::chrono::system_clock::now();
          std::this_thread::sleep_for(tick);
          system_time_point t2 = std::chrono::system_clock::now();
          system_duration dt = t2 - t1;
          // std::cerr << "Delta time  = " << dt.count() << std::endl;
          sum += dt;
          n++;
        }
        sum /= n;
        *_sysl = sum - tick;
      }
      // std::cerr << "System latency = " << _sysl->count() << std::endl;
      return *_sysl;
    }

    double to_real_with_unit(const system_duration & d_)
    {
      std::size_t count_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(d_).count();
      return count_ns * CLHEP::nanosecond;
    }

  } // namespace time

} // namespace vire

namespace {

  struct _at_start_type {
    _at_start_type() {
      vire::time::system_latency();
    }
  };

  static _at_start_type as;
}
