// vire/cmsclient/setup_infos_tui.cc
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
#include <vire/cmsclient/setup_infos_tui.h>

// Standard libraries:
#include <iomanip>

// This project:
#include <vire/auth/password_entry.h>

namespace vire {

  namespace cmsclient {

    setup_infos_tui::setup_infos_tui()
    {
      return;
    }

    bool setup_infos_tui::run(setup_infos & sinfo_)
    {
      std::string setupId;
      if (sinfo_.has_setup_id()) {
        setupId = sinfo_.get_setup_id().to_string();
      }
      std::string serverHost;
      if (sinfo_.has_host()) {
        serverHost = sinfo_.get_host();
      }
      int serverPort;
      if (sinfo_.has_port()) {
        serverPort = sinfo_.get_port();
      }
      std::string domainNamePrefix;
      if (sinfo_.has_domain_name_prefix()) {
        domainNamePrefix = sinfo_.get_domain_name_prefix();
      }
      std::string transportDriverTypeId;
      if (sinfo_.has_transport_driver_type_id()) {
        transportDriverTypeId = sinfo_.get_transport_driver_type_id().to_string();
      }
      std::string encodingDriverTypeId;
      if (sinfo_.has_encoding_driver_type_id()) {
        encodingDriverTypeId = sinfo_.get_encoding_driver_type_id().to_string();
      }
      std::string gateLogin;
      if (sinfo_.has_gate_client_user_login()) {
        gateLogin = sinfo_.get_gate_client_user_login();
      }
      std::string gatePassword;
      if (sinfo_.has_gate_client_user_password()) {
        gatePassword = sinfo_.get_gate_client_user_password();
      }

      {
        std::string tmp;
        std::clog << "Enter the setup identifier ";
        if (!setupId.empty()) {
          std::clog << '[' << setupId << ']';
        }
        std::clog << " : " << std::flush;
        std::getline(std::cin, tmp);
        if (!tmp.empty()) {
          setupId = tmp;
        }
        std::clog << std::endl;
      }

      {
        std::string tmp;
        std::clog << "Enter the Vire CMS server host ";
        if (!serverHost.empty()) {
          std::clog << '[' << serverHost << ']';
        }
        std::clog << " : " << std::flush;
        std::getline(std::cin, tmp);
        if (!tmp.empty()) {
          serverHost = tmp;
        }
        std::clog << std::endl;
      }

      try {
        std::string tmp;
        std::clog << "Enter the Vire CMS server port ";
        if (serverPort != -1) {
          std::clog << '[' << serverPort << ']';
        }
        std::clog << " : " << std::flush;
        std::getline(std::cin, tmp);
        if (!tmp.empty()) {
          int port = std::stoi(tmp);
          if (port >= 0) {
            serverPort = port;
          }
        }
        std::clog << std::endl;
      } catch (std::exception & x) {
        DT_LOG_ERROR(datatools::logger::PRIO_ERROR, "Invalid Vire CMS port input : " << x.what());
        return false;
      }

      {
        std::string tmp;
        std::clog << "Enter the setup's domain name prefix ";
        if (!domainNamePrefix.empty()) {
          std::clog << '[' << domainNamePrefix << ']';
        }
        std::clog << " : " << std::flush;
        std::getline(std::cin, tmp);
        if (!tmp.empty()) {
          domainNamePrefix = tmp;
        }
        std::clog << std::endl;
      }

      {
        std::string tmp;
        std::clog << "Enter the transport driver type ID ";
        if (!transportDriverTypeId.empty()) {
          std::clog << '[' << transportDriverTypeId << ']';
        }
        std::clog << " : " << std::flush;
        std::getline(std::cin, tmp);
        if (!tmp.empty()) {
          transportDriverTypeId = tmp;
        }
        std::clog << std::endl;
      }

      {
        std::string tmp;
        std::clog << "Enter the encoding driver type ID ";
        if (!encodingDriverTypeId.empty()) {
          std::clog << '[' << encodingDriverTypeId << ']';
        }
        std::clog << " : " << std::flush;
        std::getline(std::cin, tmp);
        if (!tmp.empty()) {
          encodingDriverTypeId = tmp;
        }
        std::clog << std::endl;
      }

      {
        std::string tmp;
        std::clog << "Enter the gate client user login ";
        if (!gateLogin.empty()) {
          std::clog << '[' << gateLogin << ']';
        }
        std::clog << " : " << std::flush;
        std::getline(std::cin, tmp);
        if (!tmp.empty()) {
          gateLogin = tmp;
        }
        std::clog << std::endl;
      }

      {
        std::string tmp;
        vire::auth::password_entry passwordEntry;
        std::ostringstream message;
        message << "Enter the gate client user password ";
        if (!gateLogin.empty()) {
          message << '[' << std::setfill ('*') << std::setw(gatePassword.size()) << '*' << ']';
        }
        message << " : " << std::flush;
        tmp = passwordEntry(message.str().c_str());
        if (!tmp.empty()) {
          gatePassword = tmp;
        }
      }
      std::cerr << std::flush;
      std::clog << std::flush;
      // std::clog << "Setup ID = " << setupId << std::endl;
      // std::clog << "Vire CMS server host = " << serverHost << std::endl;
      // std::clog << "Vire CMS server port = " << serverPort << std::endl;
      // std::clog << "Domain name prefix = " << domainNamePrefix << std::endl;
      // std::clog << "Transport driver type ID = " << transportDriverTypeId << std::endl;
      // std::clog << "Encoding driver type ID = " << encodingDriverTypeId << std::endl;
      // std::clog << "Gate login = " << gateLogin << std::endl;
      // std::clog << "Gate password = " << std::setfill ('*') << std::setw(gatePassword.size()) << '*' << std::endl;
      if (!sinfo_.has_setup_id()
          || ( sinfo_.get_setup_id().to_string() != setupId)) {
        vire::utility::instance_identifier setup_id;
        if (!setup_id.from_string(setupId)) {
          return false;
        }
        sinfo_.set_setup_id(setup_id);
      }
      if (!sinfo_.has_host()
          || ( sinfo_.get_host() != serverHost)) {
        sinfo_.set_host(serverHost);
      }
      if (!sinfo_.has_port()
          || ( sinfo_.get_port() != serverPort)) {
        sinfo_.set_port(serverPort);
      }
      if (!sinfo_.has_domain_name_prefix()
          || ( sinfo_.get_domain_name_prefix() != domainNamePrefix)) {
        sinfo_.set_domain_name_prefix(domainNamePrefix);
      }
      if (!sinfo_.has_transport_driver_type_id()
          || ( sinfo_.get_transport_driver_type_id().to_string() != transportDriverTypeId)) {
        vire::utility::model_identifier tpmodel;
        if (!tpmodel.from_string(transportDriverTypeId)) {
          return false;
        }
        sinfo_.set_transport_driver_type_id(tpmodel);
      }
      if (!sinfo_.has_encoding_driver_type_id()
          || ( sinfo_.get_encoding_driver_type_id().to_string() != encodingDriverTypeId)) {
        vire::utility::model_identifier epmodel;
        if (!epmodel.from_string(encodingDriverTypeId)) {
          return false;
        }
        sinfo_.set_encoding_driver_type_id(epmodel);
      }
      if (!sinfo_.has_gate_client_user_login()
          || ( sinfo_.get_gate_client_user_login() != gateLogin)) {
        sinfo_.set_gate_client_user_login(gateLogin);
      }
      if (!sinfo_.has_gate_client_user_password()
          || ( sinfo_.get_gate_client_user_password() != gatePassword)) {
        sinfo_.set_gate_client_user_password(gatePassword);
      }

      return true;
    }

  } // namespace cmsclient

} // namespace vire
