//! \file vire/com/access_hub.cc
//
// Copyright (c) 2017-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2017-2019 by Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/com/access_hub.h>

// Standard library:
#include <sstream>

// This project:
#include <vire/com/manager.h>
#include <vire/com/plug_factory.h>
#include <vire/com/domain_builder.h>
#include <vire/com/domain.h>
#include <vire/com/access_profile.h>

namespace vire {

  namespace com {
    
    access_hub::access_hub(const access_profile & profile_,
                 const std::string & name_,
                 const std::set<std::string> & domain_names_,
                 const bool lock_)
    {
      _profile_ = &profile_;
      _set_name_(name_);    
      _init_domains_(domain_names_);
      _plug_factory_.reset(new plug_factory(*this));
      _build_default_plugs_();
      if (lock_) {
        _plug_factory_.reset();
      }
      return;
    }

    access_hub::~access_hub()
    {
      return;
    }

    void access_hub::_init_domains_(const std::set<std::string> & domain_names_)
    {
      for (const std::string & domain_name : domain_names_) {
        domain_entry de;
        de.dom = &_profile_->get_domain(domain_name);
        _domains_with_plugs_[domain_name] = de;
      }
      return;
    }
    
    const access_profile & access_hub::get_profile() const
    {
      return *_profile_;
    }

    void access_hub::_set_name_(const std::string & name_)
    {
      DT_THROW_IF(!datatools::name_validation(name_,
                                              datatools::NV_INSTANCE | datatools::NV_NO_DOT),
                  std::logic_error,
                  "Invalid access hub name '" << name_ << "'!");
      
      _name_ = name_;
      return;
    }

    const std::string & access_hub::get_name() const
    {
      return _name_;
    }
        
    bool access_hub::has_domain(const std::string & local_domain_name_) const
    {
      return _domains_with_plugs_.count(local_domain_name_) == 1;
    }

    bool access_hub::has_plug(const std::string & local_domain_name_,
                         const std::string & plug_name_) const
    {
      domain_dict_type::const_iterator found =
        _domains_with_plugs_.find(local_domain_name_);
      if (found == _domains_with_plugs_.end()) {
        return false;
      }
      plug_dict_type::const_iterator found2 = found->second.plugs.find(plug_name_);
      if (found2 == found->second.plugs.end()) {
        return false;
      }
      return true;
    }

    bool access_hub::is_removable_plug(const std::string & local_domain_name_,
                                  const std::string & plug_name_) const
    {
      DT_THROW_IF(is_locked(), std::logic_error,  "Access hub is locked!");
      domain_dict_type::const_iterator found =
        _domains_with_plugs_.find(local_domain_name_);
      DT_THROW_IF(found == _domains_with_plugs_.end(),
                  std::logic_error,
                  "Access hub has no access to domain '" << local_domain_name_ << "'!");
      const plug_dict_type & plugs = found->second.plugs;
      plug_dict_type::const_iterator found_plug = plugs.find(plug_name_);
      DT_THROW_IF(found_plug == plugs.end(),
                  std::logic_error,
                  "Access hub has no plug named '" << plug_name_ << "' associated to domain '" << local_domain_name_ << "'!");
      const plug_entry & pe = found_plug->second;
      return pe.removable;
     }

    void access_hub::destroy_plug(const std::string & local_domain_name_,
                             const std::string & plug_name_)
    {
      DT_THROW_IF(is_locked(), std::logic_error,  "Access hub is locked!");
      domain_dict_type::iterator found =
        _domains_with_plugs_.find(local_domain_name_);
      DT_THROW_IF(found == _domains_with_plugs_.end(),
                  std::logic_error,
                  "Access hub has no access to domain '" << local_domain_name_ << "'!");
      plug_dict_type & plugs = found->second.plugs;
      plug_dict_type::iterator found_plug = plugs.find(plug_name_);
      DT_THROW_IF(found_plug == plugs.end(),
                  std::logic_error,
                  "Access hub has no plug named '" << plug_name_ << "' associated to domain '" << local_domain_name_ << "'!");
      plug_entry & pe = found_plug->second;
      DT_THROW_IF(!pe.removable, std::logic_error,
                  "Plug named '" << plug_name_ << "' associated to domain '" << local_domain_name_ << "' is not removable!");
      plugs.erase(found_plug);
      return;
    }
    
