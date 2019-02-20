//! \file vire/rabbitmq/transport_manager.cc
//
// Copyright (c) 2018-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/rabbitmq/transport_manager.h>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/library_loader.h>
// #include <bayeux/datatools/detail/DynamicLoader.h>

// This project:
#include <vire/resource_files.h>
#include <vire/rabbitmq/manager_service.h>

namespace vire {

  namespace rabbitmq {
      
    VIRE_COM_TRANSPORT_MANAGER_REGISTRATION_IMPLEMENT(transport_manager,
                                                      "vire::rabbitmq::transport_manager")

    struct transport_manager::pimpl_type
    {
      std::unique_ptr<vire::rabbitmq::manager_service> rabbitmq_mgr;
    };

    transport_manager::transport_manager()
    {
      _pimpl_.reset(new pimpl_type);
      return;
    }

    transport_manager::~transport_manager()
    {
      if (is_initialized()) {
        this->transport_manager::reset();
      }
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ transport manager was not properly reset!");
      _pimpl_.reset();
      return;
    }
    
    void transport_manager::_at_initialize_(const datatools::properties & config_)
    {
      _pimpl_->rabbitmq_mgr.reset(new vire::rabbitmq::manager_service);
      _pimpl_->rabbitmq_mgr->initialize_standalone(config_);
      return;
    }

    void transport_manager::_at_reset_()
    {
      if (_pimpl_) {
        if (_pimpl_->rabbitmq_mgr) {
          if (_pimpl_->rabbitmq_mgr->is_initialized()) {
            _pimpl_->rabbitmq_mgr->reset();
          }
          _pimpl_->rabbitmq_mgr.reset();
        }
      }
      return;
    }

    bool transport_manager::has_domain(const std::string & domain_name_) const
    {
      return _pimpl_->rabbitmq_mgr->has_vhost(domain_name_);
    }
        
    void transport_manager::add_domain(const std::string & domain_name_,
                                       const vire::com::domain_category_type category_)
    {
      DT_LOG_DEBUG(get_logging(), "Adding domain/vhost '" << domain_name_ << "'...");
      DT_THROW_IF(has_domain(domain_name_), std::logic_error,
                  "Domain/vhost '" << domain_name_ << "' already exists!");
      vhost vh(domain_name_, category_);
      _pimpl_->rabbitmq_mgr->add_vhost(vh);
      if (category_ == vire::com::DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM) {
        
      }
      return;
    }
        
    void transport_manager::remove_domain(const std::string & domain_name_)
    {
      DT_LOG_DEBUG(get_logging(), "Removing domain/vhost '" << domain_name_ << "'...");
      DT_THROW_IF(!has_domain(domain_name_), std::logic_error,
                  "Domain/vhost '" << domain_name_ << "' does not exist!");
      _pimpl_->rabbitmq_mgr->remove_vhost(domain_name_);
      return;
    }

    bool transport_manager::has_user(const std::string & login_) const
    {
      return _pimpl_->rabbitmq_mgr->has_user(login_);
    }
        
    void transport_manager::add_user(const std::string & login_,
                                     const std::string & password_,
                                     const vire::com::access_category_type category_)
    {
      DT_LOG_DEBUG(get_logging(), "Adding user '" << login_ << "'...");
      DT_THROW_IF(has_user(login_), std::logic_error,
                  "User '" << login_ << "' already exists!");
      user u(login_, password_, category_);
      _pimpl_->rabbitmq_mgr->add_user(u);
      return;
    }

    void transport_manager::remove_user(const std::string & login_) 
    {
      DT_LOG_DEBUG(get_logging(), "Removing user '" << login_ << "'...");
      DT_THROW_IF(has_user(login_), std::logic_error,
                  "User '" << login_ << "' does not exist!");
      _pimpl_->rabbitmq_mgr->remove_user(login_);
      return;
    }

    void transport_manager::add_subcontractor(const vire::com::subcontractor_info & sc_info_) 
    {
      _pimpl_->rabbitmq_mgr->add_subcontractor(sc_info_);
      return;
    }
      
    void transport_manager::remove_subcontractor(const vire::com::subcontractor_info & sc_info_) 
    {
      _pimpl_->rabbitmq_mgr->remove_subcontractor(sc_info_.id);
      return;
    }

    void transport_manager::add_client(const vire::com::client_info & client_info_) 
    {
      //_pimpl_->rabbitmq_mgr->add_client(client_info_);
      return;
    }
      
    void transport_manager::remove_client(const vire::com::client_info & client_info_) 
    {
      //_pimpl_->rabbitmq_mgr->remove_client(client_info_.id);
      return;
    }

    // void transport_manager::grant_access_to(const std::string & user_login_,
    //                                         const access_category_type user_lcategory_,
    //                                         const std::string & domain_name_,
    //                                         const domain_category_type domain_category_)
    // {
    //   DT_LOG_DEBUG(get_logging(), "Setting permissions for user '" << login_ << "' with respect to domain/vhost '" << domain_name_ << "'...");
    //   return;
    // }

  } // namespace rabbitmq

} // namespace vire
