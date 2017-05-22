//! \file vire/com/base_plug.cc
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

// Ourselves:
#include <vire/com/base_plug.h>

// This project:
#include <vire/com/manager.h>

namespace vire {

  namespace com {

    base_plug::base_plug()
    {
      return;
    }

    base_plug::base_plug(const manager & mgr_)
    {
      set_manager(mgr_);
      return;
    }

    base_plug::~base_plug()
    {
      if (is_initialized()) {
        DT_THROW(std::logic_error, "Plug has not been properly reset!");
      }
      return;
    }

    bool base_plug::has_manager() const
    {
      return _mgr_ != nullptr;
    }

    void base_plug::set_manager(const manager & mgr_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Plug is already initialized!");
      _mgr_ = &mgr_;
      return;
    }

    const manager & base_plug::get_manager() const
    {
      return *_mgr_;
    }

    bool base_plug::has_name() const
    {
      return !_name_.empty();
    }

    void base_plug::set_name(const std::string & name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Plug is already initialized!");
      _name_ = name_;
      return;
    }

    const std::string & base_plug::get_name() const
    {
      return _name_;
    }

    bool base_plug::has_domain_name() const
    {
      return !_domain_name_.empty();
    }

    void base_plug::set_domain_name(const std::string & domain_name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Plug is already initialized!");
      if (has_manager()) {
        DT_THROW_IF(!_mgr_->has_domain(domain_name_), std::logic_error,
                    "Manager has no domain named '" << domain_name_ << "'!");
      }
      _domain_name_ = domain_name_;
      return;
    }

    const std::string & base_plug::get_domain_name() const
    {
      return _domain_name_;
    }

    // void base_plug::set_drivers_config(const datatools::properties & cfg_)
    // {
    //   _drivers_config_ = cfg_;
    //   return;
    // }

    // const datatools::properties & base_plug::get_drivers_config() const
    // {
    //   return _drivers_config_;
    // }

    void base_plug::reset_messages_counters()
    {
      _received_messages_counter_ = 0;
      _sent_messages_counter_ = 0;
      return;
    }

    std::size_t base_plug::get_received_messages_counter() const
    {
      return _received_messages_counter_;
    }

    std::size_t base_plug::get_sent_messages_counter() const
    {
      return _sent_messages_counter_;
    }

    bool base_plug::is_initialized() const
    {
      return _initialized_;
    }

    void base_plug::_increment_sent_messages_counter()
    {
      _sent_messages_counter_++;
      return;
    }

    void base_plug::_decrement_sent_messages_counter()
    {
      DT_THROW_IF(_sent_messages_counter_ == 0, std::logic_error,
                  "Sent messages count is null ! Cannot decrement!");
      _sent_messages_counter_--;
      return;
    }

    // bool base_plug::has_protocol_driver() const
    // {
    //   return _protocol_driver_.get() != nullptr;
    // }

    /*
    i_protocol_driver & base_plug::grab_protocol_driver()
    {
      if (_protocol_driver_.get() == nullptr) {
        vire::com::i_protocol_driver * new_driver = nullptr;
        typedef vire::com::i_protocol_driver::factory_register_type::factory_type FactoryType;
        const FactoryType & the_factory
          = DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(vire::com::i_protocol_driver).get(get_protocol_type_id().get_name());
        // const vire::com::i_protocol_driver::FactoryType & the_factory
        //   = factory_register_.get(get_protocol_type_id().get_name());
        new_driver = the_factory();
        _protocol_driver_.reset(new_driver);
        datatools::properties protocol_config;
        _drivers_config_.export_and_rename_starting_with(protocol_config, "protocol.", "");
        _protocol_driver_->initialize(protocol_config);
      }
      return *_protocol_driver_;
    }
    */

