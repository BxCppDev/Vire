//! \file  vire/cmslapp/connection_success.h
//! \brief Connection success response emitted by the CMS server to the Vire server
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

#ifndef VIRE_CMSLAPP_CONNECTION_SUCCESS_H
#define VIRE_CMSLAPP_CONNECTION_SUCCESS_H

// Standard library:
#include <vector>

// This project:
#include <vire/cms/resource_status_record.h>

namespace vire {

  namespace cmslapp {

    /// \brief Connection success response from the CMS server to the Vire server
    ///
    /// Type id: "vire::cmslapp::connection_success", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "resources_snapshot" : [
    ///     {
    ///        "path" : "SuperNEMO://Demonstrator/CMS/Coil/PS/Control/Current/__dp_read__" ,
    ///        "timestamp" : "20160930173425.257613",
    ///        "status" : 0
    ///     },
    ///     {
    ///        "path" : "SuperNEMO://Demonstrator/CMS/Coil/PS/Control/Current/__dp_write__" ,
    ///        "timestamp" : "20160930173425.257613",
    ///        "status" : 0
    ///     },
    ///     {
    ///        "path" : "SuperNEMO://Demonstrator/CMS/Coil/PS/Control/Voltage/__dp_read__" ,
    ///        "timestamp" : "20160930173425.257613",
    ///        "status" : 0
    ///     },
    ///     ...
    ///     {
    ///        "path" : "SuperNEMO://Demonstrator/CMS/Acquisition/start" ,
    ///        "timestamp" : "20160930173425.257613",
    ///        "status" : 0
    ///     },
    ///     {
    ///        "path" : "SuperNEMO://Demonstrator/CMS/Acquisition/stop" ,
    ///        "timestamp" : "20160930173425.257613",
    ///        "status" : 4
    ///     }
    ///   ]
    /// }
    /// @encode
    class connection_success : public ::vire::utility::base_response
    {
    public:

      /// \brief Dynamic status associated to a resource
      ///
      /// Type id: "resource_status_record", version "1.0"
      ///
      /// @code JSON
      /// {
      ///   "path" : "SuperNEMO://Demonstrator/CMS/Coil/PS/Control/Current/__dp_write__" ,
      ///   "timestamp" : "20160930173425.257613",
      ///   "status" : "0010"
      /// }
      /// @encode

      /// Default constructor
      connection_success();

      /// Destructor
      virtual ~connection_success();

      /// Add a resource status record
      void add_resource_status_record(const vire::cms::resource_status_record & record_);

      /// Return the set of resource status records
      const std::vector<vire::cms::resource_status_record> & get_resource_snapshots() const;

    private:

      std::vector<vire::cms::resource_status_record> _resource_snapshots_; ///< List of resource status snapshots

      VIRE_UTILITY_PAYLOAD_INTERFACE(connection_success)

    };

  } // namespace cmslapp

} // namespace vire

BOOST_CLASS_EXPORT_KEY2(vire::cmslapp::connection_success,"vire::cmslapp::connection_success")

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cmslapp::connection_success, "vire::cmslapp::connection_success")

#endif // VIRE_CMSLAPP_CONNECTION_SUCCESS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
