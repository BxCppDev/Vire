//! \file vire/com/actor.cc
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
#include <vire/com/actor.h>

// Standard library:
#include <sstream>

// This project:
#include <vire/com/manager.h>
#include <vire/com/plug_factory.h>
#include <vire/com/domain_builder.h>
#include <vire/com/domain.h>

namespace vire {

  namespace com {

    // static
    std::string actor::to_string(const category_type category_)
    {
      switch(category_) {
      case CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM: return "server-subcontractor-system";
      case CATEGORY_SERVER_CLIENT_SYSTEM: return "server-client-system";
      case CATEGORY_SERVER_GATE: return "server-gate";
      case CATEGORY_SERVER_CMS: return "server-cms";
      case CATEGORY_CLIENT_SYSTEM : return "client-system";
      case CATEGORY_CLIENT_CMS : return "client-cms";
      case CATEGORY_CLIENT_GATE : return "client-gate";
      case CATEGORY_SUBCONTRACTOR : return "subcontractor";
      default:
        break;
      }
      return "";
    }

    // static
    bool actor::from_string(const std::string & label_, category_type & category_)
    {
      category_ = CATEGORY_INVALID;
      if (label_ == to_string(CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM)) {
        category_ = CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM;
      }
      if (label_ == to_string(CATEGORY_SERVER_CLIENT_SYSTEM)) {
        category_ = CATEGORY_SERVER_CLIENT_SYSTEM;
      }
      if (label_ == to_string(CATEGORY_SERVER_GATE)) {
        category_ = CATEGORY_SERVER_GATE;
      }
      if (label_ == to_string(CATEGORY_SERVER_CMS)) {
        category_ = CATEGORY_SERVER_CMS;
      }
      if (label_ == to_string(CATEGORY_CLIENT_SYSTEM)) {
        category_ = CATEGORY_CLIENT_SYSTEM;
      }
      if (label_ == to_string(CATEGORY_CLIENT_CMS)) {
        category_ = CATEGORY_CLIENT_CMS;
      }
      if (label_ == to_string(CATEGORY_CLIENT_GATE)) {
        category_ = CATEGORY_CLIENT_GATE;
      }
      if (label_ == to_string(CATEGORY_SUBCONTRACTOR)) {
        category_ = CATEGORY_SUBCONTRACTOR;
      }
      return category_ != CATEGORY_INVALID;
    }

    // static
    bool actor::category_requires_target(const category_type category_)
    {
      switch(category_) {
      case CATEGORY_SERVER_GATE:
      case CATEGORY_SERVER_CMS:
      case CATEGORY_CLIENT_CMS:
      case CATEGORY_CLIENT_GATE:
        return false;
      }
      return true;
    }

    /*
    // static
    std::string actor::build_name(const category_type category_,
                                  const std::string & setup_name_,
                                  const std::string & id_)
    {
      std::ostringstream out;
      if (!setup_name_.empty()) {
        out << setup_name_ << '.';
      }
      out << "vire";
      if (category_ == CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
        out << ".server" << ".SUBCONTRACTOR_sys";
      } else if (category_ == CATEGORY_SERVER_CLIENT_SYSTEM) {
        out << ".server" << ".scsys";
      } if (category_ == CATEGORY_CLIENT) {
        out << ".client";
      } else if (category_ == CATEGORY_SUBCONTRACTOR) {
        out << ".subcontractor";
      } else if (category_ == CATEGORY_SYSTEM) {
        out << ".system";
      }
      if (!id_.empty()) {
        out << '.' << id_;
      }
      return out.str();
    }
    */
    
    // actor::actor()
    // {
    //   return;
    // }

    actor::actor(const manager & com_,
                 const category_type category_,
                 const std::string & target_,
                 const std::string & name_,
                 const std::string & password_)
    {
      _com_ = &com_;
      set_name(name_);
      set_password(password_);
      set_category(category_);
      if (!target_.empty()) { 
        set_target(target_);
      }
      DT_THROW_IF(!target_.empty() && !category_requires_target(_category_),
                  std::logic_error,
                  "Unused target for actor '" << _name_
                  << "' with category '" << to_string(_category_) << "'!");
      DT_THROW_IF(!has_target() && category_requires_target(_category_),
                  std::logic_error,
                  "Missing target (client/subcontractor ID) for actor '" << _name_
                  << "' with category '" << to_string(_category_) << "'!");
      _build_domains_indexation_();
      _build_default_plugs_();
      lock();
      return;
    }

    actor::~actor()
    {
      return;
    }

    const manager & actor::get_com() const
    {
      return *_com_;
    }

    bool actor::is_valid() const
    {
      if (!has_name()) return false;
      if (!has_category()) return false;
      if (category_requires_target(_category_) && !has_target()) return false;
      return true;
    }

    bool actor::has_category() const
    {
      return _category_ != CATEGORY_INVALID;
    }

    void actor::set_category(const category_type & category_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Actor is locked!");
      _category_ = category_;
      return;
    }

    actor::category_type actor::get_category() const
    {
      return _category_;
    }

