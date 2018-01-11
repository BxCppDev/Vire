//! \file    virecmsclientmain.cxx
//! \brief   Main program for Vire CMS client application

// Copyright (c) 2017 by François MAUGER <mauger@lpccaen.in2p3.fr>
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

// This Project:
#include <vire/cmsclient/client.h>
// #include <virecmsserver_config.h>
// #include <ui_shell.h>
// #include <virecmsclient.h>

namespace dtc = datatools::configuration;

struct virecmsclient_args
{
  //!The logging priority
  datatools::logger::priority logging = datatools::logger::PRIO_FATAL;
  std::string devel_mode;                //!< Label triggering special developper mode
  std::string configuration_path;        //!< Path to main client configuration file
  // bool run_shell = false;             //!< Shell user interface
  dtc::variant_service::config variants; //!< Variant support
};

//! Handle printing of version information to given ostream
void do_version(std::ostream & out_, bool is_verbose_);

//! Handle printing of help message to given ostream
void do_help(std::ostream & out_, const bpo::options_description & od_);

//! Handle command line argument dialog
void do_cl_dialog(int argc_, char *argv_[], virecmsclient_args & parameters_);

//! Parse command line arguments to configure the Vire client parameters
void do_configure(int argc_, char * argv_[], virecmsclient_args & parameters_);

//! Run the Vire client using command line args as given
int do_virecmsclient(int argc_, char * argv_[]);

// Return kernel initialization flags for this application:
uint32_t app_kernel_init_flags();

//----------------------------------------------------------------------
// MAIN PROGRAM
//----------------------------------------------------------------------
int main(int argc_, char * argv_[])
{
  int returned_code = EXIT_SUCCESS;
  vire::initialize(argc_, argv_, app_kernel_init_flags());

  returned_code = do_virecmsclient(argc_, argv_);

  vire::terminate();
  return returned_code;
}

//----------------------------------------------------------------------
// DEFINITIONS
//----------------------------------------------------------------------

void do_version(std::ostream & out_, bool is_verbose_) {
  out_ << "virecmsclient " << vire::version::get_version() << "\n";
  if (is_verbose_) {
    out_ << "\n"
         << "Copyright (C) 2017 SuperNEMO Collaboration\n\n"
         << "virecmsclient uses the following external libraries:\n"
         << "* Vire    : " << vire::version::get_version() << "\n"
         << "* Bayeux  : " << bayeux::version::get_version() << "\n"
         << std::endl;
  }
  return;
}

void do_help(std::ostream & out_, const boost::program_options::options_description & od_) {
  do_version(out_, false);
  out_ << "Usage:\n"
       << "  virecmsclient [options]\n"
       << "Options\n"
       << od_
       << std::endl;
  return;
}

//! Exceptions for dialog
class virecmsclient_dialog_help_requested : public std::exception {};
class virecmsclient_dialog_options_error : public std::logic_error
{
public:
  virecmsclient_dialog_options_error(const std::string & what_)
    : std::logic_error(what_) {}
};

void do_cl_dialog(int argc_, char *argv_[], virecmsclient_args & params_)
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

      ("logging,L",
       bpo::value<std::string>()
       ->value_name("level"),
       "set the logging priority")

      ("devel-mode,x",
       bpo::value<std::string>()
       ->value_name("mode"),
       "set the name of the development mode")

      ("config-file,c",
       bpo::value<std::string>()
       ->value_name("file"),
       "set the name of the main configuration file for the CMS client")

      // ("shell",
      //  "run the shell user interface")
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
      //std::cerr << "[OptionsException] " << error.what() << std::endl;
      throw virecmsclient_dialog_options_error(error.what());
    }
  } catch (const std::exception & error) {
    //std::cerr << "[OptionsException] " << error.what() << std::endl;
    throw virecmsclient_dialog_options_error(error.what());
  }

  // Handle any non-bound options
  if (vm.count("help")) {
    do_help(std::cout, all_opts);
    throw virecmsclient_dialog_help_requested();
  }

  if (vm.count("version")) {
    do_version(std::cout, true);
    throw virecmsclient_dialog_help_requested();
  }

  // if (vm.count("shell")) {
  //   params_.run_shell = true;
  // }

  // Fetch the verbosity level:
  if (vm.count("logging")) {
    const std::string & logging_label = vm["logging"].as<std::string>();
    params_.logging = datatools::logger::get_priority(logging_label);
    DT_THROW_IF(params_.logging == datatools::logger::PRIO_UNDEFINED, std::logic_error,
                "Invalid logging priority label '" << logging_label << "' !");
  }

  if (vm.count("config-file")) {
    params_.configuration_path = vm["config-file"].as<std::string>();
  }

  // Check:
  DT_THROW_IF(params_.configuration_path.empty(),
              virecmsclient_dialog_options_error,
              "Missing CMS client configuration file !");

  return;
}

