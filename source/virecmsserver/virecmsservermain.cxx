//! \file    virecmsservermain.cxx
//! \brief   Main program for Vire CMS server application

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
namespace bpo = boost::program_options;
// - Bayeux:
#include <bayeux/version.h>
// - Bayeux/datatools:
#include <datatools/kernel.h>
#include <datatools/logger.h>
#include <datatools/exception.h>
#include <datatools/configuration/variant_service.h>

// Vire:
#include <vire/version.h>
#include <vire/vire.h>

// This project:
#include <vire/cmsserver/server.h>

// This Project:
#include <virecmsserver/virecmsserver_config.h>
#include <virecmsserver/ui/shell.h>

namespace dtc = datatools::configuration;

struct virecmsserver_args
{
  datatools::logger::priority logging_priority; //!< The logging priority
  std::string sandbox_mode;           //!< Label triggering special developper mode
  std::string configuration_path;     //!< Path to main server configuration file
  bool run_shell = false; //!< Shell user interface
  dtc::variant_service::config variants; //!< Variant support
};

//! Handle printing of version information to given ostream
void do_version(std::ostream & out_, bool is_verbose_);

//! Handle printing of help message to given ostream
void do_help(std::ostream & out_, const bpo::options_description & od_);

//! Handle command line argument dialog
void do_cl_dialog(int argc_, char *argv_[], virecmsserver_args & parameters_);

//! Parse command line arguments to configure the Vire server parameters
void do_configure(int argc_, char * argv_[], virecmsserver_args & parameters_);

//! Run the Vire server using command line args as given
int do_virecmsserver(int argc_, char * argv_[]);

// Return kernel initialization flags for this application:
uint32_t app_kernel_init_flags();

//----------------------------------------------------------------------
// MAIN PROGRAM
//----------------------------------------------------------------------
int main(int argc_, char * argv_[])
{
  int returned_code = EXIT_SUCCESS;
  vire::initialize(argc_, argv_, app_kernel_init_flags());

  returned_code = do_virecmsserver(argc_, argv_);

  vire::terminate();
  return returned_code;
};

//----------------------------------------------------------------------
// DEFINITIONS
//----------------------------------------------------------------------

void do_version(std::ostream & out_, bool is_verbose_) {
  out_ << "virecmsserver " << vire::version::get_version() << "\n";
  if (is_verbose_) {
    out_ << "\n"
         << "Copyright (C) 2016-2017 SuperNEMO Collaboration\n\n"
         << "virecmsserver uses the following external libraries:\n"
         << "* Vire    : " << vire::version::get_version() << "\n"
         << "* Bayeux  : " << bayeux::version::get_version() << "\n"
         << std::endl;
  }
  return;
}

void do_help(std::ostream & out_, const boost::program_options::options_description & od_) {
  do_version(out_, false);
  out_ << "Usage:\n"
       << "  virecmsserver [options]\n"
       << "Options\n"
       << od_
       << std::endl;
  return;
}

//! Exceptions for dialog
class virecmsserver_dialog_help_requested : public std::exception {};
class virecmsserver_dialog_options_error  : public std::exception {};

