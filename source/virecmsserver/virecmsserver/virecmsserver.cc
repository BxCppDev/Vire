// virecmsserver.cc - Vire CMS server
//
// Copyright (c) 2016 by François MAUGER <mauger@lpccaen.in2p3.fr>
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <virecmsserver.h>

// Standard library:
#include <string>
#include <functional>
#include <unistd.h>
#include <errno.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>

// Vire:
#include <vire/version.h>
#include <vire/vire.h>

namespace virecmsserver {

  // static
  const unsigned int server::DEFAULT_USLEEP_PERIOD;

  server::server()
  {
    _initialized_ = false;
    return;
  }

  server::~server()
  {
    if (is_initialized()) {
      reset();
    }
    return;
  }

  void server::set_sleep_period(unsigned int p_)
  {
    _usleep_time_ = p_;
    return;
  }

  void server::initialize(const datatools::properties & config_)
  {
    DT_THROW_IF(is_initialized(), std::logic_error,
                "Server is already initialized!");

    set_name("virecmsserver");
    set_display_name("Vire CMS Server");
    set_terse_description("The Vire CMS Server");
    this->datatools::enriched_base::initialize(config_, false);

    if (config_.has_key("usleep_time")) {
      int usleep_time = config_.fetch_integer("usleep_time");
      DT_THROW_IF(usleep_time < 100000 || usleep_time > 10000000,
                  std::range_error,
                  "Invalid sleep time!");
      _usleep_time_ = (unsigned int) usleep_time;
    }

    _k_.initialize(config_);

    _initialized_ = true;
    return;
  }

  void server::reset()
  {
    DT_THROW_IF(!is_initialized(), std::logic_error,
                "Server is not initialized!");
    _initialized_ = false;
    _stop_requested_ = false;
    _running_ = false;
    if (_k_.is_initialized()) {
      _k_.reset();
    }
    _usleep_time_ = DEFAULT_USLEEP_PERIOD;
    return;
  }

  bool server::is_initialized() const
  {
    return _initialized_;
  }

  void server::start()
  {
    // using std::placeholders::_1;
    std::function<void(void)> f_run = std::bind( &server::_run, this);
    _t_.reset(new std::thread(f_run));
    return;
  }

  void server::join()
  {
    DT_THROW_IF(_t_.get() == nullptr, std::logic_error, "Server is not started!");
    _t_->join();
    _t_.reset();
    return;
  }

  void server::stop()
  {
    _stop_requested_ = true;
    // _t_.reset();
    return;
  }

  const vire::cmsserver::kernel & server::get_kernel() const
  {
    return _k_;
  }

  vire::cmsserver::kernel & server::grab_kernel()
  {
    return _k_;
  }

  void server::_run()
  {
    DT_LOG_TRACE_ENTERING(get_logging_priority());
    _running_ = true;
    while (true) {
      int usleep_code = usleep(_usleep_time_);
      if (usleep_code != 0) {
        char buf[256];
        size_t buflen = 256;
        strerror_r(errno, buf, buflen);
        std::string usleep_msg(buf);
        if (errno == EINTR) {
          // Interrupted by a signal
        } else if (errno == EINVAL) {
          // Invalid usleep duration
        }
      }
      // DT_LOG_TRACE(get_logging_priority(), "Tic...");
      _run_loop_impl();
      if (is_stop_requested()) {
        DT_LOG_TRACE(get_logging_priority(), "Stop is requested.");
        break;
      }
    }
    DT_LOG_TRACE(get_logging_priority(), "Main loop ends.");
    _running_ = false;
    DT_LOG_TRACE_EXITING(get_logging_priority());
    return;
  }

  bool server::is_running() const
  {
    return _running_;
  }

  bool server::is_stop_requested() const
  {
    return _stop_requested_;
  }

  void server::_run_loop_impl()
  {
    return;
  }

} // namespace virecmsserver
