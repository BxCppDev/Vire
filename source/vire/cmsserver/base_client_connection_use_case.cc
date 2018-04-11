//! \file vire/cmsserver/base_client_connection_use_case.cc
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
#include <vire/cmsserver/base_client_connection_use_case.h>

// Third party:
// - Bayeux/datatools:

namespace vire {

  namespace cmsserver {

    VIRE_CMSSERVER_USE_CASE_REGISTRATION_IMPLEMENT(base_client_connection_use_case,
                                                   "vire::cmsserver::base_client_connection_use_case");

    base_client_connection_use_case::base_client_connection_use_case()
    {
      return;
    }

    base_client_connection_use_case::~base_client_connection_use_case()
    {
      return;
    }

    void base_client_connection_use_case::set_check_stop_period(const std::size_t p_)
    {
      _check_stop_period_ = p_;
      return;
    }

    std::size_t base_client_connection_use_case::get_check_stop_period() const
    {
      return _check_stop_period_;
    }

    void base_client_connection_use_case::_at_initialize_(const datatools::properties & config_) //override
    {
      if (config_.has_key("check_stop_period")) {

      }
       // if (config_.has_key("start_macro")) {
      //        std::string start_macro_name = config_.fetch_string("start_macro");
      //        macro start;
      //        start.load(start_macro_name);
      //        _start_macro_ = start;
      // }

      // if (config_.has_key("stop_macro")) {
      //        std::string stop_macro_name = config_.fetch_string("stop_macro");
      //        macro stop;
      //        stop.load(stop_macro_name);
      //        _stop_macro_ = start;
      // }

      return;
    }

    void base_client_connection_use_case::_at_terminate_() //override
    {
      _start_macro_ = boost::none;
      _stop_macro_ = boost::none;
      return;
    }


    void base_client_connection_use_case::_at_run_prepare_() final
    {
      return;
    }

    void base_client_connection_use_case::_at_run_terminate_() final
    {
      return;
    }

    void base_client_connection_use_case::_at_run_functional_work_loop_iteration_() final
    {
      // wait for connection (interruptible through the _check_stop_requested method)

      // client connection handling
      // ---[------------]------> t
      //
      return;
    }

  } // namespace cmsserver

} // namespace vire
