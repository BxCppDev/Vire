//! \file vire/device/physical_device.cc
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
#include <vire/device/physical_device.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/device/base_device_model.h>
#include <vire/device/logical_device.h>

namespace vire {

  namespace device {

    physical_device::physical_device()
    {
      _logical_ = 0;
      return;
    }

    void physical_device::reset()
    {
      _logical_ = 0;
      reset_instance();
      return;
    }

    physical_device::~physical_device()
    {
      reset();
      return;
    }

    bool physical_device::has_logical() const
    {
      return _logical_ != 0;
    }

    void physical_device::set_logical(const logical_device & l_)
    {
      DT_THROW_IF(has_logical(), std::logic_error,
                  "Physical device '" << get_name() << "' already has a logical !");
      _logical_ = &l_;
      return;
    }

    const logical_device & physical_device::get_logical() const
    {
      DT_THROW_IF(! has_logical(), std::logic_error,
                  "Physical device '" << get_name() << "' has no logical !");
      return *_logical_;
    }

    void physical_device::tree_dump(std::ostream& out_,
                                    const std::string& title_,
                                    const std::string& indent_,
                                    bool inherit_) const
    {
      this->base_physical::tree_dump(out_, title_, indent_, true);

      {
        out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
             << "Logical device : ";
        if (! has_logical()) {
          out_ << "<none>";
        } else {
          out_ << "'" << _logical_->get_name() << "'";
        }
        out_ << std::endl;
      }

      return;
    }

  } // end of namespace device

} // end of namespace vire
