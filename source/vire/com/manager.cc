//! \file vire/com/manager.cc
//
// Copyright (c) 2016-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/com/manager.h>

// Standard library:
#include <mutex>

// Third party
// - Boost:
#include <boost/algorithm/string.hpp>
// - Bayeux/datatools
#include <bayeux/datatools/utils.h>
#include <bayeux/datatools/exception.h>
// - BxRabbitMQ
//#include <bayeux/rabbitmq/rabbit_mgr.h>

// This project:
#include <vire/com/domain.h>
#include <vire/com/actor.h>
#include <vire/com/i_transport_manager.h>
#include <vire/resource/manager.h>
#include <vire/rabbitmq/manager_service.h>

namespace vire {

  namespace com {

    // Auto-registration of this service class in
    // a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(manager, "vire::com::manager")

    struct manager::pimpl_type
    {
      pimpl_type(manager & com_);

      // Attributes:
      manager & com;
      std::map<std::string, transport_manager_ptr> transport_managers;
      
    };

    manager::pimpl_type::pimpl_type(manager & com_)
      : com(com_)
    {
      // Load some cached infos ?
      return;
    }

    // static
    const std::string & manager::default_service_name()
    {
      static std::string _name("com");
      return _name;
    }
    
    // static
    const std::string & manager::gate_label()
    {
      static std::string _label("gate");
      return _label;
    }

    // static
    const std::string & manager::control_label()
    {
      static std::string _label("control");
      return _label;
    }
 
    // static
    const std::string & manager::monitoring_label()
    {
      static std::string _label("monitoring");
      return _label;
    }

    manager::manager(uint32_t /* flags_ */)
    {
      _initialized_ = false;
      _set_defaults_();
      // if (flags_ & XXX) {
      //   set_xxx(true);
      // }
      return;
    }

    manager::~manager()
    {
      if (is_initialized()) {
        reset();
      }
      return;
    }

    bool manager::has_domain_name_prefix() const
    {
      return !_domain_name_prefix_.empty();
    }

    void manager::set_domain_name_prefix(const std::string & prefix_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is already initialized!");
      DT_THROW_IF(!domain_builder::validate_domain_name_prefix(prefix_),
                  std::logic_error,
                  "Invalid domain name prefix '" << prefix_ << "'!");
      _domain_name_prefix_ = prefix_;
      return;
    }
      
    const std::string & manager::get_domain_name_prefix() const
    {
      return _domain_name_prefix_;
    }
    
    bool manager::has_default_transport_type_id() const
    {
      return _default_transport_type_id_.is_valid();
    }

    void manager::set_default_transport_type_id(const vire::utility::model_identifier & id_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _default_transport_type_id_ = id_;
      return;
    }

    const vire::utility::model_identifier & manager::get_default_transport_type_id() const
    {
      return _default_transport_type_id_;
    }

    bool manager::has_default_encoding_type_id() const
    {
      return _default_encoding_type_id_.is_valid();
    }

    void manager::set_default_encoding_type_id(const vire::utility::model_identifier & id_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _default_encoding_type_id_ = id_;
      return;
    }

    const vire::utility::model_identifier & manager::get_default_encoding_type_id() const
    {
      return _default_encoding_type_id_;
    }

    bool manager::has_resource_service_name() const
    {
      return !_resource_service_name_.empty();
    }
    
    void manager::set_resource_service_name(const std::string & resource_service_name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _resource_service_name_ = resource_service_name_;
      return;
    }
                        
    const std::string & manager::get_resource_service_name() const
    {
      return _resource_service_name_;
    }

    bool manager::has_resources() const
    {
      return _resources_ != nullptr;
    }

    void manager::set_resources(const vire::resource::manager & resources_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _resources_ = &resources_;
      return;
    }

    void manager::reset_resources()
    {
      _resources_ = nullptr;
      return;
    }

    const vire::resource::manager & manager::get_resources() const
    {
      DT_THROW_IF(!has_resources(), std::logic_error,
                  "No resources manager is available!");
      return *_resources_;
    }

    const domain_builder & manager::get_domain_maker() const
    {
      return _domain_maker_;
    }

    void manager::build_actor_names(std::set<std::string> & names_) const
    {
      names_.clear();
      for (const auto & p : _actors_) {
        names_.insert(p.first);
      }
      return;
    }

