// vire/cms/utils.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/cms/utils.h>

// Third party:
// - Boost:
// #include <boost/algorithm/string.hpp>
// - Bayeux/datatools:
#include <datatools/logger.h>
// This project:
#include <vire/device/mapping.h>

namespace vire {

  namespace cms {

    // static
    char repr(const boost::tribool & t_)
    {
      if (t_) {
        return '1';
      } else if (!t_) {
        return '0';
      }
      return '?';
    }

  } // namespace cms

} // namespace vire
