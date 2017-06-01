//! \file  vire/cms/resource_pubsub_success.h
//! \brief Resource Pub/Sub (un)subscribe success
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMS_RESOURCE_PUBSUB_SUCCESS_H
#define VIRE_CMS_RESOURCE_PUBSUB_success_H

// This project
#include <vire/utility/base_payload.h>

namespace vire {

  namespace cms {

    /// \brief Resource Pub/Sub success
    class resource_pubsub_success
      : public ::vire::utility::base_payload
    {
    public:

      /// Default constructor
      resource_pubsub_success();

      /// Destructor
      virtual ~resource_pubsub_success();

      /// Set the subscription flag
      void set_subscribed(bool);

      /// Return the subscription flag
      bool is_subscribed() const;

    private:

      bool _subscribed_; ///< Subscribe flag

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_fetch_status_failure)

    };

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_RESOURCE_PUBSUB_SUCCESS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
