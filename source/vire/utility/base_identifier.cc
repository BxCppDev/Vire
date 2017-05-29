// vire/utility/base_identifier.cc
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Ourselves:
#include <vire/utility/base_identifier.h>

// Standard library:
#include <sstream>

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
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>

// This project:
#include <vire/utility/json_converters.h>

// Declare a protobuf registrar instance for the message class:
#include <bayeux/protobuftools/protobuf_factory.h>
#include "vire/utility/BaseIdentifier.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::base_identifier",
                               vire::utility::BaseIdentifier)

namespace vire {

  namespace utility {

    DATATOOLS_SERIALIZATION_IMPLEMENTATION(base_identifier, "vire::utility::base_identifier")

    // VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(base_identifier, "vire::utility::base_identifier")

    base_identifier::base_identifier()
    {
      return;
    }

    base_identifier::~base_identifier()
    {
      return;
    }

    bool base_identifier::is_valid() const
    {
      if (_name_.empty()) return false;
      if (_version_) {
        if (!_version_.get().is_valid()) return false;
      }
      return true;
    }

    void base_identifier::set_name(const std::string & name_)
    {
      DT_THROW_IF(!is_name_valid(name_),
                  std::logic_error,
                  "Invalid name '" << name_ << "'!");
      _name_ = name_;
      return;
    }

    const std::string & base_identifier::get_name() const
    {
      return _name_;
    }

    bool base_identifier::has_version() const
    {
      if (_version_) return true;
      return false;
    }

    void base_identifier::set_version(const datatools::version_id & vid_)
    {
      DT_THROW_IF(!vid_.is_valid(), std::logic_error, "Invalid setup version");
      _version_ = vid_;
      return;
    }

    void base_identifier::reset_version()
    {
      _version_ = boost::none;
      return;
    }

    const datatools::version_id & base_identifier::get_version() const
    {
      return _version_.get();
    }

    void base_identifier::set(const std::string & name_, const datatools::version_id & vid_)
    {
      set_name(name_);
      reset_version();
      if (vid_.is_valid()) {
        set_version(vid_);
      }
      return;
    }

    void base_identifier::set(const std::string & name_,
                              const std::string & version_repr_)
    {
      set_name(name_);
      reset_version();
      if (!version_repr_.empty()) {
        datatools::version_id vid;
        DT_THROW_IF(!vid.from_string(version_repr_), std::logic_error,
                    "Cannot parse version identifier from '" << version_repr_ << "'!");
        set_version(vid);
      }
      return;
    }

    bool base_identifier::match(const std::string & name_,
                                const std::string & version_repr_) const
    {
      if (name_ != _name_) return false;
      if (!version_repr_.empty()) {
        if (!has_version()) return false;
        std::string ver_repr;
        _version_.get().to_string(ver_repr);
        if (ver_repr != version_repr_) return false;
      }
      return true;
    }

    void base_identifier::reset()
    {
      reset_version();
      _name_.clear();
      return;
    }

    void base_identifier::to_string(std::string & format_) const
    {
      std::ostringstream repr_oss;
      repr_oss << _name_;
      if (has_version()) {
        std::string ver_repr;
        _version_.get().to_string(ver_repr);
        repr_oss << '-' << ver_repr;
      }
      format_ = repr_oss.str();
      return;
    }

    std::string base_identifier::to_string() const
    {
      std::string repr;
      to_string(repr);
      return repr;
    }

    bool base_identifier::from_string(const std::string & repr_)
    {
      reset();
      // First hyphen is used to separate name and version tokens:
      std::size_t hyphen_pos = repr_.find('-');
      std::string name;
      std::string version_repr;
      if (hyphen_pos == repr_.npos) {
        name = repr_;
      } else {
        name = repr_.substr(0, hyphen_pos);
        version_repr = repr_.substr(hyphen_pos + 1);
      }
      if (!is_name_valid(name)) {
        return false;
      }
      datatools::version_id vid;
      if (!version_repr.empty()) {
        if (!vid.from_string(version_repr)) {
          return false;
        }
      }
      set_name(name);
      if (vid.is_valid()) {
        set_version(vid);
      }
      return true;
    }

    std::string & base_identifier::_grab_name()
    {
      return _name_;
    }

    boost::optional<datatools::version_id> & base_identifier::_grab_version()
    {
      return _version_;
    }

    void base_identifier::jsonize(jsontools::node & node_,
                                  const unsigned long int /* version_ */)
    {
      node_["name"] % _name_;
      node_["version"] % _version_;
      return;
    }

    void base_identifier::protobufize(protobuftools::message_node & node_,
                                      const unsigned long int /* version_ */)
    {
      if (node_.is_debug()) {
        DT_LOG_TRACE_ENTERING(datatools::logger::PRIO_TRACE);
      }
      node_["name"] % _name_;
      if (node_.is_serializing()) {
        std::string version_repr;
        if (_version_) {
          _version_.get().to_string(version_repr);
          node_["version"] % version_repr;
        }
      } else {
        _version_ = boost::none;
        if (node_.has_field("version")) {
          std::string version_repr;
          node_["version"] % version_repr;
          if (!version_repr.empty()) {
            datatools::version_id vid;
            DT_THROW_IF(!vid.from_string(version_repr), std::logic_error,
                        "Invalid version identifier '" << version_repr << "'!");
            _version_ = vid;
          }
        }
      }
      if (node_.is_debug()) {
        DT_LOG_TRACE_EXITING(datatools::logger::PRIO_TRACE);
      }
      return;
    }

  } // namespace utility

} // namespace vire
