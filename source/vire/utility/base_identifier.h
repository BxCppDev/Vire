//! \file  vire/utility/base_identifier.h
//! \brief Setup identifier
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_UTILITY_BASE_IDENTIFIER_H
#define VIRE_UTILITY_BASE_IDENTIFIER_H

// Standard library:
#include <string>

// Third party:
// - Boost:
#include <boost/optional.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/version_id.h>
#include <bayeux/datatools/i_serializable.h>
// - BxJsontools:
#include <bayeux/jsontools/i_jsonizable.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/i_protobufable.h>

// This project
#include <vire/utility/base_payload.h>

namespace vire {

  namespace utility {

    /// \brief Identifier of some hardware, middleware or software component
    class base_identifier
      : public base_payload
    {
    public:

      /// Default constructor
      base_identifier();

      /// Destructor
      virtual ~base_identifier();

      /// Check validity
      bool is_valid() const;

      /// Check if a candidate name is valid
      virtual bool is_name_valid(const std::string & name_) const = 0;

      /// Set the name
      void set_name(const std::string & name_);

      /// Return the name
      const std::string & get_name() const;

      /// Check if version is set
      bool has_version() const;

      /// Set the version
      void set_version(const datatools::version_id & vid_);

      /// Reset the version
      void reset_version();

      /// Return the version
      const datatools::version_id & get_version() const;

      /// Check if the identifier matches a name
      bool match(const std::string & name_, const std::string & version_repr_ = "") const;

      /// Set the name and version identifier
      void set(const std::string & name_,
               const datatools::version_id & vid_ = datatools::version_id::invalid());

      /// Set the name and version identifier
      void set(const std::string & name_,
               const std::string & version_repr_ = "");

      /// Reset/invalidate the setup identifier
      void reset();

      /// Convert to string representation
      void to_string(std::string & format_) const;

      /// Convert to string representation
      std::string to_string() const;

      /// Parse from string representation
      bool from_string(const std::string & repr_);

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

    protected:

      std::string & _grab_name();

      boost::optional<datatools::version_id> & _grab_version();

    private:

      std::string _name_; ///< Name of the object (mandatory)
      boost::optional<datatools::version_id> _version_ = boost::none; ///< Version identifier of the object (optional)

      //! Payload interface
      DATATOOLS_SERIALIZATION_DECLARATION()
      // VIRE_UTILITY_PAYLOAD_INTERFACE(base_identifier)

    };

  } // namespace utility

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::utility::base_identifier, "vire::utility::base_identifier")

#endif // VIRE_UTILITY_BASE_IDENTIFIER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