    bool manager::has_actors() const
    {
      return _actors_.size();
    }

    bool manager::has_actor(const std::string & actor_name_) const
    {
      return _actors_.find(actor_name_) != _actors_.end();
    }

    const actor & manager::get_actor(const std::string & actor_name_) const
    {
      manager * mutable_this = const_cast<manager*>(this);
      return const_cast<actor &>(mutable_this->grab_actor(actor_name_));
    }

    actor & manager::grab_actor(const std::string & actor_name_)
    {
      actor_dict_type::iterator found = _actors_.find(actor_name_);
      DT_THROW_IF(found == _actors_.end(),
                  std::logic_error,
                  "No actor with name '" << actor_name_ << "'!");
      return *found->second;
    }

    void manager::remove_actor(const std::string & actor_name_)
    {
      DT_THROW_IF(!has_actor(actor_name_),
                  std::logic_error,
                  "Manager has no actor with name '" << actor_name_ << "'!");
      _actors_.erase(actor_name_);
      return;
    }

    void manager::create_actor(const std::string & actor_name_,
                               const std::string & actor_password_,
                               const actor_category_type & actor_category_,
                               const std::string & target_id_)
    {
      DT_THROW_IF(has_actor(actor_name_),
                  std::logic_error,
                  "Manager already has an actor with name '" << actor_name_ << "'!");
      DT_THROW_IF(actor_category_ == ACTOR_CATEGORY_INVALID,
                  std::logic_error, "Invalid actor category!");
      _actors_[actor_name_] = std::make_shared<actor>(*this, actor_category_, target_id_, actor_name_, actor_password_);
      if (has_transport_management()) {
        std::cerr << "********** devel ************" << std::endl;
        std::cerr << "Create actor from the transport manager..." << std::endl;
        std::cerr << "*****************************" << std::endl;
        
      } 
      return; 
    }

    bool manager::has_domains() const
    {
      return _domains_.size();
    }

    void manager::build_domain_names(std::set<std::string> & names_) const
    {
      names_.clear();
      for (const auto & p : _domains_) {
        names_.insert(p.first);
      }
      return;
    }

    bool manager::has_domain(const std::string & domain_name_) const
    {
      return _domains_.find(domain_name_) != _domains_.end();
    }

    const domain & manager::get_domain(const std::string & domain_name_) const
    {
      domain_dict_type::const_iterator found = _domains_.find(domain_name_);
      DT_THROW_IF(found == _domains_.end(),
                  std::logic_error,
                  "No domain with identifier '" << domain_name_ << "'!");
      return *found->second;
    }

    domain & manager::grab_domain(const std::string & domain_name_)
    {
      domain_dict_type::iterator found = _domains_.find(domain_name_);
      DT_THROW_IF(found == _domains_.end(),
                  std::logic_error,
                  "No domain with identifier '" << domain_name_ << "'!");
      return *found->second;
    }

    domain &
    manager::create_domain(const std::string & domain_name_,
                           const domain_category_type & domain_category_,
                           const vire::utility::model_identifier & domain_protocol_id_,
                           const vire::utility::model_identifier & domain_encoding_id_)
    {
      DT_THROW_IF(has_domain(domain_name_),
                  std::logic_error,
                  "Manager already has a domain with ID '" << domain_name_ << "'!");
      std::shared_ptr<domain> sp;
      DT_THROW_IF(!domain_protocol_id_.is_valid(), std::logic_error, "Invalid protocol ID !");
      DT_THROW_IF(!domain_encoding_id_.is_valid(), std::logic_error, "Invalid encoding ID !");
      sp.reset(new domain(domain_name_, domain_category_, domain_protocol_id_, domain_encoding_id_));
      if (sp.get() != nullptr) {
        _domains_[domain_name_] = sp;
      }
      if (has_transport_management()) {
        std::cerr << "********** devel ************" << std::endl;
        std::cerr << "Create domain from the transport manager..." << std::endl;
        std::cerr << "*****************************" << std::endl;   
      } 
      return *_domains_.find(domain_name_)->second;
    }

    void manager::remove_domain(const std::string & domain_name_)
    {
      domain_dict_type::iterator found = _domains_.find(domain_name_);
      DT_THROW_IF(found == _domains_.end(),
                  std::logic_error,
                  "No domain with identifier '" << domain_name_ << "'!");
      _domains_.erase(found);
      return;
    }

