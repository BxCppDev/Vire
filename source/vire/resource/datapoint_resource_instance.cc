// vire/resource/datapoint_resource_instance.cc - Implementation of Vire datapoint resource instance
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
#include <vire/resource/datapoint_resource_instance.h>

namespace vire {

  namespace resource {

    datapoint_resource_instance::datapoint_resource_instance()
      : base_resource_instance(RESOURCE_CATEGORY_DATAPOINT)
    {
      _initialized_ = false;
      _model_ = 0;
      return;
    }

    datapoint_resource_instance::~datapoint_resource_instance()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool datapoint_resource_instance::has_model() const
    {
      return _model_ != 0;
    }

    void datapoint_resource_instance::set_model(const vire::device::base_datapoint_model & model_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource instance is already initialized!");
      _model_ = &model_;
      return;
    }

    void datapoint_resource_instance::reset_model()
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource instance is already initialized!");
      _model_ = 0;
      return;
    }

    const vire::device::base_datapoint_model &
    datapoint_resource_instance::get_model() const
    {
      DT_THROW_IF(!has_model(), std::logic_error,
                  "Resource instance has no datapoint model!");
      return *_model_;
    }

    bool datapoint_resource_instance::is_initialized() const
    {
      return _initialized_;
    }

    void datapoint_resource_instance::initialize(const datatools::properties & config_,
                                                 datatools::service_manager & svr_manager_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Resource instance is already initialized!");

      datatools::enriched_base::initialize(config_);

      // Checks:
      DT_THROW_IF(_model_ == 0, std::logic_error,
                  "Resource instance has no datapoint model!!");
      _initialized_ = true;
      return;
    }

    void datapoint_resource_instance::reset()
    {
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Resource instance is not initialized!");
      _initialized_ = false;

      datatools::enriched_base::reset();
      return;
    }

    void datapoint_resource_instance::print_tree(std::ostream & out_,
                                                 const boost::property_tree::ptree & options_) const
    {
      datatools::i_tree_dumpable::base_print_options popts;
      popts.configure_from(options_);

      base_resource_instance::tree_dump(out_, popts.title, popts.indent, true);

      out_ << popts.indent << datatools::i_tree_dumpable::inherit_tag(popts.inherit)
           << "Model  : ";
      if (_model_ == nullptr) {
        out_ << "<none>";
      } else {
        out_ << "'" << _model_->get_name() << "'";
      }
      out_ << std::endl;
 
      return;
    }
      

  } // namespace resource

} // namespace vire
