// vire/utility/metadata_record.cc
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
#include <vire/utility/metadata_record.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/exception.h>
// - BxJsontools:
#include <bayeux/jsontools/base_type_converters.h>
#include <bayeux/jsontools/std_type_converters.h>
// - BxProtobuftools:
#include <bayeux/protobuftools/base_type_converters.h>
#include <bayeux/protobuftools/std_type_converters.h>

// This project:
#include <vire/utility/json_converters.h>

// Declare a protobuf registrar instance for the message class:
#include <bayeux/protobuftools/protobuf_factory.h>
#include "vire/utility/MetadataRecord.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::metadata_record",
                               vire::utility::MetadataRecord)

namespace vire {

  namespace utility {

    metadata_record::metadata_record()
    {
      return;
    }

    metadata_record::metadata_record(const std::string & key_, const std::string & value_)
    {
      set_key(key_);
      set_value(value_);
      return;
    }

    metadata_record::~metadata_record()
    {
      return;
    }

    bool metadata_record::has_key() const
    {
      return !_key_.empty();
    }

    void metadata_record::set_key(const std::string & key_)
    {
      _key_ = key_;
      return;
    }

    const std::string & metadata_record::get_key() const
    {
      return _key_;
    }

    bool metadata_record::has_value() const
    {
      return !_value_.empty();
    }

    void metadata_record::set_value(const std::string & value_)
    {
      _value_ = value_;
      return;
    }

    const std::string & metadata_record::get_value() const
    {
      return _value_;
    }

    bool metadata_record::is_valid() const
    {
      if (_key_.empty()) return false;
      return true;
    }

    void metadata_record::reset()
    {
      _key_.clear();
      _value_.clear();
      return;
    }

    void metadata_record::jsonize(jsontools::node & node_,
                                  const unsigned long int /* version_ */)
    {
      node_["key"] % _key_;
      node_["value"] % _value_;
      return;
    }

    void metadata_record::protobufize(protobuftools::message_node & node_,
                                      const unsigned long int /* version_ */)
    {
      node_["key"] % _key_;
      node_["value"] % _value_;
      return;
    }

    void metadata_record::tree_dump(std::ostream & out_,
                                    const std::string & title_,
                                    const std::string & indent_,
                                    bool inherit_) const
    {
      if (! title_.empty()) out_ << indent_ << title_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Key : ";
      if (has_key()) {
        out_ << "'" << _key_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Value : ";
      if (has_value()) {
        out_ << "'" << _value_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Validity : " << (is_valid() ? "<yes>" : "<no>") << std::endl;

      return;
    }

  } // namespace utility

} // namespace vire
