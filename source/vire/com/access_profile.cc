//! \file vire/com/access_profile.cc
//
// Copyright (c) 2017-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
//                         Jean Hommet <hommet@lpccaen.in2p3.fr>
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
#include <vire/com/access_profile.h>

// Standard library:
#include <sstream>

// This project:
#include <vire/com/manager.h>
#include <vire/com/plug_factory.h>
#include <vire/com/domain_builder.h>
#include <vire/com/domain.h>
#include <vire/com/access_hub.h>

namespace vire {

  namespace com {
    
    access_profile::access_profile(const manager & com_,
                                   const access_category_type category_,
                                   const std::string & target_,
                                   const std::string & name_,
                                   const std::string & password_)
    {
      _com_ = &com_;
      set_name(name_);
      set_password(password_);
      set_category(category_);
      DT_THROW_IF(target_.empty() && access_category_requires_target(_category_),
                  std::logic_error,
                  "Missing target (client/subcontractor ID) for access profile '" << _name_
                  << "' with category '" << to_string(_category_) << "'!");
      DT_THROW_IF(!target_.empty() && !access_category_requires_target(_category_),
                  std::logic_error,
                  "Unused target '" << target_ << "' for access profile '" << _name_
                  << "' with category '" << to_string(_category_) << "'!");
      if (!target_.empty()) { 
        set_target(target_);
      }
      _build_domains_indexation_();
      return;
    }

    access_profile::~access_profile()
    {
      return;
    }

    const manager & access_profile::get_com() const
    {
      return *_com_;
    }

    bool access_profile::is_locked() const
    {
      return _locked_;
    }
    
    bool access_profile::is_valid() const
    {
      if (!has_name()) return false;
      if (!has_category()) return false;
      if (access_category_requires_target(_category_) && !has_target()) return false;
      return true;
    }

    bool access_profile::has_category() const
    {
      return _category_ != ACCESS_CATEGORY_INVALID;
    }

    void access_profile::set_category(const access_category_type & category_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Access is locked!");
      _category_ = category_;
      return;
    }

    access_category_type access_profile::get_category() const
    {
      return _category_;
    }

    bool access_profile::has_name() const
    {
      return !_name_.empty();
    }

    void access_profile::set_name(const std::string & name_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Access is locked!");
      _name_ = name_;
      return;
    }

    const std::string & access_profile::get_name() const
    {
      return _name_;
    }

    bool access_profile::has_password() const
    {
      return !_password_.empty();
    }

    void access_profile::set_password(const std::string & password_)
    {
      _password_ = password_;
      return;
    }

    const std::string & access_profile::get_password() const
    {
      return _password_;
    }

    bool access_profile::match_password(const std::string & password_) const
    {
      return _password_ == password_;
    }

    bool access_profile::has_target() const
    {
      return !_target_.empty();
    }

    void access_profile::set_target(const std::string & target_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Access is locked!");
      _target_ = target_;
      return;
    }

    const std::string & access_profile::get_target() const
    {
      return _target_;
    }

    datatools::properties & access_profile::grab_metadata()
    {
      return _metadata_;
    }

    const datatools::properties & access_profile::get_metadata() const
    {
      return _metadata_;
    }
    
    /*     
           bool access::has_plug(const std::string & plug_name_) const
           {
           return _plugs_.count(plug_name_);
           }

           bool access::add_plug(const std::string & domain_label_,
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
           return grab_plug_faccessy().make_service_client_plug(get_domain(domain_label_), plug_name_);
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
    */
    
    void access_profile::_build_domains_indexation_()
    {
      const std::string & domain_name_prefix = this->get_com().get_domain_name_prefix();
      
      if (_category_ == ACCESS_CATEGORY_CLIENT_GATE
          || _category_ == ACCESS_CATEGORY_SERVER_GATE) {
        std::string gate_domain_name = domain_builder::build_cms_clients_gate_name(domain_name_prefix);
        _domains_["gate"] = &_com_->get_domain(gate_domain_name);
        return;
      }

      if (_category_ == ACCESS_CATEGORY_SERVER_CLIENT_SYSTEM
          || _category_ == ACCESS_CATEGORY_CLIENT_SYSTEM) {
        std::string client_name = get_target();
        std::string client_system_domain_name
          = domain_builder::build_cms_client_system_name(domain_name_prefix, client_name);
        _domains_["system"] = &_com_->get_domain(client_system_domain_name);
        return;
      }
 
      if (_category_ == ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM
          || _category_ == ACCESS_CATEGORY_SUBCONTRACTOR) {
        std::string subcontractor_name = get_target();
        std::string subcontractor_system_domain_name
          = domain_builder::build_cms_subcontractor_system_name(domain_name_prefix, subcontractor_name);
        _domains_["system"] = &_com_->get_domain(subcontractor_system_domain_name);
      }
 
      if (_category_ == ACCESS_CATEGORY_SERVER_CMS
          || _category_ == ACCESS_CATEGORY_CLIENT_CMS
          || _category_ == ACCESS_CATEGORY_SUBCONTRACTOR) {
        std::string monitoring_domain_name = domain_builder::build_cms_monitoring_name(domain_name_prefix);
        _domains_["monitoring"] = &_com_->get_domain(monitoring_domain_name);
      }
 
      if (_category_ == ACCESS_CATEGORY_SERVER_CMS
          || _category_ == ACCESS_CATEGORY_CLIENT_CMS) {
        std::string control_domain_name = domain_builder::build_cms_control_name(domain_name_prefix);
        _domains_["control"] = &_com_->get_domain(control_domain_name);
      }
      
      return;
    }
 