    void manager::tree_dump(std::ostream & out_,
                            const std::string & title_,
                            const std::string & indent_,
                            bool inherit_) const
    {
      this->datatools::base_service::tree_dump(out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Resources service : ";
      if (has_resources()) {
        out_ << "'" << _resources_->get_name() << "'";
      } else {
        out_ << "<none>";
      }
      out_ << std::endl;

      out_ << indent_ << datatools::i_tree_dumpable::inherit_tag(inherit_)
           << "Domains : ";
      if (_domains_.size()) {
        out_ << std::endl;
        for (domain_dict_type::const_iterator i = _domains_.begin();
             i != _domains_.end();
             i++) {
          out_ << indent_ << datatools::i_tree_dumpable::inherit_skip_tag(inherit_);
          domain_dict_type::const_iterator j = i;
          if (++j == _domains_.end()) {
            out_ << datatools::i_tree_dumpable::last_tag;
          } else {
            out_ << datatools::i_tree_dumpable::tag;
          }
          out_ << "id='" << i->first << "' (category='"
               << to_string(i->second.get()->get_category()) << "')"
               << std::endl;
        }
      } else {
        out_ << "<none>" << std::endl;
      }
      return;
    }

    bool manager::has_app_category() const
    {
      return _app_category_ != vire::cms::application::CATEGORY_UNDEF;
    }

    const vire::cms::application::category_type manager::get_app_category() const
    {
      return _app_category_;
    }

    void manager::set_app_category(const vire::cms::application::category_type appcat_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Manager is initialized and locked!");
      _app_category_ = appcat_;
      if (_app_category_ == vire::cms::application::CATEGORY_SERVER) {
        _transport_management_ = true;
      } else {
        _transport_management_ = false;
      }
      return;
    }

    bool manager::has_transport_management() const
    {
      return _transport_management_;
    }

    bool manager::is_initialized() const
    {
      return _initialized_;
    }

    // virtual
    int manager::initialize(const datatools::properties & config_,
                            datatools::service_dict_type & service_dict_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is already initialized!");

      this->::datatools::base_service::common_initialize(config_);
      
      if (!has_app_category()) {
        if (config_.has_key("cms_application_category")) {
          std::string appcat_repr = config_.fetch_string("cms_application_category");
          vire::cms::application::category_type appcat = vire::cms::application::CATEGORY_UNDEF;
          DT_THROW_IF(!vire::cms::application::from_string(appcat_repr, appcat),
                      std::logic_error,
                      "Invalid CMS application category '" << appcat_repr << "'!");
          set_app_category(appcat);
        }
      }
      
      if (!has_resources()) {
        if (!has_resource_service_name()) {
          std::string resource_service_name;
          if (config_.has_key("resource_service_name")) {
            resource_service_name = config_.fetch_string("resource_service_name");
          } else {
            resource_service_name = vire::resource::manager::default_service_name();
          }
          set_resource_service_name(resource_service_name);
        }
      }
 
      if (!has_domain_name_prefix()) {
        if (config_.has_key("domain_name_prefix")) {
          std::string domain_name_prefix = config_.fetch_string("domain_name_prefix");
          set_domain_name_prefix(domain_name_prefix);
        }
      }

      if (!has_default_transport_type_id()) {
        if (config_.has_key("default_transport_type_id")) {
          std::string ttid = config_.fetch_string("default_transport_type_id");
          vire::utility::model_identifier transport_type_id;
          transport_type_id.from_string(ttid);
          set_default_transport_type_id(transport_type_id);
        }
      }
 
      if (!has_default_encoding_type_id()) {
        if (config_.has_key("default_encoding_type_id")) {
          std::string etid = config_.fetch_string("default_encoding_type_id");
          vire::utility::model_identifier encoding_type_id;
          encoding_type_id.from_string(etid);
          set_default_encoding_type_id(encoding_type_id);
        }
      }    

      // Default domain infos:
      std::set<std::string> domain_labels = { "control", "monitoring", "gate" };
      for (const std::string & domain_label : domain_labels) {
        domain_info_type di;
        if (domain_label == "gate") {
          di.dom_cat = DOMAIN_CATEGORY_GATE;
        } else if (domain_label == "control") {
          di.dom_cat = DOMAIN_CATEGORY_CONTROL;
        } else if (domain_label == "monitoring") {
          di.dom_cat = DOMAIN_CATEGORY_MONITORING;
        }
        di.transport_type_id = get_default_transport_type_id();
        di.encoding_type_id  = get_default_encoding_type_id();
        _domain_infos_[domain_label] = di;
      }
      
      // Overridden domain infos:
      if (config_.has_key("domains.labels")) {
        std::set<std::string> domain_labels;
        config_.fetch("domains.labels", domain_labels);
        for (const std::string & domain_label : domain_labels) {
          vire::utility::model_identifier transport_type_id;
          vire::utility::model_identifier encoding_type_id;
          {
            std::ostringstream transport_type_id_key;
            transport_type_id_key << "domains." << domain_label << ".transport_type_id";
            if (config_.has_key(transport_type_id_key.str())) {
              std::string  transport_type_id_repr = config_.fetch_string(transport_type_id_key.str());
              DT_THROW_IF(!transport_type_id.from_string(transport_type_id_repr),
                          std::logic_error,
                          "Invalid transport type ID representation '" << transport_type_id_repr << "'!");
            }
          }

          {
            std::ostringstream encoding_type_id_key;
            encoding_type_id_key << "domains." << domain_label << ".encoding_type_id";
            if (config_.has_key(encoding_type_id_key.str())) {
              std::string  encoding_type_id_repr = config_.fetch_string(encoding_type_id_key.str());
              DT_THROW_IF(!encoding_type_id.from_string(encoding_type_id_repr),
                          std::logic_error,
                          "Invalid encoding type ID representation '" << encoding_type_id_repr << "'!");
            }
          }
          domain_info_dict_type::iterator found = _domain_infos_.find(domain_label);
          DT_THROW_IF(found == _domain_infos_.end(),
                      std::logic_error,
                      "Invalid domain label '" << domain_label << "'!");
          if (transport_type_id.is_valid()) { 
            found->second.transport_type_id = transport_type_id;
          }
          if (encoding_type_id.is_valid()) {
            found->second.encoding_type_id = encoding_type_id;
          }
        }
      }

      _at_init_(config_);

      _initialized_ = true;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    int manager::reset()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error,
                  "Communication manager is not initialized!");
      _initialized_ = false;
      _at_reset_();
      _set_defaults_();
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return datatools::SUCCESS;
    }

