//! \file  vire/rabbitmq/permissions_utils.h
//! \brief RabbitMQ permissions management utilities
//
// Copyright (c) 2017 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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

#ifndef VIRE_RABBITMQ_PERMISSIONS_UTILS_H
#define VIRE_RABBITMQ_PERMISSIONS_UTILS_H

// Standard library:
#include <string>

// Third party:
#include <bayeux/rabbitmq/management.h>

namespace vire {

  namespace rabbitmq {

    struct permissions {

      static void add_exchange_service_client_perms(::rabbitmq::permissions & perms_,
                                                    const std::string exchange_name_);

      static void add_exchange_service_server_perms(::rabbitmq::permissions & perms_,
                                                    const std::string exchange_name_);

      static void add_exchange_event_producer_perms(::rabbitmq::permissions & perms_,
                                                    const std::string exchange_name_);

      static void add_exchange_event_listener_perms(::rabbitmq::permissions & perms_,
                                                    const std::string exchange_name_);

      static void add_direct_service_client_perms(::rabbitmq::permissions & perms_);

      static void add_direct_service_server_perms(::rabbitmq::permissions & perms_,
                                                  const std::string queue_name_);

      static void add_direct_event_producer_perms(::rabbitmq::permissions & perms_);

      static void add_direct_event_listener_perms(::rabbitmq::permissions & perms_,
                                                  const std::string queue_name_);

    };

  } // namespace rabbitmq

} // namespace vire

#endif // VIRE_RABBITMQ_PERMISSIONS_UTILS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
