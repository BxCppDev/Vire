// vire/cms/argument_error.cc
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

// Ourselves:
#include <vire/cms/argument_error.h>

// Third party
// - Boost
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
// BxJsontools:
#include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_string_converter.h>
#include <bayeux/protobuftools/protobufable_converter.h>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ArgumentError.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::argument_error",
                               vire::cms::ArgumentError)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(argument_error,"vire::cms::argument_error")

    // static
    const int32_t argument_error::EC_INVALID_NARGS;
    const int32_t argument_error::EC_INVALID_ARG;
    const int32_t argument_error::EC_INVALID_ARG_VALUE;

    argument_error::argument_error()
      : ::vire::utility::base_error(base_error::EC_GENERIC_FAILURE, "")
    {
      _invalid_nargs_ = 0;
      _expected_nargs_ = 0;
      return;
    }

    argument_error::~argument_error()
    {
      return;
    }

    bool argument_error::is_invalid_number_of_arguments() const
    {
      return get_code() == EC_INVALID_NARGS;
    }

    void argument_error::make_invalid_number_of_arguments(std::size_t expected_nargs_,
                                                          std::size_t invalid_nargs_)
    {
      reset();
      set_code(EC_INVALID_NARGS);
      _expected_nargs_ = expected_nargs_;
      _invalid_nargs_ = invalid_nargs_;
      set_message_format("Number of arguments [%i] does not match expected [%e]");
      return;
    }

    std::size_t argument_error::get_expected_nargs() const
    {
      return _expected_nargs_;
    }

    std::size_t argument_error::get_invalid_nargs() const
    {
      return _invalid_nargs_;
    }

    bool argument_error::is_invalid_argument() const
    {
      return get_code() == EC_INVALID_ARG;
    }

    void argument_error::make_invalid_argument(const std::string & argname_)
    {
      reset();
      set_code(EC_INVALID_ARG);
      _argument_name_ = argname_;
      set_message_format("Invalid argument '%a'");
      return;
    }

    std::string argument_error::get_invalid_argument_name() const
    {
      return _argument_name_;
    }

    bool argument_error::is_invalid_argument_value() const
    {
      return get_code() == EC_INVALID_ARG_VALUE;
    }

    void argument_error::make_invalid_argument_value(const std::string & argname_,
                                                     const std::string & value_repr_)
    {
      reset();
      set_code(EC_INVALID_ARG_VALUE);
      _argument_name_ = argname_;
      _invalid_value_repr_ = value_repr_;
      set_message_format("Invalid value '%v' for argument '%a'");
      return;
    }

    std::string argument_error::get_argument_name() const
    {
      return _argument_name_;
    }

    std::string argument_error::get_invalid_argument_value_repr() const
    {
      return _invalid_value_repr_;
    }

    // virtual
    void argument_error::_build_message(std::string & message_) const
    {
      std::string msg = get_message_format();
      if (is_invalid_number_of_arguments()) {
        msg = boost::replace_all_copy(msg, "%i", boost::lexical_cast<std::string>(_invalid_nargs_));
        msg = boost::replace_all_copy(msg, "%e", boost::lexical_cast<std::string>(_expected_nargs_));
      }
      if (is_invalid_argument()) {
        msg = boost::replace_all_copy(msg, "%a", _argument_name_);
      }
      if (is_invalid_argument_value()) {
        msg = boost::replace_all_copy(msg, "%a", _argument_name_);
        msg = boost::replace_all_copy(msg, "%v", _invalid_value_repr_);
      }
      message_ = msg;
      return;
    }

    void argument_error::reset()
    {
      _invalid_nargs_ = 0;
      _expected_nargs_ = 0;
      _argument_name_.clear();
      _invalid_value_repr_.clear();
      this->base_error::reset();
      return;
    }

    void argument_error::jsonize(jsontools::node & node_,
                                 const unsigned long int version_)
    {
      this->vire::utility::base_error::jsonize(node_, version_);
      if (is_invalid_number_of_arguments()) {
        node_["expected_nargs"] % _expected_nargs_;
        node_["invalid_nargs"] % _invalid_nargs_;
      }
      if (is_invalid_argument() || is_invalid_argument_value()) {
        node_["argument_name"] % _argument_name_;
      }
      if (is_invalid_argument_value()) {
        node_["invalid_value_repr"] % _invalid_value_repr_;
      }
      return;
    }

    void argument_error::protobufize(protobuftools::message_node & node_,
                                     const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_error, node_);
      if (is_invalid_number_of_arguments()) {
        node_["expected_nargs"] % _expected_nargs_;
        node_["invalid_nargs"] % _invalid_nargs_;
      }
      if (is_invalid_argument() || is_invalid_argument_value()) {
        node_["argument_name"] % _argument_name_;
      }
      if (is_invalid_argument_value()) {
        node_["invalid_value_repr"] % _invalid_value_repr_;
      }
      return;
    }

    void argument_error::tree_dump(std::ostream & out_,
                                   const std::string & title_,
                                   const std::string & indent_,
                                   bool inherit_) const
    {
      this->vire::utility::base_error::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Expected #args : " << _expected_nargs_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Invalid #args : " << _invalid_nargs_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Argument name : '" << _argument_name_ << "'" << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Invalid value repr. : '" << _invalid_value_repr_ << "'" << std::endl;

      return;
    }

  } // namespace cms

} // namespace vire
