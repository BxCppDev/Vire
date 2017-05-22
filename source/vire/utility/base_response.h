//! \file  vire/utility/base_response.h
//! \brief Simple base response object
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

#ifndef VIRE_UTILITY_BASE_RESPONSE_H
#define VIRE_UTILITY_BASE_RESPONSE_H

// Standard library
#include <string>
#include <iostream>

// Third party
// - Boost
#include <boost/cstdint.hpp>

// This project
#include <vire/utility/base_payload.h>

namespace vire {

  namespace utility {

    /// \brief Simple base response object
    class base_response : public base_payload
    {
    public:

      /// Default constructor
      base_response();

      /// Destructor
      virtual ~base_response();

      /// Return the category of transferable object
      virtual payload_category get_category() const final;

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

      //! Smart print
      virtual void tree_dump(std::ostream & out_ = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_ = false) const;

      //! Support for Boost-based serialization
      BOOST_SERIALIZATION_BASIC_DECLARATION()

    };

  } // namespace utility

} // namespace vire

// BOOST_CLASS_EXPORT_KEY2(vire::utility::base_response,"vire::utility::base_response")

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::utility::base_response, "vire::utility::base_response")

#endif // VIRE_UTILITY_BASE_RESPONSE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
