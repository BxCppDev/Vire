//! \file test-rabbitmq-manager_service.cxx
//
// Copyright (c) 2017-2019 by François Mauger <mauger@lpccaen.in2p3.fr>
//                            Jean Hommet <hommet@lpccaen.in2p3.fr>
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

struct params_type
{
  bool destroy = false;
  bool interactive = false;
  bool clear_password = false;
};

void test0(const params_type & params_);
void test1(const params_type & params_);
void service_set_params(vire::rabbitmq::manager_service & serv_,
                        const bool destroy_ = false);

void mgr_report(::rabbitmq::rabbit_mgr & mgr_);

int main( int argc_, char * argv_[])
{
  int error_code = EXIT_SUCCESS;

  try {
    params_type params;

    {
      // Command-line parsing:
      int iarg = 1;
      while (iarg < argc_) {
        std::string cl_token = argv_[iarg++];
        if (cl_token == "--destroy") {
          params.destroy = true;
        } else if (cl_token == "--interactive") {
          params.interactive = true;
        } else if (cl_token == "--clear-password") {
          params.clear_password = true;
        }
      }
    }
    
    test0(params);
    // test1(params);

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

void service_set_params(vire::rabbitmq::manager_service & serv_, const bool destroy_)
{
  serv_.set_name("RabbitMQManager");
  serv_.set_display_name("RabbitMQ Manager Service");
  serv_.set_terse_description("The service dedicated to the RabbitMQ server management");
  serv_.set_server_host("localhost");
  serv_.set_server_port(15672);
  serv_.set_system_user_name_prefix("__snemod__");
  serv_.set_vhost_name_prefix("/__snemod__");
  // serv_.set_vhost_name_prefix("/supernemo/demonstrator");
  serv_.set_admin_login("supernemo_adm");
  serv_.set_admin_password("sesame");
  // System users:
  serv_.set_system_user_password(vire::com::ACCESS_CATEGORY_SERVER_CMS,  "vireservercms");
  serv_.set_system_user_password(vire::com::ACCESS_CATEGORY_SERVER_GATE, "vireservergate");
  serv_.set_system_user_password(vire::com::ACCESS_CATEGORY_CLIENT_GATE, "vireclientgate");
  serv_.set_destroy_all_at_reset(destroy_);

  return;
}

void test1(const params_type & params_)
{
  std::clog << "\ntest1: Entering..." << std::endl;

  datatools::logger::priority logging = datatools::logger::PRIO_DEBUG;
  vire::rabbitmq::manager_service serv;
  serv.set_logging_priority(logging);
  service_set_params(serv, params_.destroy);

  
  // ::rabbitmq::rabbit_mgr & mgr = serv.grab_manager();
  // service_report(mgr);

  // serv.reset();

  std::clog << "\ntest0: Exiting." << std::endl;
  return;
}

void test0(const params_type & params_)
{
  std::clog << "\ntest0: Entering..." << std::endl;
  datatools::logger::priority logging = datatools::logger::PRIO_DEBUG;
  vire::rabbitmq::manager_service serv;
  serv.set_logging_priority(logging);
  service_set_params(serv, params_.destroy);

  serv.initialize_simple();
  {
    boost::property_tree::ptree options;
    options.put("title", serv.get_display_name() + " : ");
    options.put("indent", "[info] ");
    serv.print_tree(std::clog, options);
  }

  // Checks:
  ::rabbitmq::rabbit_mgr & mgr = serv.grab_manager();
  mgr_report(mgr);

  // Subcontractors:
  vire::com::subcontractor_info sc_info1;
  {
    sc_info1.id            = "cmslapp";
    sc_info1.description   = "CMS/LAPP interface";
    sc_info1.user_login    = "cmslapp";
    sc_info1.user_password = "cmslapp";
    sc_info1.sys_svr_login    = "__syscmslapp__";
    sc_info1.sys_svr_password = "__syscmslapp__";
    sc_info1.persistent    = true;
    serv.add_subcontractor(sc_info1);
  }
 
  vire::com::subcontractor_info sc_info2;
  {
    sc_info2.id            = "orleans";
    sc_info2.description   = "Orleans CMS interface";
    sc_info2.user_login    = "orleans";
    sc_info2.user_password = "orleans";
    sc_info2.sys_svr_login    = "__sysorleans__";
    sc_info2.sys_svr_password = "__sysorleans__";
    sc_info2.persistent    = false;
    serv.add_subcontractor(sc_info2);
  }

  vire::com::client_info cl_info;
  {
    cl_info.id = "XYZTUVW";
    cl_info.description = "A remote client";
    cl_info.sys_user_login    = "XYZTUVW.sys";
    cl_info.sys_user_password = "XYZTUVW";
    cl_info.sys_svr_login     = "XYZTUVW.svrsys";
    cl_info.sys_svr_password  = "XYZTUVW";
    cl_info.cms_user_login    = "XYZTUVW";
    cl_info.cms_user_password = "XYZTUVW";
    cl_info.with_control = false;
    serv.add_client(cl_info);
  }

  if (params_.interactive) {
    std::cout << "Type [Enter] to continue..." << std::endl;
    std::string word;
    std::getline(std::cin, word);
  }

  if (params_.clear_password) {
    serv.change_client_passwords(cl_info.id, "", "");
  } else {
    serv.change_client_passwords(cl_info.id, "WVUTZYX", "WVUTZYX");
  }
  
  if (params_.interactive) {
    std::cout << "Type [Enter] to quit..." << std::endl;
    std::string word;
    std::getline(std::cin, word);
  }
  
  serv.remove_client(cl_info.id);
  serv.remove_subcontractor(sc_info2.id);
  serv.remove_subcontractor(sc_info1.id);
  
  serv.reset();

  std::clog << "\ntest0: Exiting." << std::endl;
  return;
}

void mgr_report(::rabbitmq::rabbit_mgr & mgr_)
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
