//! \file vire/com/testing/test-com-domain.cxx
//! \brief Test Vire communication domain
//
// Copyright (c) 2016 by François Mauger <mauger@lpccaen.in2p3.fr>
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

// Standard library:
#include <cstdlib>
#include <string>
#include <thread>
#include <atomic>
#include <iostream>
#include <map>

// This project:
#include <vire/vire.h>
#include <vire/com/domain.h>

void test_com_domain_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::com::domain' class." << std::endl;

    bool interactive = false;
    interactive = true;

    test_com_domain_1(interactive);

    std::clog << "The end." << std::endl;
  } catch (std::exception & x) {
    std::cerr << "error: " << x.what() << std::endl;
    error_code = EXIT_FAILURE;
  } catch (...) {
    std::cerr << "error: " << "unexpected error !" << std::endl;
    error_code = EXIT_FAILURE;
  }
  vire::terminate();
  return error_code;
}

void test_com_domain_1(bool interactive_)
{
  std::clog << "\ntest_com_domain_1: basics" << std::endl;

  std::string setup_name   = "__snemod__";
  std::string transport_driver_id = "vire::rabbitmq::transport_driver";
  std::string encoding_driver_id  = "vire::com::protobuf_encoding_driver";

  // Domain #0:
  std::clog << "\ntest_com_domain_1: Domain #0" << std::endl;
  // vire::com::domain dom0;
  // dom0.set_name("/");
  // dom0.set_category(vire::com::domain::CATEGORY_GATE);
  // dom0.set_transport_driver_type_id(transport_driver_id);
  // dom0.set_encoding_driver_type_id(encoding_driver_id);
  // dom0.add_mailbox("X1",
  //                  vire::com::mailbox::MODE_SERVICE,
  //                  vire::com::mailbox::PRIVACY_PUBLIC,
  //                  "vire.snemo.cms.main.service1",
  //                  "p-------s");
  // dom0.add_mailbox("X2",
  //                  vire::com::mailbox::MODE_SERVICE,
  //                  vire::com::mailbox::PRIVACY_PUBLIC,
  //                  "vire.snemo.cms.main.service2",
  //                  "p-------s");
  // dom0.add_mailbox("Q3",
  //                  vire::com::mailbox::MODE_EVENT,
  //                  vire::com::mailbox::PRIVACY_PRIVATE,
  //                  "vire.snemo.cms.main.alarm",
  //                  "-k----p--");
  // dom0.tree_dump(std::clog, "Default communication domain: ");
  // std::clog << std::endl;

  // Domain #1:
  std::clog << "\ntest_com_domain_1: Domain #1" << std::endl;
  vire::com::domain dom1;
  std::string subcontractor_name = "cmslapp";
  dom1.set_name("/" + setup_name + "/cms/vire/subcontractors/system/" + subcontractor_name);
  dom1.set_category(vire::com::DOMAIN_CATEGORY_SUBCONTRACTOR_SYSTEM);
  dom1.add_mailbox("RequestToServer",
                   vire::com::mailbox::MODE_SERVICE,
                   vire::com::mailbox::PRIVACY_PUBLIC,
                   "vireserver.service",
                   "--sp-----");
  dom1.add_mailbox("SignalFromServer",
                   vire::com::mailbox::MODE_EVENT,
                   vire::com::mailbox::PRIVACY_PUBLIC,
                   "vireserver.event",
                   "p----s---");
  dom1.set_transport_driver_type_id(transport_driver_id);
  dom1.set_encoding_driver_type_id(encoding_driver_id);
  dom1.tree_dump(std::clog, "Server to subcontractor '" + subcontractor_name + "' system communication domain: ");
  std::clog << std::endl;

  // Domain #2:
  std::clog << "\ntest_com_domain_1: Domain #2" << std::endl;
  vire::com::domain dom2;
  dom2.set_name("/" + setup_name + "/cms/vire/clients/system/" + "clientFoo");
  dom2.set_category(vire::com::DOMAIN_CATEGORY_CLIENT_SYSTEM);
  dom2.set_transport_driver_type_id(transport_driver_id);
  dom2.set_encoding_driver_type_id(encoding_driver_id);
  dom2.tree_dump(std::clog, "Server to clients system communication domain: ");
  std::clog << std::endl;

  // Domain #3:
  std::clog << "\ntest_com_domain_1: Domain #3" << std::endl;
  vire::com::domain dom3;
  dom3.set_name("/" + setup_name + "/cms/vire/monitoring");
  dom3.set_category(vire::com::DOMAIN_CATEGORY_MONITORING);
  dom3.set_transport_driver_type_id(transport_driver_id);
  dom3.set_encoding_driver_type_id(encoding_driver_id);
  dom3.tree_dump(std::clog, "Resource monitoring communication domain: ");
  std::clog << std::endl;

  // Domain #4:
  std::clog << "\ntest_com_domain_1: Domain #4" << std::endl;
  vire::com::domain dom4;
  dom4.set_name("/" + setup_name + "/cms/vire/control");
  dom4.set_category(vire::com::DOMAIN_CATEGORY_CONTROL);
  dom4.set_transport_driver_type_id(transport_driver_id);
  dom4.set_encoding_driver_type_id(encoding_driver_id);
  dom4.tree_dump(std::clog, "Resource control communication domain: ");
  std::clog << std::endl;

  // Domain #5:
  std::clog << "\ntest_com_domain_1: Domain #5" << std::endl;
  vire::com::domain dom5;
  dom5.set_name("/" + setup_name + "/cms/vire/clients/gate");
  dom5.set_category(vire::com::DOMAIN_CATEGORY_GATE);
  dom5.set_transport_driver_type_id(transport_driver_id);
  dom5.set_encoding_driver_type_id(encoding_driver_id);
  dom5.tree_dump(std::clog, "Clients gate communication domain: ");
  std::clog << std::endl;

  return;
}
