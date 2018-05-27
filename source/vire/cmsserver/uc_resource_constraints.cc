//! \file vire/cmsserver/uc_resource_constraints.cc
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
#include <vire/cmsserver/uc_resource_constraints.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace cmsserver {
    
    uc_resource_constraints::uc_resource_constraints()
    {
      return;
    }

    uc_resource_constraints::~uc_resource_constraints()
    {
      return;
    }

    /*
    const ::vire::resource::role & base_use_case::get_minimal_role() const
    {
      if (_minimal_role_ == nullptr) {
        base_use_case * mutable_this = const_cast<base_use_case*>(this);
        mutable_this->_minimal_role_.reset(mutable_this->_create_minimal_role_());
      }
      return *(_minimal_role_.get());
    }

    bool base_use_case::has_role_expression() const
    {
      return !_role_expression_.empty();
    }

    void base_use_case::set_role_expression(const std::string & role_expression_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Use case is already initialized!");
      _role_expression_ = role_expression_;
      return;
    }

    const std::string & base_use_case::get_role_expression() const
    {
      return _role_expression_;
    }
*/
    
  } // namespace cmsserver

} // namespace vire
