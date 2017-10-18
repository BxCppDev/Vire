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

      /// Return the label associated to a Pub/Sub action
      static std::string action_to_label(const pubsub_action);

      /// Return the Pub/Sub action associated to a label
      static pubsub_action action_from_label(const std::string & label_,
                                             pubsub_action & action_);

      /// Default constructor
      resource_pubsub();

      /// Constructor
      resource_pubsub(const std::string & path_, const pubsub_action action_);

      /// Destructor
      virtual ~resource_pubsub();

      /// Reset
      void reset();

      /// Check if path is set
      bool has_path() const;

      /// Set the path
      void set_path(const std::string &);

      /// Return the path
      const std::string & get_path() const;

      /// Reset the path
      void reset_path();

      /// Check if path is set
      bool has_action() const;

      /// Set the subscribe flag
      void set_action(const pubsub_action action_);

      /// Return the subscribe flag
      pubsub_action get_action() const;

      /// Reset the path
      void reset_action();

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

    private:

      // Attributes:
      std::string   _path_;   ///< Resource path
      pubsub_action _action_ = ACTION_INVALID; ///< Pub/sub action

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_pubsub)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_pubsub,
                                                  "vire::cms::resource_pubsub")

#endif // VIRE_CMS_RESOURCE_PUBSUB_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
