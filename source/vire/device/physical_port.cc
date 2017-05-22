//! \file vire/device/physical_port.cc
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
#include <vire/device/physical_port.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/exception.h>

// This project:
#include <vire/device/logical_port.h>
#include <vire/device/physical_link.h>

namespace vire {

  namespace device {

    physical_port::physical_port()
    {
      _logical_ = 0;
      _inside_link_ = 0;
      _outside_link_ = 0;
      return;
    }

    physical_port::~physical_port()
    {
      return;
    }

    bool physical_port::has_logical() const
    {
      return _logical_ != 0;
    }

    void physical_port::set_logical(const logical_port & l_)
    {
      DT_THROW_IF(has_logical(), std::logic_error,
                  "Physical port '" << get_name() << "' already has a logical !");
      _logical_ = &l_;
      return;
    }

    const logical_port & physical_port::get_logical() const
    {
      DT_THROW_IF(! has_logical(), std::logic_error,
                  "Physical port '" << get_name() << "' has no logical !");
      return *_logical_;
    }

    bool physical_port::has_inside_link() const
    {
      return _inside_link_ != 0;
    }

    bool physical_port::has_outside_link() const
    {
      return _outside_link_ != 0;
    }

    bool physical_port::has_link(port_interface_type interface_) const
    {
      if (interface_ == PORT_INTERFACE_INSIDE) {
        return has_inside_link();
      } else if (interface_ == PORT_INTERFACE_OUTSIDE) {
        return has_outside_link();
      }
      DT_THROW(std::logic_error, "Invalid port interface type!");
    }

    void physical_port::set_inside_link(const physical_link & pl_)
    {
      _inside_link_ = &pl_;
      return;
    }

    void physical_port::reset_inside_link()
    {
      _inside_link_ = 0;
      return;
    }

    void physical_port::set_outside_link(const physical_link & pl_)
    {
      _outside_link_ = &pl_;
      return;
    }

    void physical_port::reset_outside_link()
    {
      _outside_link_ = 0;
      return;
    }

    void physical_port::set_link(const physical_link & pl_,
                                 port_interface_type interface_)
    {
      if (interface_ == PORT_INTERFACE_INSIDE) {
        set_inside_link(pl_);
      } else if (interface_ == PORT_INTERFACE_OUTSIDE) {
        set_outside_link(pl_);
      }
      DT_THROW(std::logic_error, "Invalid port interface type!");
      return;
    }

    void physical_port::reset_link(port_interface_type interface_)
    {
      if (interface_ == PORT_INTERFACE_INSIDE) {
        reset_inside_link();
      } else if (interface_ == PORT_INTERFACE_OUTSIDE) {
        reset_outside_link();
      }
      DT_THROW(std::logic_error, "Invalid port interface type!");
      return;
    }

    const physical_link & physical_port::get_inside_link() const
    {
      DT_THROW_IF(! has_inside_link(), std::logic_error,
                  "Physical port '" << get_name() << "' has no inside link !");
      return *_inside_link_;
    }

    const physical_link & physical_port::get_outside_link() const
    {
      DT_THROW_IF(! has_outside_link(), std::logic_error,
                  "Physical port '" << get_name() << "' has no outside link !");
      return *_outside_link_;
    }

    const physical_link & physical_port::get_link(port_interface_type interface_) const
    {
      if (interface_ == PORT_INTERFACE_INSIDE) {
        return get_inside_link();
      } else if (interface_ == PORT_INTERFACE_OUTSIDE) {
        return get_outside_link();
      }
      DT_THROW(std::logic_error, "Invalid port interface type!");
   }

    void physical_port::tree_dump(std::ostream & out_,
                                  const std::string & title_,
                                  const std::string & indent_,
                                  bool inherit_) const
    {
      this->base_physical::tree_dump(out_, title_, indent_, true);

      {
        out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
             << "Logical port : ";
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
