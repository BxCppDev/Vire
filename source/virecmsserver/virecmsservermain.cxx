//! \file    virecmsservermain.cxx
//! \brief   Main program for Vire CMS Server application

// Copyright (c) 2016-2018 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <iostream>
#include <exception>
#include <memory>
#include <csignal>

// Third Party:
// - Boost:
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/properties.h>
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/exception.h>
#include <bayeux/datatools/configuration/variant_service.h>

// Vire:
#include <vire/vire.h>

// This project:
#include <vire/cmsserver/server.h>

// This Project:
#include <virecmsserver/virecmsserver_config.h>
#include <virecmsserver/core.h>
#include <virecmsserver/parameters.h>
// #include <virecmsserver/ui/shell.h>

namespace bpo = boost::program_options;
namespace dtc = datatools::configuration;

//! Run the Vire Server using command line args as given
int do_virecmsserver(int argc_, char * argv_[]);

//----------------------------------------------------------------------
// MAIN PROGRAM
//----------------------------------------------------------------------
int main(int argc_, char * argv_[])
{
  int returned_code = EXIT_SUCCESS;
  vire::initialize(argc_, argv_, virecmsserver::app_kernel_init_flags());

  returned_code = do_virecmsserver(argc_, argv_);

  vire::terminate();
  return returned_code;
};

//----------------------------------------------------------------------
// DEFINITIONS
//----------------------------------------------------------------------

namespace {
  static  vire::cmsserver::server * gCmsServerHandle = nullptr;
}

void cmsserver_signal_handler(int signum_)
{
  std::cerr << "Interrupt signal (" << signum_ << ") received.\n";
  if (signum_ == SIGTERM) {
    if (gCmsServerHandle && gCmsServerHandle->get_rc().is_running()) {
      std::cerr << "Stop the CMS server...\n";
      gCmsServerHandle->stop();
    }
  }
  // if (signum_ == SIGABRT) {
  //   if (gCmsServerHandle && gCmsServerHandle->is_running()) {
  //     std::cerr << "Abort the CMS server...\n";
  //     gCmsServerHandle->reset();
  //   }
  //   exit(signum_);
  // }
  return;
}

int do_virecmsserver(int argc_, char * argv_[])
{
  // - Parameters:
  virecmsserver::parameters params;
  try {
    do_configure(argc_, argv_, params);
  } catch (virecmsserver::config_default_error & error) {
    DT_LOG_FATAL(params.logging,
                 "Unable to load the default Vire Server's configuration: " << error.what());
    return EXIT_FAILURE;
  } catch (virecmsserver::config_signal_handled & error) {
    return EXIT_SUCCESS;
  } catch (virecmsserver::config_user_error & error) {
    DT_LOG_FATAL(params.logging, "Unable to configure the Vire Server: " << error.what());
    return EXIT_FAILURE;
  }

  // - Run:
  try {

    // - Variant service:
    std::unique_ptr<dtc::variant_service> vserv;
    if (params.variants.is_active()) {
      // Create and start the variant service:
      vserv.reset(new dtc::variant_service);
      vserv->configure(params.variants);
      vserv->start();
    }

    void (*prev_handler)(int);
    prev_handler = std::signal(SIGTERM, cmsserver_signal_handler);

    // - CMS server:
    vire::cmsserver::server cms_server;
    cms_server.set_logging(params.logging);
    datatools::properties cms_server_config;
    {
      uint32_t reader_flags = 0;
      reader_flags |= datatools::properties::config::RESOLVE_PATH;
      datatools::properties::config reader(reader_flags);
      reader.read(params.configuration_path, cms_server_config);
    }
    cms_server.initialize(cms_server_config);
    if (datatools::logger::is_debug(params.logging)) {
      cms_server.tree_dump(std::cerr, "Vire CMS Server: ", "[debug] ");
    }
    gCmsServerHandle = &cms_server;

    /*
    // - Shell UI:
    bool tui = params.run_shell;
    std::unique_ptr<virecmsserver::ui::shell> sh;
    std::unique_ptr<std::thread> sh_thread;
    if (tui) {
      DT_LOG_DEBUG(params.logging,
                   "Preparing the shell...");
      sh.reset(new virecmsserver::ui::shell(cms_server));
      sh->logging = params.logging;
      sh->set_interactive(true);
      DT_LOG_DEBUG(params.logging,
                   "Shell is interactive : " << sh->is_interactive());
      sh->initialize_simple();
      DT_LOG_DEBUG(params.logging,
                   "Shell is initialized : " << sh->is_initialized());
    }

    // - Start server:
    DT_LOG_DEBUG(params.logging,
                 "Starting the server...");
    std::thread cms_server_thread(&vire::cmsserver::server::run, &cms_server);

    if (sh.get() != nullptr) {
      DT_LOG_DEBUG(params.logging,
                   "Starting the shell...");
      virecmsserver::ui::shell & shref = *sh.get();
      if (datatools::logger::is_debug(params.logging)) {
        shref.tree_dump(std::cerr, "Shell: ", "[debug] ");
      }
      sh_thread.reset(new std::thread(&datatools::ui::basic_shell::run,
                                      sh.get(),
                                      &std::cin));
    }

    // - Join threads:
    cms_server_thread.join();
    if (sh.get() != nullptr) {
      sh_thread->join();
    }

    // - Terminate:
    if (sh_thread.get() != nullptr) {
      sh_thread.reset();
    }
    if (sh.get() != nullptr) {
      if (sh->is_initialized()) {
        DT_LOG_DEBUG(params.logging,
                     "Terminating the CMS server shell UI...");
        sh->reset();
      }
    }

    gCmsServerHandle = nullptr;
    if (cms_server.is_initialized()) {
      DT_LOG_DEBUG(params.logging,
                   "Terminating the CMS server...");
      cms_server.reset();
      DT_LOG_DEBUG(params.logging,
                   "Done.");
    }
    */

    if (vserv) {
      // - Stop the variant service:
      vserv->stop();
      vserv.reset();
    }

  } catch (std::exception & error) {
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS,
                 "virecmsserver: Setup/run of Vire CMS Server threw exception: " << error.what());
    return EXIT_FAILURE;
  } catch (...) {
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS,
                 "virecmsserver: Setup/run of Vire CMS Server threw unexpected exception!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
