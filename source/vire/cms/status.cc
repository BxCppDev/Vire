//! \file vire/cms/status.cc
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cms/status.h>

// Third Party
// - Boost
#include <boost/assign.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>
#include <boost/bimap/multiset_of.hpp>

namespace {

  //! Hide Boost bimap declaration in a typedef
  struct status_lookup
  {
    typedef boost::bimap<
      boost::bimaps::set_of<std::string>,
      boost::bimaps::multiset_of<vire::cms::status::flag>
      > lookup_table;
  };

  //! Construct the lookup table.
  status_lookup::lookup_table construct_lookup_table()
  {
    status_lookup::lookup_table lt;
    boost::assign::insert(lt)
      ("failed",   vire::cms::status::failed)
      ("pending",  vire::cms::status::pending)
      ("disabled", vire::cms::status::disabled)
      ("missing",  vire::cms::status::missing)
      ;
    return lt;
  }

  //! Return priority label stripped of "status_" prefix and lowercased
  std::string get_canonical_label(const std::string & raw_)
  {
    return boost::to_lower_copy(boost::ireplace_first_copy(raw_, "status_", ""));
  }

} // namespace

namespace vire {

  namespace cms {

    status::flag status::get_flag(const std::string & label_)
    {
      static status_lookup::lookup_table a;
      if (a.empty()) a = construct_lookup_table();
      status_lookup::lookup_table::left_const_iterator p = a.left.find(label_);
      return p != a.left.end() ? p->second : status::none;
    }

    std::string status::get_flag_label(flag s_)
    {
      static status_lookup::lookup_table a;
      if (a.empty()) a = construct_lookup_table();
      status_lookup::lookup_table::right_const_iterator n = a.right.find(s_);
      return n != a.right.end() ? get_canonical_label(n->second) : "";
    }

  } // end of namespace cms

} // end of namespace vire
