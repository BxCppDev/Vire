//  vire/inspector/device_desc.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// This project:
#include <vire/inspector/device_desc.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>

namespace vire {

  namespace inspector {

    device_desc::device_desc()
    {
      return;
    }

    bool device_desc::is_valid() const
    {
      if (_full_path_.empty()) return false;
      if (!has_logical_device()) return false;
      return true;
    }

    void device_desc::set_full_path(const std::string & fp_)
    {
      _full_path_ = fp_;
      return;
    }

    const std::string & device_desc::get_full_path() const
    {
      return _full_path_;
    }

    bool device_desc::has_logical_device() const
    {
      return _dev_logical_ != nullptr;
    }

    void device_desc::set_logical_device(const vire::device::logical_device & dev_logical_)
    {
      _dev_logical_ = &dev_logical_;
      return;
    }

    const vire::device::logical_device & device_desc::get_logical_device() const
    {
      DT_THROW_IF(!has_logical_device(), std::logic_error,
                  "No logical device!");
      return *_dev_logical_;
    }

    bool device_desc::has_minfo() const
    {
      return _minfo_ != nullptr;
    }

    const vire::device::mapping_info & device_desc::get_minfo() const
    {
      DT_THROW_IF(!has_minfo(), std::logic_error,
                  "No mapping info");
      return *_minfo_;
    }

    void device_desc::set_minfo(const vire::device::mapping_info & minfo_)
    {
      _minfo_ = &minfo_;
      return;
    }

    // friend
    std::ostream & operator<<(std::ostream & out_, const device_desc & ddesc_)
    {
      out_ << "(full_path='" << ddesc_.get_full_path() << "',logical=";
      out_ << "_log_foo";
      out_ << ')';
      return out_;
    }

  } // end of namespace inspector

} // end of namespace vire
