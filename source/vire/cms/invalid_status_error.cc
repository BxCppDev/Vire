// vire/cms/invalid_status_error.cc
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

// Ourselves:
#include <vire/cms/invalid_status_error.h>

// Third party
// - Boost
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
// BxJsontools:
// #include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_string_converter.h>
//#include <bayeux/protobuftools/protobufable_converter.h>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cms/InvalidStatusError.pb.h"

BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::invalid_status_error",
                               vire::cms::InvalidStatusError)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(invalid_status_error,
                                        "vire::cms::invalid_status_error");

    // static
    const int32_t invalid_status_error::EC_MISSING;
    const int32_t invalid_status_error::EC_DISABLED;
    const int32_t invalid_status_error::EC_PENDING;
    const int32_t invalid_status_error::EC_FAILED;

    invalid_status_error::invalid_status_error()
      : ::vire::utility::base_error(base_error::EC_GENERIC_FAILURE, "")
    {
      return;
    }

    invalid_status_error::invalid_status_error(const std::string & path_,
                                               const status::flag flag_)
      : invalid_status_error()
    {
      set_path(path_);
      set_flag(flag_);
      return;
    }

    invalid_status_error::invalid_status_error(const std::string & path_,
                                               const std::string & flag_repr_)
    {
      set_path(path_);
      set_flag_repr(flag_repr_);
      return;
    }

    invalid_status_error::~invalid_status_error()
    {
      return;
    }

    bool invalid_status_error::has_path() const
    {
      return !_path_.empty();
    }

    void invalid_status_error::set_path(const std::string & path_)
    {
      _path_ = path_;
      return;
    }

    const std::string & invalid_status_error::get_path() const
    {
      return _path_;
    }

    bool invalid_status_error::has_flag_repr() const
    {
      return !_flag_repr_.empty();
    }

    void invalid_status_error::set_flag_repr(const std::string & flag_repr_)
    {
      status::flag f = status::get_flag(flag_repr_);
      DT_THROW_IF(f == status::none, std::logic_error,
                  "Invalid status flag label '" << flag_repr_ << "'");
      _flag_repr_ = flag_repr_;
      switch (f) {
      case status::failed:
        set_code(EC_FAILED);
        break;
      case status::pending:
        set_code(EC_PENDING);
        break;
      case status::disabled:
        set_code(EC_DISABLED);
        break;
      case status::missing:
        set_code(EC_MISSING);
        break;
      }
      if (!has_message_format()) {
        set_message_format("Resource '%p' is '%f'");
      }
      return;
    }

    void invalid_status_error::reset_flag_repr()
    {
      _flag_repr_.clear();
      return;
    }

    const std::string & invalid_status_error::get_flag_repr() const
    {
      return _flag_repr_;
    }

    bool invalid_status_error::has_flag() const
    {
      return has_flag_repr();
    }

    void invalid_status_error::set_flag(const status::flag flag_)
    {
      set_flag_repr(status::get_flag_label(flag_));
      return;
    }

    void invalid_status_error::reset_flag()
    {
      reset_flag_repr();
      return;
    }

    status::flag invalid_status_error::get_flag() const
    {
      return status::get_flag(_flag_repr_);
    }

    // virtual
    void invalid_status_error::_build_message(std::string & message_) const
    {
      std::string msg = get_message_format();
      if (has_path()) {
        msg = boost::replace_all_copy(msg, "%p", _path_);
      }
      if (has_flag()) {
        msg = boost::replace_all_copy(msg, "%f", _flag_repr_);
      }
      message_ = msg;
      return;
    }

    void invalid_status_error::reset()
    {
      _path_.clear();
      _flag_repr_.clear();
      this->base_error::reset();
      return;
    }

    void invalid_status_error::jsonize(jsontools::node & node_,
                                       const unsigned long int version_)
    {
      this->vire::utility::base_error::jsonize(node_, version_);
      node_["path"] % _path_;
      node_["flag_repr"] % _flag_repr_;
      return;
    }

    void invalid_status_error::protobufize(protobuftools::message_node & node_,
                                           const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_error, node_);
      node_["path"] % _path_;
      node_["flag_repr"] % _flag_repr_;
      return;
    }

    void invalid_status_error::tree_dump(std::ostream & out_,
                                           const std::string & title_,
                                           const std::string & indent_,
                                           bool inherit_) const
    {
      this->vire::utility::base_error::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Resource path : '" << _path_ << "'" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Invalid statud flag : '" << _flag_repr_ << "'" << std::endl;

      return;
    }

  } // namespace cms

} // namespace vire
