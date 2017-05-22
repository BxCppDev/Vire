//! \file vire/com/testing/test-com-domain_builder.cxx
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
#include <vire/com/domain_builder.h>
#include <vire/com/domain.h>

void test_com_domain_builder_1(bool interactive_ = false);

int main(int /* argc_ */, char ** /* argv_ */)
{
  vire::initialize();
  int error_code = EXIT_SUCCESS;
  try {
    std::clog << "Test program for the 'vire::com::domain_builder' class." << std::endl;

    bool interactive = false;
    interactive = true;

    test_com_domain_builder_1(interactive);

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

void test_com_domain_builder_1(bool interactive_)
{
  std::clog << "\ntest_com_domain_builder_1: basics" << std::endl;

  // Domain builder:
  std::string setup_name = "supernemo/demonstrator";
  vire::com::domain_builder db(setup_name);

  // Monitoring domain:
  std::clog << "\ntest_com_domain_builder_1: Building monitoring domain..." << std::endl;
  vire::com::domain monitorDom;
  db.build_monitoring_domain(monitorDom);
  monitorDom.tree_dump(std::clog, "CMS monitoring domain: ");
  std::clog << std::endl;

  // Control domain:
  std::clog << "\ntest_com_domain_builder_1: Building control domain..." << std::endl;
  vire::com::domain controlDom;
  db.build_control_domain(controlDom);
  controlDom.tree_dump(std::clog, "CMS control domain: ");
  std::clog << std::endl;

  // Clients gate domain:
  std::clog << "\ntest_com_domain_builder_1: Building clients gate domain..." << std::endl;
  vire::com::domain clientsGateDom;
  db.build_clients_gate_domain(clientsGateDom);
  clientsGateDom.tree_dump(std::clog, "CMS clients gate domain: ");
  std::clog << std::endl;

  // Clients system domains:
  std::vector<std::string> clientIds = { "aPho7uaB", "oPh6Uz1u", "leyesh0O"};
  std::vector<vire::com::domain> clientDomains;
  for (auto clientId : clientIds) {
    std::clog << "\ntest_com_domain_builder_1: Building a client system domain..." << std::endl;
    clientDomains.push_back(vire::com::domain());
    vire::com::domain & clientSystemDom = clientDomains.back();
    db.build_client_system_domain(clientSystemDom, clientId);
    clientSystemDom.tree_dump(std::clog, "CMS a client system domain: ");
    std::clog << std::endl;
  }

  // Subcontractors system domains:
  std::vector<std::string> subcontractorIds = { "cmslapp", "orleans"};
  std::vector<vire::com::domain> subcontractorDomains;
  for (auto subcontractorId : subcontractorIds) {
    std::clog << "\ntest_com_domain_builder_1: Building a subcontractor system domain..." << std::endl;
    subcontractorDomains.push_back(vire::com::domain());
    vire::com::domain & subcontractorSystemDom = subcontractorDomains.back();
    db.build_subcontractor_system_domain(subcontractorSystemDom, subcontractorId);
    subcontractorSystemDom.tree_dump(std::clog, "CMS a subcontractor system domain: ");
    std::clog << std::endl;
  }


  return;
}