    const std::shared_ptr<base_plug> & 
    access_hub::get_plug(const std::string & local_domain_name_,
                      const std::string & plug_name_) const
    {
      domain_dict_type::const_iterator found =
        _domains_with_plugs_.find(local_domain_name_);
      DT_THROW_IF(found == _domains_with_plugs_.end(),
                  std::logic_error,
                  "Access hub has no access to domain '" << local_domain_name_ << "'!");
      const plug_dict_type & plugs = found->second.plugs;
      plug_dict_type::const_iterator found_plug = plugs.find(plug_name_);
      DT_THROW_IF(found_plug == plugs.end(),
                  std::logic_error,
                  "Access hub has no plug named '" << plug_name_ << "' associated to domain '" << local_domain_name_ << "'!");
      const plug_entry & pe = found_plug->second;
      return pe.plug_handle;
    }
    
    std::shared_ptr<base_plug> & 
    access_hub::grab_plug(const std::string & local_domain_name_,
                     const std::string & plug_name_)
    {
      domain_dict_type::iterator found =
        _domains_with_plugs_.find(local_domain_name_);
      DT_THROW_IF(found == _domains_with_plugs_.end(),
                  std::logic_error,
                  "Access hub has no access to domain '" << local_domain_name_ << "'!");
      plug_dict_type & plugs = found->second.plugs;
      plug_dict_type::iterator found_plug = plugs.find(plug_name_);
      DT_THROW_IF(found_plug == plugs.end(),
                  std::logic_error,
                  "Access hub has no plug named '" << plug_name_ << "' associated to domain '" << local_domain_name_ << "'!");
      plug_entry & pe = found_plug->second;
      return pe.plug_handle;
    }
     
    void access_hub::create_plug(const std::string & local_domain_name_,
                            const std::string & plug_name_,
                            const plug_category_type plug_category_,
                            const bool removable_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Access hub '" << _name_ << "' is locked!");
      DT_THROW_IF(plug_name_.empty(), std::logic_error, "Missing new plug name for access hub '" << _name_ << "'!");
      DT_THROW_IF(plug_category_ == PLUG_INVALID, std::logic_error, "Invalid new plug category for access hub '" << _name_ << "'!");
      domain_dict_type::iterator found =
        _domains_with_plugs_.find(local_domain_name_);
      DT_THROW_IF(found == _domains_with_plugs_.end(),
                  std::logic_error,
                  "Access hub '" << _name_ << "' has no access to domain '" << local_domain_name_ << "'!");
      plug_dict_type & plugs = found->second.plugs;
      DT_THROW_IF(plugs.count(plug_name_),
                  std::logic_error,
                  "Access hub '" << _name_ << "' already has a plug named '" << plug_name_
                  << "' associated to domain '" << local_domain_name_ << "'!");    
      {
        std::string plug_name = plug_name_;
        plug_entry pe;
        pe.removable = removable_;
        if (plug_category_ == PLUG_SERVICE_CLIENT) {
          pe.plug_handle = _plug_factory_->make_service_client_plug(local_domain_name_,
                                                                    plug_name_);
        } else if (plug_category_ == PLUG_SERVICE_SERVER) {
          pe.plug_handle = _plug_factory_->make_service_server_plug(local_domain_name_,
                                                                    plug_name_);
        } else if (plug_category_ == PLUG_EVENT_EMITTER) {
          pe.plug_handle = _plug_factory_->make_event_emitter_plug(local_domain_name_,
                                                                   plug_name_);
        } else if (plug_category_ == PLUG_EVENT_LISTENER) {
          pe.plug_handle = _plug_factory_->make_event_listener_plug(local_domain_name_,
                                                                    plug_name_);
        }
        if (pe.plug_handle.get() != nullptr) {
          plugs[plug_name] = pe;
        }
      }
      return;
    }

