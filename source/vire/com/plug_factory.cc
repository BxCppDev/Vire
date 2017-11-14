//! \file vire/com/mailbox.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//                       Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/com/plug_factory.h>

// This project:
#include <vire/com/manager.h>
#include <vire/com/event_emitter_plug.h>
#include <vire/com/event_listener_plug.h>

namespace vire {

  namespace com {

    plug_factory::plug_factory(manager & mgr_)
      : _mgr_(mgr_)
    {
      return;
    }

    plug_factory::~plug_factory()
    {
      return;
    }

    void plug_factory::make_event_listener_plug(const std::string & domain_name_,
                                                const std::string & plug_name_)
    {
      DT_THROW_IF(_mgr_.has_domain(domain_name_), std::logic_error,
                  "No domain '" << domain_name_ << "'!");
      domain & dom = _mgr_.grab_domain(domain_name_);
      DT_THROW_IF(dom.has_plug(plug_name_), std::logic_error,
                  "Domain '" << domain_name_ << "' already has a plug '"
                  << plug_name_ << "'!");
      plug_ptr_type elp = _create_plug_(dom, plug_name_, PLUG_EVENT_LISTENER);
      dom.grab_plugs()[plug_name_] = elp;
      return;
    }

    void plug_factory::make_event_emitter_plug(const std::string & domain_name_,
                                               const std::string & plug_name_)
    {
      DT_THROW_IF(_mgr_.has_domain(domain_name_), std::logic_error,
                  "No domain '" << domain_name_ << "'!");
      domain & dom = _mgr_.grab_domain(domain_name_);
      DT_THROW_IF(dom.has_plug(plug_name_), std::logic_error,
                  "Domain '" << domain_name_ << "' already has a plug '"
                  << plug_name_ << "'!");
      plug_ptr_type elp = _create_plug_(dom, plug_name_, PLUG_EVENT_EMITTER);
      dom.grab_plugs()[plug_name_] = elp;
      return;
    }

    plug_ptr_type plug_factory::_create_plug_(domain & dom_,
                                              const std::string & plug_name_,
                                              const plug_category_type plug_cat_) const
    {
      std::shared_ptr<base_plug> plug_ptr;
      switch (plug_cat_) {
      PLUG_EVENT_EMITTER:
        plug_ptr.reset(new event_emitter_plug(dom_, plug_name_));
        break;
      PLUG_EVENT_LISTENER:
        plug_ptr.reset(new event_listener_plug(dom_, plug_name_));
        break;
      }
      return plug_ptr;
    }

  } // namespace com

} // namespace vire
