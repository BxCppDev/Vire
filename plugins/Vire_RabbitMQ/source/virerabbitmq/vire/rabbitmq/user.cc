//! \file vire/rabbitmq/user.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/rabbitmq/user.h>

// Third party:
// - Bayeux:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>

namespace vire {

  namespace rabbitmq {

    // static
    bool user::validate_login(const std::string & login_)
    {
      if (!datatools::name_validation(login_, datatools::NV_INSTANCE)) return false;
      return true;
    }

    user::user()
    {
      return;
    }

    user::user(const std::string & login_,
               const std::string & password_,
               const vire::com::actor::category_type category_,
               const std::string & system_domain_)
    {
      set_login(login_);
      password = password_;
      category = category_;
      if (!system_domain_.empty()) system_domain = system_domain_;
      return;
    }

    const std::string & user::get_login() const
    {
      return _login_;
    }

    void user::set_login(const std::string & login_)
    {
      DT_THROW_IF(!validate_login(login_), std::logic_error,
                  "Invalid user login '" << login_ << "'!");
      _login_ = login_;
      return;
    }

    bool user::is_valid() const
    {
      if (_login_.empty()) return false;
      if (password.empty()) return false;
      if (category == vire::com::actor::CATEGORY_INVALID) return false;
      return true;
    }

    bool user::is_server() const
    {
      return category == vire::com::actor::CATEGORY_SERVER;
    }

    bool user::is_client() const
    {
      return category == vire::com::actor::CATEGORY_CLIENT;
    }

    bool user::is_subcontractor() const
    {
      return category == vire::com::actor::CATEGORY_SUBCONTRACTOR;
    }

    std::string user::get_system_domain() const
    {
      if (system_domain.empty()) return _login_;
      return system_domain;
    }

    void user::initialize()
    {
      datatools::properties dummy;
      initialize(dummy);
      return;
    }

    void user::initialize(const datatools::properties & config_)
    {
      if (_login_.empty()) {
        if (config_.has_key("login")) {
          set_login(config_.fetch_string("login"));
        }
      }
      if (password.empty()) {
        if (config_.has_key("password")) {
          password = config_.fetch_string("password");
        }
      }
      if (category == vire::com::actor::CATEGORY_INVALID) {
        if (config_.has_key("category")) {
          category = vire::com::actor::category(config_.fetch_string("category"));
        }
      }
      DT_THROW_IF(category == vire::com::actor::CATEGORY_INVALID,
                  std::logic_error,
                  "Invalid user category!");

      if (system_domain.empty()) {
        if (config_.has_key("system_domain")) {
          system_domain = config_.fetch_string("system_domain");
        }
      }
      DT_THROW_IF(is_server() && !system_domain.empty(),
                  std::logic_error,
                  "No system domain is associated to a server user!");

      if (config_.has_key("use_monitoring")) {
        use_monitoring = config_.fetch_boolean("use_monitoring");
      }

      if (config_.has_key("use_control")) {
        use_control = config_.fetch_boolean("use_control");
      }

      return;
    }

    void user::reset()
    {
      _login_.clear();
      password.clear();
      category = vire::com::actor::CATEGORY_INVALID;
      system_domain.clear();
      return;
    }

  } // namespace rabbitmq

} // namespace vire
