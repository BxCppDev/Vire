//! \file  vire/cms/resource_base_response.h
//! \brief Resource base response
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

#ifndef VIRE_CMS_RESOURCE_BASE_RESPONSE_H
#define VIRE_CMS_RESOURCE_BASE_RESPONSE_H

// Standard Library:
#include <string>

// This project:
#include <vire/utility/base_response.h>
#include <vire/cms/resource_status_record.h>

namespace vire {

  namespace cms {

    /// \brief Resource base response
    class resource_base_response
      : public vire::utility::base_response
    {
    public:

      // Default constructor
      resource_base_response();

      /// Constructor
      resource_base_response(const resource_status_record & record_);

      /// Destructor
      virtual ~resource_base_response();

      /// Check is status is set
      bool has_status() const;

      /// Set the resource status
      void set_status(const resource_status_record & record_);

      /// Reset the resource status
      void reset_status();

      /// Return the resource status
      const resource_status_record & get_status() const;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      resource_status_record _status_; ///< Resource status

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_base_response)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_base_response,
                                                  "vire::cms::resource_base_response")

#endif // VIRE_CMS_RESOURCE_BASE_RESPONSE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
