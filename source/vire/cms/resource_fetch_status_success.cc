// vire/cms/resource_fetch_status_success.cc
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

// Ourselves:
#include <vire/cms/resource_fetch_status_success.h>

// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_string_converter.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ResourceFetchStatusSuccess.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::resource_fetch_status_success",
                               vire::cms::ResourceFetchStatusSuccess)

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(resource_fetch_status_success,
                                        "vire::cms::resource_fetch_status_success")

    resource_fetch_status_success::resource_fetch_status_success()
    {
      return;
    }

    resource_fetch_status_success::resource_fetch_status_success(const resource_status_record & status_)
    {
      set_status(status_);
      return;
    }

    resource_fetch_status_success::~resource_fetch_status_success()
    {
      return;
    }

    bool resource_fetch_status_success::has_status() const
    {
      return _status_.is_valid();
    }

    void resource_fetch_status_success::set_status(const resource_status_record & status_)
    {
      _status_ = status_;
      return;
    }

    const resource_status_record & resource_fetch_status_success::get_status() const
    {
      return _status_;
    }

    void resource_fetch_status_success::reset_status()
    {
      _status_.reset();
      return;
    }


    void resource_fetch_status_success::jsonize(jsontools::node & node_,
                                                const unsigned long int version_)
    {
      this->base_payload::jsonize(node_, version_);
      node_["status"] % _status_;
      return;
    }

    void resource_fetch_status_success::protobufize(protobuftools::message_node & node_,
                                                    const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_payload, node_);
      node_["status"] % _status_;
      return;
    }

    void resource_fetch_status_success::tree_dump(std::ostream & out_,
                                                  const std::string & title_,
                                                  const std::string & indent_,
                                                  bool inherit_) const
    {
      this->vire::utility::base_payload::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Status : ";
      if (!has_status()) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (has_status()) {
        std::ostringstream indent2;
        indent2 << indent_ << ::datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
        _status_.tree_dump(out_, "", indent2.str());
      }

      return;
    }

  } // namespace cms

} // namespace vire
