// vire/cmslapp/connection_request.cc
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

// Ourselves:
#include <vire/cmslapp/connection_success.h>

namespace vire {

  namespace cmslapp {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(connection_success,
                                        "vire::cmslapp::connection_success")

    connection_success::connection_success()
    {
      return;
    }

    connection_success::~connection_success()
    {
      return;
    }

    void connection_success::add_resource_status_record(const vire::cms::resource_status_record & record_)
    {
      _resource_snapshots_.push_back(record_);
      return;
    }

    const std::vector<vire::cms::resource_status_record> &
    connection_success::get_resource_snapshots() const
    {
      return _resource_snapshots_;
    }

  } // namespace cmslapp

} // namespace vire
