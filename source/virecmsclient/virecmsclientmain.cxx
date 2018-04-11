//! \file    virecmsclientmain.cxx
//! \brief   Main program for Vire CMS client application

// Copyright (c) 2017-2018 by François MAUGER <mauger@lpccaen.in2p3.fr>
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

// Standard Library:
#include <cstdlib>
#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>

// Third Party:
// - Bayeux/datatools:
#include <bayeux/datatools/multi_properties.h>
#include <bayeux/datatools/kernel.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/configuration/variant_service.h>

// Vire:
#include <vire/vire.h>

// This Project:
#include <virecmsclient/core.h>
#include <virecmsclient/parameters.h>
#include <vire/cmsclient/client.h>

namespace dtc = datatools::configuration;

//! Run the Vire Client using command line args as given
int do_virecmsclient(int argc_, char * argv_[]);

//----------------------------------------------------------------------
// MAIN PROGRAM
//----------------------------------------------------------------------
int main(int argc_, char * argv_[])
{
  int returned_code = EXIT_SUCCESS;
  vire::initialize(argc_, argv_, virecmsclient::app_kernel_init_flags());

  returned_code = do_virecmsclient(argc_, argv_);

  vire::terminate();
  return returned_code;
}

//----------------------------------------------------------------------
// DEFINITIONS
//----------------------------------------------------------------------
int do_virecmsclient(int argc_, char * argv_[])
{
  // - Parameters:
  virecmsclient::parameters params;
  try {
    do_configure(argc_, argv_, params);
  } catch (virecmsclient::config_default_error & error) {
    DT_LOG_FATAL(params.logging, "Unable to load the default Vire Client's configuration: " << error.what());
    return EXIT_FAILURE;
  } catch (virecmsclient::config_help_handled & error) {
    return EXIT_SUCCESS;
  } catch (virecmsclient::config_user_error & error) {
    DT_LOG_FATAL(params.logging, "Unable to configure the Vire client: " << error.what());
    return EXIT_FAILURE;
  }

  // - Run:
  try {

    // Variant service:
    std::unique_ptr<dtc::variant_service> vserv;
    if (params.variants.is_active()) {
      // Create and start the variant service:
      vserv.reset(new dtc::variant_service);
      vserv->configure(params.variants);
      vserv->start();
    }

    // Main client configuration:
    uint32_t reader_flags = 0;
    reader_flags |= datatools::multi_properties::config::RESOLVE_PATH;
    datatools::multi_properties::config reader(reader_flags);
    datatools::multi_properties cms_client_config;
    reader.read(params.configuration_path, cms_client_config);
    if (datatools::logger::is_debug(params.logging)) {
      cms_client_config.tree_dump(std::cerr, "Client configuration: ", "[debug] ");
    }

    vire::cmsclient::client cms_client;
    cms_client.set_logging_priority(params.logging);
    cms_client.set_name("virecmsclient");
    cms_client.set_display_name("Vire CMS Client");
    cms_client.set_terse_description("Main CMS client application running the Vire Control and Monitoring System");

    cms_client.initialize(cms_client_config);

    // bool tui = vire_cms_client_parameters.run_shell;
    // std::unique_ptr<virecmsclient::ui_shell> sh;
    // if (tui) {
    //   DT_LOG_DEBUG(vire_cms_client_parameters.logging_priority,
    //                "Running the shell...");
    //   sh.reset(new virecmsclient::ui_shell(cms_client));
    //   sh->logging = vire_cms_client_parameters.logging_priority;
    //   sh->set_interactive(true);
    //   DT_LOG_DEBUG(vire_cms_client_parameters.logging_priority,
    //                "Shell is interactive : " << sh->is_interactive());
    //   DT_LOG_DEBUG(vire_cms_client_parameters.logging_priority,
    //                "Shell is initialized : " << sh->is_initialized());
    //   datatools::properties sh_config;
    //   sh->initialize(sh_config);
    //   DT_LOG_DEBUG(vire_cms_client_parameters.logging_priority,
    //                "Shell is initialized : " << sh->is_initialized());
    // }

    // if (sh.get() != nullptr) {
    //   DT_LOG_DEBUG(vire_cms_client_parameters.logging_priority,
    //                "Starting the shell...");
    //   sh->start();
    // }

    // // - Start client:
    // DT_LOG_DEBUG(vire_cms_client_parameters.logging_priority,
    //              "Starting the client...");
    // cms_client.start();

    // // - Join threads:
    // cms_client.join();
    // if (sh.get() != nullptr) {
    //   sh->join();
    //   if (sh->is_initialized()) {
    //     sh->reset();
    //   }
    // }

    if (cms_client.is_initialized()) {
      DT_LOG_DEBUG(params.logging,
                   "Terminating client...");
      cms_client.reset();
      DT_LOG_DEBUG(params.logging,
                   "Done.");
    }

    if (vserv) {
      // Stop the variant service:
      vserv->stop();
      vserv.reset();
    }

  } catch (std::exception & error) {
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS,
                 "virecmsclient: Setup/run of Vire CMS client threw exception: " << error.what());
    return EXIT_FAILURE;
  } catch (...) {
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS,
                 "virecmsclient: Setup/run of Vire CMS client threw unexpected exception!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
