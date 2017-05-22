//! \file  vire/cms/resource_fetch_status_success_response.h
//! \brief Resource fetch status success response
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

#ifndef VIRE_CMS_RESOURCE_FETCH_STATUS_FAILURE_RESPONSE_H
#define VIRE_CMS_RESOURCE_FETCH_STATUS_FAILURE_RESPONSE_H

// Standard Library:
#include <string>

// Third party:
#include <boost/variant.hpp>

// This project:
#include <vire/utility/invalid_context_error.h>
#include <vire/utility/model_identifier.h>
#include <vire/cms/resource_base_response.h>
#include <vire/cms/resource_status_record.h>
#include <vire/cms/invalid_resource_error.h>

namespace vire {

  namespace cms {

    /// \brief Resource fetch status failure response
    class resource_fetch_status_failure_response
      : public resource_base_response
    {
    public:

      /// \brief Error variant
      typedef boost::variant<vire::utility::invalid_context_error,
                             vire::cms::invalid_resource_error> error_type;

      /// Default constructor
      resource_fetch_status_failure_response();

      /// Destructor
      virtual ~resource_fetch_status_failure_response();

      /// Set an invalid context error
      void set_error(const vire::utility::invalid_context_error &);

      /// Set an invalid resource error
      void set_error(const vire::cms::invalid_resource_error &);

      /// Return the error type identifier
      const vire::utility::model_identifier & get_error_type_id() const;

      /// Return the error
      error_type get_error() const;

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

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_fetch_status_failure_response)

    private:

      vire::utility::model_identifier _error_type_id_; ///< The error type identifier
      error_type                      _error_;         ///< The error object

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_fetch_status_failure_response,
                                                  "vire::cms::resource_fetch_status_failure_response")

#endif // VIRE_CMS_RESOURCE_FETCH_STATUS_FAILURE_RESPONSE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
