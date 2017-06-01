//! \file  vire/cms/resource_pubsub.h
//! \brief Resource Pub/Sub (un)subscribe
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

#ifndef VIRE_CMS_RESOURCE_PUBSUB_H
#define VIRE_CMS_RESOURCE_PUBSUB_H

// Standard Library:
#include <string>

// Third party:
#include <boost/variant.hpp>

// This project
#include <vire/utility/base_payload.h>

namespace vire {

  namespace cms {

    /// \brief Resource Pub/Sub action
    class resource_pubsub
      : public ::vire::utility::base_payload
    {
    public:

      /// \brief
      enum pubsub_action {
        ACTION_INVALID     = 0, ///< Invalid action
        ACTION_CHECK       = 1, ///< Check the Pub/sub status of the resource
        ACTION_SUBSCRIBE   = 2, ///< Ask for the Pub/sub subscription of the resource
        ACTION_UNSUBSCRIBE = 3  ///< Ask for the Pub/sub unsubscription of the resource
      };

      /// Default constructor
      resource_pubsub();

      /// Constructor
      resource_pubsub(const std::string & path_, pubsub_action action_);

      /// Destructor
      virtual ~resource_pubsub();

      /// Set the path
      void set_path(const std::string &);

      /// Return the path
      const std::string & get_path() const;

      /// Set the subscribe flag
      void set_action(pubsub_action action_);

      /// Return the subscribe flag
      bool get_action() const;

    private:

      // Attributes:
      std::string   _path_;   ///< Resource path
      pubsub_action _action_ = ACTION_INVALID; ///< Pub/sub action

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_fetch_status_failure)

    };


  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_RESOURCE_PUBSUB_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
