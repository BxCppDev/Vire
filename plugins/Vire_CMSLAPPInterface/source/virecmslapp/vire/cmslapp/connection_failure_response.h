//! \file  vire/cmslapp/connection_failure_response.h
//! \brief Connection failure response from to the CMS server to the Vire server
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

#ifndef VIRE_CMSLAPP_CONNECTION_FAILURE_RESPONSE_H
#define VIRE_CMSLAPP_CONNECTION_FAILURE_RESPONSE_H

// Standard library:
#include <string>
#include <vector>

// Third party:
// - Boost:
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
#include <vire/cms/unknown_resources_error.h>

namespace vire {

  namespace cmslapp {

    /// \brief Connection failure response from the CMS server to the Vire server
    ///
    /// Type id: "vire::cmslapp::connection_failure_response", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "error_type_id" : {
    ///      "name" : "vire::cms::unknown_resources_error",
    ///      "version" : "1.0"
    ///    },
    ///   "error" : {
    ///     "code" : "3",
    ///     "message" : "There are unknown resources"
    ///     "unknown_paths" : [
    ///       "SuperNEMO://Demonstrator/CMS/NotExistingDevice/start",
    ///       "SuperNEMO://Demonstrator/CMS/NotExistingDevice/stop"
    ///     ]
    ///   }
    /// }
    /// @encode
    class connection_failure_response
      : public ::vire::utility::base_response
    {
    public:

      /// \brief Local error codes
      enum local_error_code_type {
        INVALID_CONTEXT   = 101, ///< Context is not valid for a connection
        INVALID_SETUP_ID  = 102, ///< Requested setup is not supported/recognized
        UNKNOWN_RESOURCES = 103  ///< Some requested resources are not known/supported
      };

      /// \brief Error variant
      typedef boost::variant<vire::utility::invalid_context_error,
                             vire::utility::invalid_setup_id_error,
                             vire::cms::unknown_resources_error> error_type;

      /// Default constructor
      connection_failure_response();

      /// Destructor
      virtual ~connection_failure_response();

      /// Set an invalid context error
      void set_error(const vire::utility::invalid_context_error &);

      /// Set an invalid setup ID error
      void set_error(const vire::utility::invalid_setup_id_error &);

      /// Set an unknown resource error
      void set_error(const vire::cms::unknown_resources_error &);

      /// Return the error type identifier
      const vire::utility::model_identifier & get_error_type_id() const;

      /// Return the error
      error_type get_error() const;

      /// Main JSON (de-)serialization method
      virtual void serialize(jsontools::node & node_,
                             unsigned long int version_ = 0);

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

    private:

      vire::utility::model_identifier _error_type_id_; ///< The error type identifier
      error_type                      _error_;         ///< The error object

      VIRE_UTILITY_PAYLOAD_INTERFACE(connection_failure_response)

    };

  } // namespace cmslapp

} // namespace vire

#endif // VIRE_CMSLAPP_CONNECTION_FAILURE_RESPONSE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
