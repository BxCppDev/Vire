//! \file vire/com/plug_factory.cc
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
#include <vire/com/domain.h>
#include <vire/com/manager.h>
#include <vire/com/i_service_client_plug.h>
// #include <vire/com/i_service_server_plug.h>
// #include <vire/com/i_event_emitter_plug.h>
// #include <vire/com/i_event_listener_plug.h>
// #include <vire/resource/manager.h>
#include <vire/rabbitmq/service_client_plug.h>
#include <vire/rabbitmq/event_emitter_plug.h>
#include <vire/rabbitmq/event_listener_plug.h>

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
      
      if (theActor.get_category() == ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
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
          = std::make_shared<vire::rabbitmq::service_client_plug>(plug_name, _parent_, domain_, mailbox_name);
        _parent_._plugs_[plug_name] = new_plug;
        // Return automatic plug name:
        plug_name_ = plug_name;
        return true;
      }
      
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
      actor & theActor = _parent_;
      DT_THROW_IF(theActor.has_plug(plug_name_),
                  std::logic_error,
                  "Actor '" << theActor.get_name() << "' already has a plug named '" << plug_name_ << "'!");
           
      const domain & theDomain = domain_;
      DT_THROW_IF(theDomain.get_transport_type_id().get_name() != "rabbitmq",
                  std::logic_error,
                  "Domain '" << theDomain.get_name() << "' can only support 'rabbitmq' transport type!");
           
      if (theDomain.get_category() == DOMAIN_CATEGORY_GATE) {
        return false;
      }
           
      if (theDomain.get_category() == DOMAIN_CATEGORY_CONTROL) {
        return false;
      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        std::string this_client_system_domain_name = domain_.get_name();
        if (theActor.get_category() == ACTOR_CATEGORY_CLIENT_SYSTEM) {
          std::string this_client_name = theActor.get_target();
          DT_THROW_IF(this_client_name != domain_.get_client_identifier(),
                      std::logic_error,
                      "Client system actor '" << theActor.get_name() << "' target '"
                      << this_client_name << "' does not match the domain '"
                      << domain_.get_client_identifier() << "'!");                  
          // Compute a random plug name if not specified:
          std::string plug_name = plug_name_;
          while (plug_name.empty()) {
            plug_name = _pimpl_->random_string(default_private_plug_prefix(), 8);
            if (_parent_.has_plug(plug_name)) {
              plug_name.clear();
            }
          }
          std::shared_ptr<i_event_listener_plug> new_plug
            = std::make_shared<vire::rabbitmq::event_listener_plug>(plug_name, _parent_, domain_);
          _parent_._plugs_[plug_name] = new_plug;
          // Return automatic plug name:
          plug_name_ = plug_name;
          return true;
        } 
      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_MONITORING) {
        std::string monitoring_domain_name = domain_.get_name();
 
        if (theActor.get_category() == ACTOR_CATEGORY_CLIENT_CMS
            || theActor.get_category() == ACTOR_CATEGORY_SERVER_CMS) {
          std::string plug_name = plug_name_;
          while (plug_name.empty()) {
            plug_name = _pimpl_->random_string(default_private_plug_prefix(), 8);
            if (_parent_.has_plug(plug_name)) {
              plug_name.clear();
            }
          } 
          std::shared_ptr<i_event_listener_plug> new_plug
            = std::make_shared<vire::rabbitmq::event_listener_plug>(plug_name,
                                                                    _parent_,
                                                                    domain_);
          _parent_._plugs_[plug_name] = new_plug;
          // Return automatic plug name:
          plug_name_ = plug_name;
          return true;
        }
      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        std::string this_subcontractor_system_domain_name = domain_.get_name();

        if (theActor.get_category() == ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
          std::string this_subcontractor_name = theActor.get_target();
          DT_THROW_IF(this_subcontractor_name != domain_.get_subcontractor_identifier(),
                      std::logic_error,
                      "Server subcontractor system actor '" << theActor.get_name() << "' target '"
                      << this_subcontractor_name << "' does not match the domain '" << domain_.get_subcontractor_identifier() << "'!");
          // Compute a random plug name if not specified:
          std::string plug_name = plug_name_;
          while (plug_name.empty()) {
            plug_name = _pimpl_->random_string(default_private_plug_prefix(), 8);
            if (_parent_.has_plug(plug_name)) {
              plug_name.clear();
            }
          } 
          std::shared_ptr<i_event_listener_plug> new_plug
            = std::make_shared<vire::rabbitmq::event_listener_plug>(plug_name, _parent_, domain_);
          _parent_._plugs_[plug_name] = new_plug;
          // Return automatic plug name:
          plug_name_ = plug_name;
          return true;
        }

        if (theActor.get_category() == ACTOR_CATEGORY_SUBCONTRACTOR) {
          std::string this_subcontractor_name = theActor.get_target();
          DT_THROW_IF(this_subcontractor_name != domain_.get_subcontractor_identifier(),
                      std::logic_error,
                      "Subcontractor actor '" << theActor.get_name() << "' target '"
                      << this_subcontractor_name << "' does not match the domain '" << domain_.get_subcontractor_identifier() << "'!");
          // Compute a random plug name if not specified:
          std::string plug_name = plug_name_;
          while (plug_name.empty()) {
            plug_name = _pimpl_->random_string(default_private_plug_prefix(), 8);
            if (_parent_.has_plug(plug_name)) {
              plug_name.clear();
            }
          } 
          std::shared_ptr<i_event_listener_plug> new_plug
            = std::make_shared<vire::rabbitmq::event_listener_plug>(plug_name, _parent_, domain_);
          _parent_._plugs_[plug_name] = new_plug;
          // Return automatic plug name:
          plug_name_ = plug_name;
          return true;     
        }

      }

      return false;
    }

    bool plug_factory::make_event_emitter_plug(const domain & domain_,
                                               std::string & plug_name_,
                                               const std::string & event_label_)
    {
      actor & theActor = _parent_;
      DT_THROW_IF(theActor.has_plug(plug_name_),
                  std::logic_error,
                  "Actor '" << theActor.get_name() << "' already has a plug named '" << plug_name_ << "'!");
      
      const domain & theDomain = domain_;
      DT_THROW_IF(theDomain.get_transport_type_id().get_name() != "rabbitmq",
                  std::logic_error,
                  "Domain '" << theDomain.get_name() << "' with transport type '"
                  << theDomain.get_transport_type_id().get_name()
                  << "' is not supported! Only support for the 'rabbitmq' transport type!");
           
      if (theDomain.get_category() == DOMAIN_CATEGORY_GATE) {
        return false;
      }
           
      if (theDomain.get_category() == DOMAIN_CATEGORY_CONTROL) {
        return false;
      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        std::string this_client_system_domain_name = domain_.get_name();

        if (theActor.get_category() == ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM) {
          DT_THROW_IF(! event_label_.empty() && event_label_ != "vireserver",
                      std::logic_error,
                      "Invalid event label '" << event_label_ << "'!");
          std::string this_client_name = theActor.get_target();
          DT_THROW_IF(this_client_name != domain_.get_client_identifier(),
                      std::logic_error,
                      "Server client system actor '" << theActor.get_name() << "' target '"
                      << this_client_name << "' does not match the domain '"
                      << domain_.get_client_identifier() << "'!");                  
          std::string default_mailbox_name = event_label_ + ".event";
          // Compute a random plug name if not specified:
          std::string plug_name = plug_name_;
          while (plug_name.empty()) {
            plug_name = _pimpl_->random_string(default_private_plug_prefix(), 8);
            if (_parent_.has_plug(plug_name)) {
              plug_name.clear();
            }
          } 
          std::shared_ptr<i_event_emitter_plug> new_plug
            = std::make_shared<vire::rabbitmq::event_emitter_plug>(plug_name, _parent_, domain_, default_mailbox_name);
          _parent_._plugs_[plug_name] = new_plug;
          // Return automatic plug name:
          plug_name_ = plug_name;
          return true;
        }

      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_MONITORING) {
        std::string monitoring_domain_name = domain_.get_name();

        if (theActor.get_category() == ACTOR_CATEGORY_SUBCONTRACTOR
            || theActor.get_category() == ACTOR_CATEGORY_SERVER_CMS) {
          std::string default_mailbox_name;
          if (! event_label_.empty()) {
            if (event_label_ == alarm_event_monitoring_label()) {
              default_mailbox_name = "alarm.event";
            } else if (event_label_ == log_event_monitoring_label()) {
              default_mailbox_name = "log.event";
            } else if (event_label_ == pubsub_event_monitoring_label()) {
              default_mailbox_name = "pubsub.event";
            } else {
              DT_THROW(std::logic_error,
                       "Unknown monitoring event label '" << event_label_ << "' in plug '!"); 
            }
          } else {
            default_mailbox_name = "log.event";
          }
          std::string plug_name = plug_name_;
          while (plug_name.empty()) {
            plug_name = _pimpl_->random_string(default_private_plug_prefix(), 8);
            if (_parent_.has_plug(plug_name)) {
              plug_name.clear();
            }
          } 
          std::shared_ptr<i_event_emitter_plug> new_plug
            = std::make_shared<vire::rabbitmq::event_emitter_plug>(plug_name,
                                                                   _parent_,
                                                                   domain_,
                                                                   default_mailbox_name);
          _parent_._plugs_[plug_name] = new_plug;
          // Return automatic plug name:
          plug_name_ = plug_name;
          return true;
        }
      }

      if (theDomain.get_category() == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        std::string this_subcontractor_system_domain_name = domain_.get_name();

        if (theActor.get_category() == ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
          DT_THROW_IF(! event_label_.empty() && event_label_ != "vireserver",
                      std::logic_error,
                      "Invalid event label '" << event_label_ << "'!");
          std::string this_subcontractor_name = theActor.get_target();
          DT_THROW_IF(this_subcontractor_name != domain_.get_subcontractor_identifier(),
                      std::logic_error,
                      "Server subcontractor system actor '" << theActor.get_name() << "' target '"
                      << this_subcontractor_name << "' does not match the domain '" << domain_.get_subcontractor_identifier() << "'!");                  
          std::string default_mailbox_name = "vireserver.event";
          // Compute a random plug name if not specified:
          std::string plug_name = plug_name_;
          while (plug_name.empty()) {
            plug_name = _pimpl_->random_string(default_private_plug_prefix(), 8);
            if (_parent_.has_plug(plug_name)) {
              plug_name.clear();
            }
          } 
          std::shared_ptr<i_event_emitter_plug> new_plug
            = std::make_shared<vire::rabbitmq::event_emitter_plug>(plug_name, _parent_, domain_, default_mailbox_name);
          _parent_._plugs_[plug_name] = new_plug;
          // Return automatic plug name:
          plug_name_ = plug_name;
          return true;
        }

        if (theActor.get_category() == ACTOR_CATEGORY_SUBCONTRACTOR) {
          DT_THROW_IF(! event_label_.empty() && event_label_ != "subcontractor",
                      std::logic_error,
                      "Invalid event label '" << event_label_ << "'!");
          std::string this_subcontractor_name = theActor.get_target();
          DT_THROW_IF(this_subcontractor_name != domain_.get_subcontractor_identifier(),
                      std::logic_error,
                      "Subcontractor actor '" << theActor.get_name() << "' target '"
                      << this_subcontractor_name << "' does not match the domain '" << domain_.get_subcontractor_identifier() << "'!");                  
          std::string default_mailbox_name = "subcontractor.event";
          // Compute a random plug name if not specified:
          std::string plug_name = plug_name_;
          while (plug_name.empty()) {
            plug_name = _pimpl_->random_string(default_private_plug_prefix(), 8);
            if (_parent_.has_plug(plug_name)) {
              plug_name.clear();
            }
          } 
          std::shared_ptr<i_event_emitter_plug> new_plug
            = std::make_shared<vire::rabbitmq::event_emitter_plug>(plug_name, _parent_, domain_, default_mailbox_name);
          _parent_._plugs_[plug_name] = new_plug;
          // Return automatic plug name:
          plug_name_ = plug_name;
          return true;
        }

      }

      return false;
    }
    
  } // namespace com

} // namespace vire
