//! \file  vire/utility/invalid_context_error.h
//! \brief Invalid context error object
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

#ifndef VIRE_UTILITY_INVALID_CONTEXT_ERROR_H
#define VIRE_UTILITY_INVALID_CONTEXT_ERROR_H

// This project:
#include <vire/utility/base_error.h>

namespace vire {

  namespace utility {

    /// \brief Error occuring in case of invalid context
    ///
    /// Type id: "vire::utility::invalid_context_error", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "code" : "100",
    ///   "message" : "Server is not accepting connection now"
    /// }
    /// @encode
    class invalid_context_error : public base_error
    {
    public:

      /// Default constructor
      invalid_context_error();

      /// Constructor with error code
      invalid_context_error(const int32_t code_);

      /// Constructor with error code and message
      invalid_context_error(const int32_t code_, const std::string & message_);

      /// Destructor
      virtual ~invalid_context_error();

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      //! Support for Boost-based serialization
      DATATOOLS_SERIALIZATION_DECLARATION()

    };

  } // namespace utility

} // namespace vire

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif
#include <boost/serialization/export.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif
BOOST_CLASS_EXPORT_KEY2(vire::utility::invalid_context_error,"vire::utility::invalid_context_error")

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::utility::invalid_context_error, "vire::utility::invalid_context_error")

#endif // VIRE_UTILITY_INVALID_CONTEXT_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
