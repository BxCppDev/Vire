//! \file  vireinspector.cc
//! \brief Main program for Vire inspector application

// Copyright (c) 2015-2017 by François Mauger <mauger@lpccaen.in2p3.fr>
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
#include <string>
#include <iostream>
#include <exception>
#include <memory>

// Third Party:
// - Boost:
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
// - Bayeux/datatools:
#include <datatools/kernel.h>
#include <datatools/logger.h>
#include <datatools/exception.h>
#include <datatools/utils.h>
#include <datatools/library_loader.h>
#include <datatools/ui/utils.h>
#include <datatools/ui/basic_shell.h>
#include <datatools/kernel.h>
#include <datatools/configuration/io.h>
#include <datatools/configuration/variant_service.h>

// Vire library:
#include <vire/vire_config.h>
#include <vire/vire.h>
#include <vire/device/manager.h>
#include <vire/device/manager_command_interface.h>
#include <vire/utility/path.h>

// This Project:
#include <vire/inspector/config.h>
#include <vire/inspector/ihs_handler.h>
#include <vire/inspector/inspector_shell.h>
#include <vireinspector.h>

//----------------------------------------------------------------------
// MAIN PROGRAM
//----------------------------------------------------------------------
int main(int argc_, char * argv_[])
{
  namespace po = boost::program_options;
  namespace dtc = datatools::configuration;

  vire::initialize(argc_, argv_, vireinspector::ui::kernel_init_flags());

  int error_code = EXIT_SUCCESS;
  try {
    // The configuration parameter sets:
    vireinspector::ui::config_params params;

    // Variables map:
    po::variables_map vm;
    po::options_description optPublic;

    try {
      // Declare options for variant support:
      uint32_t po_init_flags = 0;
      po_init_flags |= dtc::variant_service::NO_LABEL;
      po_init_flags |= dtc::variant_service::NO_LOGGING;
      // po_init_flags |= dtc::variant_service::PROFILE_LOAD_DONT_IGNORE_UNKNOWN;
      po_init_flags |= dtc::variant_service::NO_TUI;
      po::options_description optVariant("Variant support");
      dtc::variant_service::init_options(optVariant,
                                         params.variants,
                                         po_init_flags);

      // Describe command line switches :
      po::options_description optsGeneral("General options");
      vireinspector::ui::build_opts(optsGeneral, params);

      // Describe Bayeux/datatools kernel options:
      po::options_description optKernel("Kernel options");
      datatools::kernel::param_type kparams;
      datatools::kernel::build_opt_desc(optKernel,
                                        kparams,
                                        vireinspector::ui::kernel_init_flags());

      // Collect all supported options in one container:
      optPublic.add(optsGeneral);
      optPublic.add(optVariant);
      optPublic.add(optKernel);

      // Configure the parser:
      po::command_line_parser cl_parser(argc_, argv_);
      cl_parser.options(optPublic);

      // Parse:
      po::parsed_options parsed = cl_parser.run();

      // Fill and notify a variable map:
      po::store(parsed, vm);
      po::notify(vm);
    } catch (std::exception & po_error) {
      vireinspector::ui::print_usage(std::cerr, optPublic);
      throw;
    }

    // Use command line arguments :
    if (vm.count("help")) {
      vireinspector::ui::print_usage(std::cout, optPublic);
      return(error_code);
    }

    if (vm.count("version")) {
      vireinspector::ui::print_version(std::cout);
      return(error_code);
    }

    // Fetch the verbosity level:
    if (vm.count("logging-priority")) {
      const std::string & logging_label = vm["logging-priority"].as<std::string>();
      params.logging = datatools::logger::get_priority(logging_label);
      DT_THROW_IF(params.logging == datatools::logger::PRIO_UNDEFINED, std::logic_error,
                  "Invalid logging priority label '" << logging_label << "' !");
    }
    DT_LOG_DEBUG(params.logging,
                 "Device manager logging priority : '" << datatools::logger::get_priority_label(params.logging ) << "'.");

    // DLL loading:
    datatools::library_loader dll_loader(params.dll_loader_config);
    for (const std::string & dll_name :  params.dlls) {
      DT_LOG_NOTICE(params.logging, "Loading DLL '" << dll_name << "'...");
      DT_THROW_IF(dll_loader.load(dll_name) != EXIT_SUCCESS,
                  std::logic_error, "Loading DLL '" << dll_name << "' failed !");
    }

    // Checks:
    DT_THROW_IF(params.device_config_filename.empty(), std::logic_error, "Missing device manager configuration file!");
    DT_LOG_NOTICE(params.logging,
                  "Device manager configuration file : '" << params.device_config_filename << "'.");
    datatools::fetch_path_with_env(params.device_config_filename);
    DT_THROW_IF(! boost::filesystem::exists(params.device_config_filename),
                std::runtime_error,
                "Device manager configuration file '" << params.device_config_filename << "' does not exist!");

    // Variant service:
    std::unique_ptr<dtc::variant_service> vServ;
    if (params.variants.is_active()) {
      // Create and start the variant service:
      vServ.reset(new dtc::variant_service);
      vServ->configure(params.variants);
      vServ->start();
      if (datatools::logger::is_debug(params.logging)) {
        vServ->get_repository().tree_dump(std::cerr, "Repository:", "[debug] ");
      }
    }

    // Device manager:
    vire::device::manager devmgr;
    devmgr.set_name("devices");
    devmgr.set_display_name("Devices");
    devmgr.set_terse_description("The device manager");
    devmgr.set_logging_priority(params.logging);
    datatools::properties devmgr_config;
    devmgr_config.read_configuration(params.device_config_filename);
    // if (datatools::logger::is_debug(params.logging)) {
    //   devmgr_config.tree_dump(std::cerr, devmgr.get_display_name() + " configuration: ", "[debug] ");
    // }
    devmgr.initialize_standalone(devmgr_config);
    if (datatools::logger::is_debug(params.logging)) {
      boost::property_tree::ptree poptions;
      poptions.put("title", devmgr.get_display_name() + ": ");
      poptions.put("indent", "[debug] ");
      poptions.put("models.list", true);
      // poptions.put("models.full", true);
      // poptions.put("tree.list_instances", true);
      // poptions.put("mapping.full", true);
      devmgr.print_tree(std::cout, poptions);
    }

    bool run_devshell = false;
    run_devshell = true;
    if (run_devshell) {
      // The inspector IHS handler:
      vire::inspector::ihs_handler devIhsh;
      devIhsh.set_device_manager(devmgr);
      devIhsh.set_logging(params.logging);
      DT_LOG_DEBUG(params.logging, "Initialization of the IHS handler...");
      devIhsh.initialize();
      DT_LOG_DEBUG(params.logging, "IHS handler is initialized.");

      // The shell:
      std::unique_ptr<vire::inspector::inspector_shell> devSh;
      devSh.reset(new vire::inspector::inspector_shell(devIhsh));
      devSh->set_name("vireinspector");
      devSh->set_host(devmgr.get_setup_label());
      devSh->set_user("nemodev");
      // devSh->set_title("vireinspector");
      devSh->set_logging(params.logging);
      devSh->set_version(datatools::version_id(1,0));
      devSh->set_prompt("%H:%W> ");
      devSh->set_continuation_prompt("> ");
      devSh->set_exit_on_error(false);
      devSh->set_using_splash(true);
      devSh->set_using_readline(false);
      if (params.interactive) {
        devSh->set_using_readline(true);
        devSh->set_using_history(true);
        devSh->set_history_add_only_on_success(true);
        devSh->set_history_filename("~/.vireinspector.history");
        devSh->set_history_truncate(100);
      }
      // devSh->set_ihs(devIhsh.grab_ihs());
      std::string home_path = datatools::ui::path::root_path();
      // home_path = devmgr.get_setup_label() + vire::utility::path::setup_separator() + vire::utility::path::root_symbol();
      devSh->set_default_path(home_path);
      devSh->initialize_simple();
      if (params.logging >= datatools::logger::PRIO_DEBUG) {
        devSh->tree_dump(std::clog, devSh->get_name() + ": ", "[debug] ");
      }

      // Run the shell session:
      error_code = devSh->run();

      // Terminate the shell:
      devSh->reset();
      devSh.reset();
    }

    if (vServ) {
      // Stop the variant service:
      vServ->stop();
      vServ.reset();
    }

  } catch (const std::exception & error) {
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS, error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_ERROR(datatools::logger::PRIO_ALWAYS, "Unexpected Exception !");
    error_code = EXIT_FAILURE;
  }

  vire::terminate();
  return error_code;
}
