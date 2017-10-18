// vire/cms/resource_pubsub_request.cc
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Ourselves
#include <vire/cms/resource_pubsub.h>


namespace vire {

  namespace cms {

    resource_pubsub::resource_pubsub()
    {
      return;
    }

    resource_pubsub::resource_pubsub(const std::string & path_,
                                                     pubsub_action action_)
    {
      set_path(path_);
      set_action(action_);
      return;
    }

    resource_pubsub::~resource_pubsub()
    {
      return;
    }

    void resource_pubsub::set_path(const std::string & path_)
    {
      _path_ = path_;
      return;
    }

    const std::string & resource_pubsub::get_path() const
    {
      return _path_;
    }

    void resource_pubsub::set_action(pubsub_action action_)
    {
      _action_ = action_;
      return;
    }

    bool resource_pubsub::get_action() const
    {
      return _action_;
    }

    resource_pubsub_success_response::resource_pubsub_success_response()
    {
      return;
    }

    resource_pubsub_success_response::~resource_pubsub_success_response()
    {
      return;
    }

    void resource_pubsub_success_response::set_subscribed(bool s_)
    {
      _subscribed_ = s_;
      return;
    }

    bool resource_pubsub_success_response::is_subscribed() const
    {
      return _subscribed_;
    }

    resource_pubsub_failure_response::resource_pubsub_failure_response()
    {
      return;
    }

    resource_pubsub_failure_response::~resource_pubsub_failure_response()
    {
      return;
    }

    void resource_pubsub_failure_response::set_error(const vire::utility::invalid_context_error & e_)
    {
      _error_type_id_.set_name("vire::utility::invalid_context_error");
      _error_ = e_;
      return;
    }

    void resource_pubsub_failure_response::set_error(const vire::cms::invalid_resource_error & e_)
    {
      _error_type_id_.set_name("vire::cms::invalid_resource_error");
      _error_ = e_;
      return;
    }

    void resource_pubsub_failure_response::set_error(const vire::cms::no_pubsub_resource_error & e_)
    {
      _error_type_id_.set_name("vire::cms::no_pubsub_resource_error");
      _error_ = e_;
      return;
    }

    void resource_pubsub_failure_response::set_error(const vire::cms::invalid_status_error & e_)
    {
      _error_type_id_.set_name("vire::cms::invalid_status_error");
      _error_ = e_;
      return;
    }

    const vire::utility::model_identifier &
    resource_pubsub_failure_response::get_error_type_id() const
    {
      return _error_type_id_;
    }

    resource_pubsub_failure_response::error_type
    resource_pubsub_failure_response::get_error() const
    {
      return _error_;
    }

  } // namespace cms

} // namespace vire
