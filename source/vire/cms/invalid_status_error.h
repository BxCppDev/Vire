//! \file  vire/cms/invalid_status_error.h
//! \brief Invalid resource status error object
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

#ifndef VIRE_CMS_INVALID_STATUS_ERROR_H
#define VIRE_CMS_INVALID_STATUS_ERROR_H

// This project:
#include <vire/utility/base_error.h>
#include <vire/cms/status.h>

namespace vire {

  namespace cms {

    /// \brief Error occuring in case of invalid resource status
    ///
    /// Type id: "vire::cms::invalid_status_error", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "code" : "102",
    ///   "message_format" : "Resource '%p' is %f",
    ///   "path" : "SuperNEMO://Demonstrator/CMS/DAQ/start",
    ///   "flag_repr" : "disabled"
    /// }
    /// @encode
    class invalid_status_error
      : public ::vire::utility::base_error
    {
    public:

      static const int32_t EC_MISSING  = ::vire::utility::base_error::EC_MAXIMUM_SYSTEM + 1;
      static const int32_t EC_FAILED   = EC_MISSING  + 1;
      static const int32_t EC_DISABLED = EC_FAILED   + 1;
      static const int32_t EC_PENDING  = EC_DISABLED + 1;

      /// Default constructor
      invalid_status_error();

      /// Constructor with path and flag
      invalid_status_error(const std::string & path_, const status::flag flag_);

      /// Constructor with path and flag
      invalid_status_error(const std::string & path_, const std::string & flag_repr_);

      /// Destructor
      virtual ~invalid_status_error();

      /// Check resource path
      bool has_path() const;

      /// Set the resource path
      void set_path(const std::string &);

      /// Return the resource path
      const std::string & get_path() const;

      /// Check invalid flag
      bool has_flag() const;

      /// Set the invalid flag
      void set_flag(const status::flag);

      /// Reset the invalid flag
      void reset_flag();

      /// Return the invalid flag
      status::flag get_flag() const;

      /// Check invalid flag
      bool has_flag_repr() const;

      /// Set the representation of the invalid flag
      void set_flag_repr(const std::string & flag_repr_);

      /// Reset the representation of the invalid flag
      void reset_flag_repr();

      /// Return the representation of the invalid flag
      const std::string & get_flag_repr() const;

      /// Clear all internal informations
      virtual void reset();

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

    protected:

      virtual void _build_message(std::string & message_) const final;

    private:

      std::string _path_;      ///< Resource path
      std::string _flag_repr_; ///< Status flag that causes the error

      VIRE_UTILITY_PAYLOAD_INTERFACE(invalid_resource_error)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::invalid_status_error, "vire::cms::invalid_status_error")


#endif // VIRE_CMS_INVALID_STATUS_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
