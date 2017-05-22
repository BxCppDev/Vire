//! \file vire/device/port_types.cc
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
#include <vire/device/port_types.h>

// Third Party
// - Boost
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>
#include <boost/bimap/multiset_of.hpp>

namespace {

  //! Hide Boost bimap declaration in a typedef
  struct port_type_lookup {
    typedef boost::bimap<
      boost::bimaps::set_of<std::string>,
      boost::bimaps::multiset_of<vire::device::port_type>
      > lookup_table;
  };

  //! Construct the lookup table.
  port_type_lookup::lookup_table construct_lookup_table()
  {
    port_type_lookup::lookup_table lt;
    boost::assign::insert(lt)
      ("undefined",              vire::device::PORT_UNDEFINED)
      ("generic",                vire::device::PORT_GENERIC)
      ("plug",                   vire::device::PORT_PLUG)
      ("method",                 vire::device::PORT_METHOD)
      ;
    return lt;
  }

  //! Return priority label stripped of "TYPE_" prefix and lowercased
  std::string get_canonical_label(const std::string & raw_)
  {
    return boost::to_lower_copy(boost::ireplace_first_copy(raw_, "PORT_", ""));
  }

} // namespace

namespace vire {

  namespace device {

    port_type get_port_type(const std::string & name_)
    {
      static port_type_lookup::lookup_table a;
      if (a.empty()) a = construct_lookup_table();

      port_type_lookup::lookup_table::left_const_iterator p = a.left.find(name_);
      return p != a.left.end() ? p->second : PORT_UNDEFINED;
    }

    std::string get_port_type_label(port_type dt_)
    {
      static port_type_lookup::lookup_table a;
      if (a.empty()) a = construct_lookup_table();

      port_type_lookup::lookup_table::right_const_iterator n = a.right.find(dt_);
      return n != a.right.end() ? get_canonical_label(n->second) : "";
    }

    bool match_port_type(port_type pt1_, port_type pt2_)
    {
      if (pt1_ == PORT_GENERIC || pt2_ == PORT_GENERIC) return true;
      if (pt1_ == pt2_) return true;
      return false;
    }

  } // end of namespace device

} // end of namespace vire
