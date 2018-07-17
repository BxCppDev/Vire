//! \file vire/rabbitmq/vhost.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/rabbitmq/vhost.h>

// Third party:
// - Bayeux:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>

// This package:
#include <vire/com/domain.h>
#include <vire/com/domain_builder.h>

namespace vire {

  namespace rabbitmq {

    // static
    bool vhost::validate_name(const std::string & name_)
    {
      if (name_.empty()) {
        return false;
      }
      return vire::com::domain::validate_domain_name(name_);
    }

    vhost::vhost()
    {
      return;
    }

    vhost::vhost(const std::string & name_,
                 const vire::com::domain_category_type category_)
    {
      set_name(name_);
      set_category(category_);
      return;
    }

    const std::string & vhost::get_name() const
    {
      return _name_;
    }

    void vhost::set_name(const std::string & name_)
    {
      DT_THROW_IF(!validate_name(name_), std::logic_error,
                  "Invalid vhost name '" << name_ << "'!");
      _name_ = name_;
      return;
    }

    bool vhost::is_complete() const
    {
      if (_name_.empty()) return false;
      if (_category_ == vire::com::DOMAIN_CATEGORY_INVALID) return false;
      return true;
    }

    const vire::com::domain_category_type vhost::get_category() const
    {
      return _category_;
    }
  
    void vhost::set_category(const vire::com::domain_category_type category_)
    {
      _category_ = category_;
      return;
    }

    void vhost::initialize()
    {
      datatools::properties dummy;
      initialize(dummy);
      return;
    }

    void vhost::initialize(const datatools::properties & config_)
    {
      if (_name_.empty()) {
        if (config_.has_key("name")) {
          set_name(config_.fetch_string("name"));
        }
      }

      if (_category_ == vire::com::DOMAIN_CATEGORY_INVALID) {
        if (config_.has_key("category")) {
          std::string category_repr = config_.fetch_string("category");
          vire::com::domain_category_type category;
          DT_THROW_IF(!vire::com::from_string(category_repr, category),
                      std::logic_error,
                      "Not a valid use category '" << category_repr << "'!");
          set_category(category);
        }
      }
       
      DT_THROW_IF(!is_complete(),
                  std::logic_error,
                  "RabbitMQ vhost is not complete!");
 
      return;
    }

    void vhost::reset()
    {
      _name_.clear();
      _category_ = vire::com::DOMAIN_CATEGORY_INVALID;
       return;
    }

  } // namespace rabbitmq

} // namespace vire
