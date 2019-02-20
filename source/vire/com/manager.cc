//! \file vire/com/manager.cc
//
// Copyright (c) 2016-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/com/access_profile.h>
#include <vire/com/i_transport_manager.h>
#include <vire/resource/manager.h>
#include <vire/rabbitmq/manager_service.h>

namespace vire {

  namespace com {

    // Auto-registration of this service class in
    // a central service database of Bayeux/datatools:
    DATATOOLS_SERVICE_REGISTRATION_IMPLEMENT(manager, "vire::com::manager")

    /// Transport manager record
    struct transport_manager_entry
    {
      std::string name;
      std::string manager_type_id;
      std::string driver_type_id;
      datatools::properties config;
      transport_manager_ptr pmgr;
    };
    
    /// PIMPL record
    struct manager::pimpl_type
    {
      pimpl_type(manager & com_);

      // Attributes:
      manager & com; ///< Handle to the communication manager
      std::map<std::string, transport_manager_entry> transport_managers; ///< Dictionary of transport manager records
      std::map<std::string, std::string> transport_driver_to_managers; ///< Transport driver to manager association map 
      
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

    bool manager::has_system_access_login_prefix() const
    {
      return !_system_access_login_prefix_.empty();
    }

    void manager::set_system_access_login_prefix(const std::string & prefix_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is already initialized!");
      DT_THROW_IF(!datatools::name_validation(prefix_,
                                              datatools::NV_INSTANCE | datatools::NV_NO_DOT),
                  std::logic_error,
                  "Invalid access profile login prefix '" << prefix_ << "'!");
      _system_access_login_prefix_ = prefix_;
      return;
    }
      
    const std::string & manager::get_system_access_login_prefix() const
    {
      return _system_access_login_prefix_;
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
    
    bool manager::has_default_transport_driver_type_id() const
    {
      return _default_transport_driver_type_id_.is_valid();
    }

    void manager::set_default_transport_driver_type_id(const vire::utility::model_identifier & id_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _default_transport_driver_type_id_ = id_;
      return;
    }

    const vire::utility::model_identifier & manager::get_default_transport_driver_type_id() const
    {
      return _default_transport_driver_type_id_;
    }

    bool manager::has_default_encoding_driver_type_id() const
    {
      return _default_encoding_driver_type_id_.is_valid();
    }

    void manager::set_default_encoding_driver_type_id(const vire::utility::model_identifier & id_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _default_encoding_driver_type_id_ = id_;
      return;
    }

    const vire::utility::model_identifier & manager::get_default_encoding_driver_type_id() const
    {
      return _default_encoding_driver_type_id_;
    }

    bool manager::has_resources_service_name() const
    {
      return !_resources_service_name_.empty();
    }
    
    void manager::set_resources_service_name(const std::string & resources_service_name_)
    {
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "Communication manager is initialized!");
      _resources_service_name_ = resources_service_name_;
      return;
    }
                        
