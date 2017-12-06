//! \file vire/device/base_datapoint_model.cc
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
#include <vire/device/base_datapoint_model.h>

// This project:
#include <vire/utility/rw_access.h>
// #include <vire/device/base_method_port_model.h>
#include <vire/device/physical_port.h>

namespace vire {

  namespace device {

    VIRE_DEVICE_REGISTRATION_IMPLEMENT(base_datapoint_model,
                                       "vire::device::base_datapoint_model");

    // static
    const std::string & base_datapoint_model::read_method_name()
    {
      static const std::string _n("__dp_read__");
      return _n;
    }

    // static
    const std::string & base_datapoint_model::write_method_name()
    {
      static const std::string _n("__dp_write__");
      return _n;
    }

    void base_datapoint_model::_set_defaults()
    {
      set_type(TYPE_DATAPOINT);
      set_allow_embedded_devices(true);
      _rw_access_ = vire::utility::RW_INVALID;
      _standard_interface_ = true;
      return;
    }

    base_datapoint_model::base_datapoint_model()
    {
      _set_defaults();
      return;
    }

    base_datapoint_model::~base_datapoint_model()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool base_datapoint_model::has_rw_access() const
    {
      return ::vire::utility::is_valid(_rw_access_);
    }

    void base_datapoint_model::set_rw_access(vire::utility::rw_access_type rw_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Datapoint model is already initialized!");
      _rw_access_ = rw_;
      return;
    }

    vire::utility::rw_access_type base_datapoint_model::get_rw_access() const
    {
      return _rw_access_;
    }

    bool base_datapoint_model::has_data_description() const
    {
      return _dd_.is_valid();
    }

    const datatools::introspection::data_description &
    base_datapoint_model::get_data_description() const
    {
      return _dd_;
    }

    void base_datapoint_model::set_data_description(const datatools::introspection::data_description & dd_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Datapoint model '" << get_name() << "' is already initialized!");
      _dd_ = dd_;
      return;
    }

    void base_datapoint_model::set_standard_interface(bool si_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error, "Datapoint model '" << get_name() << "' is already initialized!");
      _standard_interface_ = si_;
      return;
    }

    bool base_datapoint_model::is_standard_interface() const
    {
      return _standard_interface_;
    }

    const base_datapoint_model::method_port_model_dict_type &
    base_datapoint_model::get_standard_interface_method_port_models() const
    {
      return _si_mpm_;
    }

    void base_datapoint_model::_at_initialize(const datatools::properties & config_,
                                              model_repository & models_)
    {
      this->_datapoint_initialize(config_, models_);
      if (is_standard_interface()) {
        _install_standard_interface(config_);
      }
      return;
    }

    void base_datapoint_model::_at_reset()
    {
      this->_datapoint_reset();
      return;
    }

    void base_datapoint_model::_datapoint_initialize(const datatools::properties & config_,
                                                     model_repository & /* models_ */)
    {
      if (!has_rw_access()) {
        if (config_.has_key("rw_access")) {
          const std::string & rw_label = config_.fetch_string("rw_access");
          ::vire::utility::rw_access_type rw = vire::utility::RW_INVALID;
          DT_THROW_IF(!::vire::utility::from_string(rw_label, rw),
                      std::logic_error,
                      "Invalid R/W access label '" << rw_label << "'!");
          set_rw_access(rw);
        }
      }
      DT_THROW_IF(!has_rw_access(), std::logic_error, "Missing R/W access!");

      if (!has_data_description()) {
        datatools::properties dd_config;
        config_.export_and_rename_starting_with(dd_config, "data.", "");
        // XXX
        dd_config.tree_dump(std::cerr, "Data description params: ", "[DEVEL] ");
        if (dd_config.size()) {
          try {
            _dd_.initialize(dd_config);
          } catch (std::logic_error & error) {
            DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS,
                         "Invalid data description for datapoit model '" << get_name() << "': "
                         << error.what());
            throw error;
          }
        }
      }

      if (config_.has_key("standard_interface")) {
        set_standard_interface(config_.fetch_boolean("standard_interface"));
      }

