//! \file  vire/utility/base_payload.h
//! \brief Base class for payload objects transported through the protocol
//
// Copyright (c) 2016 by Jean Hommet <hommet@lpccaen.in2p3.fr>,
//                       François Mauger <mauger@lpccaen.in2p3.fr>,
//                       Jérôme Poincheval <poincheval@lpccaen.in2p3.fr>
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

#ifndef VIRE_UTILITY_BASE_PAYLOAD_H
#define VIRE_UTILITY_BASE_PAYLOAD_H

// Standard library:
#include <string>
#include <memory>

// Third party:
// - Boost:
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif
#include <boost/serialization/export.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif
// - Bayeux/datatools:
#include <bayeux/datatools/i_serializable.h>
#include <bayeux/datatools/i_cloneable.h>
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/factory_macros.h>
// - BxJsontools:
#include <bayeux/jsontools/i_jsonizable.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/i_protobufable.h>

// This project:
#include "vire/base_object_protobuf.h"
#include "vire/utility/payload_macros.h"

namespace vire {

  namespace utility {

    /// \brief Abstract base class for all protocol objects
    class base_payload
      : public ::datatools::i_serializable
      , public ::datatools::i_cloneable
      , public ::datatools::i_tree_dumpable
      , public ::jsontools::i_jsonizable
      , public ::protobuftools::i_protobufable
     {
    public:

      /// Default constructor
      base_payload();

      /// Destructor
      virtual ~base_payload();

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

      //! Support for Boost-based serialization
      BOOST_SERIALIZATION_BASIC_DECLARATION()

      /// Interface of the factory
      DATATOOLS_FACTORY_SYSTEM_REGISTER_INTERFACE(base_payload)

    };

    typedef std::shared_ptr<base_payload>       payload_ptr_type
    ;
    typedef std::shared_ptr<const base_payload> const_payload_ptr_type;

  } // namespace utility

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::utility::base_payload, "vire::utility::base_payload")

#endif // VIRE_UTILITY_BASE_PAYLOAD_H

// Local Variables: --
// Mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
