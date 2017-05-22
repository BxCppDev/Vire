//! \file vire/cms/method_argument.h
//! \brief Representation of an input or output method argument
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

#ifndef VIRE_CMS_METHOD_ARGUMENT_H
#define VIRE_CMS_METHOD_ARGUMENT_H

// Standard Library:
#include <string>

// Third party
// - Bayeux/datatools:
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/i_cloneable.h>
// - BxJsontools:
#include <bayeux/jsontools/i_jsonizable.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/i_protobufable.h>

namespace vire {

  namespace cms {

    /// \brief Scalar argument
    class method_argument
      : public ::datatools::i_serializable
      , public ::datatools::i_cloneable
      , public ::datatools::i_tree_dumpable
      , public ::jsontools::i_jsonizable
      , public ::protobuftools::i_protobufable
    {
    public:

      /// Default constructor:
      method_argument();

      /// Constructor:
      method_argument(const std::string & name_);

      /// Constructor:
      method_argument(const std::string & name_,
                      const std::string & value_repr_);

      /// Constructor:
      method_argument(const std::string & name_,
                      const std::string & value_repr_,
                      const std::string & meta_);

      /// Check validity
      bool is_valid() const;

      /// Check if argument name is set
      bool has_name() const;

      /// Set the argument name
      void set_name(const std::string & name_);

      /// Reset the argument name
      void reset_name();

      /// Return the argument name
      const std::string & get_name() const;

      /// Check if argument value representation is set
      bool has_value_repr() const;

      /// Set the argument value representation
      void set_value_repr(const std::string & name_);

      /// Return the argument value representation
      const std::string & get_value_repr() const;

      /// Reset the argument value representation
      void reset_value_repr();

      /// Check if metadata is set
      bool has_meta() const;

      /// Set the metadata
      void set_meta(const std::string & meta_);

      /// Return the metadata
      const std::string & get_meta() const;

      /// Reset the metadata
      void reset_meta();

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

      /// Macro to declare basic support for cloning
      DATATOOLS_CLONEABLE_DECLARATION(method_argument);

      /// Macro to declare basic support for serialization
      DATATOOLS_SERIALIZATION_DECLARATION();

    private:

      std::string _name_;       ///< Name of the argument
      std::string _value_repr_; ///< String representation of the argument's value
      std::string _meta_;       ///< Auxiliary metadata

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::method_argument, "vire::cms::method_argument")

#endif // VIRE_CMS_METHOD_ARGUMENT_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
