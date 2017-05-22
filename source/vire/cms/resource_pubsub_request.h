//! \file  vire/cms/resource_pubsub_request.h
//! \brief Resource Pub/Sub (un)subscribe request
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

#ifndef VIRE_CMS_RESOURCE_PUBSUB_REQUEST_H
#define VIRE_CMS_RESOURCE_PUBSUB_REQUEST_H

// Standard Library:
#include <string>

// Third party:
#include <boost/variant.hpp>

// This project
#include <vire/utility/base_request.h>
#include <vire/utility/base_response.h>
#include <vire/utility/model_identifier.h>
#include <vire/utility/invalid_context_error.h>
#include <vire/cms/invalid_resource_error.h>
#include <vire/cms/invalid_status_error.h>
#include <vire/cms/no_pubsub_resource_error.h>

namespace vire {

  namespace cms {

    /// \brief Resource Pub/Sub action request
    class resource_pubsub_request : public ::vire::utility::base_request
    {
    public:

      /// \brief
      enum pubsub_action {
        ACTION_INVALID     = 0, ///< Invalid action
        ACTION_CHECK       = 1, ///< Check the Pub/sub status of the resource
        ACTION_SUBSCRIBE   = 2, ///< Ask for the Pub/sub subscription of the resource
        ACTION_UNSUBSCRIBE = 3  ///< Ask for the Pub/sub unsubscription of the resource
      };

      /// Default constructor
      resource_pubsub_request();

      /// Constructor
      resource_pubsub_request(const std::string & path_, pubsub_action action_);

      /// Destructor
      virtual ~resource_pubsub_request();

      /// Set the path
      void set_path(const std::string &);

      /// Return the path
      const std::string & get_path() const;

      /// Set the subscribe flag
      void set_action(pubsub_action action_);

      /// Return the subscribe flag
      bool get_action() const;

    private:

      // Attributes:
      std::string   _path_;   ///< Resource path
      pubsub_action _action_ = ACTION_INVALID; ///< Pub/sub action

    };


    /// \brief Resource Pub/Sub success response
    class resource_pubsub_success_response
      : public ::vire::utility::base_response
    {
    public:

      /// Default constructor
      resource_pubsub_success_response();

      /// Destructor
      virtual ~resource_pubsub_success_response();

      /// Set the subscription flag
      void set_subscribed(bool);

      /// Return the subscription flag
      bool is_subscribed() const;

    private:

      // vire::cms::resource_status_record  _status_;
      bool _subscribed_; ///< Subscribe flag

    };


    /// \brief Resource Pub/Sub failure response
    class resource_pubsub_failure_response
      : public ::vire::utility::base_response
    {
    public:

      /// Default constructor
      resource_pubsub_failure_response();

      /// Destructor
      virtual ~resource_pubsub_failure_response();

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

    };

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_RESOURCE_PUBSUB_REQUEST_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
