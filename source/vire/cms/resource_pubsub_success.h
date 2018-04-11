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
#define VIRE_CMS_RESOURCE_PUBSUB_SUCCESS_H

// This project
#include <vire/utility/base_payload.h>
#include <vire/cms/resource_exec_success.h>

// Third party
// - Boost
#include <boost/optional.hpp>

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
      void set_subscribed(const bool);

      /// Return the subscription flag
      bool is_subscribed() const;

      /// Check if audit is set
      bool has_audit() const;

      /// Return the audit (if set)
      const resource_exec_success & get_audit() const;

      /// Set the audit
      void set_audit(const resource_exec_success &);

      /// Reset the audit
      void reset_audit();

      /// Smart print
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

      bool                                   _subscribed_; ///< Subscription flag
      boost::optional<resource_exec_success> _audit_;      ///< Resource audit (timestamped status+value)


      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_pubsub_success)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_pubsub_success,
                                                  "vire::cms::resource_pubsub_success")

#endif // VIRE_CMS_RESOURCE_PUBSUB_SUCCESS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
