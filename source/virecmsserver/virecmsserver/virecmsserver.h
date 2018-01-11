// virecmsserver.h - Vire CMS server
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

#ifndef VIRECMSSERVER_H
#define VIRECMSSERVER_H

// Standard Library:
#include <string>
#include <memory>
#include <thread>

// Third Party:
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/enriched_base.h>

// Vire:
// #include <vire/cmsserver/kernel.h>

namespace virecmsserver {

  //! \brief The CMS server
  class server
    : public datatools::enriched_base
  {

  public:

    static const unsigned int DEFAULT_USLEEP_PERIOD = 250000; // us

    //! Default constructor
    server();

    //! Destructor
    virtual ~server();

    void set_sleep_period(unsigned int);

    //! Initialization from a container of properties
    void initialize(const datatools::properties & config_);

    //! Clear
    void reset();

    //! Check initialization flag
    bool is_initialized() const;

    //! Start
    void start();

    //! Stop
    void stop();

    //! Join
    void join();

    //! Check if stop is requested
    bool is_stop_requested() const;

    //! Check if server is running
    bool is_running() const;

    //! Return
    const vire::cmsserver::kernel & get_kernel() const;

    //! Return
    vire::cmsserver::kernel & grab_kernel();

  protected:

    //! Run
    void _run();

    void _run_loop_impl();

  private:

    // Configuration:
    unsigned int _usleep_time_ = DEFAULT_USLEEP_PERIOD;

    // Management and internal working data:
    bool _initialized_ = false;
    bool _running_     = false;
    bool _stop_requested_ = false;
    vire::cmsserver::server _k_;
    std::unique_ptr<std::thread> _t_;

  };

} // namespace virecmsserver

#endif // VIRECMSSERVER_H


// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
