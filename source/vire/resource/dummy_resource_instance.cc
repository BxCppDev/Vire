// vire/resource/dummy_resource_instance.cc - Implementation of Vire dummy resource instance
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
#include <vire/resource/dummy_resource_instance.h>

namespace vire {

  namespace resource {

    dummy_resource_instance::dummy_resource_instance()
      : base_resource_instance(RESOURCE_CATEGORY_USER)
    {
      _initialized_ = false;
      return;
    }

    dummy_resource_instance::~dummy_resource_instance()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool dummy_resource_instance::is_initialized() const
    {
      return _initialized_;
    }

    void dummy_resource_instance::initialize(const datatools::properties & config_,
                                             datatools::service_manager &)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource instance is already initialized!");

      datatools::enriched_base::initialize(config_);

      _initialized_ = true;
      return;
    }

    void dummy_resource_instance::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Resource instance is not initialized!");
      _initialized_ = false;

      datatools::enriched_base::reset();
      return;
    }

  } // namespace resource

} // namespace vire
