// vire/utility/base_payload.cc
//
// Copyright (c) 2016 by Jean Hommet <hommet@lpccaen.in2p3.fr>,
//                       François Mauger <mauger@lpccaen.in2p3.fr>,
//                       Jérôme Poincheval <poincheval@lpccaen.in2p3.fr>
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
#include <vire/utility/base_payload.h>

// Declare a protobuf registrar instance for the message class:
#include <bayeux/protobuftools/protobuf_factory.h>
#include "vire/utility/BasePayload.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::base_payload",
                               vire::utility::BasePayload)

namespace vire {

  namespace utility {

    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_payload,
                                                    "vire::utility::base_payload/__system__")

    base_payload::base_payload()
    {
      return;
    }

    base_payload::~base_payload()
    {
      return;
    }

    void base_payload::tree_dump(std::ostream & out_,
                                 const std::string & title_,
                                 const std::string & indent_,
                                 bool inherit_) const
    {
      if (! title_.empty()) out_ << indent_ << title_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Payload type identifier : '" << get_serial_tag() << "'" << std::endl;

      return;
    }

    void base_payload::jsonize(jsontools::node & /* node_ */,
                               const unsigned long int /* version_ */)
    {
      return;
    }

    void base_payload::protobufize(protobuftools::message_node & /* node_ */,
                                   const unsigned long int /* version_ */)
    {
      return;
    }

  } // namespace utility

} // namespace vire
