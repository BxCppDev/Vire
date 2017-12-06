//! \file  vire/cms/disconnection_request.h
//! \brief Connection request to the CMS server by the Vire server
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

#ifndef VIRE_CMS_DISCONNECTION_REQUEST_H
#define VIRE_CMS_DISCONNECTION_REQUEST_H

// Standard library:
#include <string>
#include <vector>

// This project:
#include <vire/utility/base_payload.h>
#include <vire/utility/instance_identifier.h>

// Ourselves:
#include <vire/utility/base_payload.h>

namespace vire {

  namespace cms {

    /// \brief Disconnection request from the Vire server to the CMS server
    ///
    /// Type id: "vire::cms::disconnection_request", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "setup_id": {
    ///      "name"    : "snemo",
    ///      "version" : "1.0.2"
    ///   }
    /// }
    /// @encode
    class disconnection_request
      : public vire::utility::base_payload
    {
    public:
      /// Default constructor
      disconnection_request();

      /// Destructor
      virtual ~disconnection_request();

      /// Reset
      void reset();

      /// Check if setup identifier is set
      bool has_setup_id() const;

      /// Reset the setup identifier
      void reset_setup_id();

      /// Set the setup identifier
      void set_setup_id(const vire::utility::instance_identifier & setup_id_);

      /// Return the setup identifier
      const vire::utility::instance_identifier & get_setup_id() const;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      /// Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      // Attributes:
      vire::utility::instance_identifier _setup_id_;  ///< The setup identifier

      VIRE_UTILITY_PAYLOAD_INTERFACE(disconnection_request)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::disconnection_request,
                                                  "vire::cms::disconnection_request")

#endif // VIRE_CMS_DISCONNECTION_REQUEST_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