    const domain & access_hub::get_domain(const std::string & local_domain_name_) const
    {
      domain_dict_type::const_iterator found = _domains_with_plugs_.find(local_domain_name_);
      DT_THROW_IF(found == _domains_with_plugs_.end(),
                  std::logic_error,
                  "No domain named '" << local_domain_name_ << "'");
      return *found->second.dom;
    }
    
    void access_hub::build_list_of_domain_local_names(std::set<std::string> & domain_names_) const
    {
      domain_names_.clear();
      for (const auto & p : _domains_with_plugs_) {
        domain_names_.insert(p.first);
      }
      return;
    }
 
    void access_hub::_build_default_plugs_()
    {
      plug_factory & factory = *this->_plug_factory_.get();
      access_category_type category = _profile_->get_category();

      if (_profile_->get_category() == ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
        // System domain:
        create_plug(domain_system_label(),
                    "subcontractor.service.client",
                    PLUG_SERVICE_CLIENT,
                    false);
        create_plug(domain_system_label(),
                    "vireserver.service.server",
                    PLUG_SERVICE_SERVER,
                    false);
        create_plug(domain_system_label(),
                    "vireserver.event.emitter",
                    PLUG_EVENT_EMITTER,
                    false);
        create_plug(domain_system_label(),
                    "subcontractor.event.listener",
                    PLUG_EVENT_LISTENER,
                    false);
      }
 
      if (_profile_->get_category() == ACCESS_CATEGORY_SUBCONTRACTOR) {
        // System domain:
        create_plug(domain_system_label(),
                    "vireserver.service.client",
                    PLUG_SERVICE_CLIENT,
                    false);
        create_plug(domain_system_label(),
                    "subcontractor.service.server",
                    PLUG_SERVICE_SERVER,
                    false);
        create_plug(domain_system_label(),
                    "subcontractor.event.emitter",
                    PLUG_EVENT_EMITTER,
                    false);
        create_plug(domain_system_label(),
                    "vireserver.event.listener",
                    PLUG_EVENT_LISTENER,
                    false);
        // Monitoring domain:
        create_plug(domain_monitoring_label(),
                    "resource_request.service.server",
                    PLUG_SERVICE_SERVER,
                    false);
        create_plug(domain_monitoring_label(),
                    "event.emitter",
                    PLUG_EVENT_EMITTER,
                    false);
      }

      if (_profile_->get_category() == ACCESS_CATEGORY_SERVER_GATE) {
        // Gate domain:
        create_plug(domain_gate_label(),
                    "gate.service.server",
                    PLUG_SERVICE_SERVER,
                    false);
      }

      if (_profile_->get_category() == ACCESS_CATEGORY_CLIENT_GATE) {
        // Gate domain:
        create_plug(domain_gate_label(),
                    "gate.service.client",
                    PLUG_SERVICE_CLIENT,
                    false);
      }

      /*
      if (_profile_->get_category() == ACCESS_CATEGORY_SERVER_CLIENT_SYSTEM) {
        {
          std::string plug_name = "vireserver.service.server";
          factory.make_service_server_plug(get_domain(domain_system_name()), plug_name);
        }
        {
          std::string plug_name = "vireserver.event.emitter";
          factory.make_event_emitter_plug(get_domain(domain_system_name()), plug_name);
        }
      }
  
      if (_profile_->get_category() == ACCESS_CATEGORY_CLIENT_SYSTEM) {
        {
          std::string plug_name = "vireserver.service.client";
          factory.make_service_client_plug(get_domain(domain_system_name()), plug_name);
        }
        {
          std::string plug_name = "vireserver.event.listener";
          factory.make_event_listener_plug(get_domain(domain_system_name()), plug_name);
        }
      }
  
      if (_profile_->get_category() == ACCESS_CATEGORY_CLIENT_CMS) {
        {
          std::string plug_name = "control.service.server";
          factory.make_service_server_plug(get_domain(domain_control_name()), plug_name);
        }
        {
          std::string plug_name = "monitoring.service.client";
          factory.make_service_client_plug(get_domain(domain_monitoring_name()), plug_name);
        }
        {
          std::string plug_name = "monitoring.event.listener";
          factory.make_event_listener_plug(get_domain(domain_monitoring_name()), plug_name);
        }
      }
 
      if (_profile_->get_category() == ACCESS_CATEGORY_SERVER_CMS) {  
        {
          std::string plug_name = "control.service.client";
          factory.make_service_client_plug(get_domain(domain_control_name()), plug_name);
        }
        { 
          std::string plug_name = "monitoring.service.client";
          factory.make_service_client_plug(get_domain(domain_monitoring_name()), plug_name);
        }
        { 
          std::string plug_name = "monitoring.service.server";
          factory.make_service_server_plug(get_domain(domain_monitoring_name()), plug_name);
        }
        {
          std::string plug_name = "monitoring.event.emitter";
          factory.make_event_emitter_plug(get_domain(domain_monitoring_name()), plug_name);
        }
        {
          std::string plug_name = "monitoring.event.listener";
          factory.make_event_listener_plug(get_domain(domain_monitoring_name()), plug_name);
        }
      }

     */

      return;
    }
    
