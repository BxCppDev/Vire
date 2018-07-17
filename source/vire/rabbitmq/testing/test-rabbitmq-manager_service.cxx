//! \file test-rabbitmq-manager_service.cxx
//
// Copyright (c) 2017-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <vire/rabbitmq/manager_service.h>

// Standard library:
#include <iostream>

// Third party:
// - Bayeux/datatools
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/utils.h>
// - Bayeux/rabbitmq
#include <bayeux/rabbitmq/rabbit_mgr.h>

// This project:
#include <vire/com/domain_builder.h>

void test0();

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {

    test0();

  } catch (std::exception& error) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(datatools::logger::PRIO_FATAL,
                 "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  return error_code;
}

void report(::rabbitmq::rabbit_mgr & mgr_);

void test0()
{
  std::clog << "\ntest0: Entering..." << std::endl;
  datatools::logger::priority logging = datatools::logger::PRIO_DEBUG;
  vire::rabbitmq::manager_service serv;
  serv.set_name("RabbitMQManager");
  serv.set_display_name("RabbitMQ Manager Service");
  serv.set_terse_description("The service dedicated to the RabbitMQ server management");
  serv.set_logging_priority(logging);
  serv.set_server_host("localhost");
  serv.set_server_port(15672);
  serv.set_vhost_name_prefix("/supernemo/demonstrator");
  serv.set_admin_login("supernemo_adm");
  serv.set_admin_password("sesame");
  // serv.set_destroy_all_at_reset(true);

  // The Vire server user (default):
  vire::rabbitmq::user server_cms_user("vireservercms", "vireservercms", vire::com::ACTOR_CATEGORY_SERVER_CMS);
  DT_LOG_DEBUG(logging, "Add system user '" << server_cms_user.get_login() << "'...");
  serv.add_system_user(server_cms_user);
  
  vire::rabbitmq::user server_gate_user("vireservergate", "vireservergate", vire::com::ACTOR_CATEGORY_SERVER_GATE);
  DT_LOG_DEBUG(logging, "Add system user '" << server_gate_user.get_login() << "'...");
  serv.add_system_user(server_gate_user);
  
  vire::rabbitmq::user client_gate_user("vireclientgate", "vireclientgate", vire::com::ACTOR_CATEGORY_CLIENT_GATE);
  DT_LOG_DEBUG(logging, "Add system user '" << client_gate_user.get_login() << "'...");
  serv.add_system_user(client_gate_user);

  serv.initialize_simple();
  serv.tree_dump(std::clog, "RabbitMQ manager service: ", "[info] ");

  // Subcontractors:

  {
    std::string sc_id = "cmslapp";

    {
      // Vhost:
      std::string vhost_name
        = vire::com::domain_builder::build_cms_subcontractor_system_name(serv.get_vhost_name_prefix(),
                                                                         sc_id);
      vire::rabbitmq::vhost vh(vhost_name,
                               vire::com::DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM);
      if (!serv.has_vhost(vh.get_name())) {
        DT_LOG_DEBUG(logging, "Add subcontractor system vhost '" << vh.get_name() << "'...");
        serv.add_vhost(vh);
      }
    }

    {
      // Server side:
      std::string sc_user_login = "sys." + sc_id;
      vire::rabbitmq::user sc_user(sc_user_login,
                                   "Dsdqexsk34",
                                   vire::com::ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM);
      if (!serv.has_user(sc_user.get_login())) {
        DT_LOG_DEBUG(logging, "Add server subcontractor system user '" << sc_user.get_login() << "'...");
        serv.add_user(sc_user);
      }
    }

    {
      // Subcontractor side:
      std::string sc_user_login = sc_id;
      vire::rabbitmq::user sc_user(sc_user_login,
                                   "cmslapp",
                                   vire::com::ACTOR_CATEGORY_SUBCONTRACTOR);
      if (!serv.has_user(sc_user.get_login())) {
        DT_LOG_DEBUG(logging, "Add subcontractor user '" << sc_user.get_login() << "'...");
        serv.add_user(sc_user);
      }
    }
    
  }

  {
    std::string sc_id = "orleans";

    {
      // Vhost:
      std::string vhost_name
        = vire::com::domain_builder::build_cms_subcontractor_system_name(serv.get_vhost_name_prefix(),
                                                                         sc_id);
      vire::rabbitmq::vhost vh(vhost_name,
                               vire::com::DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM);
      if (!serv.has_vhost(vh.get_name())) {
        DT_LOG_DEBUG(logging, "Add subcontractor system vhost '" << vh.get_name() << "'...");
        serv.add_vhost(vh);
      }
    }

    {
      // Server side:
      std::string sc_user_login = "sys." + sc_id;
      vire::rabbitmq::user sc_user(sc_user_login,
                                   "6E8AEDd",
                                   vire::com::ACTOR_CATEGORY_SERVER_SUBCONTRACTOR_SYSTEM);
      if (!serv.has_user(sc_user.get_login())) {
        DT_LOG_DEBUG(logging, "Add server subcontractor system user '" << sc_user.get_login() << "'...");
        serv.add_user(sc_user);
      }
    }
    
    {
      // Subcontractor side:
      std::string sc_user_login = sc_id;
      vire::rabbitmq::user sc_user(sc_user_login,
                                   "orleans",
                                   vire::com::ACTOR_CATEGORY_SUBCONTRACTOR);
      if (!serv.has_user(sc_user.get_login())) {
        DT_LOG_DEBUG(logging, "Add subcontractor user '" << sc_user.get_login() << "'...");
        serv.add_user(sc_user);
      }
    }
    
  }

  
  // Clients:

  {
    std::string client_id = "wxCVbn";

    {
      // Vhost:
      std::string vhost_name
        = vire::com::domain_builder::build_cms_client_system_name(serv.get_vhost_name_prefix(),
                                                                  client_id);
      vire::rabbitmq::vhost vh(vhost_name,
                               vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM);
      if (!serv.has_vhost(vh.get_name())) {
        DT_LOG_DEBUG(logging, "Add client system vhost '" << vh.get_name() << "'...");
        serv.add_vhost(vh);
      }
    }
  
    {
      // Server side:
      std::string user_login = "sys_server." + client_id;
      vire::rabbitmq::user user(user_login,
                                "Shh2D89mjD",
                                vire::com::ACTOR_CATEGORY_SERVER_CLIENT_SYSTEM);
      if (!serv.has_user(user.get_login())) {
        DT_LOG_DEBUG(logging, "Add server client system user '" << user.get_login() << "'...");
        serv.add_user(user);
      }
    }

    {
      // Client side:
      std::string sys_user_login = "sys_client." + client_id;
      vire::rabbitmq::user sys_user(sys_user_login,
                                    "sys.wxCVbn",
                                    vire::com::ACTOR_CATEGORY_CLIENT_SYSTEM);
      if (!serv.has_user(sys_user.get_login())) {
        DT_LOG_DEBUG(logging, "Add client CMS user '" << sys_user.get_login() << "'...");
        serv.add_user(sys_user);
      }

      std::string cms_user_login = client_id;
      vire::rabbitmq::user cms_user(cms_user_login,
                                    "Gez9dZdf",
                                    vire::com::ACTOR_CATEGORY_CLIENT_CMS);
      if (!serv.has_user(cms_user.get_login())) {
        DT_LOG_DEBUG(logging, "Add client CMS user '" << cms_user.get_login() << "'...");
        serv.add_user(cms_user);
      }
    }
  }

  // Checks:
  ::rabbitmq::rabbit_mgr & mgr = serv.grab_manager();
  report(mgr);

  serv.reset();

  std::clog << "\ntest0: Exiting." << std::endl;
  return;
}

void report(::rabbitmq::rabbit_mgr & mgr_)
{

  {
    ::rabbitmq::user::list users;
    ::rabbitmq::error_response err;
    mgr_.list_users(users, err);
    std::clog << "Number of users : " << users.size() << std::endl;
    for (const auto & u : users) {
      std::clog << " - user : '" << u.name << "'" << std::endl;
    }
  }

  {
    ::rabbitmq::vhost::list vhosts;
    ::rabbitmq::error_response err;
    mgr_.list_vhosts(vhosts, err);
    std::clog << "Number of virtual hosts : " << vhosts.size() << std::endl;
    for (auto v : vhosts) {
      std::clog << " - vhost : '" << v.name << "'" << std::endl;
    }
  }

  return;
}
