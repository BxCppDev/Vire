//! \file  vire/cms/resource_status_record.h
//! \brief A device/resource (dynamic) status record
//
// Copyright (c) 2016-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_CMS_RESOURCE_STATUS_RECORD_H
#define VIRE_CMS_RESOURCE_STATUS_RECORD_H

// Standard Library:
#include <string>

// Third party
// - Boost
#include <boost/cstdint.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/i_tree_dump.h>
#include <bayeux/datatools/bit_mask.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

// This project
#include <vire/time/utils.h>

// - BxJsontools:
#include <bayeux/jsontools/i_jsonizable.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/i_protobufable.h>

namespace vire {

  namespace cms {

    /// \brief Device/resource dynamic status record transferable between Vire components
    ///
    /// Type id: "vire::cms::resource_status_record", version "1.0"
    ///
    /// @code JSON
    /// {
    ///   "path" : "SuperNEMO://Demonstrator/CMS/Coil/PS/Control/Current/__dp_write__" ,
    ///   "timestamp" : "20160610T012723.425362",
    ///   "flags" : "0010"
    ///

    /// @encode
    class resource_status_record
      : public ::datatools::i_serializable
      , public ::datatools::i_cloneable
      , public ::jsontools::i_jsonizable
      , public ::protobuftools::i_protobufable
      , public ::datatools::i_tree_dumpable
    {
    public:

      enum flags_bits {
        MISSING_BIT  = datatools::bit_mask::bit00,
        FAILED_BIT   = datatools::bit_mask::bit01,
        PENDING_BIT  = datatools::bit_mask::bit02,
        DISABLED_BIT = datatools::bit_mask::bit03
      };

      /// Default constructor
      resource_status_record();

      /// Constructor
      resource_status_record(const std::string & path_,
                             const boost::posix_time::ptime & t_,
                             bool missing_,
                             bool failed_,
                             bool pending_,
                             bool disabled_);

      /// Destructor
      virtual ~resource_status_record();

      /// Check if the record is valid
      bool is_valid() const;

      /// Check if the resource/device path is set
      bool has_path() const;

      /// Set resource/device path
      void set_path(const std::string & path_);

      /// Return resource/device path
      const std::string & get_path() const;

      /// Check if the timestamp is set
      bool has_timestamp() const;

      /// Set timestamp
      void set_timestamp(const boost::posix_time::ptime & t_);

      /// Return timestamp
      const boost::posix_time::ptime & get_timestamp() const;

      /// Set resource flags
      void set_flags(uint16_t flags_);

      /// Get resource flags
      uint16_t get_flags() const;

      /// Reset
      void reset();

      /// Check the failed bit
      bool is_failed() const;

      /// Check the pending bit
      bool is_pending() const;

      /// Check the disabled bit
      bool is_disabled() const;

      /// Check the missing bit
      bool is_missing() const;

      /// Set the failed bit
      void set_failed();

      /// Set the pending bit
      void set_pending();

      /// Set the disabled bit
      void set_disabled();

      /// Set the missing bit
      void set_missing();

      /// Unset the failed bit
      void unset_failed();

      /// Unset the pending bit
      void unset_pending();

      /// Unset the disabled bit
      void unset_disabled();

      /// Unset the missing bit
      void unset_missing();

      /// Unset all bits
      void unset_all();

      /// Build a representation of the status bitset
      void status_to_string(std::string & repr_) const;

      /// Return a representation of the status bitset
      std::string get_status_string() const;

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

      std::string              _path_;      ///< Resource/device path
      boost::posix_time::ptime _timestamp_; ///< Last modification timestamp
      bool                     _missing_;   ///< Missing bit
      bool                     _failed_;    ///< Failed bit
      bool                     _pending_;   ///< Pending bit
      bool                     _disabled_;  ///< Disabled bit

      VIRE_UTILITY_PAYLOAD_INTERFACE(resource_status_record)

      //! Reflection interface
      DR_CLASS_RTTI();

    };

  } // namespace cms

} // namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::cms::resource_status_record);
#endif // Q_MOC_RUN

#endif // VIRE_CMS_RESOURCE_STATUS_RECORD_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
