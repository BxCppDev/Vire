// vire/cms/device_status_change_event.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cms/device_status_change_event.h>

// Third Party:
// - datatools:
#include <datatools/exception.h>
#include <datatools/logger.h>
#include <datatools/utils.h>
// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_string_converter.h>

// Declare a protobuf registrar instance for the message class:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ResourceStatusChangeEvent.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::device_status_change_event",
                               vire::cms::ResourceStatusChangeEvent)

// This project:
#include <vire/base_object_protobuf.h>

namespace vire {

  namespace cms {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(device_status_change_event,
                                        "vire::cms::device_status_change_event")

    device_status_change_event::device_status_change_event()
    {
      return;
    }

    device_status_change_event::device_status_change_event(const resource_status_record & status_)
    {
      set_status(status_);
      return;
    }

    device_status_change_event::~device_status_change_event()
    {
      return;
    }

    bool device_status_change_event::has_status() const
    {
      return _status_.is_valid();
    }

    void device_status_change_event::set_status(const resource_status_record & status_)
    {
      _status_ = status_;
      return;
    }

    const resource_status_record & device_status_change_event::get_status() const
    {
      return _status_;
    }

    void device_status_change_event::reset_status()
    {
      _status_.reset();
      return;
    }

    void device_status_change_event::jsonize(jsontools::node & node_,
                                              const unsigned long int version_)
    {
      this->base_event::jsonize(node_, version_);
      node_["status"] % _status_;
      return;
    }

    void device_status_change_event::protobufize(protobuftools::message_node & node_,
                                                   const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_event, node_);
      node_["status"] % _status_;
      return;
    }

    void device_status_change_event::tree_dump(std::ostream & out_,
                                                 const std::string & title_,
                                                 const std::string & indent_,
                                                 bool inherit_) const
    {
      this->base_event::tree_dump(out_, title_, indent_, true);

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
