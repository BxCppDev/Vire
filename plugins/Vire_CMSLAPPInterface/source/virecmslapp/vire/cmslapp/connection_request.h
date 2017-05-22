//! \file  vire/cmslapp/connection_request.h
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

#ifndef VIRE_CMSLAPP_CONNECTION_REQUEST_H
#define VIRE_CMSLAPP_CONNECTION_REQUEST_H

// Standard library:
#include <string>
#include <vector>

// Third party
// - Boost
#include <boost/cstdint.hpp>
#include <boost/variant.hpp>

// This project:
#include <vire/utility/instance_identifier.h>
#include <vire/utility/base_request.h>
#include <vire/utility/base_response.h>
#include <vire/utility/base_error.h>
#include <vire/utility/invalid_context_error.h>
#include <vire/utility/invalid_setup_id_error.h>
#include <vire/utility/model_identifier.h>
#include <vire/utility/instance_identifier.h>
#include <vire/cms/resource_status_record.h>

namespace vire {

  namespace cmslapp {

    /// \brief Connection request from the Vire server to the CMS server
    ///
    /// Type id: "vire::cmslapp::connection_request", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "setup_id": {
    ///      "name"    : "snemo",
    ///      "version" : "1.0.2"
    ///   },
    ///   "requested_resources" : [
    ///     "SuperNEMO://Demonstrator/CMS/Coil/PS/Control/Current/__dp_read__" ,
    ///     "SuperNEMO://Demonstrator/CMS/Coil/PS/Control/Current/__dp_write__" ,
    ///     "SuperNEMO://Demonstrator/CMS/Coil/PS/Control/Voltage/__dp_read__" ,
    ///     ...
    ///     "SuperNEMO://Demonstrator/CMS/Acquisition/start",
    ///     "SuperNEMO://Demonstrator/CMS/Acquisition/stop"
    ///   ]
    /// }
    /// @encode
    class connection_request
      : public ::vire::utility::base_request
    {
    public:

      /// Default constructor
      connection_request();

      /// Destructor
      virtual ~connection_request();

      /// Set the setup identifier
      void set_setup_id(const vire::utility::instance_identifier & setup_id_);

      /// Return the setup identifier
      const vire::utility::instance_identifier & get_setup_id() const;

      /// Add a requested resource path
      void add_requested_resource(const std::string & path_);

      /// Return the set of requested resource paths
      const std::vector<std::string> & get_requested_resources() const;

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
      std::vector<std::string> _requested_resources_; ///< The set of requested resources addressed by path

      VIRE_UTILITY_PAYLOAD_INTERFACE(connection_request)

    };

  } // namespace cmslapp

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cmslapp::connection_request,
                                                  "vire::cmslapp::connection_request")

#endif // VIRE_CMSLAPP_CONNECTION_REQUEST_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
