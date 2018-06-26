// vire/cms/monitoring.cc
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

// Ourselves:
#include <vire/cms/monitoring.h>

namespace vire {

  namespace cms {

    std::string value_monitoring_mode_label(const value_monitoring_mode_type mode_)
    {
      if (mode_ == VALUE_MONITORING_POLLING) return std::string("polling");
      if (mode_ == VALUE_MONITORING_ONCHANGE) return std::string("onchange");
      return std::string("");
    }

    value_monitoring_mode_type value_monitoring_mode(const std::string & label_)
    {
      if (label_ == "polling") return VALUE_MONITORING_POLLING;
      if (label_ == "onchange") return VALUE_MONITORING_ONCHANGE;
      return VALUE_MONITORING_INVALID;
    }

  } // namespace cms

} // namespace vire
