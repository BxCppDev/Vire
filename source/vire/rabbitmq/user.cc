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
               const user_category category_)
    {
      set_login(login_);
      set_password(password_);
      set_category(category_);
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

    bool user::has_password() const
    {
      return _password_.length();
    }
    
    void user::set_password(const std::string & password_)
    {
      _password_ = password_;
      return;
    }

    bool user::match_password(const std::string & word_) const
    {
      return word_ == _password_;
    }

    bool user::is_complete() const
    {
      if (_login_.empty()) return false;
      if (_password_.empty()) return false;
      if (_category_ == CATEGORY_INVALID) return false;
      return true;
    }

    bool user::is_server() const
    {
      return _category_ == CATEGORY_SERVER;
    }

    bool user::is_client() const
    {
      return _category_ == CATEGORY_CLIENT;
    }

    bool user::is_subcontractor() const
    {
      return _category_ == CATEGORY_SUBCONTRACTOR;
    }

    bool user::is_system() const
    {
      return _category_ == CATEGORY_SYSTEM;
    }

    const user::user_category user::get_category() const
    {
      return _category_;
    }
  
    void user::set_category(const user_category category_)
    {
      _category_ = category_;
      return;
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
      
      if (_password_.empty()) {
        if (config_.has_key("password")) {
          set_password(config_.fetch_string("password"));
        }
      }

      if (_category_ == CATEGORY_INVALID) {
        if (config_.has_key("category")) {
          std::string category_repr = config_.fetch_string("category");
          if (category_repr == "server") {
            set_category(CATEGORY_SERVER);
          } else if (category_repr == "client") {
            set_category(CATEGORY_CLIENT);
          } else if (category_repr == "system") {
            set_category(CATEGORY_SYSTEM);
          } else if (category_repr == "subcontractor") {
            set_category(CATEGORY_SUBCONTRACTOR);
          } else {
            DT_THROW(std::logic_error,
                     "Not a valid use category '" << category_repr << "'!");
          }
        }
      }
 
      if (config_.has_key("use_monitoring")) {
        use_monitoring = config_.fetch_boolean("use_monitoring");
      }

      if (config_.has_key("use_control")) {
        use_control = config_.fetch_boolean("use_control");
      }
      
      DT_THROW_IF(!is_complete(),
                  std::logic_error,
                  "RabbitMQ user is not complete!");
 
      return;
    }

    void user::reset()
    {
      _login_.clear();
      _password_.clear();
      _category_ = CATEGORY_INVALID;
      use_monitoring = true;
      use_control    = false;
      return;
    }

  } // namespace rabbitmq

} // namespace vire