    void manager::_set_defaults_()
    {
      _transport_management_ = false;
      _app_category_ = vire::cms::application::CATEGORY_UNDEF;
      set_logging_priority(datatools::logger::PRIO_FATAL);
      return;
    }

    void manager::_at_reset_transport_managers_()
    {
      if (_pimpl_) {
        for (const auto & p : _pimpl_->transport_managers) {
          p.second->reset();
        }
        _pimpl_->transport_managers.clear();
      }
      return;
    }

    void manager::_at_init_transport_managers_(const datatools::properties & config_)
    {
      std::set<std::string> transmgr_names;
      if (config_.has_key("names")) {
        config_.fetch("names", transmgr_names);
        if (_default_transport_type_id_.get_name() == "rabbitmq") {
          for (const auto & trans_name : transmgr_names) {

            std::ostringstream transmgr_type_id_key;
            transmgr_type_id_key << trans_name << ".type_id";
            DT_THROW_IF(!config_.has_key(transmgr_type_id_key.str()),
                        std::logic_error,
                        "Missing '" << trans_name << "' transport manager type ID!");
            std::string transmgr_type_id = config_.fetch_string(transmgr_type_id_key.str());
            
            std::ostringstream transmgr_config_path_key;
            transmgr_config_path_key << trans_name << ".config_path";
            DT_THROW_IF(!config_.has_key(transmgr_config_path_key.str()),
                        std::logic_error,
                        "Missing '" << trans_name << "' transport manager config path!");
            std::string transmgr_config_path = config_.fetch_path(transmgr_config_path_key.str());
            datatools::fetch_path_with_env(transmgr_config_path);
            datatools::properties transmgr_config;
            transmgr_config.read_configuration(transmgr_config_path);

            i_transport_manager::factory_register_type & sys_factory_register
              =  DATATOOLS_FACTORY_GRAB_SYSTEM_REGISTER(i_transport_manager);
            DT_THROW_IF(! sys_factory_register.has(transmgr_type_id),
                        std::logic_error,
                        "No transport manager type ID '" << transmgr_type_id
                        << "' factory is known from the system register!");
            const i_transport_manager::factory_register_type::factory_type & the_factory
              = sys_factory_register.get(transmgr_type_id);
            transport_manager_ptr transmgrPtr(the_factory());
            transmgrPtr->set_name(trans_name);
            transmgrPtr->initialize(transmgr_config);
            _pimpl_->transport_managers[trans_name] = transmgrPtr;
          }
        }
      }
      return;
    }
    
