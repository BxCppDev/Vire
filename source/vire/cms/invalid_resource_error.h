//! \file  vire/cms/invalid_resource_error.h
//! \brief Invalid resource id error object
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

#ifndef VIRE_CMS_INVALID_RESOURCE_ERROR_H
#define VIRE_CMS_INVALID_RESOURCE_ERROR_H

// This project:
#include <vire/utility/base_error.h>

namespace vire {

  namespace cms {

    /// \brief Error occuring in case of invalid resource identifier (path or ID)
    ///
    /// Type id: "vire::resource::invalid_resource_error", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "code" : "3",
    ///   "message" : "Resource %p is not recognized",
    ///   "invalid_resource_path" : "foo",
    ///   "invalid_resource_id"   : "1234"
    /// }
    /// @encode
    class invalid_resource_error
      : public ::vire::utility::base_error
    {
    public:

      static const int32_t EC_INVALID_PATH = ::vire::utility::base_error::EC_MAXIMUM_SYSTEM + 1;
      static const int32_t EC_INVALID_ID   = EC_INVALID_PATH + 1;

      /// Default constructor
      invalid_resource_error();

      /// Constructor with bad ID
      invalid_resource_error(int32_t bad_id_);

      /// Constructor with bad path
      invalid_resource_error(const std::string & bad_path_);

      /// Destructor
      virtual ~invalid_resource_error();

      /// Check invalid resource path
      bool is_invalid_resource_path() const;

      /// Check invalid resource path
      bool has_invalid_resource_path() const;

      /// Set the invalid resource path
      void set_invalid_resource_path(const std::string &);

      /// Return the invalid resource path
      const std::string & get_invalid_resource_path() const;

      /// Check invalid UID
      bool has_invalid_resource_id() const;

      /// Set the invalid resource version representation
      void set_invalid_resource_id(int32_t);

      /// Return the invalid resource version representation
      int32_t get_invalid_resource_id() const;

      /// Clear all internal informations
      virtual void reset();

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           const unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_ = 0);

    protected:

      virtual void _build_message(std::string & message_) const final;

    private:

      std::string _invalid_resource_path_;    ///< Invalid resource path
      int32_t     _invalid_resource_id_ = -1; ///< Invalid UID

      VIRE_UTILITY_PAYLOAD_INTERFACE(invalid_resource_error)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::invalid_resource_error, "vire::cms::invalid_resource_error")

#endif // VIRE_CMS_INVALID_RESOURCE_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
