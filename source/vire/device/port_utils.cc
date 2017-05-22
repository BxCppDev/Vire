//! \file vire/device/port_utils.cc
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
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves
#include "vire/device/port_utils.h"

// Third Party
// - Boost
#include "boost/assign.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/bimap.hpp"
#include "boost/bimap/set_of.hpp"
#include "boost/bimap/multiset_of.hpp"

namespace {

  //! Hide Boost bimap declaration in a typedef
  struct port_io_type_lookup {
    typedef boost::bimap<
      boost::bimaps::set_of<std::string>,
      boost::bimaps::multiset_of<vire::device::port_io_type>
      > lookup_table;
  };

  //! Hide Boost bimap declaration in a typedef
  struct port_interface_type_lookup {
    typedef boost::bimap<
      boost::bimaps::set_of<std::string>,
      boost::bimaps::multiset_of<vire::device::port_interface_type>
      > lookup_table;
  };

  //! Construct the lookup table.
  port_io_type_lookup::lookup_table construct_io_lookup_table()
  {
    port_io_type_lookup::lookup_table lt;
    boost::assign::insert(lt)
      ("undefined", vire::device::PORT_IO_UNDEFINED)
      ("generic",   vire::device::PORT_IO_GENERIC)
      ("in",        vire::device::PORT_IO_IN)
      ("out",       vire::device::PORT_IO_OUT)
      ("in/out",    vire::device::PORT_IO_IN_OUT)
      ;
    return lt;
  }

  //! Construct the lookup table.
  port_interface_type_lookup::lookup_table construct_interface_lookup_table()
  {
    port_interface_type_lookup::lookup_table lt;
    boost::assign::insert(lt)
      ("undefined", vire::device::PORT_INTERFACE_UNDEFINED)
      ("inside",    vire::device::PORT_INTERFACE_INSIDE)
      ("outside",   vire::device::PORT_INTERFACE_OUTSIDE)
      ;
    return lt;
  }

  //! Return priority label stripped of "PORT_IO_" prefix and lowercased
  std::string get_port_io_canonical_label(const std::string & raw_)
  {
    return boost::to_lower_copy(boost::ireplace_first_copy(raw_, "PORT_IO_", ""));
  }

  //! Return priority label stripped of "PORT_INTERFACE_" prefix and lowercased
  std::string get_port_interface_canonical_label(const std::string & raw_)
  {
    return boost::to_lower_copy(boost::ireplace_first_copy(raw_, "PORT_INTERFACE_", ""));
  }

} // namespace

namespace vire {

  namespace device {

    port_io_type get_port_io_type(const std::string & name_)
    {
      static port_io_type_lookup::lookup_table a;
      if (a.empty()) a = construct_io_lookup_table();

      port_io_type_lookup::lookup_table::left_const_iterator p = a.left.find(name_);
      return p != a.left.end() ? p->second : PORT_IO_UNDEFINED;
    }

    std::string get_port_io_type_label(port_io_type dt_)
    {
      static port_io_type_lookup::lookup_table a;
      if (a.empty()) a = construct_io_lookup_table();

      port_io_type_lookup::lookup_table::right_const_iterator n = a.right.find(dt_);
      return n != a.right.end() ? get_port_io_canonical_label(n->second) : "";
    }

    port_interface_type get_port_interface_type(const std::string & name_)
    {
     static port_interface_type_lookup::lookup_table a;
      if (a.empty()) a = construct_interface_lookup_table();

      port_interface_type_lookup::lookup_table::left_const_iterator p = a.left.find(name_);
      return p != a.left.end() ? p->second : PORT_INTERFACE_UNDEFINED;
    }

    std::string get_port_interface_type_label(port_interface_type pit_)
    {
      static port_interface_type_lookup::lookup_table a;
      if (a.empty()) a = construct_interface_lookup_table();

      port_interface_type_lookup::lookup_table::right_const_iterator n = a.right.find(pit_);
      return n != a.right.end() ? get_port_interface_canonical_label(n->second) : "";
    }

  } // end of namespace device

} // end of namespace vire
