//! \file vire/device/mapping_info.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/device/mapping_info.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/device/logical_device.h>
#include <vire/device/logical_port.h>

namespace vire {

  namespace device {

    mapping_info::mapping_info()
    {
      return;
    }

    mapping_info::~mapping_info()
    {
      reset();
      return;
    }

    bool mapping_info::has_mapping_id() const
    {
      return _mapping_id_.is_valid();
    }

    void mapping_info::set_mapping_id(const geomtools::geom_id & mid_)
    {
      _mapping_id_ = mid_;
      return;
    }

    const geomtools::geom_id & mapping_info::get_mapping_id() const
    {
      return _mapping_id_;
    }

    bool mapping_info::has_parent_device_id() const
    {
      return _parent_device_id_.is_valid();
    }

    void mapping_info::set_parent_device_id(const geomtools::geom_id & pdid_)
    {
      _parent_device_id_ = pdid_;
      return;
    }

    const geomtools::geom_id & mapping_info::get_parent_device_id() const
    {
      return _parent_device_id_;
    }

    bool mapping_info::has_daughter_device_ids() const
    {
      return _daughter_device_ids_.size();
    }

    const std::set<geomtools::geom_id> &
    mapping_info::get_daughter_device_ids() const
    {
      return _daughter_device_ids_;
    }

    void mapping_info::_add_daughter_device_id_(const geomtools::geom_id & mid_)
    {
      _daughter_device_ids_.insert(mid_);
      return;
    }

    bool mapping_info::is_valid() const
    {
      if (!this->instance_info::is_valid()) return false;
      if (!_mapping_id_.is_valid()) return false;
      return true;
    }

    void mapping_info::reset()
    {
      _mapping_id_.reset();
      this->instance_info::reset();
      return;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_, const mapping_info & mi_)
    {
      out_ << mi_._mapping_id_ << "="
           << "{";
      out_ << "logical=";
      if (mi_.has_logical()) {
        if (mi_.is_device()) {
          out_ << mi_.get_logical_device().get_name();
          out_ << " (device)";
        } else if (mi_.is_port()) {
          out_ << mi_.get_logical_port().get_name();
          out_ << " (port)";
        }
      } else {
        out_ << "<none>";
      }
      out_ << ";";
      out_ << "parent=";
      if (mi_.has_parent_device_id()) {
        out_ << mi_.get_parent_device_id();
      } else {
        out_ << "<none>";
      }
      if (mi_.is_device()) {
        out_ << ";";
        out_ << "S/N=";
        if (mi_.has_serial_number()) {
          out_ << "'" << mi_.get_serial_number() << "'";
        } else {
          out_ << "<none>";
        }
      }
      out_ << "}";
      return out_;
    }

  } // end of namespace device

} // end of namespace vire