    bool actor::has_name() const
    {
      return !_name_.empty();
    }

    void actor::set_name(const std::string & name_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Actor is locked!");
      _name_ = name_;
      return;
    }

    const std::string & actor::get_name() const
    {
      return _name_;
    }

    bool actor::has_password() const
    {
      return !_password_.empty();
    }

    void actor::set_password(const std::string & password_)
    {
      _password_ = password_;
      return;
    }

    const std::string & actor::get_password() const
    {
      return _password_;
    }

    bool actor::match_password(const std::string & password_) const
    {
      return _password_ == password_;
    }

    bool actor::has_target() const
    {
      return !_target_.empty();
    }

    void actor::set_target(const std::string & target_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Actor is locked!");
      _target_ = target_;
      return;
    }

    const std::string & actor::get_target() const
    {
      return _target_;
    }

    datatools::properties & actor::grab_metadata()
    {
      return _metadata_;
    }

    const datatools::properties & actor::get_metadata() const
    {
      return _metadata_;
    }
     
    bool actor::has_plug(const std::string & plug_name_) const
    {
      return _plugs_.count(plug_name_);
    }

    bool actor::add_plug(const std::string & domain_label_,
                         const plug_category_type plug_category_,
                         std::string & plug_name_)
    {

      // DT_THROW_IF(!get_com().has_domain(domain_name_),
      //                  std::logic_error,
      //                  "No 

      if (!has_domain(domain_label_)) {
        return false;
      }
      
      switch (plug_category_) {
      case PLUG_EVENT_EMITTER :
        
        break;
      case PLUG_EVENT_LISTENER :
        
        break;
      case PLUG_SERVICE_CLIENT :
        return grab_plug_factory().make_service_client_plug(get_domain(domain_label_), plug_name_);
        break;
      case PLUG_SERVICE_SERVER :
        return grab_plug_factory().make_service_client_plug(get_domain(domain_label_), plug_name_);  
        break;
      }
      return false;
    }
  
    void actor::remove_plug(const std::string & plug_name_)
    {
      DT_THROW_IF(!has_plug(plug_name_),
                  std::logic_error,
                  "No plug named '" << plug_name_ << "'!");
      _plugs_.erase(plug_name_);
      return;
    }

    const std::shared_ptr<base_plug> & actor::get_plug(const std::string & plug_name_) const
    {
      plug_dict_type::const_iterator found = _plugs_.find(plug_name_);
      DT_THROW_IF(found == _plugs_.end(),
                  std::logic_error,
                  "No plug named '" << plug_name_ << "'!");
      return found->second;
    }
    
    const actor::plug_dict_type & actor::get_plugs() const
    {
      return _plugs_;
    }
 
    plug_factory & actor::grab_plug_factory()
    {
      if (_plug_factory_.get() == nullptr) {
        _plug_factory_.reset(new plug_factory(*this));
      }
      return *_plug_factory_;
    }
           
    void actor::_build_domains_indexation_()
    {
      const std::string & domain_name_prefix = this->get_com().get_domain_name_prefix();
      
      if (_category_ == CATEGORY_CLIENT_GATE || _category_ == CATEGORY_SERVER_GATE) {
        std::string gate_domain_name = domain_builder::build_cms_clients_gate_name(domain_name_prefix);
        _domains_["gate"] = &_com_->get_domain(gate_domain_name);
        return;
      }

      if (_category_ == CATEGORY_SERVER_CLIENT_SYSTEM || _category_ == CATEGORY_CLIENT_SYSTEM) {
        std::string client_name = get_target();
        std::string client_system_domain_name
          = domain_builder::build_cms_client_system_name(domain_name_prefix, client_name);
        _domains_["system"] = &_com_->get_domain(client_system_domain_name);
        return;
      }
 
      if (_category_ == CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM || _category_ == CATEGORY_SUBCONTRACTOR) {
        std::string subcontractor_name = get_target();
        std::string subcontractor_system_domain_name
          = domain_builder::build_cms_subcontractor_system_name(domain_name_prefix, subcontractor_name);
        _domains_["system"] = &_com_->get_domain(subcontractor_system_domain_name);
      }
 
      if (_category_ == CATEGORY_SERVER_CMS || _category_ == CATEGORY_CLIENT_CMS || _category_ == CATEGORY_SUBCONTRACTOR) {
        std::string monitoring_domain_name = domain_builder::build_cms_monitoring_name(domain_name_prefix);
        _domains_["monitoring"] = &_com_->get_domain(monitoring_domain_name);
      }
 
      if (_category_ == CATEGORY_SERVER_CMS || _category_ == CATEGORY_CLIENT_CMS) {
        std::string control_domain_name = domain_builder::build_cms_control_name(domain_name_prefix);
        _domains_["control"] = &_com_->get_domain(control_domain_name);
      }
      
      return;
    }
 
    bool actor::has_domain(const std::string & domain_label_) const
    {
      return _domains_.count(domain_label_);
    }
        
    const domain & actor::get_domain(const std::string & domain_label_) const
    {
      domain_dict_type::const_iterator found = _domains_.find(domain_label_);
      DT_THROW_IF(found == _domains_.end(),
                  std::logic_error,
                  "No domain labelled '" << domain_label_ << "'");
      return *found->second;
    }
         
