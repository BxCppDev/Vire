// vire/resource/base_role_builder.cc
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/resource/base_role_builder.h>

// Ourselves:
#include <vire/resource/manager.h>

namespace vire {

  namespace resource {

    base_role_builder::base_role_builder()
    {
      _logging_priority = datatools::logger::PRIO_FATAL;
      _resource_manager_ = 0;
      return;
    }

    base_role_builder::~base_role_builder()
    {
      // DT_THROW_IF(is_initialized(), std::runtime_error,
      //             "BUG! Builder has not been reset properly at destruction!");
      return;
    }

    void base_role_builder::set_logging_priority(datatools::logger::priority p_)
    {
      _logging_priority = p_;
      return;
    }

    datatools::logger::priority base_role_builder::get_logging_priority() const
    {
      return _logging_priority;
    }

    bool base_role_builder::has_resource_manager() const
    {
      return _resource_manager_ != 0;
    }

    void base_role_builder::set_resource_manager(::vire::resource::manager & resource_manager_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Builder is already initialized!");
      DT_THROW_IF(resource_manager_.is_initialized(), std::logic_error,
                  "Resource manager is already initialized!");

      _resource_manager_ = &resource_manager_;
      return;
    }

    const ::vire::resource::manager & base_role_builder::get_resource_manager() const
    {
      DT_THROW_IF(!has_resource_manager(), std::logic_error,
                  "Builder has no resource manager!");
      return *_resource_manager_;
    }

    ::vire::resource::manager & base_role_builder::_grab_resource_manager()
    {
      return *_resource_manager_;
    }

    void base_role_builder::initialize_simple()
    {
      datatools::properties dummy_config;
      initialize(dummy_config);
      return;
    }

  } // namespace resource

} // namespace vire
