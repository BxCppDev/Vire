//! \file  vire/cms/resource_status_failure.h
//! \brief Resource status failure response
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

#ifndef VIRE_CMS_RESOURCE_STATUS_FAILURE_H
#define VIRE_CMS_RESOURCE_STATUS_FAILURE_H

// Standard Library:
#include <string>

// Third party:
#include <boost/variant.hpp>

// This project:
#include <vire/utility/invalid_context_error.h>
#include <vire/utility/model_identifier.h>
#include <vire/utility/base_payload.h>
#include <vire/cms/resource_status_record.h>
#include <vire/cms/invalid_resource_error.h>

namespace vire {

  namespace cms {

    /// \brief Resource status failure
    class resource_status_failure
      : public ::vire::utility::base_payload
    {
    public:

      /// \brief Error variant
      typedef boost::variant<vire::cms::invalid_resource_error,
                             vire::cms::invalid_status_error> error_type;

      /// Default constructor
      resource_status_failure();

      /// Destructor
      virtual ~resource_status_failure();

      /// Set an invalid resource error
      void set_error(const vire::cms::invalid_resource_error &);

      /// Set an invalid status error
      void set_error(const vire::cms::invalid_status_error &);

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

    private:

      resource_status_record          _status_;        ///< Resource status
      vire::utility::model_identifier _error_type_id_; ///< The error type identifier
      error_type                      _error_;         ///< The error object

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_status_failure)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_status_failure,
                                                  "vire::cms::resource_status_failure")

#endif // VIRE_CMS_RESOURCE_STATUS_FAILURE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
