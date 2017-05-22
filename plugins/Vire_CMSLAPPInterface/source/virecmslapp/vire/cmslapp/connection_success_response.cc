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
#include <vire/cmslapp/connection_success_response.h>

namespace vire {

  namespace cmslapp {

    connection_success_response::connection_success_response()
    {
      return;
    }

    connection_success_response::~connection_success_response()
    {
      return;
    }

    void connection_success_response::add_resource_status_record(const vire::cms::resource_status_record & record_)
    {
      _resources_snapshot_.push_back(record_);
      return;
    }

    const std::vector<vire::cms::resource_status_record> &
    connection_success_response::get_resources_snapshot() const
    {
      return _resources_snapshot_;
    }

  } // namespace cmslapp

} // namespace vire