//----------------------------------------------------------------------
//! Exceptions for configuration
class virecmsclient_config_default_error : public std::exception {};
class virecmsclient_config_help_handled : public std::exception {};
class virecmsclient_config_user_error : public std::logic_error
{
public:
  virecmsclient_config_user_error(const std::string & what_)
    : std::logic_error(what_) {}
};

//! Parse command line arguments to configure the simulation parameters
void do_configure(int argc_, char * argv_[], virecmsclient_args & parameters_)
{
  // - Default configuration:
  try {
    parameters_.logging   = datatools::logger::PRIO_FATAL;
    parameters_.devel_mode       = "";
    parameters_.configuration_path = "";
    // parameters_.run_shell = false;
    // - Possible "~/.vire/cmsclient.rc" config
    //   or       "~/.vire/SuperNEMO/cms/client.rc" config
    //   or       "/etc/vire/SuperNEMO/cms/client.rc" config
  } catch (std::exception& e) {
    throw virecmsclient_config_default_error();
  }

  // - CL dialog configuration:
  try {
    do_cl_dialog(argc_, argv_, parameters_);
  } catch (virecmsclient_dialog_help_requested & error) {
    throw virecmsclient_config_help_handled();
  } catch (virecmsclient_dialog_options_error & error) {
    throw virecmsclient_config_user_error(error.what());
  } catch (std::exception & error) {
    throw virecmsclient_config_user_error(error.what());
  }

  return;
}

int do_virecmsclient(int argc_, char * argv_[])
{
  // - Parameters:
  virecmsclient_args vire_cms_client_parameters;
  try {
    do_configure(argc_, argv_, vire_cms_client_parameters);
  } catch (virecmsclient_config_default_error & error) {
    DT_LOG_FATAL(vire_cms_client_parameters.logging, "Unable to load the default Vire client's configuration: " << error.what());
    return EXIT_FAILURE;
  } catch (virecmsclient_config_help_handled & error) {
    return EXIT_SUCCESS;
  } catch (virecmsclient_config_user_error & error) {
    DT_LOG_FATAL(vire_cms_client_parameters.logging, "Unable to configure the Vire client: " << error.what());
    return EXIT_FAILURE;
  }

  // - Run:
  try {

    // Variant service:
    std::unique_ptr<dtc::variant_service> vserv;
    if (vire_cms_client_parameters.variants.is_active()) {
      // Create and start the variant service:
      vserv.reset(new dtc::variant_service);
      vserv->configure(vire_cms_client_parameters.variants);
      vserv->start();
    }

    // Main client configuration:
    uint32_t reader_flags = 0;
    reader_flags |= datatools::properties::config::RESOLVE_PATH;
    datatools::properties::config reader(reader_flags);
    datatools::properties cms_client_config;
    reader.read(vire_cms_client_parameters.configuration_path, cms_client_config);
    if (datatools::logger::is_debug(vire_cms_client_parameters.logging)) {
      cms_client_config.tree_dump(std::cerr, "Client configuration: ", "[debug] ");
    }

    vire::cmsclient::client cms_client;
    cms_client.set_logging(vire_cms_client_parameters.logging);
    // cms_client.set_name("virecmsclient");
    // cms_client.set_display_name("Vire CMS client");
    // cms_client.set_terse_description("Main CMS client application running the Vire Control and Monitoring System");
    // cms_client.set_sleep_period(1000000);

    // cms_client.initialize(cms_client_config);

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
      DT_LOG_DEBUG(vire_cms_client_parameters.logging,
                   "Terminating client...");
      cms_client.reset();
      DT_LOG_DEBUG(vire_cms_client_parameters.logging,
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
