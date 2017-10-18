//! \file  vire/cms/resource_exec_error.h
//! \brief Resource execution error object
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

#ifndef VIRE_RESOURCE_RESOURCE_EXEC_ERROR_H
#define VIRE_RESOURCE_RESOURCE_EXEC_ERROR_H

// This project:
#include <vire/utility/base_error.h>
#include <vire/cms/status.h>
#include <vire/time/utils.h>

namespace vire {

  namespace cms {

    /// \brief Error occuring in case of invalid resource status
    class resource_exec_error
      : public ::vire::utility::base_error
    {
    public:

      static const int32_t EC_INVALID_EXEC_CONTEXT = ::vire::utility::base_error::EC_MAXIMUM_SYSTEM + 1;
      static const int32_t EC_TIMEOUT              = EC_INVALID_EXEC_CONTEXT + 1;

      /// Default constructor
      resource_exec_error();

      /// Destructor
      virtual ~resource_exec_error();

      /// Check an invalid context error
      bool is_invalid_context_error() const;

      /// Make an invalid context error
      void make_invalid_context_error(const std::string & context_what_ = "");

      /// Check a timeout error
      bool is_timeout_error() const;

      /// Make a timeout error
      void make_timeout_error(const boost::posix_time::ptime & deadline_);

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

      boost::posix_time::ptime _deadline_; ///< Deadline

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_exec_error)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::resource_exec_error, "vire::cms::resource_exec_error")

#endif // VIRE_CMS_RESOURCE_EXEC_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
