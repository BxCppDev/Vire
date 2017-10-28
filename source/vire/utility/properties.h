//! \file vire/utility/properties.h
//! \brief Representation of an input or output method argument
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_UTILITY_PROPERTIES_H
#define VIRE_UTILITY_PROPERTIES_H

// Standard Library:
#include <string>

// Third party
// - Boost
#include <boost/cstdint.hpp>

// This project
#include <vire/utility/base_payload.h>
#include <vire/utility/instance_identifier.h>
#include <vire/utility/model_identifier.h>
#include <vire/utility/metadata_record.h>

namespace vire {

  namespace utility {

    /// \brief Arbitrary collection of properties (key/value pairs)
    class properties
      : public ::vire::utility::base_payload
    {
    public:

      /// Default constructor:
      properties();

      /// Check validity
      bool is_valid() const;

      /// Check if identifier is set
      bool has_id() const;

      /// Reset the identifier
      void reset_id();

      /// Set the identifier
      void set_id(const vire::utility::instance_identifier & id_);

      /// Return the identifier
      const vire::utility::instance_identifier & get_id() const;

      /// Check if model is set
      bool has_model() const;

      /// Reset the model
      void reset_model();

      /// Set the model
      void set_model(const vire::utility::model_identifier & model_);

      /// Return the model
      const vire::utility::model_identifier & get_model() const;

      //! Check if a property exists
      bool has_property(const std::string & key_) const;

      //! Add a property
      void add_property(const metadata_record & record_);

      //! Add a property
      void add_property(const std::string & key_,
                        const std::string & value_,
                        const std::string & type_ = "",
                        const std::string & add_ = "");

      //! Return a property
      const vire::utility::metadata_record &
      get_property(const std::string & key_) const;

      //! Fetch a property
      bool fetch_property(const std::string & key_,
                          vire::utility::metadata_record & property_) const;

      //! Remove property
      void remove_property(const std::string & key_);

      //! Remove all properties
      void remove_properties();

      //! Build the list of property keys
      void build_keys(std::set<std::string> & keys_) const;

      /// Reset
      void reset();

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

    private:

      instance_identifier          _id_;         ///< The identifier
      model_identifier             _model_;      ///< The model
      std::vector<metadata_record> _properties_; ///< Collection of properties

      VIRE_UTILITY_PAYLOAD_INTERFACE(properties)

    };

  } // namespace utility

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::utility::properties,
                                                  "vire::utility::properties")

#endif // VIRE_UTILITY_PROPERTIES_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