      return;
    }

    void base_datapoint_model::_datapoint_reset()
    {
      _dd_.reset();
      _set_defaults();
      return;
    }

    void base_datapoint_model::_install_standard_interface(const datatools::properties & config_)
    {
      // std::cerr << "DEVEL: " << "base_datapoint_model::_install_standard_interface: " << get_name() << std::endl;

      if (vire::utility::is_readable(_rw_access_)) {
        datatools::logger::priority old_logging = get_logging_priority();
        set_logging_priority(datatools::logger::PRIO_DEBUG);
       _build_read_method(config_);
        set_logging_priority(old_logging);
      }

      if (vire::utility::is_writable(_rw_access_)) {
        _build_write_method(config_);
      }

      // std::cerr << "DEVEL: " << "base_datapoint_model::_install_standard_interface: " << get_name() << ". Done." << std::endl;
      return;
    }


    void base_datapoint_model::_build_read_method(const datatools::properties & config_)
    {
      DT_LOG_DEBUG(get_logging_priority(), "Entering...");

      // Read method:
      std::shared_ptr<datatools::introspection::method> read_method_sp(new datatools::introspection::method);
      datatools::introspection::method & read_method = *read_method_sp.get();
      read_method.set_name(read_method_name());
      read_method.set_terse_description("Read the datapoint's cached value");
      read_method.set_constness(true);

      {
        datatools::introspection::argument return_arg;
        return_arg.set_name("return");
        return_arg.set_description("Datapoint's current cached value");
        return_arg.set_access(datatools::introspection::ACCESS_OUTPUT);
        datatools::introspection::data_description & return_arg_dd = return_arg.grab_data_description();
        return_arg_dd = _dd_;
        read_method.add_argument(return_arg);
      }

      method_port_model_ptr_type mpm_ptr(new base_method_port_model);
      base_method_port_model & mpm = *mpm_ptr;
      mpm.set_name(read_method_name() + ".model");
      mpm.set_terse_description("Read method port model for the '" + get_name() + "' datapoint model");
      mpm.set_method(read_method_sp);
      mpm.initialize_simple();
      DT_THROW_IF(_si_mpm_.find(mpm.get_name()) != _si_mpm_.end(),
                  std::logic_error,
                  "Standard interface already contains a method port with name '" << mpm.get_name() << "'!");
      _si_mpm_[mpm.get_name()] = mpm_ptr;
      physical_port & read_method_port = add_port(read_method_name(), mpm);
      DT_LOG_DEBUG(get_logging_priority(), "Read method port: ");
      if (datatools::logger::is_debug(get_logging_priority())) {
        read_method_port.tree_dump(std::cerr, "", "[debug] ");
      }

      DT_LOG_DEBUG(get_logging_priority(), "Exiting.");
      return;
    }

    void base_datapoint_model::_build_write_method(const datatools::properties & config_)
    {
      std::shared_ptr<datatools::introspection::method> write_method_sp(new datatools::introspection::method);
      datatools::introspection::method & write_method = *write_method_sp.get();
      write_method.set_name(write_method_name());
      write_method.set_terse_description("Write the datapoint's setpoint value");
      write_method.set_constness(true);

      {
        datatools::introspection::argument setpoint_arg;
        setpoint_arg.set_name("setpoint");
        setpoint_arg.set_description("Datapoint's setpoint value");
        setpoint_arg.set_access(datatools::introspection::ACCESS_INPUT);
        datatools::introspection::data_description & setpoint_arg_dd = setpoint_arg.grab_data_description();
        setpoint_arg_dd = _dd_;
        write_method.add_argument(setpoint_arg);
      }

      method_port_model_ptr_type mpm_ptr(new base_method_port_model);
      base_method_port_model & mpm = *mpm_ptr;
      mpm.set_name(write_method_name() + ".model");
      mpm.set_terse_description("Write method port model for the '" + get_name() + "' datapoint model");
      mpm.set_method(write_method_sp);
      mpm.initialize_simple();
      DT_THROW_IF(_si_mpm_.find(mpm.get_name()) != _si_mpm_.end(),
                  std::logic_error,
                  "Standard interface already contains a method port with name '" << mpm.get_name() << "'!");
      _si_mpm_[mpm.get_name()] = mpm_ptr;
      physical_port & write_method_port = add_port(write_method_name(), mpm);
      DT_LOG_DEBUG(get_logging_priority(), "Write method port: ");
      if (datatools::logger::is_debug(get_logging_priority())) {
        write_method_port.tree_dump(std::cerr, "", "[debug] ");
      }

      return;
    }

    void base_datapoint_model::tree_dump(std::ostream & out_,
                                         const std::string & title_,
                                         const std::string & indent_,
                                         bool inherit_) const
    {
      this->base_device_model::tree_dump(out_, title_, indent_, true);

      // Default interface:
      out_ << indent_
           << i_tree_dumpable::tag
           << "Standard interface : "
           << is_standard_interface()
           << std::endl;

      // R/W access:
      out_ << indent_
           << i_tree_dumpable::tag
           << "R/W access : ";
      if (! has_rw_access()) {
        out_ << "<none>";
      } else {
        out_ << ::vire::utility::to_string(_rw_access_);
      }
      out_ << std::endl;

      // Data description:
      out_ << indent_
           << i_tree_dumpable::inherit_skip_tag(inherit_)
           << "Data description : ";
      if (! has_data_description()) {
        out_ << "<none>";
      }
      out_ << std::endl;
      if (has_data_description()) {
        std::ostringstream indent2;
        indent2 << indent_ << i_tree_dumpable::inherit_skip_tag(inherit_);
        _dd_.tree_dump(out_, "", indent2.str());
      }

      return;
    }

  } // end of namespace device

} // end of namespace vire