void do_cl_dialog(int argc_, char *argv_[], virecmsserver_args & params_)
{
  // Bind command line parser to exposed parameters:
  namespace bpo = boost::program_options;
  bpo::variables_map vm;
  bpo::options_description all_opts;

  try {

    // Describe the allowed options for this application:
    bpo::options_description opts("Allowed options");
    opts.add_options()
      ("help,h", "print this help message")

      ("version", "print version number")

      ("logging-priority,L",
       bpo::value<std::string>()
       ->value_name("level"),
       "set the logging priority")

      ("config-file,c",
       bpo::value<std::string>()
       ->value_name("file"),
       "set the name of the main configuration file for the CMS server")

      ("ui-shell",
       "run the shell user interface")
      ;

    // Describe positional arguments for this application:
    bpo::positional_options_description args;
    args.add("config-file", 1);

    // Describe Bayeux/datatools kernel options:
    bpo::options_description kopts("Bayeux/datatools kernel options");
    datatools::kernel::param_type kparams;
    datatools::kernel::build_opt_desc(kopts, kparams, app_kernel_init_flags());

    // Declare options for variant support:
    bpo::options_description vopts("Variant support");
    uint32_t po_init_flags = 0;
    po_init_flags |= dtc::variant_service::NO_LABEL;
    po_init_flags |= dtc::variant_service::NO_LOGGING;
    // po_init_flags |= dtc::variant_service::PROFILE_LOAD_DONT_IGNORE_UNKNOWN;
    po_init_flags |= dtc::variant_service::NO_TUI;
    dtc::variant_service::init_options(vopts,
                                       params_.variants,
                                       po_init_flags);

    // Collect all supported options in one container:
    all_opts.add(opts);
    all_opts.add(kopts);
    all_opts.add(vopts);

    // Configure the parser:
    bpo::command_line_parser cl_parser(argc_, argv_);
    cl_parser.options(all_opts);
    cl_parser.positional(args);

    // Parse:
    bpo::parsed_options parsed = cl_parser.run();
    // Fill and notify a variable map:
    bpo::store(parsed, vm);
    bpo::notify(vm);
  } catch (const bpo::required_option & error) {
    // We need to handle help/version even if required_option thrown
    if (!vm.count("help") && !vm.count("version")) {
      std::cerr << "[OptionsException] " << error.what() << std::endl;
      throw virecmsserver_dialog_options_error();
    }
  } catch (const std::exception & error) {
    std::cerr << "[OptionsException] " << error.what() << std::endl;
    throw virecmsserver_dialog_options_error();
  }

  // Handle any non-bound options
  if (vm.count("help")) {
    do_help(std::cout, all_opts);
    throw virecmsserver_dialog_help_requested();
  }

  if (vm.count("version")) {
    do_version(std::cout, true);
    throw virecmsserver_dialog_help_requested();
  }

  if (vm.count("ui-shell")) {
    params_.run_shell = true;
  }

  // Fetch the verbosity level:
  if (vm.count("logging-priority")) {
    const std::string & logging_label = vm["logging-priority"].as<std::string>();
    params_.logging_priority = datatools::logger::get_priority(logging_label);
    DT_THROW_IF(params_.logging_priority == datatools::logger::PRIO_UNDEFINED, std::logic_error,
                "Invalid logging priority label '" << logging_label << "' !");
  }

  if (vm.count("config-file")) {
    params_.configuration_path = vm["config-file"].as<std::string>();
  }

  return;
}

//----------------------------------------------------------------------
//! Exceptions for configuration
class virecmsserver_config_default_error : public std::exception {};
class virecmsserver_config_help_handled : public std::exception {};
class virecmsserver_config_user_error : public std::exception {};

//! Parse command line arguments to configure the simulation parameters
void do_configure(int argc_, char * argv_[], virecmsserver_args & parameters_)
{
  // - Default configuration:
  try {
    parameters_.logging_priority   = datatools::logger::PRIO_FATAL;
    parameters_.sandbox_mode       = "";
    parameters_.configuration_path = "";
    parameters_.run_shell = false;
    // - Possible "~/.vire/cmsserver.rc" config
    //   or       "~/.vire/SuperNEMO/cms/server.rc" config
    //   or       "/etc/vire/SuperNEMO/cms/server.rc" config
  } catch (std::exception& e) {
    throw virecmsserver_config_default_error();
  }

  // - CL dialog configuration:
  try {
    do_cl_dialog(argc_, argv_, parameters_);
  } catch (virecmsserver_dialog_help_requested & error) {
    throw virecmsserver_config_help_handled();
  } catch (virecmsserver_dialog_options_error & error) {
    throw virecmsserver_config_user_error();
  }

  // // Check:
  // DT_THROW_IF(params_.configuration_path.empty(), std::logic_error,
  //             "Missing CMS server configuration file !");

  if (parameters_.configuration_path.empty()) {
    DT_LOG_NOTICE(parameters_.logging_priority, "Using default CMS server configuration file.");
    parameters_.configuration_path = "@snemo:config/snemo/demonstrator/cms/server/main.conf";
  }

  return;
}

namespace {
  static  vire::cmsserver::server * gCmsServerHandle = nullptr;
}

