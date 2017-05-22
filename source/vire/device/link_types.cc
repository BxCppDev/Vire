//! \file vire/device/link_types.cc
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
#include "vire/device/link_types.h"

// Third Party
// - Boost
#include "boost/assign.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/bimap.hpp"
#include "boost/bimap/set_of.hpp"
#include "boost/bimap/multiset_of.hpp"

namespace {

  //! Hide Boost bimap declaration in a typedef
  struct link_type_lookup {
    typedef boost::bimap<
      boost::bimaps::set_of<std::string>,
      boost::bimaps::multiset_of<vire::device::link_type>
      > lookup_table;
  };

  //! Construct the lookup table.
  link_type_lookup::lookup_table construct_lookup_table()
  {
    link_type_lookup::lookup_table lt;
    boost::assign::insert(lt)
      ("undefined",  vire::device::LINK_UNDEFINED)
      ("generic",    vire::device::LINK_GENERIC)
      ("pipe",       vire::device::LINK_PIPE)
      ("cable",      vire::device::LINK_CABLE)
      ("trace",      vire::device::LINK_TRACE)
      ("bus",        vire::device::LINK_BUS)
      ("dependency", vire::device::LINK_DEPENDENCY)
      ;
    return lt;
  }

  //! Return priority label stripped of "TYPE_" prefix and lowercased
  std::string get_canonical_label(const std::string & raw_)
  {
    return boost::to_lower_copy(boost::ireplace_first_copy(raw_, "LINK_", ""));
  }

} // namespace

namespace vire {

  namespace device {

    link_type get_link_type(const std::string & name_)
    {
      static link_type_lookup::lookup_table a;
      if (a.empty()) a = construct_lookup_table();

      link_type_lookup::lookup_table::left_const_iterator p = a.left.find(name_);
      return p != a.left.end() ? p->second : LINK_UNDEFINED;
    }

    std::string get_link_type_label(link_type dt_)
    {
      static link_type_lookup::lookup_table a;
      if (a.empty()) a = construct_lookup_table();

      link_type_lookup::lookup_table::right_const_iterator n = a.right.find(dt_);
      return n != a.right.end() ? get_canonical_label(n->second) : "";
    }

    bool match_link_type(link_type lt1_, link_type lt2_)
    {
      if (lt1_ == LINK_GENERIC || lt2_ == LINK_GENERIC) return true;
      if (lt1_ == lt2_) return true;
      return false;
    }

  } // end of namespace device

} // end of namespace vire
