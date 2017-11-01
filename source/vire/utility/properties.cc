// vire/utility/properties.cc
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
#include <vire/utility/properties.h>

// Third party:
// Boost:
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
// Bayeux:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/units.h>
// BxJsontools:
#include <bayeux/jsontools/std_type_converters.h>
// BxProtobuftools:
#include <bayeux/protobuftools/protobuf_factory.h>
#include <bayeux/protobuftools/std_string_converter.h>
#include <bayeux/protobuftools/std_vector_converter.h>

// This project:
#include "vire/utility/Properties.pb.h"
BXPROTOBUFTOOLS_REGISTER_CLASS("vire::utility::properties",
                               vire::utility::Properties)

namespace vire {

  namespace utility {

    VIRE_UTILITY_PAYLOAD_IMPLEMENTATION(properties,
                                        "vire::utility::properties");

    properties::properties()
    {
      return;
    }

    bool properties::is_valid() const
    {
      if (!has_id()) {
        return false;
      }
      return true;
    }

    bool properties::has_id() const
    {
      return _id_.is_valid();
    }

    void properties::reset_id()
    {
      _id_.reset();
      return;
    }

    void properties::set_id(const vire::utility::instance_identifier & id_)
    {
      _id_ = id_;
      return;
    }

    const vire::utility::instance_identifier &
    properties::get_id() const
    {
      return _id_;
    }

    bool properties::has_model() const
    {
      return _model_.is_valid();
    }

    void properties::reset_model()
    {
      _model_.reset();
      return;
    }

    void properties::set_model(const vire::utility::model_identifier & model_)
    {
      _model_ = model_;
      return;
    }

    const vire::utility::model_identifier &
    properties::get_model() const
    {
      return _model_;
    }

    bool properties::has_property(const std::string & key_) const
    {
      for (const auto & prop : _properties_) {
        if (prop.get_key() == key_) return true;
      }
      return false;
    }

    void properties::add_property(const metadata_record & record_)
    {
      DT_THROW_IF(!record_.is_valid(), std::logic_error, "Invalid property record!");
      DT_THROW_IF(has_property(record_.get_key()), std::logic_error, "Property '" << record_.get_key() << "' already exists!");
      _properties_.push_back(record_);
      return;
    }

    void properties::add_property(const std::string & key_,
                                  const std::string & value_,
                                  const std::string & type_,
                                  const std::string & add_)
    {
      DT_THROW_IF(has_property(key_), std::logic_error, "Property '" << key_ << "' already exists!");
      metadata_record mr(key_, value_, type_, add_);
      _properties_.push_back(mr);
      return;
    }

    bool properties::fetch_property(const std::string & key_,
                                    vire::utility::metadata_record & property_) const
    {
      property_.reset();
      for (const auto & prop : _properties_) {
        if (prop.get_key() == key_) {
          property_ = prop;
          return true;
        }
      }
      return false;
    }

    const vire::utility::metadata_record &
    properties::get_property(const std::string & key_) const
    {
      const vire::utility::metadata_record * p = nullptr;
      for (const auto & prop : _properties_) {
        if (prop.get_key() == key_) {
          p = &prop;
          break;
        }
      }
      DT_THROW_IF(p == nullptr, std::logic_error, "No property with key '" << key_ << "'!");
      return *p;
    }

    void properties::remove_property(const std::string & key_)
    {
      std::size_t index = _properties_.size();
      for (std::size_t i = 0; i < _properties_.size(); i++) {
        if (_properties_[i].get_key() == key_) {
          index = i;
          break;
        }
      }
      if (index < _properties_.size()) {
        _properties_.erase(_properties_.begin()+index);
      } else {
        DT_THROW(std::logic_error, "No property with key '" << key_ << "'!");
      }
      return;
    }

    void properties::build_keys(std::set<std::string> & keys_) const
    {
      keys_.clear();
      for (const auto & prop : _properties_) {
        keys_.insert(prop.get_key());
      }
      return;
    }

    void properties::remove_properties()
    {
      _properties_.clear();
      return;
    }

    std::size_t properties::get_number_of_properties() const
    {
      return _properties_.size();
    }

    void properties::reset()
    {
      remove_properties();
      reset_model();
      reset_id();
      return;
    }

    void properties::import_from(const datatools::properties & bxprops_, const uint32_t flags_)
    {
      bool skip_on_error = false;
      if (flags_ & IO_SKIP_ON_ERROR) {
        skip_on_error = true;
      }
      if (flags_ & IO_CLEAR_TARGET) {
        this->remove_properties();
      }
      return;
    }

