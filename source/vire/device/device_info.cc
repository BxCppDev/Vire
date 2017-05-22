//! \file vire/device/device_info.cc
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
#include <vire/device/device_info.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:

namespace vire {

  namespace device {

    device_info::device_info()
    {
      _logical_ = 0;
      return;
    }

    device_info::~device_info()
    {
      return;
    }

    bool device_info::has_parent_device_id() const
    {
      return _parent_device_id_.is_valid();
    }

    void device_info::set_parent_device_id(const geomtools::geom_id & pdid_)
    {
      _parent_device_id_ = pdid_;
      return;
    }

    const geomtools::geom_id & device_info::get_parent_device_id() const
    {
      return _parent_device_id_;
    }


    bool device_info::has_device_id() const
    {
      return _device_id_.is_valid();
    }

    void device_info::set_device_id(const geomtools::geom_id & did_)
    {
      _device_id_ = did_;
      return;
    }

    const geomtools::geom_id & device_info::get_device_id() const
    {
      return _device_id_;
    }

    bool device_info::has_logical() const
    {
      return _logical_ != 0;
    }

    void device_info::set_logical(const logical_device & l_)
    {
      _logical_ = &l_;
    }

    const logical_device & device_info::get_logical() const
    {
      DT_THROW_IF(! has_logical(), std::logic_error,
                  "Missing logical device !");
      return *_logical_;
    }

    bool device_info::has_serial_number() const
    {
      return ! _serial_number_.empty();
    }

    void device_info::set_serial_number(const std::string & sn_)
    {
      _serial_number_ = sn_;
      return;
    }

    const std::string & device_info::get_serial_number() const
    {
      return _serial_number_;
    }

    bool device_info::is_valid() const
    {
      return _device_id_.is_valid() && has_logical() && _logical_->is_initialized();
    }

    void device_info::reset()
    {
      _device_id_.reset();
      _parent_device_id_.reset();
      _logical_ = 0;
      return;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_, const device_info & ci_)
    {
      out_ << ci_._device_id_ << "="
           << "{"
           << "logical=";
      if (ci_.has_logical()) {
        out_ << ci_.get_logical().get_name();
      } else {
        out_ << "<none>";
      }
      out_ << ";";
      out_ << "parent=";
      if (ci_.has_parent_device_id()) {
        out_ << ci_.get_parent_device_id();
      } else {
        out_ << "<none>";
      }
      out_ << ";";
      out_ << "S/N=";
      if (ci_.has_serial_number()) {
        out_ << "'" << ci_.get_serial_number() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << "}";
      return out_;
    }

  } // end of namespace device

} // end of namespace vire
