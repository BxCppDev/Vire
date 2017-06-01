// vire/utility/resource_base_request.cc
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
#include <vire/cms/resource_base_request.h>

// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_string_converter.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ResourceBaseRequest.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::resource_base_request",
                               vire::cms::ResourceBaseRequest)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(resource_base_request,
                                        "vire::cms::resource_base_request")

    resource_base_request::resource_base_request()
    {
      return;
    }

    resource_base_request::resource_base_request(const std::string & path_)
    {
      set_path(path_);
      return;
    }

    resource_base_request::~resource_base_request()
    {
      return;
    }

    bool resource_base_request::has_path() const
    {
      return !_path_.empty();
    }

    void resource_base_request::set_path(const std::string & path_)
    {
      _path_ = path_;
      return;
    }

    void resource_base_request::reset_path()
    {
      _path_.clear();
      return;
    }

    const std::string & resource_base_request::get_path() const
    {
      return _path_;
    }

    void resource_base_request::jsonize(jsontools::node & node_,
                                        const unsigned long int version_)
    {
      this->vire::utility::base_request::jsonize(node_, version_);
      node_["path"] % _path_;
       return;
    }

    void resource_base_request::protobufize(protobuftools::message_node & node_,
                                            const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_request, node_);
      node_["path"] % _path_;
      return;
    }

    void resource_base_request::tree_dump(std::ostream & out_,
                                          const std::string & title_,
                                          const std::string & indent_,
                                          bool inherit_) const
    {
      this->vire::utility::base_request::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Path : '" << get_path() << "'" << std::endl;

      return;
    }

  } // namespace cms

} // namespace vire
