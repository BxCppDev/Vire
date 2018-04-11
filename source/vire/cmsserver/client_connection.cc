//! \file vire/cmsserver/client_connection.cc
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
#include <vire/cmsserver/client_connection.h>

// This project:
#include <vire/time/utils.h>
#include <vire/cmsserver/session.h>

namespace vire {

  namespace cmsserver {

    client_connection::client_connection()
      : _time_interval_(vire::time::invalid_time_interval())
    {
      return;
    }

    client_connection::~client_connection()
    {
      return;
    }

    void client_connection::set_mother_session(const session & s_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Client connection is already initialized!");
      _mother_session_ = &s_;
      return;
    }

    bool client_connection::has_mother_session() const
    {
      return _mother_session_ != nullptr;
    }

    const session & client_connection::get_mother_session() const
    {
      return *_mother_session_;
    }

    bool client_connection::has_id() const
    {
      return _id_ >= 0;
    }

    void client_connection::set_id(const int32_t id_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Client connection is already initialized!");
      _id_ = id_ < 0 ? -1 : id_;
      return;
    }

    int32_t client_connection::get_id() const
    {
      return _id_;
    }

    bool client_connection::has_user_login() const
    {
      return ! _user_login_.empty();
    }

    const std::string & client_connection::get_user_login() const
    {
      return _user_login_;
    }

    void client_connection::set_user_login(const std::string & user_login_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Client connection is already initialized!");
      _user_login_ = user_login_;
      return;
    }

    bool client_connection::has_user_password() const
    {
      return ! _user_password_.empty();
    }

    const std::string & client_connection::get_user_password() const
    {
      return _user_password_;
    }

    void client_connection::set_user_password(const std::string & user_password_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Client connection is already initialized!");
      _user_password_ = user_password_;
      return;
    }

    bool client_connection::has_time_interval() const
    {
      return vire::time::is_valid(_time_interval_);
    }

    const boost::posix_time::time_period & client_connection::get_time_interval() const
    {
      return _time_interval_;
    }

    void client_connection::set_time_interval(const boost::posix_time::time_period & time_interval_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Client connection is already initialized!");
      _time_interval_ = time_interval_;
      return;
    }

    bool client_connection::is_initialized() const
    {
      return _initialized_;
    }

    bool client_connection::is_valid() const
    {
      if (_id_ == -1) return false;
      if (_user_login_.empty()) return false;
      if (_user_password_.empty()) return false;
      if (!vire::time::is_valid(_time_interval_)) return false;
      if (_mother_session_ == nullptr) return false;
      return true;
    }

    void client_connection::initialize()
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Client connection is already initialized!");
      DT_THROW_IF(!is_valid(), std::logic_error, "Client connection is not valid! Initalization failed!");

       _at_connect_();

      _initialized_ = true;
      return;
    }

    void client_connection::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error, "Client connection is not initialized!");
      _initialized_ = false;

      _at_disconnect_();

      _id_ = -1;
      _user_login_.clear();
      _user_password_.clear();
      vire::time::invalidate(_time_interval_);
      _mother_session_ = nullptr;
      return;
    }

    void client_connection::_at_connect_()
    {

      return;
    }

    void client_connection::_at_disconnect_()
    {
      return;
    }

  } // namespace cmsserver

} // namespace vire
