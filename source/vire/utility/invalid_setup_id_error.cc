// vire/utility/invalid_setup_id_error.cc
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
#include <vire/utility/invalid_setup_id_error.h>

// Third party
// - Boost
#include <boost/algorithm/string/replace.hpp>
// - BxJsontools:
#include <jsontools/base_type_converters.h>
#include <jsontools/std_type_converters.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>
#include <bayeux/protobuftools/protobuf_factory.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/utility/InvalidSetupIdError.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::invalid_setup_id_error",
                               vire::utility::InvalidSetupIdError)

namespace vire {

  namespace utility {

    DATATOOLS_SERIALIZATION_IMPLEMENTATION(invalid_setup_id_error,"vire::utility::invalid_setup_id_error")

    invalid_setup_id_error::invalid_setup_id_error()
      : ::vire::utility::base_error(base_error::EC_GENERIC_FAILURE, "")
    {
      return;
    }

    invalid_setup_id_error::invalid_setup_id_error(const int32_t code_, const std::string & token_)
      : invalid_setup_id_error()
    {
      if (code_ == EC_INVALID_NAME) {
        set_invalid_setup_name(token_);
      } else if (code_ == EC_INVALID_VERSION) {
        set_invalid_setup_version(token_);
      }
      return;
    }

    invalid_setup_id_error::~invalid_setup_id_error()
    {
      return;
    }

    bool invalid_setup_id_error::has_invalid_setup_name() const
    {
      return !_invalid_setup_name_.empty();
    }

    void invalid_setup_id_error::set_invalid_setup_name(const std::string & bad_name_)
    {
      if (!has_message_format()) {
        set_message_format("Invalid setup name='%n'");
      }
      set_code(EC_INVALID_NAME);
      _invalid_setup_version_.clear();
      _invalid_setup_name_ = bad_name_;
      return;
    }

    const std::string & invalid_setup_id_error::get_invalid_setup_name() const
    {
      return _invalid_setup_name_;
    }

    bool invalid_setup_id_error::has_invalid_setup_version() const
    {
      return !_invalid_setup_version_.empty();
    }

    void invalid_setup_id_error::set_invalid_setup_version(const std::string & bad_version_)
    {
      if (!has_message_format()) {
        set_message_format("Invalid setup version=[%v]");
      }
      set_code(EC_INVALID_VERSION);
      _invalid_setup_name_.clear();
      _invalid_setup_version_ = bad_version_;
      return;
    }

    const std::string & invalid_setup_id_error::get_invalid_setup_version() const
    {
      return _invalid_setup_version_;
    }

    // virtual
    void invalid_setup_id_error::_build_message(std::string & message_) const
    {
      if (has_invalid_setup_name()) {
        message_ = boost::replace_all_copy(get_message_format(), "%n", _invalid_setup_name_);
      } else if (has_invalid_setup_version()) {
        message_ = boost::replace_all_copy(get_message_format(), "%v", _invalid_setup_version_);
      }
      return;
    }

    void invalid_setup_id_error::reset()
    {
      _invalid_setup_name_.clear();
      _invalid_setup_version_.clear();
      this->base_error::reset();
      return;
    }

    void invalid_setup_id_error::jsonize(jsontools::node & node_,
                                         const unsigned long int /* version_ */)
    {
      this->base_error::jsonize(node_);
      node_["invalid_setup_name"] % _invalid_setup_name_;
      node_["invalid_setup_version"] % _invalid_setup_version_;
      return;
    }

    void invalid_setup_id_error::protobufize(protobuftools::message_node & node_,
                                             const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_error,node_);
      node_["invalid_setup_name"] % _invalid_setup_name_;
      node_["invalid_setup_version"] % _invalid_setup_version_;
      return;
    }

    void invalid_setup_id_error::tree_dump(std::ostream & out_,
                                           const std::string & title_,
                                           const std::string & indent_,
                                           bool inherit_) const
    {
      this->base_error::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Invalid setup name : '" << _invalid_setup_name_ << "'" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Invalid setup version : '" << _invalid_setup_version_ << "'" << std::endl;

      return;
    }

  } // namespace utility

} // namespace vire
