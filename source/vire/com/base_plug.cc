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

    std::string plug_category_to_label(const plug_category_type cat_)
    {
      switch(cat_) {
      case PLUG_EVENT_EMITTER: return std::string("event_emitter");
      case PLUG_EVENT_LISTENER: return std::string("event_listener");
      case PLUG_SERVICE_CLIENT: return std::string("service_client");
      case PLUG_SERVICE_SERVER: return std::string("service_server");
      default: return std::string();
      }
    }

    base_plug::base_plug()
    {
      return;
    }

    base_plug::base_plug(domain & dom_,
                         const std::string & name_,
                         const plug_category_type category_)
    {
      set_domain(dom_);
      set_name(name_);
      set_category(category_);
      return;
    }

    base_plug::~base_plug()
    {
      if (is_initialized()) {
        DT_THROW(std::logic_error, "Plug has not been properly reset!");
      }
      return;
    }

    bool base_plug::is_valid() const
    {
      if (!has_domain()) return false;
      if (!has_name()) return false;
      if (!has_category()) return false;
      return true;
    }

    bool base_plug::has_domain() const
    {
      return _dom_ != nullptr;
    }

    void base_plug::set_domain(domain & dom_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Plug is already initialized!");
      _dom_ = &dom_;
      return;
    }

    const domain & base_plug::get_domain() const
    {
      return *_dom_;
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

    bool base_plug::has_category() const
    {
      return _category_ != PLUG_INVALID;
    }

    void base_plug::set_category(const plug_category_type & category_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Plug is already initialized!");
      _category_ = category_;
      return;
    }

    plug_category_type base_plug::get_category() const
    {
      return _category_;
    }

    bool base_plug::is_initialized() const
    {
      return _initialized_;
    }

    void base_plug::_set_initialized(bool i_)
    {
      _initialized_ = i_;
      return;
    }

    void base_plug::_base_init()
    {
      DT_THROW_IF(!has_domain(), std::logic_error,
                  "Plug has no domain!");
      DT_THROW_IF(!has_name(), std::logic_error,
                  "Plug has no name!");
      DT_THROW_IF(!has_category(), std::logic_error,
                  "Plug '" << get_name() << "' has no category!");
      return;
    }

    void base_plug::_base_reset()
    {
      _initialized_ = false;
      _category_ = PLUG_INVALID;
      _name_.clear();
      _dom_ = nullptr;
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
           << "Domain : '" << _dom_->get_name() << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name : '" << _name_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Category : '" << _category_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Initialized : " << std::boolalpha << is_initialized() << std::endl;

      return;
    }

  } // namespace com

} // namespace vire
