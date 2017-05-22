// vire/cms/resource_status_record.cc - Resource status record
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

// Ourselves
#include <vire/cms/resource_status_record.h>

// Standard library:
#include <bitset>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/exception.h>
// - BxJsontools:
#include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/std_type_converters.h>
#include <bayeux/jsontools/boost_type_converters.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>
#include <bayeux/protobuftools/boost_datetime_converters.h>

// This project
#include <vire/cms/status.h>
#include "vire/cms/ResourceStatusRecord.pb.h"
#include <vire/utility/path.h>

BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::resource_status_record",
                               vire::cms::ResourceStatusRecord)

namespace vire {

  namespace cms {

    DATATOOLS_CLONEABLE_IMPLEMENTATION(resource_status_record)

    DATATOOLS_SERIALIZATION_IMPLEMENTATION(resource_status_record,
                                           "vire::cms::resource_status_record");

    resource_status_record::resource_status_record()
      : resource_status_record("", vire::time::invalid_time(), false, false, false, false)
    {
      return;
    }

    resource_status_record::resource_status_record(const std::string & path_,
                                                   const boost::posix_time::ptime & t_,
                                                   bool missing_,
                                                   bool failed_,
                                                   bool pending_,
                                                   bool disabled_)
      : _path_(path_)
      , _timestamp_(t_)
      , _missing_(missing_)
      , _failed_(failed_)
      , _pending_(pending_)
      , _disabled_(disabled_)
    {
      return;
    }

    resource_status_record::~resource_status_record()
    {
      return;
    }

    bool resource_status_record::has_path() const
    {
      return ! _path_.empty();
    }

    void resource_status_record::set_path(const std::string & path_)
    {
      DT_THROW_IF(! vire::utility::path::validate_path(path_),
                  std::logic_error,
                  "Invalid resource/device path '" << path_ << "'!");
      _path_ = path_;
      return;
    }

    const std::string & resource_status_record::get_path() const
    {
      return _path_;
    }

    bool resource_status_record::has_timestamp() const
    {
      return vire::time::is_valid(_timestamp_);
    }

    void resource_status_record::set_timestamp(const boost::posix_time::ptime & t_)
    {
      _timestamp_ = t_;
      return;
    }

    const boost::posix_time::ptime & resource_status_record::get_timestamp() const
    {
      return _timestamp_;
    }

    void resource_status_record::set_flags(uint16_t flags_)
    {
      if (flags_ & MISSING_BIT) {
        set_missing();
      } else {
        unset_missing();
      }
      if (flags_ & FAILED_BIT) {
        set_failed();
      } else {
        unset_failed();
      }
      if (flags_ & PENDING_BIT) {
        set_pending();
      } else {
        unset_pending();
      }
      if (flags_ & DISABLED_BIT) {
        set_disabled();
      } else {
        unset_disabled();
      }
      return;
    }

    uint16_t resource_status_record::get_flags() const
    {
      uint16_t flags = 0;
      if (is_missing()) {
        flags |= MISSING_BIT;
      }
      if (is_failed()) {
        flags |= FAILED_BIT;
      }
      if (is_pending()) {
        flags |= PENDING_BIT;
      }
      if (is_disabled()) {
        flags |= DISABLED_BIT;
      }
      return flags;
    }

    void resource_status_record::reset()
    {
      _path_.clear();
      _missing_ = false;
      _failed_ = false;
      _pending_ = false;
      _disabled_ = false;
      return;
    }

    bool resource_status_record::is_valid() const
    {
      if (! has_path()) return false;
      if (! has_timestamp()) return false;
      return true;
    }

    bool resource_status_record::is_failed() const
    {
      return _failed_;
    }

    bool resource_status_record::is_pending() const
    {
      return _pending_;
    }

    bool resource_status_record::is_disabled() const
    {
      return _disabled_;
    }

    bool resource_status_record::is_missing() const
    {
      return _missing_;
    }

    void resource_status_record::set_failed()
    {
      _failed_ = true;
      return ;
    }

    void resource_status_record::set_pending()
    {
      _pending_ = true;
      return;
    }

    void resource_status_record::set_disabled()
    {
      _disabled_ = true;
      return;
    }

    void resource_status_record::set_missing()
    {
      _missing_ = true;
      return;
    }

    void resource_status_record::unset_failed()
    {
      _failed_ = false;
      return;
    }

    void resource_status_record::unset_pending()
    {
      _pending_ = false;
      return;
    }

    void resource_status_record::unset_disabled()
    {
      _disabled_ = false;
      return;
    }

    void resource_status_record::unset_missing()
    {
      _missing_ = false;
      return;
    }

    void resource_status_record::unset_all()
    {
      unset_failed();
      unset_pending();
      unset_disabled();
      unset_missing();
      return;
    }

    void resource_status_record::tree_dump(std::ostream & out_,
                                           const std::string & title_,
                                           const std::string & indent_,
                                           bool inherit_) const
    {
      if (! title_.empty()) out_ << indent_ << title_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Path : ";
      if (has_path()) {
        out_ << "'" << _path_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Timestamp : ";
      if (has_timestamp()) {
        out_ << "[" << vire::time::to_string(_timestamp_) << "]";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Flags : " << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::skip_tag << ::datatools::i_tree_dumpable::tag
           << "Missing  : " << (_missing_ ? "<yes>" : "<no>") << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::skip_tag << ::datatools::i_tree_dumpable::tag
           << "Failed   : " << (_failed_ ? "<yes>" : "<no>") << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::skip_tag << ::datatools::i_tree_dumpable::tag
           << "Pending  : " << (_pending_ ? "<yes>" : "<no>") << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::skip_tag << ::datatools::i_tree_dumpable::last_tag
           << "Disabled : " << (_disabled_  ? "<yes>" : "<no>")<< std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Validity : " << (is_valid() ? "<yes>" : "<no>") << std::endl;

      return;
    }

    void resource_status_record::jsonize(jsontools::node & node_,
                                         const unsigned long int /* version_ */)
    {
      node_["path"] % _path_;
      node_["timestamp"] % _timestamp_;
      node_["missing"] % _missing_;
      node_["failed"] % _failed_;
      node_["pending"] % _pending_;
      node_["disabled"] % _disabled_;
      return;
    }

    void resource_status_record::protobufize(protobuftools::message_node & node_,
                                             const unsigned long int /* version_ */)
    {
      node_["path"] % _path_;
      node_["timestamp"] % _timestamp_;
      node_["missing"] % _missing_;
      node_["failed"] % _failed_;
      node_["pending"] % _pending_;
      node_["disabled"] % _disabled_;
      return;
    }

    void resource_status_record::status_to_string(std::string & repr_) const
    {
       std::bitset<4> bits;
       bits[0] = _missing_;
       bits[1] = _failed_;
       bits[2] = _pending_;
       bits[3] = _disabled_;
       repr_ = bits.to_string();
       return;
    }

    std::string resource_status_record::get_status_string() const
    {
      std::string repr;
      status_to_string(repr);
      return repr;
    }

  } // namespace cms

} // namespace vire