    void actor::_build_default_plugs_()
    {
      plug_factory & factory = this->grab_plug_factory();
      
      if (_category_ == CATEGORY_SERVER_GATE) {
        std::string plug_name = "gate.service.server";
        factory.make_service_server_plug(get_domain("gate"), plug_name);
      }

      if (_category_ == CATEGORY_CLIENT_GATE) {
        std::string plug_name = "gate.service.client";
        factory.make_service_client_plug(get_domain("gate"), plug_name);
      }

      if (_category_ == CATEGORY_SERVER_CLIENT_SYSTEM) {
        {
          std::string plug_name = "vireserver.service.server";
          factory.make_service_server_plug(get_domain("system"), plug_name);
        }
        {
          std::string plug_name = "vireserver.event.emitter";
          factory.make_event_emitter_plug(get_domain("system"), plug_name);
        }
      }
  
      if (_category_ == CATEGORY_CLIENT_SYSTEM) {
        {
          std::string plug_name = "vireserver.service.client";
          factory.make_service_client_plug(get_domain("system"), plug_name);
        }
        {
          std::string plug_name = "vireserver.event.listener";
          factory.make_event_listener_plug(get_domain("system"), plug_name);
        }
      }
  
      if (_category_ == CATEGORY_CLIENT_CMS) {
        {
          std::string plug_name = "control.service.server";
          factory.make_service_server_plug(get_domain("control"), plug_name);
        }
        {
          std::string plug_name = "monitoring.service.client";
          factory.make_service_client_plug(get_domain("monitoring"), plug_name);
        }
        {
          std::string plug_name = "monitoring.event.listener";
          factory.make_event_listener_plug(get_domain("monitoring"), plug_name);
        }
      }
 
      if (_category_ == CATEGORY_SERVER_CMS) {  
        {
          std::string plug_name = "control.service.client";
          factory.make_service_client_plug(get_domain("control"), plug_name);
        }
        { 
          std::string plug_name = "monitoring.service.client";
          factory.make_service_client_plug(get_domain("monitoring"), plug_name);
        }
        { 
          std::string plug_name = "monitoring.service.server";
          factory.make_service_server_plug(get_domain("monitoring"), plug_name);
        }
        {
          std::string plug_name = "monitoring.event.emitter";
          factory.make_event_emitter_plug(get_domain("monitoring"), plug_name);
        }
        {
          std::string plug_name = "monitoring.event.listener";
          factory.make_event_listener_plug(get_domain("monitoring"), plug_name);
        }
      }

      if (_category_ == CATEGORY_SUBCONTRACTOR) {
        {
          std::string plug_name = "monitoring.service.server";
          factory.make_service_server_plug(get_domain("monitoring"), plug_name);
        }
        {
          std::string plug_name = "monitoring.event.emitter";
          factory.make_event_emitter_plug(get_domain("monitoring"), plug_name);       
        }    
        {
          std::string plug_name = "vireserver.service.client";
          factory.make_service_client_plug(get_domain("system"), plug_name);
        }
        {
          std::string plug_name = "vireserver.event.listener";
          factory.make_event_listener_plug(get_domain("system"), plug_name);
        }
        {
          std::string plug_name = "subcontractor.service.server";
          factory.make_service_server_plug(get_domain("system"), plug_name);
        }
        {
          std::string plug_name = "subcontractor.event.emitter";
          factory.make_event_emitter_plug(get_domain("system"), plug_name);
        }
      }

      if (_category_ == CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
        {
          std::string plug_name = "vireserver.service.server";
          factory.make_service_server_plug(get_domain("system"), plug_name);
        }
        {
          std::string plug_name = "vireserver.event.emitter";
          factory.make_event_emitter_plug(get_domain("system"), plug_name);
        }
        {
          std::string plug_name = "subcontractor.service.client";
          factory.make_service_client_plug(get_domain("system"), plug_name);
        }
        {
          std::string plug_name = "subcontractor.event.listener";
          factory.make_event_listener_plug(get_domain("system"), plug_name);
        }
      }

      return;
    }

    bool actor::is_locked() const
    {
      return _locked_;
    }

    void actor::lock()
    {
      _locked_ = true;
      return;
    }
                  
    void actor::tree_dump(std::ostream & out_,
                         const std::string & title_,
                         const std::string & indent_,
                         bool inherit_) const
    {
      if (!title_.empty()) {
        out_ << indent_ << title_ << std::endl;
      }

      // out_ << indent_ << datatools::i_tree_dumpable::tag
      //      << "Validity : " << std::boolalpha << is_valid() << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Category : '" << to_string(_category_) << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Name     : '" << _name_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Password : '" << _password_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Target   : '" << _target_ << "'" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Metadata : [" << _metadata_.size() << "]" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Domains  : [" << _domains_.size() << "]" << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Plugs    : [" << _plugs_.size() << "]" << std::endl;

      return;
    }

  } // namespace com

} // namespace vire
