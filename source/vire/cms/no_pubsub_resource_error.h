//! \file  vire/cms/no_pubsub_resource_error.h
//! \brief Error about a resource with no Pub/Sub support
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

#ifndef VIRE_CMS_NO_PUBSUB_RESOURCE_ERROR_H
#define VIRE_CMS_NO_PUBSUB_RESOURCE_ERROR_H

// This project:
#include <vire/utility/base_error.h>

namespace vire {

  namespace cms {

    /// \brief Error occuring in case of invalid resource identifier (path or ID)
    ///
    /// Type id: "vire::cms::no_pubsub_resource_error", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "code" : "3",
    ///   "message_format" : "Resource %p has no pub/sub support",
    ///   "path" : "foo"
    /// }
    /// @encode
    class no_pubsub_resource_error
      : public ::vire::utility::base_error
    {
    public:

      static const int32_t EC_NO_PUBSUB_SUPPORT = ::vire::utility::base_error::EC_MAXIMUM_SYSTEM + 1;

      /// Default constructor
      no_pubsub_resource_error();

      /// Constructor with path
      no_pubsub_resource_error(const std::string & path_);

      /// Destructor
      virtual ~no_pubsub_resource_error();

      /// Check resource path
      bool has_path() const;

      /// Set the resource path
      void set_path(const std::string &);

      /// Return the path
      const std::string & get_path() const;

      /// Clear all internal informations
      virtual void reset();

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

    protected:

      virtual void _build_message(std::string & message_) const final;

    private:

      std::string _path_; ///< Resource path

      VIRE_UTILITY_PAYLOAD_INTERFACE(no_pubsub_resource_error)

    };

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_NO_PUBSUB_RESOURCE_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
