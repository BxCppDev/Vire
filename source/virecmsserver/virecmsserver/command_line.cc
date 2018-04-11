// Ourselves
#include "virecmsserver/command_line.h"

// Third Party:
// - Bayeux:
#include <bayeux/version.h>
#include <bayeux/datatools/kernel.h>
#include <bayeux/datatools/configuration/variant_service.h>

// Vire:
#include <vire/version.h>

// This project
#include <virecmsserver/core.h>

namespace virecmsserver {

  void do_version(std::ostream & out_, bool is_verbose_)
  {
    out_ << "virecmsserver " << vire::version::get_version() << "\n";
    if (is_verbose_) {
      out_ << "\n"
           << "Copyright (C) 2016-2018 SuperNEMO Collaboration\n\n"
           << "virecmsserver uses the following external libraries:\n"
           << "* Vire    : " << vire::version::get_version() << "\n"
           << "* Bayeux  : " << bayeux::version::get_version() << "\n"
           << "* Boost   : " << BOOST_VERSION << "\n"
           << std::endl
           << std::endl;
    }
    return;
  }

  void do_help(std::ostream & out_, const boost::program_options::options_description & od_)
  {
    do_version(out_, false);
    out_ << "Usage:\n"
         << "  virecmsserver [options]\n"
         << "Options\n"
         << od_
         << std::endl;
    return;
  }

  void do_cl_dialog(int argc_, char *argv_[], command_line & cl_args_)
  {
    namespace bpo = boost::program_options;
    namespace dtc = datatools::configuration;
    // Bind command line parser to exposed parameters:
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
         "set the name of the main configuration file for the CMS Server")

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

      // // Declare options for variant support:
      // bpo::options_description vopts("Variant support");
      // uint32_t po_init_flags = 0;
      // po_init_flags |= dtc::variant_service::NO_LABEL;
      // po_init_flags |= dtc::variant_service::NO_LOGGING;
      // // po_init_flags |= dtc::variant_service::PROFILE_LOAD_DONT_IGNORE_UNKNOWN;
      // po_init_flags |= dtc::variant_service::NO_TUI;
      // dtc::variant_service::init_options(vopts,
      //                                    params_.variants,
      //                                    po_init_flags);

      // Collect all supported options in one container:
      all_opts.add(opts);
      all_opts.add(kopts);
      // all_opts.add(vopts);

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
        throw dialog_options_error(error.what());
      }
    } catch (const std::exception & error) {
      //std::cerr << "[OptionsException] " << error.what() << std::endl;
      throw dialog_options_error(error.what());
    }

    // Handle any non-bound options
    if (vm.count("help")) {
      do_help(std::cout, all_opts);
      throw dialog_help_requested();
    }

    if (vm.count("version")) {
      do_version(std::cout, true);
      throw dialog_version_requested();
    }

    // if (vm.count("ui-shell")) {
    //   params_.run_shell = true;
    // }

    // Fetch the verbosity level:
    if (vm.count("logging-priority")) {
      const std::string & logging_label = vm["logging-priority"].as<std::string>();
      cl_args_.logging = datatools::logger::get_priority(logging_label);
      DT_THROW_IF(cl_args_.logging == datatools::logger::PRIO_UNDEFINED,
                  dialog_options_error,
                  "Invalid logging priority label '" << logging_label << "' !");
    }

    if (vm.count("config-file")) {
      cl_args_.configuration_path = vm["config-file"].as<std::string>();
    }

    // Check:
    DT_THROW_IF(cl_args_.configuration_path.empty(),
                dialog_options_error,
                "Missing CMS server configuration file !");

    return;
  }

}  // namespace virecmsserver