    bool access_hub::is_locked() const
    {
      return _plug_factory_.get() == nullptr;
    }
                    
    void access_hub::print_tree(std::ostream & out_,
                           const boost::property_tree::ptree & options_) const
    {
      base_print_options popts;
      popts.configure_from(options_);
      std::ostringstream outs;
      if (!popts.title.empty()) {
        outs << popts.indent << popts.title << std::endl;
      }

      outs << popts.indent << tag
           << "Profile  : [@" << _profile_ << ']' << std::endl;

      outs << popts.indent << tag
           << "Name     : '" << _name_ << "'" << std::endl;

      outs << popts.indent << tag
           << "Domains with plugs : [" << _domains_with_plugs_.size() << "]" << std::endl;
      for (domain_dict_type::const_iterator i = _domains_with_plugs_.begin();
           i != _domains_with_plugs_.end();
           i++) {
        domain_dict_type::const_iterator j = i;
        outs << popts.indent << skip_tag;
        if (++j == _domains_with_plugs_.end()) {
          outs << last_tag;
        } else {
          outs << tag;
        }
        const plug_dict_type & plugs = i->second.plugs;
        outs << "Local domain '" << i->first << "' has " << i->second.plugs.size() << " plug(s)";
        outs << std::endl;
        {
          std::size_t counter = 0;
          for (plug_dict_type::const_iterator j = i->second.plugs.begin();
               j != i->second.plugs.end();
               j++) {
            const plug_entry & pe = j->second;
            outs << popts.indent << skip_tag;
            if (counter++ == i->second.plugs.size()) {
              outs << last_tag;
            } else {
              outs << tag;
            }
            outs << "Plug #" << counter ;
            outs << " : " << std::endl;
          }
        }
      }

      outs << popts.indent << tag
           << "Plug factory : ";
      if (_plug_factory_.get() == nullptr) {
        outs << "<none>";
      } else {
        outs << "[@" << _plug_factory_.get() << ']';
      }
      outs << std::endl;
 
      outs << popts.indent << inherit_tag(popts.inherit)
           << "Locked : " << std::boolalpha << is_locked() << std::endl;

      out_ << outs.str();
      return;
    }

  } // namespace com

} // namespace vire
