//! \file vire/device/instance_info.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/device/instance_info.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/device/logical_device.h>
#include <vire/device/logical_port.h>

namespace vire {

  namespace device {

    instance_info::instance_info()
    {
      _logical_device_ = nullptr;
      _logical_port_   = nullptr;
      return;
    }

    instance_info::~instance_info()
    {
      return;
    }

    bool instance_info::has_path() const
    {
      return ! _path_.empty();
    }

    const std::string & instance_info::get_path() const
    {
      return _path_;
    }

    void instance_info::set_path(const std::string & path_)
    {
      _path_ = path_;
      return;
    }

    bool instance_info::is_device() const
    {
      return _logical_device_ != nullptr;
    }

    bool instance_info::is_port() const
    {
      return _logical_port_ != nullptr;
    }

    bool instance_info::has_logical() const
    {
      return has_logical_device() || has_logical_port();
    }

    bool instance_info::has_logical_device() const
    {
      return _logical_device_ != nullptr;
    }

    bool instance_info::has_logical_port() const
    {
      return _logical_port_ != nullptr;
    }

    void instance_info::set_logical_device(const logical_device & l_)
    {
      _logical_device_ = &l_;
      _logical_port_ = nullptr;
      return;
    }

    const logical_device & instance_info::get_logical_device() const
    {
      DT_THROW_IF(! is_device(), std::logic_error, "Missing logical device !");
      return *_logical_device_;
    }

    void instance_info::set_logical_port(const logical_port & l_)
    {
      _logical_port_ = &l_;
      _logical_device_ = nullptr;
      return;
    }

    const logical_port & instance_info::get_logical_port() const
    {
      DT_THROW_IF(! is_port(), std::logic_error, "Missing logical port !");
      return *_logical_port_;
    }

    bool instance_info::has_serial_number() const
    {
      return ! _serial_number_.empty();
    }

    void instance_info::set_serial_number(const std::string & sn_)
    {
      DT_THROW_IF(!is_device(), std::logic_error, "Only devices can have a serial number!");
      _serial_number_ = sn_;
      return;
    }

    const std::string & instance_info::get_serial_number() const
    {
      return _serial_number_;
    }

    bool instance_info::is_valid() const
    {
      if (! has_logical()) return false;
      if (is_device() && !_logical_device_->is_initialized())  return false;
      if (is_port() && !_logical_port_->is_initialized())  return false;
      return true;
    }

    bool instance_info::has_metadata() const
    {
      return _metadata_.size();
    }

    void instance_info::set_metadata(const datatools::properties & metadata_)
    {
      _metadata_ = metadata_;
      return;
    }

    const datatools::properties & instance_info::get_metadata() const
    {
      return _metadata_;
    }

    datatools::properties & instance_info::grab_metadata()
    {
      return _metadata_;
    }

    void instance_info::reset()
    {
      _metadata_.clear();
      _serial_number_.clear();
      _logical_device_ = nullptr;
      _logical_port_   = nullptr;
      _path_.clear();
      return;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_, const instance_info & ii_)
    {
      out_ << "{path=" << ii_.get_path() << ';';
      out_ << "logical=";
      if (ii_.has_logical()) {
        if (ii_.is_device()) {
          out_ << ii_.get_logical_device().get_name();
          out_ << " (device)";
        } else if (ii_.is_port()) {
          out_ << ii_.get_logical_port().get_name();
          out_ << " (port)";
        }
      } else {
        out_ << "<none>";
      }
      if (ii_.is_device()) {
        out_ << ";";
        out_ << "S/N=";
        if (ii_.has_serial_number()) {
          out_ << "'" << ii_.get_serial_number() << "'";
        } else {
          out_ << "<none>";
        }
      }
      out_ << "}";
      return out_;
    }

  } // end of namespace device

} // end of namespace vire
