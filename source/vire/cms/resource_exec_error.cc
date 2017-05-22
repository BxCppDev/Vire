// vire/cms/resource_exec_error.cc - Error at resource execution
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
#include <vire/cms/resource_exec_error.h>

// Third party:
// - Boost:
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
#include <bayeux/jsontools/boost_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_string_converter.h>
#include <bayeux/protobuftools/boost_datetime_converters.h>

// This project:
#include <vire/base_object_protobuf.h>
#include "vire/cms/ResourceExecError.pb.h"

BXPROTOBUFTOOLS_REGISTER_CLASS("vire::cms::resource_exec_error",
                               vire::cms::ResourceExecError)

namespace vire {

  namespace cms {

    // static
    const int32_t resource_exec_error::EC_INVALID_EXEC_CONTEXT;
    const int32_t resource_exec_error::EC_TIMEOUT;

    resource_exec_error::resource_exec_error()
      : ::vire::utility::base_error(EC_INVALID_EXEC_CONTEXT, "")
    {
      return;
    }

    resource_exec_error::~resource_exec_error()
    {
      return;
    }

    bool resource_exec_error::is_invalid_context_error() const
    {
      return get_code() == EC_INVALID_EXEC_CONTEXT;
    }

    bool resource_exec_error::is_timeout_error() const
    {
      return get_code() == EC_TIMEOUT;
    }

    void resource_exec_error::make_invalid_context_error(const std::string & what_)
    {
      if (!has_message_format()) {
        if (! what_.empty()) {
          set_message_format(what_);
        } else {
          set_message_format("Invalid execution context");
        }
      }
      set_code(EC_INVALID_EXEC_CONTEXT);
      ::vire::time::invalidate(_deadline_);
      return;
    }

    void resource_exec_error::make_timeout_error(const boost::posix_time::ptime & deadline_)
    {
      if (!has_message_format()) {
        set_message_format("Deadline [%d] was reached");
      }
      set_code(EC_TIMEOUT);
      _deadline_ = deadline_;
      return;
    }

    void resource_exec_error::reset()
    {
      ::vire::time::invalidate(_deadline_);
      this->base_error::reset();
      return;
    }

    // virtual
    void resource_exec_error::_build_message(std::string & message_) const
    {
      std::string msg = get_message_format();
      if (is_invalid_context_error()) {
      }
      if (is_timeout_error()) {
        msg = boost::replace_all_copy(msg, "%d", boost::lexical_cast<std::string>(_deadline_));
      }
      message_ = msg;
      return;
    }

    void resource_exec_error::jsonize(jsontools::node & node_,
                                      const unsigned long int version_)
    {
      this->vire::utility::base_error::jsonize(node_, version_);
      if (get_code() == EC_INVALID_EXEC_CONTEXT) {
      }
      if (get_code() == EC_TIMEOUT) {
        node_["deadline"] % _deadline_;
      }
      return;
    }

    void resource_exec_error::protobufize(protobuftools::message_node & node_,
                                          const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(vire::utility::base_error, node_);
      if (get_code() == EC_INVALID_EXEC_CONTEXT) {
      }
      if (get_code() == EC_TIMEOUT) {
        node_["deadline"] % _deadline_;
      }
      return;
    }

  } // namespace cms

} // namespace vire
