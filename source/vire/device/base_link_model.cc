//! \file vire/device/base_link_model.cc
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
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vire/device/base_link_model.h>

// - Bayeux/datatools:
#include <datatools/detail/reflection_macros.h>

// This project:
#include <vire/device/logical_link.h>

namespace vire {

  namespace device {

    DATATOOLS_FACTORY_SYSTEM_REGISTER_IMPLEMENTATION(base_link_model,
                                                     "vire::device::base_link_model/__system__");

    VIRE_DEVICE_LINK_REGISTRATION_IMPLEMENT(base_link_model, "vire::device::base_link_model");

    bool base_link_model::is_virtual() const
    {
      return _virtual_;
    }

    void base_link_model::set_virtual(bool v_)
    {
      _virtual_ = v_;
      return;
    }

    bool base_link_model::has_type() const
    {
      return _type_ != LINK_UNDEFINED;
    }

    void base_link_model::reset_type()
    {
      _type_ = LINK_UNDEFINED;
      return;
    }

    void base_link_model::set_type(link_type type_)
    {
      _type_ = type_;
      return;
    }

    link_type base_link_model::get_type() const
    {
      return (link_type) _type_;
    }

    bool base_link_model::has_start_port_type() const
    {
      return _start_port_type_ != PORT_UNDEFINED;
    }

    void base_link_model::reset_start_port_type()
    {
      _start_port_type_ = PORT_UNDEFINED;
      return;
    }

    void base_link_model::set_start_port_type(port_type type_)
    {
      _start_port_type_ = type_;
      return;
    }

    port_type base_link_model::get_start_port_type() const
    {
      return _start_port_type_;
    }

    bool base_link_model::has_stop_port_type() const
    {
      return _stop_port_type_ != PORT_UNDEFINED;
    }

    void base_link_model::reset_stop_port_type()
    {
      _stop_port_type_ = PORT_UNDEFINED;
      return;
    }

    void base_link_model::set_stop_port_type(port_type type_)
    {
      _stop_port_type_ = type_;
      return;
    }

    port_type base_link_model::get_stop_port_type() const
    {
      return _stop_port_type_;
    }

    /*
    bool base_link_model::has_start_port_io_type() const
    {
      return _start_port_io_type_ != PORT_IO_UNDEFINED;
    }

    void base_link_model::reset_start_port_io_type()
    {
      _start_port_io_type_ = PORT_IO_UNDEFINED;
      return;
    }

    void base_link_model::set_start_port_io_type(port_io_type io_type_)
    {
      _start_port_io_type_ = io_type_;
      return;
    }

    port_io_type base_link_model::get_start_port_io_type() const
    {
      return (port_io_type) _start_port_io_type_;
    }

    bool base_link_model::has_stop_port_io_type() const
    {
      return _stop_port_io_type_ != PORT_IO_UNDEFINED;
    }

    void base_link_model::reset_stop_port_io_type()
    {
      _stop_port_io_type_ = PORT_IO_UNDEFINED;
      return;
    }

    void base_link_model::set_stop_port_io_type(port_io_type io_type_)
    {
      _stop_port_io_type_ = io_type_;
      return;
    }

    port_io_type base_link_model::get_stop_port_io_type() const
    {
      return (port_io_type) _stop_port_io_type_;
    }
    */

    bool base_link_model::has_format() const
    {
      return !_format_.empty();
    }

    void base_link_model::set_format(const std::string & f_)
    {
      _format_ = f_;
      return;
    }

    void base_link_model::reset_format()
    {
      _format_.clear();
      return;
    }

    const std::string & base_link_model::get_format() const
    {
      return _format_;
    }

    base_link_model::base_link_model()
    {
      set_logging_priority(datatools::logger::PRIO_FATAL);
      _initialized_ = false;
      _set_defaults();
      return;
    }

    base_link_model::base_link_model(link_type type_)
    {
      set_logging_priority(datatools::logger::PRIO_FATAL);
      _initialized_ = false;
      _set_defaults();
      set_type(type_);
      return;
    }

    base_link_model::~base_link_model()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool base_link_model::is_initialized() const
    {
      return _initialized_;
    }

    void base_link_model::_set_defaults()
    {
      _virtual_ = false;
      _type_ = LINK_UNDEFINED;
      _start_port_type_ = PORT_UNDEFINED;
      _stop_port_type_ = PORT_UNDEFINED;
      // _start_port_io_type_ = PORT_IO_UNDEFINED;
      // _stop_port_io_type_ = PORT_IO_UNDEFINED;
      return;
    }

    void base_link_model::_set_initialized_(bool init_)
    {
      _initialized_ = init_;
      return;
    }

    void base_link_model::initialize_simple()
    {
      datatools::properties dummy_config;
      this->initialize(dummy_config);
      return;
    }

