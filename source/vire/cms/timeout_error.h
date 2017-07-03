//! \file  vire/cms/timeout_error.h
//! \brief Timeout error object
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

#ifndef VIRE_CMS_TIMEOUT_ERROR_H
#define VIRE_CMS_TIMEOUT_ERROR_H

// This project:
#include <vire/utility/base_error.h>

namespace vire {

  namespace cms {

    /// \brief Error occuring in case of timeout error
    ///
    /// Type id: "vire::cms::timeout_error", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "code" : "100",
    ///   "message" : "Resource 'SuperNEMO://Demonstrator/CMS/DAQ/start' is disabled",
    ///   "deadline" : "1480935355T120345.32323"
    /// }
    /// @encode
    class timeout_error
      : public ::vire::utility::base_error
    {
    public:

      static const int32_t EC_DEADLINE  = ::vire::utility::base_error::EC_MAXIMUM_SYSTEM + 1;

      /// Default constructor
      timeout_error();

      /// Destructor
      virtual ~timeout_error();

      /// Check
      bool is_deadline() const;

      /// Make a deadline timeout error
      void set_deadline(const boost::posix_time::ptime & deadline_);

      /// Return the expected number of timeouts
      const boost::posix_time::ptime & get_deadline() const;

      /// Reset
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

      boost::posix_time::ptime _deadline_;        ///< Deadline timestamp

      VIRE_UTILITY_PAYLOAD_INTERFACE(timeout_error)

    };

  } // namespace cms

} // namespace vire

// Bind the C++ class to a specific protobuf message class
#include <bayeux/protobuftools/protobuf_utils.h>
BXPROTOBUFTOOLS_CLASS_BIND_TO_REGISTERED_PROTOBUF(vire::cms::timeout_error, "vire::cms::timeout_error")

#endif // VIRE_CMS_TIMEOUT_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
