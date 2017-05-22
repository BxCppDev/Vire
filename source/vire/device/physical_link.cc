//! \file vire/device/physical_link.cc
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
#include <vire/device/physical_link.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/device/logical_link.h>
#include <vire/device/physical_port.h>

namespace vire {

  namespace device {

    physical_link::physical_link()
    {
      return;
    }

    physical_link::~physical_link()
    {
      _link_info_.reset();
      return;
    }

    bool physical_link::has_link_info() const
    {
      return _link_info_.is_valid();
    }

    void physical_link::set_link_info(const device_link_info & dli_)
    {
      DT_THROW_IF(has_link_info(), std::logic_error,
                  "Physical link '" << get_name() << "' already has link info !");
      _link_info_ = dli_;
      return;
    }

    const device_link_info & physical_link::get_link_info() const
    {
      DT_THROW_IF(!has_link_info(), std::logic_error,
                  "Physical link '" << get_name() << "' has no link info !");
      return _link_info_;
    }

    void physical_link::tree_dump(std::ostream & out_,
                                  const std::string & title_,
                                  const std::string & indent_,
                                  bool inherit_) const
    {
      this->base_physical::tree_dump(out_, title_, indent_, true);

      {
        out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
             << "Link info: ";
        if (! has_link_info()) {
          out_ << "<none>";
        }
        out_ << std::endl;
        if (has_link_info()) {
          std::ostringstream indent2;
          indent2 << indent_ << i_tree_dumpable::inherit_skip_tag(inherit_);
          _link_info_.tree_dump(out_, "", indent2.str());
        }
      }

      return;
    }

  } // end of namespace device

} // end of namespace vire