    void manager::_at_init_(const datatools::properties & config_)
    {
      // DT_THROW_IF(!has_resources(), std::logic_error, "Missing 'resources' service!");
      
      _domain_maker_.set_domain_name_prefix(_domain_name_prefix_);
      _domain_maker_.set_transport_type_id(_default_transport_type_id_);
      _domain_maker_.set_encoding_type_id(_default_encoding_type_id_);

      _pimpl_.reset(new pimpl_type(*this));

      if (_transport_management_) {
        datatools::properties transport_manager_config;
        config_.export_and_rename_starting_with(transport_manager_config,
                                                "transport_manager.",
                                                "");
        _at_init_transport_managers_(transport_manager_config);
      }
      
      _build_default_domains_();
      
      return;
    }

    void manager::_at_reset_()
    {
      if (_transport_management_) {
        _at_reset_transport_managers_();
      }
      
      if (_pimpl_) {
        _pimpl_.reset();
      }
      _actors_.clear();
      _domains_.clear();
      _resources_ = nullptr;
      _domain_maker_.reset();
      _default_encoding_type_id_.reset();
      _default_transport_type_id_.reset();
      _domain_name_prefix_.clear();
      _resource_service_name_.clear();
      _transport_management_ = false;
      _app_category_ = vire::cms::application::CATEGORY_UNDEF;
      return;
    }

    void manager::_build_default_domains_()
    {
 
      if (_app_category_ == vire::cms::application::CATEGORY_SERVER
          || _app_category_ == vire::cms::application::CATEGORY_CLIENT) {
        // Check and create client gate:
        std::string gate_sys_domain_name
          = vire::com::domain_builder::build_cms_clients_gate_name(this->get_domain_maker().get_domain_name_prefix());
        if (!this->has_domain(gate_sys_domain_name)) {
          domain_info_dict_type::const_iterator found = _domain_infos_.find(gate_label());
          const domain_info_type & dom_info = found->second;
          vire::com::domain & gate_sys_domain = this->create_domain(gate_sys_domain_name,
                                                                    dom_info.dom_cat,
                                                                    dom_info.transport_type_id,
                                                                    dom_info.encoding_type_id); 
          this->get_domain_maker().build_clients_gate_domain(gate_sys_domain);
        }
      }
     
      {
        // Check and create monitoring domain:
        std::string monitoring_domain_name
          = vire::com::domain_builder::build_cms_monitoring_name(this->get_domain_maker().get_domain_name_prefix());
        if (!this->has_domain(monitoring_domain_name)) {
          domain_info_dict_type::const_iterator found = _domain_infos_.find(monitoring_label());
          const domain_info_type & dom_info = found->second;
          vire::com::domain & monitoring_domain = this->create_domain(monitoring_domain_name,
                                                                      dom_info.dom_cat,
                                                                      dom_info.transport_type_id,
                                                                      dom_info.encoding_type_id); 
          this->get_domain_maker().build_monitoring_domain(monitoring_domain);
        }
      }
      
      if (_app_category_ == vire::cms::application::CATEGORY_SERVER
          || _app_category_ == vire::cms::application::CATEGORY_CLIENT) {
        // Check and create control domain:
        std::string control_domain_name
          = vire::com::domain_builder::build_cms_control_name(this->get_domain_maker().get_domain_name_prefix());
        if (!this->has_domain(control_domain_name)) {
          domain_info_dict_type::const_iterator found = _domain_infos_.find(control_label());
          const domain_info_type & dom_info = found->second;
          vire::com::domain & control_domain = this->create_domain(control_domain_name,
                                                                   dom_info.dom_cat,
                                                                   dom_info.transport_type_id,
                                                                   dom_info.encoding_type_id); 
          this->get_domain_maker().build_control_domain(control_domain);
        }
      }
      
      return;
    }

  } // namespace com

} // namespace vire
