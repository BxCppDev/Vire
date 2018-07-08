//! \file vire/rabbitmq/transport_manager.cc
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
      _pimpl_.reset();
      DT_THROW_IF(is_initialized(), std::logic_error,
                  "RabbitMQ transport manager was not properly reset!");
      return;
    }
    
    void transport_manager::_at_initialize_(const datatools::properties & config_)
    {
      datatools::properties rabbitmgr_config;
      config_.export_and_rename_starting_with(rabbitmgr_config, "rabbitmq.", "");
      _pimpl_->rabbitmq_mgr.reset(new vire::rabbitmq::manager_service);
      _pimpl_->rabbitmq_mgr->initialize_standalone(rabbitmgr_config);
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
      return false;
    }
        
    void transport_manager::add_domain(const std::string & domain_name_,
                                       const vire::com::domain_category_type category_)
    {
      return;
    }
        
    void transport_manager::remove_domain(const std::string & domain_name_)
    {
      return;
    }

    bool transport_manager::has_user(const std::string & login_) const
    {
      return false;
    }
        
    void transport_manager::add_user(const std::string & login_,
                                     const std::string & password_,
                                     const vire::com::actor_category_type category_)
    {
      return;
    }

    void transport_manager::remove_user(const std::string & login_) 
    {
      return;
    }

  } // namespace rabbitmq

} // namespace vire
