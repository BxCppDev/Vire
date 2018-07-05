//! \file vire/com/plug_factory_factorymailbox.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Standard library:
#include <string>
#include <random>
#include <algorithm>

// This project:
#include <vire/com/actor.h>
#include <vire/com/manager.h>
#include <vire/com/i_service_client_plug.h>
// #include <vire/com/i_service_server_plug.h>
// #include <vire/com/i_event_emitter_plug.h>
// #include <vire/com/i_event_listener_plug.h>
// #include <vire/resource/manager.h>
#include <vire/com/rabbitmq_service_client_plug.h>

namespace {
  std::string const default_random_chars = 
    "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
}

namespace vire {

  namespace com {

    struct plug_factory::pimpl_type
    {
      pimpl_type()
      {
        return;
      }
      
      std::string random_string(const std::string & prefix_ = "",
                                size_t length_ = 8,
                                const std::string & allowed_chars_ = default_random_chars)
      {
        std::mt19937_64 gen { std::random_device()() };
        std::uniform_int_distribution<size_t> dist { 0, allowed_chars_.length()-1 };
        std::string output(prefix_);
        std::generate_n(std::back_inserter(output),
                        length_,
                        [&] { return allowed_chars_[dist(gen)]; });
        return output;
      }
      
    };

    // static
    const std::string & plug_factory::default_private_plug_prefix()
    {
      static const std::string _p("__vire-com-plug.");
      return _p;
    }
   
    plug_factory::plug_factory(actor & parent_)
      : _parent_(parent_)
    {
      _pimpl_.reset(new pimpl_type);
      return;
    }

    plug_factory::~plug_factory()
    {
      _pimpl_.reset();
      return;
    }

    const actor & plug_factory::get_parent() const
    {
      return _parent_;
    }

    bool
    plug_factory::make_service_client_plug(const domain & domain_,
                                           std::string & plug_name_)
    {
      
      actor & theActor = _parent_;
      DT_THROW_IF(theActor.has_plug(plug_name_),
                  std::logic_error,
                  "Actor '" << theActor.get_name() << "' already has a plug named '" << plug_name_ << "'!");
      
      const domain & theDomain = domain_;
      DT_THROW_IF(theDomain.get_transport_type_id().get_name() != "rabbitmq",
                  std::logic_error,
                  "Domain '" << theDomain.get_name() << "' can only support 'rabbitmq' transport type!");
      
      if (theActor.get_category() == actor::CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
        std::string this_subcontractor_name = theActor.get_target();
        std::string this_subcontractor_system_domain_name = domain_.get_name();
        std::string mailbox_name = "subcontractor.service";
        // Compute a random plug name if not specified:
        std::string plug_name = plug_name_;
        while (plug_name.empty()) {
          plug_name = _pimpl_->random_string(default_private_plug_prefix(), 8);
          if (_parent_.has_plug(plug_name)) {
            plug_name.clear();
          }
        } 
        std::shared_ptr<i_service_client_plug> new_plug
          = std::make_shared<rabbitmq_service_client_plug>(plug_name, _parent_, domain_, mailbox_name);
        _parent_._plugs_[plug_name] = new_plug;
        // Return automatic plug name:
        plug_name_ = plug_name;
        return true;
      }
 
      /*
      if (actor_.get_category() == actor::CATEGORY_CLIENT) {
        std::string this_client_name = _com_mgr_.get_actor().get_name();
        std::string this_client_system_domain_name
          = domain_builder::build_cms_client_system_name(_com_mgr_.get_domain_name_prefix(),
                                                         this_client_name);
        if (this_client_system_domain_name != domain_name_) {
          return new_plug;
        }
      }
      auto encoder = std::make_shared<protobuf_encoding_driver>();
      */
      
      plug_name_.clear();
      return false;
    }

    bool plug_factory::make_service_server_plug(const domain & domain_,
                                                std::string & plug_name_)
    {
      
      return false;
    }

    bool plug_factory::make_event_listener_plug(const domain & domain_,
                                                std::string & plug_name_)
    {
      
      return false;
    }

    bool plug_factory::make_event_emitter_plug(const domain & domain_,
                                               std::string & plug_name_)
    {
      
      return false;
    }
    
  } // namespace com

} // namespace vire
