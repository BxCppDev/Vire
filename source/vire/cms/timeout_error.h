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
#include <vire/cms/status.h>

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
      static const int32_t EC_ELAPSED   = EC_DEADLINE + 1;

      /// Default constructor
      timeout_error();

      /// Destructor
      virtual ~timeout_error();

      /// Check
      bool is_deadline() const;

      /// Make a deadline timeout error
      void make_deadline(const boost::posix_time::ptime & deadline_);

      /// Return the expected number of timeouts
      const boost::posix_time::ptime & get_deadline() const;

      /// Check
      bool is_elapsed() const;

      /// Make a elapsed timeout error
      void make_elapsed(const boost::posix_time::time_duration & elapsed_);

      /// Return the expected number of timeouts
      const boost::posix_time::time_duration & get_elapsed() const;

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

      /// Macro to declare basic support for cloning
      DATATOOLS_CLONEABLE_DECLARATION(timeout_error);

      /// Macro to declare basic support for serialization
      DATATOOLS_SERIALIZATION_DECLARATION();

    protected:

      virtual void _build_message(std::string & message_) const final;

    private:

      boost::posix_time::ptime _deadline_; ///< Deadline timestamp
      boost::posix_time::time_duration _elapsed_; ///< Elapsed time

    };

  } // namespace cms

} // namespace vire

#endif // VIRE_CMS_TIMEOUT_ERROR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
