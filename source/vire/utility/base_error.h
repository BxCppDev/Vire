//! \file  vire/utility/base_error.h
//! \brief Simple base error object
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

#ifndef VIRE_UTILITY_BASE_ERROR_H
#define VIRE_UTILITY_BASE_ERROR_H

// Standard library
#include <string>
#include <iostream>

// Third party
// - Boost
#include <boost/cstdint.hpp>
// - Bayeux:
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_cloneable.h>
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/serialization_macros.h>
// - BxJsontools:
#include <bayeux/jsontools/i_jsonizable.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/i_protobufable.h>

namespace vire {

  namespace utility {

    /// \brief Simple base error report
    ///
    /// Type id: "vire::utility::base_error", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "code" : "1",
    ///   "message" : "Generic error"
    /// }
    /// @encode
    class base_error :
      public ::datatools::i_serializable,
      public ::datatools::i_cloneable,
      public ::jsontools::i_jsonizable,
      public ::protobuftools::i_protobufable,
      public ::datatools::i_tree_dumpable
    {
    public:

      // System code
      static const int32_t EC_SUCCESS         = 0;  ///< No error
      static const int32_t EC_GENERIC_FAILURE = 1;  ///< Generic error code
      static const int32_t EC_MAXIMUM_SYSTEM  = 99; ///< Maximum system error code

      /// Default constructor
      base_error();

      /// Constructor with error code
      explicit base_error(const int32_t code_);

      /// Constructor with error code and message
      base_error(const int32_t code_, const std::string & message_format_);

      /// Constructor with error message
      explicit base_error(const std::string & message_format_);

      /// Destructor
      virtual ~base_error();

      /// Check if error is in success state
      bool is_success() const;

      /// Check if error is in failure state
      bool is_failure() const;

      /// Set the error code
      void set_code(const int32_t code_);

      /// Return the error code
      int32_t get_code() const;

      /// Check if error message format is set
      bool has_message_format() const;

      /// Return the error message format
      const std::string & get_message_format() const;

      /// Set the error message format
      void set_message_format(const std::string & message_format_);

      /// Set the error message format
      /// @see set_message_format
      void set_message(const std::string & message_format_);

      /// Clear the error message format
      void clear_message_format();

      /// Check if error message is set
      bool has_message() const;

      /// Return the error message
      const std::string & get_message() const;

      /// Clear all internal informations
      virtual void reset();

      /// Build a failure record
      void make_failure(const int32_t code_, const std::string & message_format_);

      /// Build a success record
      void make_success(const std::string & message_ = "");

      /// Print
      void print(std::ostream & out_) const;

      /// Conversion to string
      std::string to_string() const;

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

    protected:

      /// Compute the message from the message format
      virtual void _build_message(std::string & message_) const;

    private:

      // Configuration:
      int32_t     _code_;           ///< Error code
      std::string _message_format_; ///< Error message format

      // Working data:
      std::string _cached_message_; ///< Cached error message

      //! Support for Boost-based serialization
      DATATOOLS_SERIALIZATION_DECLARATION()

      //! Support for cloning
      DATATOOLS_CLONEABLE_DECLARATION(base_error)

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
BOOST_CLASS_EXPORT_KEY2(vire::utility::base_error,"vire::utility::base_error")

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::utility::base_error, "vire::utility::base_error")

#endif // VIRE_UTILITY_BASE_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
