//! \file  vire/utility/unsupported_payload_error.h
//! \brief Invalid setup id error object
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_UTILITY_UNSUPPORTED_PAYLOAD_ERROR_H
#define VIRE_UTILITY_UNSUPPORTED_PAYLOAD_ERROR_H

// This project:
#include <vire/utility/base_error.h>

namespace vire {

  namespace utility {

    /// \brief Error occuring in case of invalid setup identifier (name or version)
    ///
    /// Type id: "vire::utility::unsupported_payload_error", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "type_id" : "vire::unknown::payload",
    ///   "type_version" : "666-unsupported"
    /// }
    /// @encode
    class unsupported_payload_error
      : public base_error
    {
    public:
      
      static const int32_t EC_PAYLOAD_TYPE_ID = ::vire::utility::base_error::EC_MAXIMUM_SYSTEM + 1;
      static const int32_t EC_PAYLOAD_TYPE_VERSION = EC_PAYLOAD_TYPE_ID + 1;
      
      /// Default constructor
      unsupported_payload_error();

      /// Constructor with type or version  
      unsupported_payload_error(const int32_t code_,
				const std::string & type_id_,
				const std::string & type_version_ = "");

      /// Destructor
      virtual ~unsupported_payload_error();

      /// Check the type ID representation
      bool has_type_id() const;

      /// Set the type ID representation
      void set_type_id(const std::string &);

      /// Return the type ID representation
      const std::string & get_type_id() const;

      /// Check the type version representation
      bool has_type_version() const;

      /// Set the type version representation
      void set_type_version(const std::string &);

      /// Return the type version representation
      const std::string & get_type_version() const;

      /// Clear all internal informations
      virtual void reset();

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

      virtual void _build_message(std::string & message_) const final;

    private:

      std::string _type_id_;      ///< Type ID of the unsupported payload
      std::string _type_version_; ///< Type version of the unsupported payload

      //! Payload interface
      VIRE_UTILITY_PAYLOAD_INTERFACE(unsupported_payload_error)

    };

  } // namespace utility

} // namespace vire

BOOST_CLASS_EXPORT_KEY2(vire::utility::unsupported_payload_error,
			"vire::utility::unsupported_payload_error")

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::utility::unsupported_payload_error,
						  "vire::utility::unsupported_payload_error")

#endif // VIRE_UTILITY_UNSUPPORTED_PAYLOAD_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