    bool access_profile::has_domain(const std::string & domain_label_) const
    {
      return _domains_.count(domain_label_);
    }

    
    const domain & access_profile::get_domain(const std::string & domain_label_) const
    {
      domain_dict_type::const_iterator found = _domains_.find(domain_label_);
      DT_THROW_IF(found == _domains_.end(),
                  std::logic_error,
                  "No domain labelled '" << domain_label_ << "'");
      return *found->second;
    }
    
    void access_profile::build_list_of_domain_local_labels(std::set<std::string> & domain_labels_) const
    {
      domain_labels_.clear();
      for (const auto & p : _domains_) {
        domain_labels_.insert(p.first);
      }
      return;
    }
 
    void access_profile::build_list_of_domain_names(std::set<std::string> & domain_names_) const
    {
      domain_names_.clear();
      for (const auto & p : _domains_) {
        const domain & domainRef = *p.second;
        bool doInsert = true;
        if (doInsert) {
          domain_names_.insert(domainRef.get_name());
        }
      }
      return;
    }
                    
    void access_profile::print_tree(std::ostream & out_,
                                    const boost::property_tree::ptree & options_) const
    {
      base_print_options popts;
      popts.configure_from(options_);
      std::ostringstream outs;
      if (!popts.title.empty()) {
        outs << popts.indent << popts.title << std::endl;
      }
  
      outs << popts.indent << tag
           << "Com      : " << (_com_ != nullptr ? "'"+_com_->get_name()+"'" : "<none>" ) << std::endl;

      outs << popts.indent << tag
           << "Category : '" << to_string(_category_) << "'" << std::endl;

      outs << popts.indent << tag
           << "Target   : ";
      if (has_target()) {
        outs << "'" << _target_ << "'";
      } else {
        outs << "<none>";
      }
      outs << std::endl;

      outs << popts.indent << tag
           << "Name     : '" << _name_ << "'" << std::endl;

      outs << popts.indent << tag
           << "Password : '" << _password_ << "'" << std::endl;

      outs << popts.indent << tag
           << "Metadata : [" << _metadata_.size() << "]" << std::endl;

      outs << popts.indent << tag
           << "Domains  : [" << _domains_.size() << "]" << std::endl;
      for (domain_dict_type::const_iterator i = _domains_.begin();
           i != _domains_.end();
           i++) {
        domain_dict_type::const_iterator j = i;
        outs << popts.indent << skip_tag;
        if (++j == _domains_.end()) {
          outs << last_tag;
        } else {
          outs << tag;
        }
        const domain & dom = *i->second;
        outs << "Domains '" << i->first << "' : '" << dom.get_name() << "'";
        outs << std::endl;
      }
      outs << popts.indent << inherit_tag(popts.inherit)
           << "End." << std::endl;

      out_ << outs.str();
      return;
    }
 
    access_hub_ptr_type access_profile::create_access_hub(const std::string & name_,
                                                          const std::string & select_domain_,
                                                          const bool lock_) const
    {
      access_hub_ptr_type accessPtr;
      DT_THROW_IF(!datatools::name_validation(name_,
                                              datatools::NV_INSTANCE | datatools::NV_NO_DOT),
                  std::logic_error,
                  "Invalid access name '" << name_ << "'!");
      std::set<std::string> domain_labels;
      if (select_domain_ == "all" ) {
        build_list_of_domain_local_labels(domain_labels);
      } else {
        DT_THROW_IF(_domains_.count(select_domain_) == 1,
                    std::logic_error,
                    "Unsupported domain selection '" << select_domain_ << "' in access profile of category '"
                    << vire::com::to_string( _category_) << "'!");
        domain_labels.insert(select_domain_);
      }
      if (_category_ == ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM) {
        DT_THROW_IF(domain_labels.size() != 1,
                    std::logic_error,
                    "Set of domain labels too large for access name '" << name_ << "'!");        
      }
      accessPtr.reset(new access_hub(*this, name_, domain_labels, lock_));
      return accessPtr;
    }
 
  } // namespace com

} // namespace vire
