//! \file vire/com/plug_factory.cc
//
// Copyright (c) 2018-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2018-2019 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/com/access_hub.h>
#include <vire/com/access_profile.h>
#include <vire/com/domain.h>
#include <vire/com/manager.h>
#include <vire/com/i_service_client_plug.h>
// #include <vire/com/i_service_server_plug.h>
// #include <vire/com/i_event_emitter_plug.h>
// #include <vire/com/i_event_listener_plug.h>
// #include <vire/resource/manager.h>
#include <vire/rabbitmq/event_emitter_plug.h>
#include <vire/rabbitmq/event_listener_plug.h>
#include <vire/rabbitmq/service_client_plug.h>
// #include <vire/rabbitmq/service_server_plug.h>

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
    const std::set<std::string> & plug_factory::supported_transport_driver_type_ids()
    {
      static std::set<std::string> _s;
      if (_s.empty()) {
        _s.insert("vire::rabbitmq::transport_driver");
      }
      return _s;
    }

    // Compute a random plug name if not specified:
    // while (plug_name.empty()) {
    //   plug_name = _pimpl_->random_string(default_private_plug_prefix(), 8);
    //   if (_parent_.has_plug(plug_name)) {
    //     plug_name.clear();
    //   }
    // } 
 
    // // static
    // const std::string & plug_factory::default_private_plug_prefix()
    // {
    //   static const std::string _p("__vire-com-plug.");
    //   return _p;
    // }
   
    plug_factory::plug_factory(const access_hub & parent_)
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

    const access_hub & plug_factory::get_parent() const
    {
      return _parent_;
    }

    // XXX
    std::shared_ptr<i_event_listener_plug>
    plug_factory::make_event_listener_plug(const std::string & domain_name_,
                                           const std::string & plug_name_,
                                           const std::string & /* event_name_ */)
    {
      std::shared_ptr<i_event_listener_plug> new_plug;
      const access_hub & theHub = _parent_;
      DT_THROW_IF(theHub.has_plug(domain_name_, plug_name_),
                  std::logic_error,
                  "Access hub '" << theHub.get_name() << "' already has a plug named '" << plug_name_ << "'!");       
      const domain & theDomain = theHub.get_domain(domain_name_);
      DT_THROW_IF(supported_transport_driver_type_ids().count(theDomain.get_transport_driver_type_id().get_name()) == 0,
                  std::logic_error,
                  "Domain '" << theDomain.get_name() << "' does not supports '" << theDomain.get_transport_driver_type_id().get_name() << "' transport type!");
      
      if (theDomain.get_category() == DOMAIN_CATEGORY_GATE) {
      }
           
      if (theDomain.get_category() == DOMAIN_CATEGORY_CONTROL) {
      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        std::string this_client_system_domain_name = theDomain.get_name();
        if (theHub.get_profile().get_category() == ACCESS_CATEGORY_CLIENT_SYSTEM) {
          std::string this_client_name = theHub.get_profile().get_target();
          DT_THROW_IF(this_client_name != theDomain.get_client_identifier(),
                      std::logic_error,
                      "Client system access hub '" << theHub.get_name() << "' target '"
                      << this_client_name << "' does not match the domain '"
                      << theDomain.get_client_identifier() << "'!");
          //
          //                            subscribe
          //  [vireserver.event] <--------------------- [event listener plug @ Vire client]
          // 
          new_plug = std::make_shared<vire::rabbitmq::event_listener_plug>(plug_name_,
                                                                           _parent_,
                                                                           theDomain);
        } 
      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_MONITORING) {
        std::string monitoring_domain_name = theDomain.get_name();
 
        if (theHub.get_profile().get_category() == ACCESS_CATEGORY_CLIENT_CMS
            || theHub.get_profile().get_category() == ACCESS_CATEGORY_SERVER_CMS) {
          //
          //                                  subscribe
          //  [alarm|log|pubsub.event] <--------------------- [event listener plug @ Vire client/server]
          // 
          new_plug = std::make_shared<vire::rabbitmq::event_listener_plug>(plug_name_,
                                                                           _parent_,
                                                                           theDomain);
        }
      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        std::string this_subcontractor_system_domain_name = theDomain.get_name();

        if (theHub.get_profile().get_category() == ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
          std::string this_subcontractor_name = theHub.get_profile().get_target();
          DT_THROW_IF(this_subcontractor_name != theDomain.get_subcontractor_identifier(),
                      std::logic_error,
                      "Server subcontractor system access hub '" << theHub.get_name() << "' target '"
                      << this_subcontractor_name << "' does not match the domain '" << theDomain.get_subcontractor_identifier() << "'!");
          //
          //                               subscribe
          //  [subcontractor.event] <--------------------- [event listener plug @ Vire server]
          // 
          new_plug = std::make_shared<vire::rabbitmq::event_listener_plug>(plug_name_, _parent_, theDomain);
        }

        if (theHub.get_profile().get_category() == ACCESS_CATEGORY_SUBCONTRACTOR) {
          std::string this_subcontractor_name = theHub.get_profile().get_target();
          DT_THROW_IF(this_subcontractor_name != theDomain.get_subcontractor_identifier(),
                      std::logic_error,
                      "Subcontractor access hub '" << theHub.get_name() << "' target '"
                      << this_subcontractor_name << "' does not match the domain '" << theDomain.get_subcontractor_identifier() << "'!");
         //
          //                             subscribe
          //  [vireserver.event] <--------------------- [event listener plug @ Vire subcontractor]
          // 
          new_plug = std::make_shared<vire::rabbitmq::event_listener_plug>(plug_name_, _parent_, theDomain);
        }

      }

      return new_plug;
    }

    std::shared_ptr<i_event_emitter_plug>
    plug_factory::make_event_emitter_plug(const std::string & domain_name_,
                                          const std::string & plug_name_,
                                          const std::string & event_label_)
    {
      std::shared_ptr<i_event_emitter_plug> new_plug;
      const access_hub & theHub = _parent_;
      DT_THROW_IF(theHub.has_plug(domain_name_,plug_name_),
                  std::logic_error,
                  "Access hub '" << theHub.get_name() << "' already has a plug named '" << plug_name_ << "'!");
      const domain & theDomain = theHub.get_domain(domain_name_);
      DT_THROW_IF(supported_transport_driver_type_ids().count(theDomain.get_transport_driver_type_id().get_name()) == 0,
                  std::logic_error,
                  "Domain '" << theDomain.get_name() << "' does not supports '" << theDomain.get_transport_driver_type_id().get_name() << "' transport type!");
      access_category_type access_category =  theHub.get_profile().get_category();
      if (theDomain.get_category() == DOMAIN_CATEGORY_GATE) {
      }
           
      if (theDomain.get_category() == DOMAIN_CATEGORY_CONTROL) {
      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        std::string this_client_system_domain_name = theDomain.get_name();

        if (access_category == ACCESS_CATEGORY_SERVER_CLIENT_SYSTEM) {
          DT_THROW_IF(! event_label_.empty() && event_label_ != "vireserver",
                      std::logic_error,
                      "Invalid event name '" << event_label_ << "'!");
          std::string this_client_name = theHub.get_profile().get_target();
          DT_THROW_IF(this_client_name != theDomain.get_client_identifier(),
                      std::logic_error,
                      "Server client system access hub '" << theHub.get_name() << "' target '"
                      << this_client_name << "' does not match the domain '"
                      << theDomain.get_client_identifier() << "'!");                  
          std::string default_mailbox_name = event_label_ + ".event";
          new_plug = std::make_shared<vire::rabbitmq::event_emitter_plug>(plug_name_,
                                                                          _parent_,
                                                                          theDomain,
                                                                          default_mailbox_name);
        }

      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_MONITORING) {
        std::string monitoring_domain_name = theDomain.get_name();

        if (access_category == ACCESS_CATEGORY_SUBCONTRACTOR
            || access_category == ACCESS_CATEGORY_SERVER_CMS) {
          std::string default_mailbox_name = "log.event";
          if (! event_label_.empty()) {
            if (event_label_ == alarm_event_monitoring_label()) {
              default_mailbox_name = "alarm.event";
            } else if (event_label_ == log_event_monitoring_label()) {
              default_mailbox_name = "log.event";
            } else if (event_label_ == pubsub_event_monitoring_label()) {
              default_mailbox_name = "pubsub.event";
            } else {
              DT_THROW(std::logic_error,
                       "Unknown monitoring event name '" << event_label_ << "' in plug '!"); 
            }
          } 
          new_plug = std::make_shared<vire::rabbitmq::event_emitter_plug>(plug_name_,
                                                                          _parent_,
                                                                          theDomain,
                                                                          default_mailbox_name);
        }
      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        std::string this_subcontractor_system_domain_name = theDomain.get_name();

        if (access_category == ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
          DT_THROW_IF(! event_label_.empty() && event_label_ != "vireserver",
                      std::logic_error,
                      "Invalid event name '" << event_label_ << "'!");
          std::string this_subcontractor_name = theHub.get_profile().get_target();
          DT_THROW_IF(this_subcontractor_name != theDomain.get_subcontractor_identifier(),
                      std::logic_error,
                      "Server subcontractor system access hub '" << theHub.get_name() << "' target '"
                      << this_subcontractor_name << "' does not match the domain '" << theDomain.get_subcontractor_identifier() << "'!");                  
          std::string default_mailbox_name = "vireserver.event";
          new_plug = std::make_shared<vire::rabbitmq::event_emitter_plug>(plug_name_,
                                                                          _parent_,
                                                                          theDomain,
                                                                          default_mailbox_name);
        }

        if (access_category == ACCESS_CATEGORY_SUBCONTRACTOR) {
          DT_THROW_IF(! event_label_.empty() && event_label_ != "subcontractor",
                      std::logic_error,
                      "Invalid event name '" << event_label_ << "'!");
          std::string this_subcontractor_name = theHub.get_profile().get_target();
          DT_THROW_IF(this_subcontractor_name != theDomain.get_subcontractor_identifier(),
                      std::logic_error,
                      "Subcontractor access hub '" << theHub.get_name() << "' target '"
                      << this_subcontractor_name << "' does not match the domain '" << theDomain.get_subcontractor_identifier() << "'!");                  
          std::string default_mailbox_name = "subcontractor.event";
          new_plug = std::make_shared<vire::rabbitmq::event_emitter_plug>(plug_name_,
                                                                          _parent_,
                                                                          theDomain,
                                                                          default_mailbox_name);
         }

      }

      return new_plug;
    }

    std::shared_ptr<i_service_client_plug>
    plug_factory::make_service_client_plug(const std::string & domain_name_,
                                           const std::string & plug_name_)
    {     
      std::shared_ptr<i_service_client_plug> new_plug;
      const access_hub & theHub = _parent_;
      DT_THROW_IF(theHub.has_plug(domain_name_, plug_name_),
                  std::logic_error,
                  "Actor '" << theHub.get_name() << "' already has a plug named '" << plug_name_ << "'!");
      const domain & theDomain = theHub.get_domain(domain_name_);
      DT_THROW_IF(supported_transport_driver_type_ids().count(theDomain.get_transport_driver_type_id().get_name()) == 0,
                  std::logic_error,
                  "Domain '" << theDomain.get_name() << "' does not supports '" << theDomain.get_transport_driver_type_id().get_name() << "' transport type!");
      // if (get_parent().get_profile().get_category()
      //     == ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
      //   std::string this_subcontractor_name = theHub.get_profile().get_target();
      //   std::string this_subcontractor_system_domain_name = theDomain.get_name();
      //   new_plug = std::make_shared<vire::rabbitmq::service_client_plug>(plug_name_,
      //                                                                    _parent_,
      //                                                                    theDomain);
      // }
      
      return new_plug;
    }
 
    std::shared_ptr<i_service_server_plug>
    plug_factory::make_service_server_plug(const std::string & domain_name_,
                                           const std::string & plug_name_)
    {
      std::shared_ptr<i_service_server_plug> new_plug;
      const access_hub & theHub = _parent_;
      DT_THROW_IF(theHub.has_plug(domain_name_, plug_name_),
                  std::logic_error,
                  "Actor '" << theHub.get_name() << "' already has a plug named '" << plug_name_ << "'!");
      const domain & theDomain = theHub.get_domain(domain_name_);
      DT_THROW_IF(supported_transport_driver_type_ids().count(theDomain.get_transport_driver_type_id().get_name()) == 0,
                  std::logic_error,
                  "Domain '" << theDomain.get_name() << "' does not supports '" << theDomain.get_transport_driver_type_id().get_name() << "' transport type!");

      return new_plug;
    }
   
  } // namespace com

} // namespace vire
