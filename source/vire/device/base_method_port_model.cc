//! \file vire/device/base_method_port_model.cc
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/device/base_method_port_model.h>

// This project:
#include <vire/device/logical_port.h>

namespace vire {

  namespace device {

    VIRE_DEVICE_PORT_REGISTRATION_IMPLEMENT(base_method_port_model,
                                            "vire::device::base_method_port_model");

    void base_method_port_model::_set_defaults()
    {
      _rw_access_ = vire::utility::RW_INVALID;
      return;
    }

    base_method_port_model::base_method_port_model() : base_port_model(PORT_METHOD)
    {
      _set_defaults();
      return;
    }

    base_method_port_model::~base_method_port_model()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool base_method_port_model::has_rw_access() const
    {
      return ::vire::utility::is_valid(_rw_access_);
    }

    void base_method_port_model::set_rw_access(vire::utility::rw_access_type rw_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Method port model is already initialized!");
      _rw_access_ = rw_;
      return;
    }

    vire::utility::rw_access_type base_method_port_model::get_rw_access() const
    {
      return _rw_access_;
    }

    bool base_method_port_model::has_method() const
    {
      return _method_.get() != nullptr;
    }

    void base_method_port_model::set_method(const method_ptr_type & meth_ptr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Method port model is already initialized!");
      DT_THROW_IF(meth_ptr_.get() == nullptr, std::logic_error, "Null method pointer!");
      DT_THROW_IF(!meth_ptr_->is_valid(), std::logic_error, "Method is not valid!");
      _method_ = meth_ptr_;
      return;
    }

    void base_method_port_model::reset_method()
    {
      _method_.reset();
      return;
    }

    const ::datatools::introspection::method & base_method_port_model::get_method() const
    {
      DT_THROW_IF(! has_method(), std::logic_error, "Missing method!");
      return *_method_;
    }

    void base_method_port_model::tree_dump(std::ostream & out_,
                                           const std::string & title_,
                                           const std::string & indent_,
                                           bool inherit_) const
    {
      this->base_port_model::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Method : ";
      if (! has_method()) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (has_method()) {
        std::ostringstream indent2;
        indent2 << indent_ << i_tree_dumpable::inherit_skip_tag(inherit_);
        _method_->tree_dump(out_, "", indent2.str());
      }

      return;
    }

    void base_method_port_model::_at_initialize(const datatools::properties & config_)
    {
      this->base_port_model::_at_initialize(config_);


      if (!has_method()) {
        _method_.reset(new datatools::introspection::method);
        _method_->set_name(get_name());
        datatools::properties method_config;
        config_.export_and_rename_starting_with(method_config, "method.", "");
        _method_->initialize(method_config);
      } else {
        // std::cerr << "DEVEL: " << "base_method_port_model::_at_initialize: "
        //           << "Method description object already exists."
        //           << std::endl;
      }

      DT_THROW_IF(!has_method(), std::logic_error, "Missing method!");
      DT_THROW_IF(!get_method().is_valid(), std::logic_error, "Invalid method!");

      if (_method_->has_constness()) {
        if (_method_->is_constness()) {
          set_rw_access(vire::utility::RW_READABLE);
        }
      }

      if (!has_rw_access()) {
        if (config_.has_key("rw_access")) {
          const std::string & rw_label = config_.fetch_string("rw_access");
          ::vire::utility::rw_access_type rw = vire::utility::RW_INVALID;
          DT_THROW_IF(!::vire::utility::from_string(rw_label, rw),
                      std::logic_error,
                      "Invalid R/W access label '" << rw_label << "'!");
          set_rw_access(rw);
        } else {
          set_rw_access(vire::utility::RW_WRITABLE);
        }
      }
      DT_THROW_IF(!has_rw_access(), std::logic_error, "Missing R/W access!");

      return;
    }

    void base_method_port_model::_at_reset()
    {
      if (has_method()) {
        _method_->reset();
        _method_.reset();
      }
      _set_defaults();
      this->base_port_model::_at_reset();
      return;
    }

  } // end of namespace device

} // end of namespace vire