    const std::string & manager::get_resources_service_name() const
    {
      return _resources_service_name_;
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

    void manager::build_access_profile_names(std::set<std::string> & names_) const
    {
      names_.clear();
      for (const auto & p : _access_profiles_) {
        names_.insert(p.first);
      }
      return;
    }

    bool manager::has_access_profiles() const
    {
      return _access_profiles_.size();
    }

    bool manager::has_access_profile(const std::string & access_profile_name_) const
    {
      return _access_profiles_.find(access_profile_name_) != _access_profiles_.end();
    }

    const access_profile & manager::get_access_profile(const std::string & access_profile_name_) const
    {
      manager * mutable_this = const_cast<manager*>(this);
      return const_cast<access_profile &>(mutable_this->grab_access_profile(access_profile_name_));
    }

    access_profile & manager::grab_access_profile(const std::string & access_profile_name_)
    {
      access_profile_dict_type::iterator found = _access_profiles_.find(access_profile_name_);
      DT_THROW_IF(found == _access_profiles_.end(),
                  std::logic_error,
                  "No access profile with name '" << access_profile_name_ << "'!");
      return *found->second;
    }

    void manager::remove_access_profile(const std::string & access_profile_name_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!has_access_profile(access_profile_name_),
                  std::logic_error,
                  "Manager has no access profile with name '" << access_profile_name_ << "'!");

      if (has_transport_management()) {
        DT_LOG_DEBUG(get_logging_priority(), "Com manager has transport management.");
        for (auto & ptransmgr : _pimpl_->transport_managers) {
          DT_LOG_DEBUG(get_logging_priority(), "Processing access profile '" << access_profile_name_ << "' removal with transport manager '" << ptransmgr.first << "'...");
          transport_manager_entry & tme = ptransmgr.second;
          i_transport_manager & transmgr = *tme.pmgr;
          if (transmgr.has_user(access_profile_name_)) {
            DT_LOG_DEBUG(get_logging_priority(), "Removing access '" << access_profile_name_ << "' from the transport manager '" << ptransmgr.first << "'.");
            transmgr.remove_user(access_profile_name_);
            DT_LOG_DEBUG(get_logging_priority(), "Access profile '" << access_profile_name_ << "' has been removed from the transport manager '" << ptransmgr.first << "'.");
          } else {
            DT_LOG_DEBUG(get_logging_priority(), "Access profile '" << access_profile_name_ << "' does not exists in the transport manager '" <<  ptransmgr.first << "'.");
          }
        }        
      }
      _access_profiles_.erase(access_profile_name_);
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
     
    void manager::create_sc_transport(const subcontractor_info & sc_info_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_THROW_IF(!is_initialized(), std::logic_error, "Cannot create S/C transport resources! Com manager is not initialized!");
      if (_app_category_ == vire::cms::application::CATEGORY_SERVER) {
        access_profile & svr_ap = create_access_profile(sc_info_.sys_svr_login,
                                                        sc_info_.sys_svr_password,
                                                        ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM,
                                                        sc_info_.id);
      }
      access_profile & sc_ap = create_access_profile(sc_info_.user_login,
                                                     sc_info_.user_password,
                                                     ACCESS_CATEGORY_SUBCONTRACTOR,
                                                     sc_info_.id);
      domain & sc_sys_dom = create_domain(sc_info_.system_domain_name,
                                          DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM,
                                          sc_info_.system_transport_driver_type_id,
                                          sc_info_.system_encoding_driver_type_id);

      if (has_transport_management()) {
        DT_LOG_DEBUG(get_logging_priority(), "Com manager has transport management.");
        std::string transport_driver_type_id = get_default_transport_driver_type_id().get_name();
        if (sc_sys_dom.has_transport_driver_type_id()) {
          transport_driver_type_id = sc_sys_dom.get_transport_driver_type_id().get_name();
        }
        DT_THROW_IF(_pimpl_->transport_driver_to_managers.count(transport_driver_type_id) == 0,
                    std::logic_error,
                    "No transport manager is associated to domain '" << sc_info_.system_domain_name << "' transport driver of type '" << transport_driver_type_id << "'!");
        std::string transport_manager_name = _pimpl_->transport_driver_to_managers.find(transport_driver_type_id)->second;
        transport_manager_entry & tme = _pimpl_->transport_managers.find(transport_manager_name)->second;
        i_transport_manager & transmgr = *tme.pmgr;
        transmgr.add_subcontractor(sc_info_);
      }

      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }     

    void manager::remove_sc_transport(const subcontractor_info & sc_info_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      // DT_THROW_IF(!is_initialized(), std::logic_error, "Cannot remove S/C transport resources! Com manager is not initialized!");
      if (has_domain(sc_info_.system_domain_name)) {
        remove_domain(sc_info_.system_domain_name);
      }
      if (has_access_profile(sc_info_.user_login)) {
        remove_access_profile(sc_info_.user_login);
      }
      if (_app_category_ == vire::cms::application::CATEGORY_SERVER) {
        if (has_access_profile(sc_info_.sys_svr_login)) {
          remove_access_profile(sc_info_.sys_svr_login);
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }     
 
    access_profile & manager::create_access_profile(const std::string & access_profile_name_,
                                                    const std::string & access_profile_password_,
                                                    const access_category_type access_category_,
                                                    const std::string & target_id_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      DT_LOG_DEBUG(get_logging_priority(), "Access profile '" << access_profile_name_ << "'...");
      DT_THROW_IF(has_access_profile(access_profile_name_),
                  std::logic_error,
                  "Manager already has an access profile with name '" << access_profile_name_ << "'!");
      DT_THROW_IF(access_category_ == ACCESS_CATEGORY_INVALID,
                  std::logic_error, "Invalid access category!");
      _access_profiles_[access_profile_name_] =
        std::make_shared<access_profile>(*this,
                                         access_category_,
                                         target_id_,
                                         access_profile_name_,
                                         access_profile_password_);
      
      if (has_transport_management()) {
        DT_LOG_DEBUG(get_logging_priority(), "Com manager has transport management.");
        for (auto & ptransmgr : _pimpl_->transport_managers) {
          DT_LOG_DEBUG(get_logging_priority(), "Processing new access profile '" << access_profile_name_ << "' with transport manager '" << ptransmgr.first << "'...");
          transport_manager_entry & tme = ptransmgr.second;
          i_transport_manager & transmgr = *tme.pmgr;
          if (! transmgr.has_user(access_profile_name_)) {
            DT_LOG_DEBUG(get_logging_priority(), "Creating access '" << access_profile_name_ << "' from the transport manager '" << ptransmgr.first << "'.");
            transmgr.add_user(access_profile_name_, access_profile_password_, access_category_);
            DT_LOG_DEBUG(get_logging_priority(), "Access profile '" << access_profile_name_ << "' has been created by the transport manager '" << ptransmgr.first << "'.");
          } else {
            DT_LOG_DEBUG(get_logging_priority(), "Access profile '" << access_profile_name_ << "' already exists for the transport manager '" <<  ptransmgr.first << "'.");
          }
        }        
      }
      
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return *_access_profiles_.find(access_profile_name_)->second.get(); 
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
                           const domain_category_type domain_category_,
                           const vire::utility::model_identifier & domain_transport_driver_type_id_,
                           const vire::utility::model_identifier & domain_encoding_driver_type_id_)
    {
      DT_THROW_IF(has_domain(domain_name_),
                  std::logic_error,
                  "Manager already has a domain with ID '" << domain_name_ << "'!");
      std::shared_ptr<domain> sp;
      DT_THROW_IF(!domain_transport_driver_type_id_.is_valid(), std::logic_error, "Invalid transport driver type ID !");
      DT_THROW_IF(!domain_encoding_driver_type_id_.is_valid(), std::logic_error, "Invalid encoding driver type ID !");
      sp.reset(new domain(domain_name_, domain_category_, domain_transport_driver_type_id_, domain_encoding_driver_type_id_));
      if (sp.get() != nullptr) {
        _domains_[domain_name_] = sp;
      }
      domain & dom = *_domains_.find(domain_name_)->second;
      if (domain_category_ == DOMAIN_CATEGORY_GATE) {
        this->get_domain_maker().build_clients_gate_domain(dom);
      }
      if (domain_category_ == DOMAIN_CATEGORY_CONTROL) {
        this->get_domain_maker().build_control_domain(dom);
      }
      if (domain_category_ == DOMAIN_CATEGORY_MONITORING) {
        this->get_domain_maker().build_monitoring_domain(dom);
      }
      if (domain_category_ == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        this->get_domain_maker().build_subcontractor_system_domain(dom, dom.get_subcontractor_identifier());
      }
      if (domain_category_ == DOMAIN_CATEGORY_CLIENT_SYSTEM) {
        this->get_domain_maker().build_client_system_domain(dom, dom.get_client_identifier());
      }

      // Transport management:
      if (has_transport_management()) {
        std::string transport_driver_type_id = get_default_transport_driver_type_id().get_name();
        if (dom.has_transport_driver_type_id()) {
          transport_driver_type_id = dom.get_transport_driver_type_id().get_name();
        }
        DT_THROW_IF(_pimpl_->transport_driver_to_managers.count(transport_driver_type_id) == 0,
                    std::logic_error,
                    "No transport manager is associated to domain '" << domain_name_ << "' transport driver of type '" << transport_driver_type_id << "'!");
        std::string transport_manager_name = _pimpl_->transport_driver_to_managers.find(transport_driver_type_id)->second;
        transport_manager_entry & tme = _pimpl_->transport_managers.find(transport_manager_name)->second;
        i_transport_manager & transmgr = *tme.pmgr;
        if (transmgr.has_domain(domain_name_)) {
          DT_LOG_DEBUG(get_logging_priority(), "Domain '" << domain_name_ << "' already exists for the transport manager '" << transport_manager_name << "'.");
        } else {
          DT_LOG_DEBUG(get_logging_priority(), "Creating domain '" << domain_name_ << "' from the transport manager '" << transport_manager_name << "'.");
          transmgr.add_domain(domain_name_, dom.get_category());
          DT_LOG_DEBUG(get_logging_priority(), "Domain '" << domain_name_ << "' has been created by the transport manager '" << transport_manager_name << "'.");
          // if (domain_category_ == DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
          //  XXX
          // }
        }
      }
      
      return dom;
    }

    void manager::remove_domain(const std::string & domain_name_)
    {
      domain_dict_type::iterator found = _domains_.find(domain_name_);
      DT_THROW_IF(found == _domains_.end(),
                  std::logic_error,
                  "No domain with identifier '" << domain_name_ << "'!");
      domain & dom = *found->second;
      if (has_transport_management()) {
        std::string transport_driver_type_id = get_default_transport_driver_type_id().get_name();
        if (dom.has_transport_driver_type_id()) {
          transport_driver_type_id = dom.get_transport_driver_type_id().get_name();
        }
        DT_THROW_IF(_pimpl_->transport_driver_to_managers.count(transport_driver_type_id) == 0,
                    std::logic_error,
                    "No transport manager is associated to domain '" << domain_name_ << "' transport driver of type '" << transport_driver_type_id << "'!");
        std::string transport_manager_name = _pimpl_->transport_driver_to_managers.find(transport_driver_type_id)->second;
        transport_manager_entry & tme = _pimpl_->transport_managers.find(transport_manager_name)->second;
        i_transport_manager & transmgr = *tme.pmgr;
        if (! transmgr.has_domain(domain_name_)) {
          DT_LOG_DEBUG(get_logging_priority(), "Domain '" << domain_name_ << "' does not exist for the transport manager '" << transport_manager_name << "'.");
        } else {
          if (dom.is_client_system()) {
            transmgr.remove_domain(domain_name_);
            DT_LOG_DEBUG(get_logging_priority(), "Removing domain '" << domain_name_ << "' from the transport manager '" << transport_manager_name << "'.");
            DT_LOG_DEBUG(get_logging_priority(), "Domain '" << domain_name_ << "' has been removed by the transport manager '" << transport_manager_name << "'.");
          } else {
            DT_LOG_DEBUG(get_logging_priority(), "Preserving system domain '" << domain_name_ << "' from the transport manager '" << transport_manager_name << "'.");
          }
        }
      } 
      _domains_.erase(found);
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
        if (!has_resources_service_name()) {
          std::string resources_service_name;
          if (config_.has_key("resources_service_name")) {
            resources_service_name = config_.fetch_string("resources_service_name");
          } else {
            resources_service_name = vire::resource::manager::default_service_name();
          }
          set_resources_service_name(resources_service_name);
        }
      }
 
      if (!has_system_access_login_prefix()) {
        if (config_.has_key("system_access_login_prefix")) {
          std::string system_access_login_prefix = config_.fetch_string("system_access_login_prefix");
          set_system_access_login_prefix(system_access_login_prefix);
        }
      }
 
      if (!has_domain_name_prefix()) {
        if (config_.has_key("domain_name_prefix")) {
          std::string domain_name_prefix = config_.fetch_string("domain_name_prefix");
          set_domain_name_prefix(domain_name_prefix);
        }
      }

      if (!has_default_transport_driver_type_id()) {
        if (config_.has_key("default_transport_driver_type_id")) {
          std::string ttid = config_.fetch_string("default_transport_driver_type_id");
          vire::utility::model_identifier transport_driver_type_id;
          transport_driver_type_id.from_string(ttid);
          set_default_transport_driver_type_id(transport_driver_type_id);
        }
      }
 
      if (!has_default_encoding_driver_type_id()) {
        if (config_.has_key("default_encoding_driver_type_id")) {
          std::string etid = config_.fetch_string("default_encoding_driver_type_id");
          vire::utility::model_identifier encoding_driver_type_id;
          encoding_driver_type_id.from_string(etid);
          set_default_encoding_driver_type_id(encoding_driver_type_id);
        }
      }    

      // Default access profile infos:
      static std::set<std::string> access_profile_labels;
      if (access_profile_labels.size() == 0) {
        access_profile_labels.insert(server_cms_access_label());
        access_profile_labels.insert(server_gate_access_label());
        access_profile_labels.insert(client_gate_access_label());
        // access_profile_labels.insert(server_subcontractor_system_access_label());
        // access_profile_labels.insert(server_client_system_access_label());
      }
      for (const std::string & access_profile_label : access_profile_labels) {
        access_profile_info_type upi;
        std::string access_profile_name = make_system_login(_system_access_login_prefix_, access_profile_label);
        upi.name = access_profile_name;
        upi.password = access_profile_label;
        if (access_profile_label == server_cms_access_label()) {
          upi.category = ACCESS_CATEGORY_SERVER_CMS;
        } else if (access_profile_label == server_gate_access_label()) {
          upi.category = ACCESS_CATEGORY_SERVER_GATE;
        } else if (access_profile_label == client_gate_access_label()) {
          upi.category = ACCESS_CATEGORY_CLIENT_GATE;
        }
        /* 
        else if (access_profile_label == server_subcontractor_system_access_label()) {
          upi.category = ACCESS_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM;
        } else if (access_profile_label == server_client_system_access_label()) {
          upi.category = ACCESS_CATEGORY_SERVER_CLIENT_SYSTEM;
        } 
        */
        _access_profile_infos_[access_profile_label] = upi;
      }
          
      // Overridden access profile infos:
      if (config_.has_key("access_profiles.labels")) {
        std::set<std::string> access_profile_labels;
        config_.fetch("access_profiles.labels", access_profile_labels);
        for (const std::string & access_profile_label : access_profile_labels) {
          access_profile_info_dict_type::iterator found = _access_profile_infos_.find(access_profile_label);
          DT_THROW_IF(found == _access_profile_infos_.end(),
                      std::logic_error,
                      "Invalid access profile label '" << access_profile_label << "'!");
          access_profile_info_type & api = found->second;
          {
            std::ostringstream key;
            key << "access_profiles." << access_profile_label << ".login";
            if (config_.has_key(key.str())) {
              std::string login = config_.fetch_string(key.str());
              api.name = login;
            }
          }
          {
            std::ostringstream key;
            key << "access_profiles." << access_profile_label << ".password";
            if (config_.has_key(key.str())) {
              std::string password = config_.fetch_string(key.str());
              api.password = password;
            }
          }
        }
      }

      // Default domain infos:
      static std::set<std::string> domain_labels;
      if (domain_labels.size() == 0) {
        domain_labels.insert(domain_gate_label());
        domain_labels.insert(domain_control_label());
        domain_labels.insert(domain_monitoring_label());
      }
      for (const std::string & domain_label : domain_labels) {
        domain_info_type di;
        if (domain_label == domain_gate_label()) {
          di.category = DOMAIN_CATEGORY_GATE;
        } else if (domain_label == domain_control_label()) {
          di.category = DOMAIN_CATEGORY_CONTROL;
        } else if (domain_label == domain_monitoring_label()) {
          di.category = DOMAIN_CATEGORY_MONITORING;
        }
        di.transport_driver_type_id = get_default_transport_driver_type_id();
        di.encoding_driver_type_id  = get_default_encoding_driver_type_id();
        _domain_infos_[domain_label] = di;
      }
      
      // Overridden domain infos:
      if (config_.has_key("domains.labels")) {
        std::set<std::string> domain_labels;
        config_.fetch("domains.labels", domain_labels);
        for (const std::string & domain_label : domain_labels) {
          domain_info_dict_type::iterator found = _domain_infos_.find(domain_label);
          DT_THROW_IF(found == _domain_infos_.end(),
                      std::logic_error,
                      "Invalid domain label '" << domain_label << "'!");
          domain_info_type & di = found->second;
          vire::utility::model_identifier transport_driver_type_id;
          vire::utility::model_identifier encoding_driver_type_id;
          {
            std::ostringstream transport_driver_type_id_key;
            transport_driver_type_id_key << "domains." << domain_label << ".transport_driver_type_id";
            if (config_.has_key(transport_driver_type_id_key.str())) {
              std::string  transport_driver_type_id_repr = config_.fetch_string(transport_driver_type_id_key.str());
              DT_THROW_IF(!transport_driver_type_id.from_string(transport_driver_type_id_repr),
                          std::logic_error,
                          "Invalid transport type ID representation '" << transport_driver_type_id_repr << "'!");
            }
          }

          {
            std::ostringstream encoding_driver_type_id_key;
            encoding_driver_type_id_key << "domains." << domain_label << ".encoding_driver_type_id";
            if (config_.has_key(encoding_driver_type_id_key.str())) {
              std::string  encoding_driver_type_id_repr = config_.fetch_string(encoding_driver_type_id_key.str());
              DT_THROW_IF(!encoding_driver_type_id.from_string(encoding_driver_type_id_repr),
                          std::logic_error,
                          "Invalid encoding type ID representation '" << encoding_driver_type_id_repr << "'!");
            }
          }
          if (transport_driver_type_id.is_valid()) { 
            di.transport_driver_type_id = transport_driver_type_id;
          }
          if (encoding_driver_type_id.is_valid()) {
            di.encoding_driver_type_id = encoding_driver_type_id;
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
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (_pimpl_) {
        for (auto & p : _pimpl_->transport_managers) {
          if (p.second.pmgr->is_initialized()) {
            DT_LOG_DEBUG(get_logging_priority(), "Reset transport manager '" << p.first << "' : ");
            if (datatools::logger::is_debug(get_logging_priority())) {
              p.second.pmgr->print_tree(std::cerr);
            }
            p.second.pmgr->reset();
          }
        }
        _pimpl_->transport_driver_to_managers.clear();
        _pimpl_->transport_managers.clear();
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::_at_init_transport_managers_(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      std::set<std::string> transmgr_names;
      if (config_.has_key("names")) {
        config_.fetch("names", transmgr_names);
        for (const auto & trans_name : transmgr_names) {
          DT_LOG_DEBUG(get_logging_priority(), "Initializing transport manager '" << trans_name << "'...");
          // Type of transport manager:
          std::ostringstream transmgr_type_id_key;
          transmgr_type_id_key << trans_name << ".type_id";
          DT_THROW_IF(!config_.has_key(transmgr_type_id_key.str()),
                      std::logic_error,
                      "Missing '" << trans_name << "' transport manager type ID!");
          std::string transmgr_type_id = config_.fetch_string(transmgr_type_id_key.str());
          DT_LOG_DEBUG(get_logging_priority(), "Transport manager type = '" << transmgr_type_id << "'.");
          DT_THROW_IF(transmgr_type_id != "vire::rabbitmq::transport_manager",
                      std::logic_error,
                      "Transport manager of type '" << transmgr_type_id  << "' is not supported!");
          // Configuration of transport manager:
          std::ostringstream transmgr_config_path_key;
          transmgr_config_path_key << trans_name << ".config_path";
          DT_THROW_IF(!config_.has_key(transmgr_config_path_key.str()),
                      std::logic_error,
                      "Missing '" << trans_name << "' transport manager config path!");
          std::string transmgr_config_path = config_.fetch_path(transmgr_config_path_key.str());
          datatools::fetch_path_with_env(transmgr_config_path);
          datatools::properties transmgr_config;
          transmgr_config.read_configuration(transmgr_config_path);
          if (datatools::logger::is_debug(get_logging_priority())) {
            DT_LOG_DEBUG(get_logging_priority(), "Transport manager configuration: ");
            transmgr_config.print_tree(std::cerr);
          }
          // Type of transport driver:
          std::ostringstream transmgr_driver_type_id_key;
          transmgr_driver_type_id_key << trans_name << ".driver_type_id";
          DT_THROW_IF(!config_.has_key(transmgr_driver_type_id_key.str()),
                      std::logic_error,
                      "Missing transport driver type ID associated with the '" << trans_name << "' transport manager!");
          std::string transmgr_driver_type_id = config_.fetch_path(transmgr_driver_type_id_key.str());

          i_transport_manager::factory_register_type & sys_factory_register
            =  DATATOOLS_FACTORY_GRAB_SYSTEM_REGISTER(i_transport_manager);
          DT_THROW_IF(! sys_factory_register.has(transmgr_type_id),
                      std::logic_error,
                      "No transport manager type ID '" << transmgr_type_id
                      << "' factory is known from the system register!");
          const i_transport_manager::factory_register_type::factory_type & the_factory
            = sys_factory_register.get(transmgr_type_id);
          transport_manager_ptr transmgrPtr(the_factory());
          if (has_system_access_login_prefix()) {
            if (!transmgr_config.has_key("system_user_name_prefix")) {
              transmgr_config.store("system_user_name_prefix", _system_access_login_prefix_);
            }
          }
          transmgrPtr->set_name(trans_name);
          transmgrPtr->initialize(transmgr_config);
          transport_manager_entry tme;
          tme.name = trans_name;
          tme.manager_type_id = transmgr_type_id;
          tme.driver_type_id = transmgr_driver_type_id;
          tme.config = transmgr_config;
          tme.pmgr = transmgrPtr;
          _pimpl_->transport_managers[trans_name] = tme;
          _pimpl_->transport_driver_to_managers[transmgr_driver_type_id] = trans_name;
        }
      }
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }
    
    void manager::_at_init_(const datatools::properties & config_)
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      // DT_THROW_IF(!has_resources(), std::logic_error, "Missing 'resources' service!");
      DT_THROW_IF(!has_app_category(), std::logic_error, "Missing application category!");
      if (get_app_category() == vire::cms::application::CATEGORY_SERVER) {
        DT_THROW_IF(!has_system_access_login_prefix(), std::logic_error, "Missing system access login prefix!");
      }
      DT_THROW_IF(!has_domain_name_prefix(), std::logic_error, "Missing domain name prefix!");
      DT_THROW_IF(!has_default_transport_driver_type_id(), std::logic_error, "Missing default transport driver type identifier!");
      DT_THROW_IF(!has_default_encoding_driver_type_id(), std::logic_error, "Missing default encoding driver type identifier!");
      
      DT_LOG_DEBUG(get_logging_priority(), "Initializing domain maker...");
      _domain_maker_.set_domain_name_prefix(_domain_name_prefix_);
      _domain_maker_.set_transport_driver_type_id(_default_transport_driver_type_id_);
      _domain_maker_.set_encoding_driver_type_id(_default_encoding_driver_type_id_);

      _pimpl_.reset(new pimpl_type(*this));

      if (_transport_management_) {
        DT_LOG_DEBUG(get_logging_priority(), "Initializing transport managers...");
        datatools::properties transport_manager_config;
        config_.export_and_rename_starting_with(transport_manager_config,
                                                "transport_manager.",
                                                "");
        _at_init_transport_managers_(transport_manager_config);
      }
      
      DT_LOG_DEBUG(get_logging_priority(), "Building default domains...");
      _build_default_domains_();
          
      DT_LOG_DEBUG(get_logging_priority(), "Building default access profiles...");
      _build_default_access_profiles_();
  
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::_at_reset_()
    {
      DT_LOG_TRACE_ENTERING(get_logging_priority());
      if (_pimpl_) {
        _pimpl_.reset();
      }
      _access_profiles_.clear();
      _domains_.clear();
      _resources_ = nullptr;

      if (_transport_management_) {
        DT_LOG_DEBUG(get_logging_priority(), "Reset transport managers...");
        _at_reset_transport_managers_();
        DT_LOG_DEBUG(get_logging_priority(), "Transport managers have been destroyed.");
      }
      _domain_maker_.reset();
     
      _domain_infos_.clear();
      _access_profile_infos_.clear();
      _default_encoding_driver_type_id_.reset();
      _default_transport_driver_type_id_.reset();
      _system_access_login_prefix_.clear();
      _domain_name_prefix_.clear();
      _resources_service_name_.clear();
      _transport_management_ = false;
      _app_category_ = vire::cms::application::CATEGORY_UNDEF;
      DT_LOG_TRACE_EXITING(get_logging_priority());
      return;
    }

    void manager::_build_default_access_profiles_()
    {
      for (access_profile_info_dict_type::const_iterator i = _access_profile_infos_.begin();
           i != _access_profile_infos_.end();
           i++) {
        const access_profile_info_type & api = i->second;
        DT_LOG_DEBUG(get_logging_priority(), "About to create default access profile '" << api.name << "' of category '" << to_string(api.category) << "'.");
        bool add_it = false;
        if (api.category == ACCESS_CATEGORY_SERVER_GATE || 
            api.category == ACCESS_CATEGORY_SERVER_CMS || 
            api.category == ACCESS_CATEGORY_CLIENT_GATE) {
          if (_app_category_ == vire::cms::application::CATEGORY_SERVER) {
            add_it = true;
          }
        }
        if (api.category == ACCESS_CATEGORY_CLIENT_GATE){
          if (_app_category_ == vire::cms::application::CATEGORY_CLIENT) {
            add_it = true;
          }
        }
        if (add_it) {
          DT_LOG_DEBUG(get_logging_priority(),
                       "Creating default access profile '" << api.name << "' of category '" << to_string(api.category) << "'...");
          vire::com::access_profile & ap = this->create_access_profile(api.name,
                                                                     api.password,
                                                                     api.category);
          DT_LOG_DEBUG(get_logging_priority(), "Default access profile '" << api.name << "' has been created.");
        }
      }
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
          domain_info_dict_type::const_iterator found = _domain_infos_.find(domain_gate_label());
          const domain_info_type & dom_info = found->second;
          DT_LOG_DEBUG(get_logging_priority(), "Creating domain '" << gate_sys_domain_name << "'...");
          vire::com::domain & gate_sys_domain = this->create_domain(gate_sys_domain_name,
                                                                    dom_info.category,
                                                                    dom_info.transport_driver_type_id,
                                                                    dom_info.encoding_driver_type_id);  
        }
      }
     
      {
        // Check and create monitoring domain:
        std::string monitoring_domain_name
          = vire::com::domain_builder::build_cms_monitoring_name(this->get_domain_maker().get_domain_name_prefix());
        if (!this->has_domain(monitoring_domain_name)) {

          domain_info_dict_type::const_iterator found = _domain_infos_.find(domain_monitoring_label());
          const domain_info_type & dom_info = found->second;
          DT_LOG_DEBUG(get_logging_priority(), "Creating domain '" << monitoring_domain_name << "'...");
          vire::com::domain & monitoring_domain = this->create_domain(monitoring_domain_name,
                                                                      dom_info.category,
                                                                      dom_info.transport_driver_type_id,
                                                                      dom_info.encoding_driver_type_id); 
        }
      }
      
      if (_app_category_ == vire::cms::application::CATEGORY_SERVER
          || _app_category_ == vire::cms::application::CATEGORY_CLIENT) {
        // Check and create control domain:
        std::string control_domain_name
          = vire::com::domain_builder::build_cms_control_name(this->get_domain_maker().get_domain_name_prefix());
        if (!this->has_domain(control_domain_name)) {
          domain_info_dict_type::const_iterator found = _domain_infos_.find(domain_control_label());
          const domain_info_type & dom_info = found->second;
          DT_LOG_DEBUG(get_logging_priority(), "Creating domain '" << control_domain_name << "'...");
          vire::com::domain & control_domain = this->create_domain(control_domain_name,
                                                                   dom_info.category,
                                                                   dom_info.transport_driver_type_id,
                                                                   dom_info.encoding_driver_type_id); 
        }
      }
      
      return;
    }
     
    void manager::print_tree(std::ostream & out_,
                             const boost::property_tree::ptree & options_) const
    {
      base_print_options popts;
      popts.configure_from(options_);
      std::ostringstream outs;
      this->base_service::print_tree(outs,
                                     base_print_options::force_inheritance(options_));
              
      outs << popts.indent << tag
           << "Application category : "
           << "'" << vire::cms::application::to_string(this->_app_category_) << "'" << std::endl;
 
      outs << popts.indent << tag
           << "Transport management : "
           << std::boolalpha << _transport_management_ << std::endl;
 
      outs << popts.indent << tag
           << "Resources service name : "
           << "'" << _resources_service_name_ << "'" << std::endl;
 
      outs << popts.indent << tag
           << "System access login prefix : "
           << "'" << _system_access_login_prefix_ << "'" << std::endl;
 
      outs << popts.indent << tag
           << "Domain name prefix : "
           << "'" << _domain_name_prefix_ << "'" << std::endl;
 
      outs << popts.indent << tag
           << "Default transport type ID : "
           << "'" << _default_transport_driver_type_id_.to_string() << "'" << std::endl;
 
      outs << popts.indent << tag
           << "Default encoding type ID : "
           << "'" << _default_encoding_driver_type_id_.to_string() << "'" << std::endl;

      outs << popts.indent << tag
           << "Access profile infos : "
           << _access_profile_infos_.size()
           << std::endl;
      for (access_profile_info_dict_type::const_iterator i = _access_profile_infos_.begin();
           i != _access_profile_infos_.end();
           i++) {
        outs <<  popts.indent << skip_tag;
        access_profile_info_dict_type::const_iterator j = i;
        j++;
        if (j == _access_profile_infos_.end()) {
          outs << last_tag;
        } else {
          outs << tag;
        }
        outs << "Label '" << i->first << "' (category='" << vire::com::to_string(i->second.category) << "')";
        outs << std::endl;
      }
 
      outs << popts.indent << tag
           << "Domain maker : "
           << std::endl;
      {
        boost::property_tree::ptree options2;
        options2.put(base_print_options::indent_key(), popts.indent + tags::skip_item());
        _domain_maker_.print_tree(outs, options2);
      }
      outs << popts.indent << tag
           << "Domain infos : "
           << _domain_infos_.size()
           << std::endl;
      for (domain_info_dict_type::const_iterator i = _domain_infos_.begin();
           i != _domain_infos_.end();
           i++) {
        outs <<  popts.indent << skip_tag;
        domain_info_dict_type::const_iterator j = i;
        j++;
        if (j ==_domain_infos_.end()) {
          outs << last_tag;
        } else {
          outs << tag;
        }
        outs << "Label '" << i->first << "' (category='" << vire::com::to_string(i->second.category) << "')";
        outs << std::endl;
      }
  
      outs << popts.indent << tag
           << "Resources services : ";
      if (_resources_ == nullptr) {
        outs << "<none>";
      } else {
        outs << "'" << _resources_->get_name() << "'";
      }
      outs << std::endl;
  
      outs << popts.indent << tag
           << "Domains : ";
      if (_domains_.size()) {
        outs << std::endl;
        for (domain_dict_type::const_iterator i = _domains_.begin();
             i != _domains_.end();
             i++) {
          outs << popts.indent << skip_tag;
          domain_dict_type::const_iterator j = i;
          if (++j == _domains_.end()) {
            outs << last_tag;
          } else {
            outs << tag;
          }
          outs << "Name='" << i->first << "' (category='"
               << to_string(i->second.get()->get_category()) << "')"
               << std::endl;
        }
      } else {
        outs << "<none>" << std::endl;
      }

      if (_pimpl_) {
        outs << popts.indent << tag
             << "Transport managers : ";
        if (_resources_ == nullptr) {
          outs << "<none>";
        } else {
          outs << _pimpl_->transport_managers.size();
        }
        outs << std::endl;
        if (_pimpl_->transport_managers.size()) {
          for (std::map<std::string, transport_manager_entry>::const_iterator i = _pimpl_->transport_managers.begin();
               i != _pimpl_->transport_managers.end();
               i++) {
            outs << popts.indent << skip_tag;
            std::map<std::string, transport_manager_entry>::const_iterator j = i;
            if (++j == _pimpl_->transport_managers.end()) {
              outs << last_tag;
            } else {
              outs << tag;
            }
            outs << "Name='" << i->first << "' of type '" << i->second.manager_type_id << "'" 
                 << std::endl;
          }
        }       
      }
      
      if (!popts.inherit) {
        outs << popts.indent << inherit_tag(popts.inherit)
             << "End."
             << std::endl;
      }

      out_ << outs.str();
      return;
    }

  } // namespace com

} // namespace vire
