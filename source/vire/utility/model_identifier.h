//! \file  vire/utility/model_identifier.h
//! \brief Model identifier
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

#ifndef VIRE_UTILITY_MODEL_IDENTIFIER_H
#define VIRE_UTILITY_MODEL_IDENTIFIER_H

// Standard library:
#include <string>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/version_id.h>

// This project:
#include <vire/utility/base_identifier.h>

namespace vire {

  namespace utility {

    /// \brief Identifier of a hardware or software model
    class model_identifier :
      public base_identifier
    {
    public:

      /// Default constructor
      model_identifier();

      /// Constructor
      explicit model_identifier(const std::string & name_);

      /// Constructor
      model_identifier(const std::string & name_,
                       const datatools::version_id & vid_);

      /// Destructor
      virtual ~model_identifier();

      /// Check if a candidate name is valid
      virtual bool is_name_valid(const std::string & name_) const final;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      //! Support for Boost-based serialization
      BOOST_SERIALIZATION_BASIC_DECLARATION()

    };

  } // namespace utility

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::utility::model_identifier, "vire::utility::model_identifier")

#endif // VIRE_UTILITY_MODEL_IDENTIFIER_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