    void base_link_model::initialize(const datatools::properties & config_)
    {
      DT_LOG_TRACE(get_logging_priority(), "Entering...");
      DT_THROW_IF(is_initialized(),
                  std::logic_error,
                  "Link model '" << get_name() << "' is already initialized !");

      // Basic initialization:
      this->datatools::enriched_base::initialize(config_, false);

      if (config_.has_key("virtual")) {
        set_virtual(config_.fetch_boolean("virtual"));
      }

      if (! has_type()) {
        if (config_.has_key("type")) {
          set_type(get_link_type(config_.fetch_string("type")));
        }
      }

      if (! has_start_port_type()) {
        if (config_.has_key("start_port_type")) {
          set_start_port_type(get_port_type(config_.fetch_string("start_port_type")));
        }
      }

      if (! has_stop_port_type()) {
        if (config_.has_key("stop_port_type")) {
          set_stop_port_type(get_port_type(config_.fetch_string("stop_port_type")));
        }
      }

      /*
      if (! has_start_port_io_type()) {
        if (config_.has_key("start_port_io_type")) {
          set_start_port_io_type(get_port_io_type(config_.fetch_string("start_port_io_type")));
        }
      }

      if (! has_stop_port_io_type()) {
        if (config_.has_key("stop_port_io_type")) {
          set_stop_port_io_type(get_port_io_type(config_.fetch_string("stop_port_io_type")));
        }
      }
      */

      if (! has_format()) {
        if (config_.has_key("format")) {
          set_format(config_.fetch_string("format"));
        }
      }

      // Initialization:
      DT_LOG_TRACE(get_logging_priority(), "Initialization is starting...");
      _at_initialize(config_);
      DT_LOG_TRACE(get_logging_priority(), "Initialization done.");

      // Post initialization:
      DT_LOG_TRACE(get_logging_priority(), "Post-initialization is starting...");
      _create_logical_();
      _logical_.grab().set_name(make_logical_name(this->get_name()));
      _logical_.grab().initialize();
      // ... populate the logical link... nothing here
      DT_LOG_TRACE(get_logging_priority(), "Post-initialization done.");

      _set_initialized_(true);
      DT_LOG_TRACE(get_logging_priority(), "Exiting.");
      return;
    }

    void base_link_model::reset()
    {
      DT_THROW_IF(! is_initialized(),
                  std::logic_error,
                  "Link model '" << get_name() << "' is not initialized !");
      _set_initialized_(false);

      // Pre reset:
      _logical_.reset();

      // Reset:
      _at_reset();

      _format_.clear();
      _set_defaults();
      this->enriched_base::clear();
      return;
    }

    bool base_link_model::has_logical() const
    {
      return _logical_.has_data();
    }

    const logical_link & base_link_model::get_logical() const
    {
      DT_THROW_IF(! has_logical(), std::logic_error,
                  "Link model '" << get_name() << "' has no logical link !");
      return _logical_.get();
    }

    logical_link & base_link_model::_grab_logical()
    {
      DT_THROW_IF(! has_logical(), std::logic_error,
                  "Link model '" << get_name() << "' has no logical link !");
      return _logical_.grab();
    }

    void base_link_model::_create_logical_()
    {
      if (! _logical_.has_data()) {
        _logical_.reset(new logical_link);
        _logical_.grab().set_model(*this);
      }
      return;
    }

    void base_link_model::_at_initialize(const datatools::properties & config_)
    {
      if (! has_type()) {
        set_type(LINK_GENERIC);
      }
      if (! has_start_port_type()) {
        set_start_port_type(PORT_GENERIC);
      }
      if (! has_stop_port_type()) {
        set_stop_port_type(PORT_GENERIC);
      }
      return;
    }

    void base_link_model::_at_reset()
    {
      reset_start_port_type();
      reset_stop_port_type();
      reset_type();
      return;
    }

    void base_link_model::tree_dump(std::ostream& out_,
                                      const std::string& title_,
                                      const std::string& indent_,
                                      bool inherit_) const
    {
      this->enriched_base::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << i_tree_dumpable::tag
           << "Virtual : " << is_virtual()
           << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Type : '" << get_link_type_label(_type_)
           << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Start port type : '" << get_port_type_label(_start_port_type_)
           << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Stop port type : '" << get_port_type_label(_stop_port_type_)
           << "'" << std::endl;

      /*
      out_ << indent_ << i_tree_dumpable::tag
           << "Start port I/O type : '" << get_port_io_type_label(_start_port_io_type_)
           << "'" << std::endl;

      out_ << indent_ << i_tree_dumpable::tag
           << "Stop port I/O type : '" << get_port_io_type_label(_stop_port_io_type_)
           << "'" << std::endl;
      */

      out_ << indent_ << i_tree_dumpable::inherit_tag(inherit_)
           << "Format : ";
      if (has_format()) {
        out_ << "'" << _format_ << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      return;
    }

  } // end of namespace device

} // end of namespace vire
