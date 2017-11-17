//! \file test_manager_service.cxx
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
  datatools::logger::priority logging = datatools::logger::PRIO_TRACE;
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

  // // The Vire server user (default):
  // vire::rabbitmq::user server_user("vireserver", "vireserver", vire::com::actor::CATEGORY_SERVER);
  // if (!serv.has_server_user()) {
  //   DT_LOG_DEBUG(logging, "Add server user '" << server_user.login << "'...");
  //   serv.add_server_user(server_user);
  // }

  // The Vire subcontractor users:
  vire::rabbitmq::user cmslapp_user("cmslapp", "cmslapp", vire::com::actor::CATEGORY_SUBCONTRACTOR);
  // cmslapp_user.static_domain = "cmslapp";
  if (!serv.has_static_user(cmslapp_user.get_login())) {
    DT_LOG_DEBUG(logging, "Add static user '" << cmslapp_user.get_login() << "'...");
    serv.add_static_user(cmslapp_user);
  }

  vire::rabbitmq::user orleans_user("orleans", "orleans", vire::com::actor::CATEGORY_SUBCONTRACTOR);
  if (!serv.has_static_user(orleans_user.get_login())) {
    DT_LOG_DEBUG(logging, "Add static user '" << orleans_user.get_login() << "'...");
    serv.add_static_user(orleans_user);
  }

  serv.initialize_simple();
  serv.tree_dump(std::clog, "RabbitMQ manager service: ", "[info] ");

  // Add a client with access to both monitoring and control domains:
  serv.add_client_user("wxCVbn", "wxCVbn", true, true);
  // Add a client with access to only monitoring domain:
  serv.add_client_user("gtrYzD", "gtrYzD", true , false);

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
