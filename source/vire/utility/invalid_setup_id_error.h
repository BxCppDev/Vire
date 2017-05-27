//! \file  vire/utility/invalid_setup_id_error.h
//! \brief Invalid setup id error object
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

#ifndef VIRE_UTILITY_INVALID_SETUP_ID_ERROR_H
#define VIRE_UTILITY_INVALID_SETUP_ID_ERROR_H

// This project:
#include <vire/utility/base_error.h>

namespace vire {

  namespace utility {

    /// \brief Error occuring in case of invalid setup identifier (name or version)
    ///
    /// Type id: "vire::utility::invalid_setup_id_error", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "invalid_setup_name" : "Atlas",
    ///   "invalid_setup_version" : "Phase_1"
    /// }
    /// @encode
    class invalid_setup_id_error : public base_error
    {
    public:

      static const int32_t EC_INVALID_NAME = ::vire::utility::base_error::EC_MAXIMUM_SYSTEM + 1;
      static const int32_t EC_INVALID_VERSION = EC_INVALID_NAME + 1;

      /// Default constructor
      invalid_setup_id_error();

      /// Constructor with bad name or version
      invalid_setup_id_error(const int32_t code_, const std::string & bad_name_);

      /// Destructor
      virtual ~invalid_setup_id_error();

      /// Check invalid setup name
      bool has_invalid_setup_name() const;

      /// Set the invalid setup name
      void set_invalid_setup_name(const std::string &);

      /// Return the invalid setup name
      const std::string & get_invalid_setup_name() const;

      /// Check invalid setup version
      bool has_invalid_setup_version() const;

      /// Set the invalid setup version representation
      void set_invalid_setup_version(const std::string &);

      /// Return the invalid setup version representation
      const std::string & get_invalid_setup_version() const;

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

      std::string _invalid_setup_name_;    ///< Invalid setup name
      std::string _invalid_setup_version_; ///< Invalid setup version representation

      //! Payload interface
      VIRE_UTILITY_PAYLOAD_INTERFACE(invalid_setup_id_error)

    };

  } // namespace utility

} // namespace vire

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#endif
#include <boost/serialization/export.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif
BOOST_CLASS_EXPORT_KEY2(vire::utility::invalid_setup_id_error,"vire::utility::invalid_setup_id_error")

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::utility::invalid_setup_id_error, "vire::utility::invalid_setup_id_error")

#endif // VIRE_UTILITY_INVALID_SETUP_ID_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
