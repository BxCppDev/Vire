//! \file vire/device/device_types.cc
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
#include "vire/device/device_types.h"

// Third Party
// - Boost
#include "boost/assign.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/bimap.hpp"
#include "boost/bimap/set_of.hpp"
#include "boost/bimap/multiset_of.hpp"

namespace {

  //! Hide Boost bimap declaration in a typedef
  struct device_type_lookup
  {
    typedef boost::bimap<
      boost::bimaps::set_of<std::string>,
      boost::bimaps::multiset_of<vire::device::device_type>
      > lookup_table;
  };

  //! Construct the lookup table.
  device_type_lookup::lookup_table construct_lookup_table()
  {
    device_type_lookup::lookup_table lt;
    boost::assign::insert(lt)
      ("undefined",              vire::device::TYPE_UNDEFINED)
      ("generic",                vire::device::TYPE_GENERIC)
      ("setup",                  vire::device::TYPE_SETUP)
      ("rack",                   vire::device::TYPE_RACK)
      ("crate",                  vire::device::TYPE_CRATE)
      ("module",                 vire::device::TYPE_MODULE)
      ("module_board",           vire::device::TYPE_MODULE_BOARD)
      ("module_mezzanine_board", vire::device::TYPE_MODULE_MEZZANINE_BOARD)
      ("chip",                   vire::device::TYPE_CHIP)
      ("chip_pld",               vire::device::TYPE_CHIP_PLD)
      ("chip_asic",              vire::device::TYPE_CHIP_ASIC)
      ("chip_eprom",             vire::device::TYPE_CHIP_EPROM)
      ("wire",                   vire::device::TYPE_WIRE)
      ("cable",                  vire::device::TYPE_CABLE)
      ("flat_ribbon_cable",      vire::device::TYPE_FLAT_RIBBON_CABLE)
      ("cable_strand",           vire::device::TYPE_CABLE_STRAND)
      ("firmware",               vire::device::TYPE_FIRMWARE)
      ("software",               vire::device::TYPE_SOFTWARE)
      ("datapoint",              vire::device::TYPE_DATAPOINT)
      ("register",               vire::device::TYPE_REGISTER)
      ;
    return lt;
  }

  //! Return priority label stripped of "TYPE_" prefix and lowercased
  std::string get_canonical_label(const std::string & raw_)
  {
    return boost::to_lower_copy(boost::ireplace_first_copy(raw_, "TYPE_", ""));
  }

} // namespace

namespace vire {

  namespace device {

    device_type get_device_type(const std::string & name_)
    {
      static device_type_lookup::lookup_table a;
      if (a.empty()) a = construct_lookup_table();

      device_type_lookup::lookup_table::left_const_iterator p = a.left.find(name_);
      return p != a.left.end() ? p->second : TYPE_UNDEFINED;
    }

    std::string get_device_type_label(device_type dt_)
    {
      static device_type_lookup::lookup_table a;
      if (a.empty()) a = construct_lookup_table();

      device_type_lookup::lookup_table::right_const_iterator n = a.right.find(dt_);
      return n != a.right.end() ? get_canonical_label(n->second) : "";
    }

  } // end of namespace device

} // end of namespace vire