    void properties::export_to(datatools::properties & bxprops_, const uint32_t flags_) const
    {
      bool skip_on_error = false;
      if (flags_ & IO_SKIP_ON_ERROR) {
        skip_on_error = true;
      }
      if (flags_ & IO_CLEAR_TARGET) {
        bxprops_.erase_all();
      }
      for (std::size_t i = 0; i != _properties_.size(); i++) {
        const metadata_record & mdr = _properties_[i];
        const std::string & key = mdr.get_key();
        const std::string & type = mdr.get_type();
        const std::string & value_repr = mdr.get_value();
        const std::string & add = mdr.get_additional();
        // Check type:
        if (type == "boolean" || type == "bool") {
          bool val = true;
          try {
            val = boost::lexical_cast<bool>(value_repr);
          } catch (...) {
            if (skip_on_error) continue;
            throw;
          }
          bxprops_.store_boolean(key, val);
        } else if (type == "int" || type == "integer") {
          int val = 0;
          try {
            val = boost::lexical_cast<int>(value_repr);
          } catch (...) {
            if (skip_on_error) continue;
            throw;
          }
           bxprops_.store_integer(key, val);
        } else if (type == "float" || type == "real" || type == "double") {
          double val = datatools::invalid_real();
          try {
            val = boost::lexical_cast<double>(value_repr);
          } catch (...) {
            if (skip_on_error) continue;
            throw;
          }
          if (!add.empty()) {
            if (boost::starts_with(add, "unit=")) {
              std::string unit_str = add.substr(5);
              double uval = 1.0;
              try {
                uval = datatools::units::get_unit(unit_str);
              } catch (...) {
                if (skip_on_error) continue;
                throw;
              }
              val *= uval;
              bxprops_.store_real_with_explicit_unit(key, val);
              bxprops_.set_unit_symbol(key, unit_str);
            } else {
              bxprops_.store_real(key, val);
            }
          } else {
            bxprops_.store_real(key, val);
          }
        } else if (type.empty() || type == "str"  || type == "string") {
          bxprops_.store_string(key, value_repr);
          if (!add.empty()) {
            if (add == "path") {
              bxprops_.set_explicit_path(key, true);
            }
          }
        } else {
          if (skip_on_error) {
            continue;
          }
          DT_THROW(std::logic_error,
                   "Cannot export property '" << key << "' due to unsupported type '" << type << "'!");
        }
      }
      return;
    }

    void properties::jsonize(jsontools::node & node_,
                             const unsigned long int version_)
    {
      this->base_payload::jsonize(node_, version_);
      node_["id"] % _id_;
      node_["model"] % _model_;
      node_["properties"] % _properties_;


      return;
    }

    void properties::protobufize(protobuftools::message_node & node_,
                                 const unsigned long int /* version_ */)
    {
      VIRE_PROTOBUFIZE_PROTOBUFABLE_BASE_OBJECT(base_payload, node_);
      node_["id"] % _id_;
      node_["model"] % _model_;
      node_["properties"] % _properties_;
      return;
    }

    void properties::tree_dump(std::ostream & out_,
                               const std::string & title_,
                               const std::string & indent_,
                               bool inherit_) const
    {
      base_payload::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Setup ID : ";
      if (has_id()) {
        out_ << "'" << _id_.to_string() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Model : ";
      if (has_model()) {
        out_ << "'" << _model_.to_string() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << ::datatools::i_tree_dumpable::tag
           << "Properties : [" << _properties_.size() << "]";
      out_ << std::endl;
      {
        std::size_t counter = 0;
        for (const auto & prop : _properties_) {
          out_ << indent_ << ::datatools::i_tree_dumpable::skip_tag;
          std::ostringstream indent2;
          indent2 << indent_ << ::datatools::i_tree_dumpable::skip_tag;
          if (counter + 1 == _properties_.size()) {
            out_ << ::datatools::i_tree_dumpable::last_tag;
            indent2 << ::datatools::i_tree_dumpable::last_skip_tag;
          } else {
            out_ << ::datatools::i_tree_dumpable::tag;
            indent2 << ::datatools::i_tree_dumpable::skip_tag;
          }
          out_ << "Property #" << counter << ":" << std::endl;
          // std::ostringstream indent2;
          // indent2 << indent_ << ::datatools::i_tree_dumpable::skip_tag;
          // if (counter == _properties_.size()) {
          //   indent2 << ::datatools::i_tree_dumpable::last_tag;
          // } else {
          //   indent2 << ::datatools::i_tree_dumpable::tag;
          // }
          prop.tree_dump(out_, "", indent2.str(), false);
          counter++;
        }
      }

      out_ << indent_ << ::datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Validity : " << std::boolalpha << is_valid() << std::endl;

      return;
    }

  } // namespace utility

} // namespace vire
