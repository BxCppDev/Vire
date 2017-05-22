//! \file  vire/cms/invalid_user_error.h
//! \brief Invalid user id error object
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

#ifndef VIRE_CMS_INVALID_USER_ERROR_H
#define VIRE_CMS_INVALID_USER_ERROR_H

// This project:
#include <vire/utility/base_error.h>

namespace vire {

  namespace cms {

    /// \brief Error occuring in case of invalid user identifier (name or version)
    class invalid_user_error
      : public ::vire::utility::base_error
    {
    public:

      static const int32_t EC_INVALID_NAME = ::vire::utility::base_error::EC_MAXIMUM_SYSTEM + 1;
      static const int32_t EC_INVALID_UID  = EC_INVALID_NAME + 1;

      /// Default constructor
      invalid_user_error();

      /// Constructor with bad UID
      invalid_user_error(int32_t bad_uid_);

      /// Constructor with bad user name
      invalid_user_error(const std::string & bad_name_);

      /// Destructor
      virtual ~invalid_user_error();

      /// Check invalid user name
      bool has_invalid_user_name() const;

      /// Set the invalid user name
      void set_invalid_user_name(const std::string &);

      /// Return the invalid user name
      const std::string & get_invalid_user_name() const;

      /// Check invalid UID
      bool has_invalid_user_id() const;

      /// Set the invalid user version representation
      void set_invalid_user_id(int32_t);

      /// Return the invalid user version representation
      int32_t get_invalid_user_id() const;

      /// Clear all internal informations
      virtual void reset();

      /// Main JSON (de-)serialization method
      virtual void jsonize(jsontools::node & node_,
                           unsigned long int version_ = 0);

      /// Main Protobuf (de-)serialization method
      virtual void protobufize(protobuftools::message_node & node_,
                               const unsigned long int version_);

    protected:

      virtual void _build_message(std::string & message_) const final;

    private:

      std::string _invalid_user_name_;    ///< Invalid user name
      int32_t     _invalid_user_id_ = -1; ///< Invalid UID

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::invalid_user_error, "vire::cms::invalid_user_error")

#endif // VIRE_CMS_INVALID_USER_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
