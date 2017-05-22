// vire/cms/resource_exec_request.cc
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
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves
#include <vire/cms/resource_exec_request.h>

// Third Party:
// - datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/utils.h>
// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/protobufable_converter.h>
#include <bayeux/protobuftools/std_vector_converter.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ResourceExecRequest.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::resource_exec_request",
                               vire::cms::ResourceExecRequest)

// This project:
#include <vire/base_object_protobuf.h>

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(resource_exec_request,
                                        "vire::cms::resource_exec_request")

    resource_exec_request::resource_exec_request()
    {
      return;
    }

    resource_exec_request::resource_exec_request(const std::string & path_)
      : resource_base_request(path_)
    {
      return;
    }

    resource_exec_request::~resource_exec_request()
    {
      return;
    }

    bool resource_exec_request::has_input_argument(const std::string & name_) const
    {
      for (const auto & arg : _input_arguments_) {
        if (arg.get_name() == name_) return true;
      }
      return false;
    }

    bool resource_exec_request::has_input_arguments() const
    {
      return _input_arguments_.size() > 0;
    }

    void resource_exec_request::add_input_argument(const std::string & name_,
                                                   const std::string & value_repr_)
    {
      DT_THROW_IF(has_input_argument(name_), std::logic_error,
                  "Input argument with name '" << name_ << "' already exists!");
      method_argument arg(name_, value_repr_);
      _input_arguments_.push_back(arg);
      return;
    }

    void resource_exec_request::add_input_argument(const std::string & name_,
                                                   const std::string & value_repr_,
                                                   const std::string & meta_)
    {
      DT_THROW_IF(has_input_argument(name_), std::logic_error,
                  "Input argument with name '" << name_ << "' already exists!");
      method_argument arg(name_, value_repr_, meta_);
      _input_arguments_.push_back(arg);
      return;
    }

    std::size_t resource_exec_request::get_number_of_input_arguments() const
    {
      return _input_arguments_.size();
    }

    const method_argument &
    resource_exec_request::get_input_argument(const std::size_t index_) const
    {
      DT_THROW_IF(index_ >= _input_arguments_.size(),
                  std::range_error,
                  "Invalid input argument index!");
      return _input_arguments_[index_];
    }

    const method_argument &
    resource_exec_request::get_input_argument(const std::string & name_) const
    {
      for (const auto & iarg : _input_arguments_) {
        if (iarg.get_name() == name_) return iarg;
      }
      DT_THROW(std::logic_error, "Invalid input argument name '" << name_ << "'!");
    }

    const std::vector<method_argument> &
    resource_exec_request::get_input_arguments() const
    {
      return _input_arguments_;
    }

    void resource_exec_request::clear_input_arguments()
    {
      _input_arguments_.clear();
      return;
    }

    void resource_exec_request::jsonize(jsontools::node & node_,
                                        const unsigned long int version_)
    {
      this->resource_base_request::jsonize(node_, version_);
      node_["input_arguments"] % _input_arguments_;
      return;
    }

    void resource_exec_request::protobufize(protobuftools::message_node & node_,
                                            const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(resource_base_request, node_);
      node_["input_arguments"] % _input_arguments_;
      return;
    }

    void resource_exec_request::tree_dump(std::ostream & out_,
                                          const std::string & title_,
                                          const std::string & indent_,
                                          bool inherit_) const
    {
      this->resource_base_request::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Input arguments : ";
      if (has_input_arguments()) {
        out_ << '[' <<  _input_arguments_.size() << "]";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;
      {
        std::size_t counter = 0;
        for (const auto & iarg : _input_arguments_) {
          out_ << indent_ << ::datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
          if (++counter == _input_arguments_.size()) {
            out_ << ::datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << ::datatools::i_tree_dumpable::tag;
          }
          out_ << "Method argument #" << (counter - 1) << " : name='" << iarg.get_name()
               << "' value='" << iarg.get_value_repr() << "'";
          if (iarg.has_meta()) {
             out_ << " meta='" << iarg.get_meta() << "'";
          }
          out_ << std::endl;
        }
      }

      return;
    }

  } // namespace cms

} // namespace vire
