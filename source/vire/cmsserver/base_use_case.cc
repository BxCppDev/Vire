//! \file vire/cmsserver/base_use_case.cc
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
#include <vire/cmsserver/base_use_case.h>

namespace vire {

  namespace cmsserver {

    // Factory system register :
    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_use_case,
                                                     "vire::cmsserver::base_use_case/_system_")

    base_use_case::base_use_case()
    {
      return;
    }

    base_use_case::~base_use_case()
    {
      if (is_initialized()) {
        DT_THROW(std::logic_error,
                 "Use case is still initialized!");
      }
      return;
    }

    void base_use_case::set_mother_session(session & s_)
    {
      _mother_session_ = &s_;
      return;
    }

    bool base_use_case::has_up_max_duration() const
    {
      return vire::time::is_valid(_up_max_duration_);
    }

    void base_use_case::set_up_max_duration(const boost::posix_time::time_duration & d_)
    {
      _up_max_duration_ = d_;
      return;
    }

    const boost::posix_time::time_duration & base_use_case::get_up_max_duration() const
    {
      return _up_max_duration_;
    }

    bool base_use_case::has_work_min_duration() const
    {
      return vire::time::is_valid(_work_min_duration_);
    }

    void base_use_case::set_work_min_duration(const boost::posix_time::time_duration & d_)
    {
      _work_min_duration_ = d_;
      return;
    }

    const boost::posix_time::time_duration & base_use_case::get_work_min_duration() const
    {
      return _work_min_duration_;
    }

    bool base_use_case::has_work_max_duration() const
    {
      return vire::time::is_valid(_work_max_duration_);
    }

    void base_use_case::set_work_max_duration(const boost::posix_time::time_duration & d_)
    {
      _work_max_duration_ = d_;
      return;
    }

    const boost::posix_time::time_duration & base_use_case::get_work_max_duration() const
    {
      return _work_max_duration_;
    }

    bool base_use_case::has_down_max_duration() const
    {
      return vire::time::is_valid(_down_max_duration_);
    }

    void base_use_case::set_down_max_duration(const boost::posix_time::time_duration & d_)
    {
      _down_max_duration_ = d_;
      return;
    }

    const boost::posix_time::time_duration & base_use_case::get_down_max_duration() const
    {
      return _down_max_duration_;
    }

    bool base_use_case::is_initialized() const
    {
      return _initialized_;
    }

    void base_use_case::initialize_simple()
    {
      datatools::properties dummy_config;
      initialize(dummy_config);
      return;
    }

    void base_use_case::initialize(const datatools::properties & config_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Use case is already initialized!");

      base_use_case::_init_(config_);

      _at_initialized_(config_);

      _initialized_ = true;
      return;
    }

    void base_use_case::_init_(const datatools::properties & config_)
    {
      DT_THROW_IF(_mother_session_ == nullptr, std::logic_error,
                  "No mother session is set!");

      if (! has_up_max_duration()) {
        if (config_.has_key("up_max_duration")) {
          double d = config_.fetch_real("up_max_duration");
          // XXX
        }
      }

      if (! has_work_min_duration()) {
        if (config_.has_key("work_min_duration")) {
          double d = config_.fetch_real("work_min_duration");
          // XXX
        }
      }

      if (! has_work_max_duration()) {
        if (config_.has_key("work_max_duration")) {
          double d = config_.fetch_real("work_max_duration");
          // XXX
        }
      }

      if (! has_down_max_duration()) {
        if (config_.has_key("down_max_duration")) {
          double d = config_.fetch_real("down_max_duration");
          // XXX
        }
      }

      return;
    }

    void base_use_case::_reset_()
    {
      vire::time::invalidate_time_duration(_up_max_duration_);
      vire::time::invalidate_time_duration(_work_min_duration_);
      vire::time::invalidate_time_duration(_work_max_duration_);
      vire::time::invalidate_time_duration(_down_max_duration_);
      _mother_session_ = nullptr;
      return;
    }

    void base_use_case::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Use case is not initialized!");
      _initialized_ = false;

      _at_reset_();

      _reset_();

      return;
    }

    void base_use_case::up()
    {
      _at_up_();
      return;
    }

    void base_use_case::work()
    {
      _at_work_();
      return;
    }

    void base_use_case::down()
    {
      _at_down_();
      return;
    }

  } // namespace cmsserver

} // namespace vire