    /*
    vire::utility::i_encoding_driver & base_plug::grab_encoding_driver()
    {
      if (_encoding_driver_.get() == nullptr) {
        vire::utility::i_encoding_driver * new_driver = nullptr;
        typedef vire::utility::i_encoding_driver::factory_register_type::factory_type FactoryType;
        const FactoryType & the_factory
          = DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(vire::utility::i_encoding_driver).get(get_encoding_type_id().get_name());
        const vire::utility::i_encoding_driver::FactoryType & the_factory = factory_register_.get(entry.get_service_id());
        new_driver = the_factory();
        _encoding_driver_.reset(new_driver);
        datatools::properties encoding_config;
        _drivers_config_.export_and_rename_starting_with(encoding_config, "encoding.", "");
        _encoding_driver_->initialize(encoding_config);
      }
      return *_encoding_driver_;
    }
    */

    void base_plug::_increment_received_messages_counter()
    {
      _received_messages_counter_++;
      return;
    }

    void base_plug::_decrement_received_messages_counter()
    {
      DT_THROW_IF(_received_messages_counter_ == 0, std::logic_error,
                  "Received messages count is null ! Cannot decrement!");
      _received_messages_counter_--;
      return;
    }

    void base_plug::_set_initialized(bool i_)
    {
      _initialized_ = i_;
      return;
    }

    void base_plug::_base_init()
    {
      DT_THROW_IF(!has_name(), std::logic_error,
                  "Plug has no name!");
      DT_THROW_IF(!has_manager(), std::logic_error,
                  "Plug '" << get_name() << "' has no parent communication manager!");
      DT_THROW_IF(!has_domain_name(), std::logic_error,
                  "Plug '" << get_domain_name() << "' has no domain name!");
      DT_THROW_IF(!_mgr_->has_domain(get_domain_name()), std::logic_error,
                  "Parent communication manager has no domain named '" << get_domain_name() << "'!");
      _dom_ = _mgr_->get_domain(get_domain_name());
      _init_encoding_driver_();
      return;
    }

    void base_plug::_base_reset()
    {
      _reset_encoding_driver_();
      _dom_ = nullptr;
      reset_messages_counters();
      _domain_name_.clear();
      _name_.clear();
      _mgr_ = nullptr;
      _config_.clear();
      _initialized_ = false;
      return;
    }

    void base_plug::_init_encoding_driver_()
    {
      // vire::utility::i_encoding_driver * new_driver = nullptr;
      typedef vire::utility::i_encoding_driver::factory_register_type::factory_type FactoryType;
      const std::string & encoding_type_id = _dom_->get_encoding_type_id().get_name();
      const FactoryType & the_factory
        = DATATOOLS_FACTORY_GET_SYSTEM_REGISTER(vire::utility::i_encoding_driver).get(encoding_type_id);
      //const vire::utility::i_encoding_driver::FactoryType & the_factory = factory_register_.get(entry.get_service_id());
      //new_driver = the_factory();
      _encoding_.reset(the_factory());
      datatools::properties encoding_config;
      _config_.export_and_rename_starting_with(encoding_config, "encoding.", "");
      if (encoding_config.size()) {
        _encoding_->initialize(encoding_config);
      } else {
        _encoding_->initialize_simple();
      }
      return;
    }

    void base_plug::_reset_encoding_driver_()
    {
      _encoding_->reset();
      _encoding_.reset();
      return;
    }

    void base_plug::tree_dump(std::ostream & out_,
                              const std::string & title_,
                              const std::string & indent_,
                              bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Initialized : " << std::boolalpha << is_initialized() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Manager : ";
      if (has_manager()) {
        out_ << "[@" << _mgr_ << "]";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name : '" << _name_ << "'" << std::endl;

      // out_ << indent_ << datatools::i_tree_dumpable::tag
      //      << "Full identifier : '" << get_full_key() << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Domain name : '" << _domain_name_ << "'" << std::endl;

      // out_ << indent_ << datatools::i_tree_dumpable::tag
      //      << "Encoding type identifier : '" << _encoding_type_id_ << "'" << std::endl;

      // out_ << indent_ << datatools::i_tree_dumpable::tag
      //      << "Protocol type identifier : '" << _protocol_type_id_.to_string() << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Sent messages counter : [" << _sent_messages_counter_ << "]" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Received messages counter : [" << _received_messages_counter_ << "]" << std::endl;

      return;
    }

  } // namespace com

} // namespace vire