void cmsserver_signal_handler(int signum_)
{
  std::cerr << "Interrupt signal (" << signum_ << ") received.\n";
  if (signum_ == SIGTERM) {
    if (gCmsServerHandle && gCmsServerHandle->is_running()) {
      std::cerr << "Stop the CMS server...\n";
      gCmsServerHandle->request_stop();
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
  virecmsserver_args vire_cms_server_parameters;
  try {
    do_configure(argc_, argv_, vire_cms_server_parameters);
  } catch (virecmsserver_config_default_error & error) {
    std::cerr << "Unable to configure core of Vire server" << std::endl;
    return EXIT_FAILURE;
  } catch (virecmsserver_config_help_handled & error) {
    return EXIT_SUCCESS;
  } catch (virecmsserver_config_user_error & error) {
    return EXIT_FAILURE;
  }

  // - Run:
  try {

    // - Variant service:
    std::unique_ptr<dtc::variant_service> vserv;
    if (vire_cms_server_parameters.variants.is_active()) {
      // Create and start the variant service:
      vserv.reset(new dtc::variant_service);
      vserv->configure(vire_cms_server_parameters.variants);
      vserv->start();
    }

    void (*prev_handler)(int);
    prev_handler = std::signal(SIGTERM, cmsserver_signal_handler);

    // - CMS server:
    vire::cmsserver::server cms_server;
    cms_server.set_logging(vire_cms_server_parameters.logging_priority);
    datatools::properties cms_server_config;
    {
      uint32_t reader_flags = 0;
      reader_flags |= datatools::properties::config::RESOLVE_PATH;
      datatools::properties::config reader(reader_flags);
      reader.read(vire_cms_server_parameters.configuration_path, cms_server_config);
    }
    cms_server.initialize(cms_server_config);
    if (datatools::logger::is_debug(vire_cms_server_parameters.logging_priority)) {
      cms_server.tree_dump(std::cerr, "Vire CMS Server: ", "[debug] ");
    }
    gCmsServerHandle = &cms_server;

    // - Shell UI:
    bool tui = vire_cms_server_parameters.run_shell;
    std::unique_ptr<virecmsserver::ui::shell> sh;
    std::unique_ptr<std::thread> sh_thread;
    if (tui) {
      DT_LOG_DEBUG(vire_cms_server_parameters.logging_priority,
                   "Preparing the shell...");
      sh.reset(new virecmsserver::ui::shell(cms_server));
      sh->logging = vire_cms_server_parameters.logging_priority;
      sh->set_interactive(true);
      DT_LOG_DEBUG(vire_cms_server_parameters.logging_priority,
                   "Shell is interactive : " << sh->is_interactive());
      sh->initialize_simple();
      DT_LOG_DEBUG(vire_cms_server_parameters.logging_priority,
                   "Shell is initialized : " << sh->is_initialized());
    }

    // - Start server:
    DT_LOG_DEBUG(vire_cms_server_parameters.logging_priority,
                 "Starting the server...");
    std::thread cms_server_thread(&vire::cmsserver::server::run, &cms_server);

    if (sh.get() != nullptr) {
      DT_LOG_DEBUG(vire_cms_server_parameters.logging_priority,
                   "Starting the shell...");
      virecmsserver::ui::shell & shref = *sh.get();
      if (datatools::logger::is_debug(vire_cms_server_parameters.logging_priority)) {
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
        DT_LOG_DEBUG(vire_cms_server_parameters.logging_priority,
                     "Terminating the CMS server shell UI...");
        sh->reset();
      }
    }

    gCmsServerHandle = nullptr;
    if (cms_server.is_initialized()) {
      DT_LOG_DEBUG(vire_cms_server_parameters.logging_priority,
                   "Terminating the CMS server...");
      cms_server.reset();
      DT_LOG_DEBUG(vire_cms_server_parameters.logging_priority,
                   "Done.");
    }

    if (vserv) {
      // - Stop the variant service:
      vserv->stop();
      vserv.reset();
    }

  } catch (std::exception & error) {
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS,
                 "virecmsserver: Setup/run of Vire CMS server threw exception: " << error.what());
    return EXIT_FAILURE;
  } catch (...) {
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS,
                 "virecmsserver: Setup/run of Vire CMS server threw unexpected exception!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

uint32_t app_kernel_init_flags()
{
  uint32_t kernel_init_flags = 0;
  kernel_init_flags |= datatools::kernel::init_no_help;
  kernel_init_flags |= datatools::kernel::init_no_splash;
  kernel_init_flags |= datatools::kernel::init_no_inhibit_libinfo;
  kernel_init_flags |= datatools::kernel::init_no_libinfo_logging;
  kernel_init_flags |= datatools::kernel::init_no_variant;
  kernel_init_flags |= datatools::kernel::init_no_inhibit_variant;
  kernel_init_flags |= datatools::kernel::init_no_locale_category;
  kernel_init_flags |= datatools::kernel::init_no_inhibit_qt_gui;
  return kernel_init_flags;
}
