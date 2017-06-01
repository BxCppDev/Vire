//! \file  vire/cms/resource_pubsub_failure.h
//! \brief Resource Pub/Sub (un)subscribe failure
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

#ifndef VIRE_CMS_RESOURCE_PUBSUB_FAILURE_H
#define VIRE_CMS_RESOURCE_PUBSUB_failure_H

// This project
#include <vire/utility/base_payload.h>
#include <vire/utility/model_identifier.h>
#include <vire/utility/invalid_context_error.h>
#include <vire/cms/invalid_resource_error.h>
#include <vire/cms/invalid_status_error.h>
#include <vire/cms/no_pubsub_resource_error.h>

namespace vire {

  namespace cms {

    /// \brief Resource Pub/Sub failure
    class resource_pubsub_failure
      : public ::vire::utility::base_payload
    {
    public:

      /// Default constructor
      resource_pubsub_failure();

      /// Destructor
      virtual ~resource_pubsub_failure();

      // /// \brief Local error codes
      // enum local_error_code_type {
      //   INVALID_CONTEXT    = 1,
      //   INVALID_RESOURCE   = 2,
      //   NO_PUBSUB_RESOURCE = 3,
      //   INVALID_STATUS     = 4
      // };

      /// \brief Error variant
      typedef boost::variant<vire::utility::invalid_context_error,
                             vire::cms::invalid_resource_error,
                             vire::cms::no_pubsub_resource_error,
                             vire::cms::invalid_status_error> error_type;

      /// Set an invalid context error
      void set_error(const vire::utility::invalid_context_error &);

      /// Set an invalid resource error
      void set_error(const vire::cms::invalid_resource_error &);

      /// Set a no Pub/Sub resource error
      void set_error(const vire::cms::no_pubsub_resource_error &);

      /// Set an invalid  status error
      void set_error(const vire::cms::invalid_status_error &);

      /// Return the error type identifier
      const vire::utility::model_identifier & get_error_type_id() const;

      /// Return the error
      error_type get_error() const;

    private:

      vire::utility::model_identifier _error_type_id_; ///< The error type identifier
      error_type                      _error_;         ///< The error object

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_fetch_status_failure)

    };

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_RESOURCE_PUBSUB_FAILURE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
