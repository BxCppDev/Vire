// vire/resource/base_resource_instance.cc - Implementation of Vire base resource instance
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

// Ourselves:
#include <vire/resource/base_resource_instance.h>

namespace vire {

  namespace resource {

    base_resource_instance::base_resource_instance(resource_category_type rc_)
      : _resource_category_(rc_)
    {
      return;
    }

    base_resource_instance::~base_resource_instance()
    {
      return;
    }

    bool base_resource_instance::has_resource_category() const
    {
      return _resource_category_ != RESOURCE_CATEGORY_INVALID;
    }

    resource_category_type base_resource_instance::get_resource_category() const
    {
      return _resource_category_;
    }

    void base_resource_instance::set_resource_category(resource_category_type rc_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource is already initialized!");
      _resource_category_ = rc_;
      return;
    }

    bool base_resource_instance::is_method() const
    {
      return _resource_category_ == RESOURCE_CATEGORY_METHOD;
    }

    bool base_resource_instance::is_datapoint() const
    {
      return _resource_category_ == RESOURCE_CATEGORY_DATAPOINT;
    }

    bool base_resource_instance::is_user() const
    {
      return _resource_category_ == RESOURCE_CATEGORY_USER;
    }

    void base_resource_instance::initialize_simple()
    {
      datatools::properties dummy_config;
      initialize_standalone(dummy_config);
      return;
    }

    void base_resource_instance::initialize_standalone(const datatools::properties & config_)
    {
      datatools::service_manager dummy_smgr;
      initialize(config_, dummy_smgr);
      return;
    }

    void base_resource_instance::_base_initialize(const datatools::properties & config_)
    {
      datatools::enriched_base::initialize(config_);

      if (!has_resource_category()) {
        if (config_.has_key("resource_category")) {
          const std::string & rc_label = config_.fetch_string("resource_category");
          set_resource_category(label_to_resource_category(rc_label));
        }
      }

      return;
    }

    void base_resource_instance::_base_reset()
    {
      _resource_category_ = RESOURCE_CATEGORY_INVALID;
      datatools::enriched_base::reset();
      return;
    }

    void base_resource_instance::tree_dump(std::ostream & out_,
                                           const std::string & title_,
                                           const std::string & indent_,
                                           bool inherit_) const
    {
      this->datatools::enriched_base::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Category : ";
      if (has_resource_category()) {
        out_ << "'" << resource_category_to_label(_resource_category_) << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Initialized : " << is_initialized() << std::endl;

      return;
    }

  } // namespace resource

} // namespace vire
