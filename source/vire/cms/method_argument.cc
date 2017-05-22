// vire/cms/method_argument.cc
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
#include <vire/cms/method_argument.h>

// Third party:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>
// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_string_converter.h>

// This project:
#include "vire/cms/MethodArgument.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::method_argument",
                               vire::cms::MethodArgument)

namespace vire {

  namespace cms {

    DATATOOLS_CLONEABLE_IMPLEMENTATION(method_argument)

    DATATOOLS_SERIALIZATION_IMPLEMENTATION(method_argument, "vire::cms::method_argument");

    method_argument::method_argument()
    {
      return;
    }

    method_argument::method_argument(const std::string & name_)
    {
      set_name(name_);
      return;
    }

    method_argument::method_argument(const std::string & name_,
                                     const std::string & value_repr_)
    {
      set_name(name_);
      set_value_repr(value_repr_);
      return;
    }

    method_argument::method_argument(const std::string & name_,
                                     const std::string & value_repr_,
                                     const std::string & meta_)
    {
      set_name(name_);
      set_value_repr(value_repr_);
      set_meta(meta_);
      return;
    }

    bool method_argument::is_valid() const
    {
      if (!has_name()) {
        return false;
      }
      return true;
    }

    bool method_argument::has_name() const
    {
      return !_name_.empty();
    }

    void method_argument::set_name(const std::string & name_)
    {
      DT_THROW_IF(!datatools::name_validation(name_,
                                              datatools::NV_INSTANCE | datatools::NV_NO_DOT),
                  std::logic_error,
                  "Invalid argument name '" << name_ << "'!");
      _name_ = name_;
      return;
    }

    void method_argument::reset_name()
    {
      _name_.clear();
      return;
    }

    const std::string & method_argument::get_name() const
    {
      return _name_;
    }

    bool method_argument::has_value_repr() const
    {
      return ! _value_repr_.empty();
    }

    void method_argument::set_value_repr(const std::string & value_repr_)
    {
      DT_THROW_IF(value_repr_.empty(), std::logic_error,
                  "Empty value representation!");
      _value_repr_ = value_repr_;
      return;
    }

    const std::string & method_argument::get_value_repr() const
    {
      return _value_repr_;
    }

    void method_argument::reset_value_repr()
    {
      _value_repr_.clear();
      return;
    }

    bool method_argument::has_meta() const
    {
      return !_meta_.empty();
    }

    void method_argument::set_meta(const std::string & meta_)
    {
      _meta_ = meta_;
      return;
    }

    void method_argument::reset_meta()
    {
      _meta_.clear();
      return;
    }

    const std::string & method_argument::get_meta() const
    {
      return _meta_;
    }

    void method_argument::reset()
    {
      reset_name();
      reset_value_repr();
      reset_meta();
      return;
    }

    void method_argument::jsonize(jsontools::node & node_,
                                        const unsigned long int version_)
    {
      node_["name"] % _name_;
      node_["value_repr"] % _value_repr_;
      node_["meta"] % _meta_;
      return;
    }

    void method_argument::protobufize(protobuftools::message_node & node_,
                                            const unsigned long int /* version_ */)
    {
      node_["name"] % _name_;
      node_["value_repr"] % _value_repr_;
      node_["meta"] % _meta_;
      return;
    }

    void method_argument::tree_dump(std::ostream & out_,
                                    const std::string & title_,
                                    const std::string & indent_,
                                    bool inherit_) const
    {
      if (! title_.empty()) out_ << indent_ << title_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Name : ";
      if (has_name()) {
        out_ << "'" << _name_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Value representation : ";
      if (has_value_repr()) {
        out_ << "'" << _value_repr_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Meta : ";
      if (has_meta()) {
        out_ << "'" <<  _meta_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Validity : " << (is_valid() ? "<yes>" : "<no>") << std::endl;

      return;
    }

  } // namespace cms

} // namespace vire
