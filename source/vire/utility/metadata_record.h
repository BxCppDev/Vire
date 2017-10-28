//! \file  vire/utility/metadata_record.h
//! \brief Metadate key/value record
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

#ifndef VIRE_UTILITY_METADATA_RECORD_H
#define VIRE_UTILITY_METADATA_RECORD_H

// Standard library:
#include <string>

// Third party:
// - Boost:
#include <boost/optional.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/version_id.h>
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_tree_dump.h>
// - BxJsontools:
#include <bayeux/jsontools/i_jsonizable.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/i_protobufable.h>

namespace vire {

  namespace utility {

    /// \brief Metadata record
    ///
    /// A metadata record is stored as a key/value pair, optionally
    /// associated to an additional information.
    /// All attributes have a textual representation.
    /// Examples:
    /// \code
    /// key : "length"
    /// value : "3.45 m"
    /// additional : "const"
    /// \endcode
    class metadata_record
      : public ::datatools::i_tree_dumpable
      , public ::jsontools::i_jsonizable
      , public ::protobuftools::i_protobufable
    {
    public:

      /// Default constructor
      metadata_record();

      /// Constructor
      metadata_record(const std::string & key_,
                      const std::string & value_,
                      const std::string & type_ = "",
                      const std::string & additional_ = "");

      /// Destructor
      virtual ~metadata_record();

      /// Check if a key is set
      bool has_key() const;

      /// Set the key
      void set_key(const std::string & key_);

      /// Return the key
      const std::string & get_key() const;

      /// Check if a value is set
      bool has_value() const;

      /// Set the value
      void set_value(const std::string & value_);

      /// Return the value
      const std::string & get_value() const;

      /// Check if a type is set
      bool has_type() const;

      /// Set the type
      void set_type(const std::string & type_);

      /// Return the type
      const std::string & get_type() const;

      /// Check if a additional is set
      bool has_additional() const;

      /// Set the additional
      void set_additional(const std::string & additional_);

      /// Return the additional
      const std::string & get_additional() const;

       /// Check validity
      bool is_valid() const;

      /// Reset
      void reset();

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      /// Predicate function
      static bool has_key(const metadata_record & mdr_, const std::string & key_)
      {
        return mdr_._key_ == key_;
      }

      struct has_key_predicate // : public std::function<bool, metadata_record>
      {
        has_key_predicate(const std::string & key_) : _key_(key_) {}
        bool operator()(const metadata_record & mdr_) const {
          return mdr_._key_ == _key_;
        }
      private:
        std::string _key_;
      };

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Support for Boost-based serialization
      BOOST_SERIALIZATION_BASIC_DECLARATION();

    private:

      std::string _key_;        ///< Metadata key
      std::string _value_;      ///< Metadata value representation
      std::string _type_;       ///< Metadata type
      std::string _additional_; ///< Metadata additional information

    };

  } // namespace utility

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::utility::metadata_record, "vire::utility::metadata_record")

#endif // VIRE_UTILITY_METADATA_RECORD_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
