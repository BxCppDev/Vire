//! \file vire/cmsserver/process_info.cc
//
// Copyright (c) 2017 by Jean Hommet <hommet@lpccaen.in2p3.fr>
//                       François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/cmsserver/process_info.h>

// Third party:
// - Bayeux:
#include <bayeux/datatools/exception.h>

namespace vire {

  namespace cmsserver {

    // Serialization implementation
    DATATOOLS_SERIALIZATION_IMPLEMENTATION(process_info, "vire::cmsserver::process_info")

    process_info::process_info()
    {
      return;
    }

    process_info::~process_info()
    {
      return;
    }

    bool process_info::is_valid() const
    {
      if (!has_name()) return false;
      if (!has_model_id()) return false;
      return true;
    }

    void process_info::reset()
    {
      _name_.clear();
      _description_.clear();
      _model_id_.clear();
      _config_urn_.clear();
      _config_path_.clear();
      _config_.clear();
      return;
    }

    bool process_info::has_name() const
    {
      return !_name_.empty();
    }

    void process_info::set_name(const std::string & name_)
    {
      _name_ = name_;
      return;
    }

    const std::string & process_info::get_name() const
    {
      return _name_;
    }

    bool process_info::has_description() const
    {
      return !_description_.empty();
    }

    void process_info::set_description(const std::string & description_)
    {
      _description_ = description_;
      return;
    }

    const std::string & process_info::get_description() const
    {
      return _description_;
    }

    bool process_info::has_model_id() const
    {
      return !_model_id_.empty();
    }

    void process_info::set_model_id(const std::string & model_id_)
    {
      _model_id_ = model_id_;
      return;
    }

    const std::string & process_info::get_model_id() const
    {
      return _model_id_;
    }

    bool process_info::has_config_urn() const
    {
      return !_config_.empty();
    }

    void process_info::set_config_urn(const std::string & config_urn_)
    {
      _config_urn_ = config_urn_;
      return;
    }

    const std::string & process_info::get_config_urn() const
    {
      return _config_urn_;
    }

    bool process_info::has_config_path() const
    {
      return !_config_.empty();
    }

    void process_info::set_config_path(const std::string & config_path_)
    {
      _config_path_ = config_path_;
      return;
    }

    const std::string & process_info::get_config_path() const
    {
      return _config_path_;
    }

    const datatools::properties & process_info::get_config() const
    {
      return _config_;
    }

    datatools::properties & process_info::grab_config()
    {
      return _config_;
    }

    void process_info::set_config(const datatools::properties & config_)
    {
      _config_ = config_;
      return;
    }

    void process_info::tree_dump(std::ostream & out_,
                                 const std::string & title_,
                                 const std::string & indent_,
                                 bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << i_tree_dumpable::tag
           << "Name        : ";
      out_ << "'" << _name_ << "'";
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Description : ";
      out_ << "'" << _description_ << "'";
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Model ID    : ";
      out_ << "'" << _model_id_ << "'";
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Config URN  : ";
      out_ << "'" << _config_urn_ << "'";
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Config path : ";
      out_ << "'" << _config_path_ << "'";
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Config      : ";
      out_ << "[" << _config_.size() << "]";
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Valid       : " << std::boolalpha << is_valid() << std::endl;

      return;
    }

  } // namespace cmsserver

} // namespace vire
