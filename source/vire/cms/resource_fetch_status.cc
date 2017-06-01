// vire/cms/resource_fetch_status.cc
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

// Ourselves:
#include <vire/cms/resource_fetch_status.h>

// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_string_converter.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ResourceFetchStatus.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::resource_fetch_status",
                               vire::cms::ResourceFetchStatus)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(resource_fetch_status,
                                        "vire::cms::resource_fetch_status")

    resource_fetch_status::resource_fetch_status()
    {
      return;
    }

    resource_fetch_status::resource_fetch_status(const std::string & path_)
    {
      set_path(path_);
      return;
    }

    resource_fetch_status::~resource_fetch_status()
    {
      return;
    }

    bool resource_fetch_status::has_path() const
    {
      return !_path_.empty();
    }

    void resource_fetch_status::set_path(const std::string & path_)
    {
      _path_ = path_;
      return;
    }

    void resource_fetch_status::reset_path()
    {
      _path_.clear();
      return;
    }

    const std::string & resource_fetch_status::get_path() const
    {
      return _path_;
    }

    void resource_fetch_status::jsonize(jsontools::node & node_,
                                                const unsigned long int version_)
    {
      this->vire::utility::base_payload::jsonize(node_, version_);
      node_["path"] % _path_;
      return;
    }

    void resource_fetch_status::protobufize(protobuftools::message_node & node_,
                                            const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_payload, node_);
      node_["path"] % _path_;
       return;
    }

    void resource_fetch_status::tree_dump(std::ostream & out_,
                                          const std::string & title_,
                                          const std::string & indent_,
                                          bool inherit_) const
    {
      this->vire::utility::base_payload::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Path : '" << get_path() << "'" << std::endl;

      return;
    }

  } // namespace cms

} // namespace vire
