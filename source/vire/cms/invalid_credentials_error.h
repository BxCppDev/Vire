//! \file  vire/cms/invalid_credentials_error.h
//! \brief Invalid credentials error object
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

#ifndef VIRE_CMS_INVALID_CREDENTIALS_ERROR_H
#define VIRE_CMS_INVALID_CREDENTIALS_ERROR_H

// This project:
#include <vire/utility/base_error.h>

namespace vire {

  namespace cms {

    /// \brief Error occuring in case of invalid user identifier (name or version)
    class invalid_credentials_error
      : public ::vire::utility::base_error
    {
    public:

      /// Default constructor
      invalid_credentials_error();

      /// Destructor
      virtual ~invalid_credentials_error();

      /// Clear all internal informations
      virtual void reset();

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                             unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_);

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::invalid_credentials_error, "vire::cms::invalid_credentials_error")

#endif // VIRE_CMS_INVALID_CREDENTIALS_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
