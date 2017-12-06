//! \file  vire/cmsserver/utils.h
//! \brief The CMS server utilities
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

#ifndef VIRE_CMSSERVER_UTILS_H
#define VIRE_CMSSERVER_UTILS_H

// Standard Library:
#include <string>
#include <map>

// Third party:
// - Bayeux/datatools:
#include <datatools/logger.h>

namespace vire {

  namespace cmsserver {

    datatools::logger::priority logging(datatools::logger::priority p_
                                        = datatools::logger::PRIO_UNDEFINED);

    typedef std::map<int32_t, std::size_t> cardinalities_request_type;

    struct service {

      /// Default name of the main logging service
      static const std::string & default_log_name();

      /// Default name of the user management service
      static const std::string & default_users_name();

      /// Default name of the user authentication service
      static const std::string & default_authentication_name();

      /// Default name of the device description service
      static const std::string & default_devices_name();

      /// Default name of the resource description service
      static const std::string & default_resources_name();

      /// Default name of the gate service
      static const std::string & default_gate_name();

      /// Default name of the agenda service
      static const std::string & default_agenda_name();

      /// Default name of the Pub/Sub controller service
      static const std::string & default_pubsub_name();

      /// Default name of the control service
      static const std::string & default_control_name();

      /// Default name of the communication service
      static const std::string & default_com_name();

    };

  } // namespace cmsserver

} // namespace vire

#endif // VIRE_CMSSERVER_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
