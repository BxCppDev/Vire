// Ourselves
#include "virecmsserver/parameters.h"
#include "virecmsserver/command_line.h"

namespace virecmsserver {

  //! Parse command line arguments to configure the simulation parameters
  void do_configure(int argc_, char * argv_[], parameters & params_)
  {
    // - Default configuration:
    try {
      params_.logging   = datatools::logger::PRIO_FATAL;
      params_.sandbox_mode       = "";
      params_.configuration_path = "";
      params_.run_shell = false;
      // - Possible "~/.vire/cmsserver.rc" config
      //   or       "~/.vire/SuperNEMO/cms/server.rc" config
      //   or       "/etc/vire/SuperNEMO/cms/server.rc" config
    } catch (std::exception & e) {
      throw config_default_error();
    }

    // - Command line dialog configuration:
    command_line cl_args;
    try {
      cl_args.logging = datatools::logger::PRIO_FATAL;
      cl_args.configuration_path = "";
      do_cl_dialog(argc_, argv_, cl_args);
    } catch (dialog_help_requested & error) {
      throw config_signal_handled();
    } catch (dialog_version_requested & error) {
      throw config_signal_handled();
    } catch (dialog_options_error & error) {
      throw config_user_error(error.what());
    } catch (std::exception & error) {
      throw config_user_error(error.what());
    }

    // Feed input from command line to params:
    params_.logging = cl_args.logging;
    params_.configuration_path = cl_args.configuration_path;

    if (params_.configuration_path.empty()) {
      DT_LOG_NOTICE(params_.logging, "Using default CMS Server configuration file.");
      params_.configuration_path = "@snemo:config/snemo/demonstrator/cms/server/main.conf";
    }

    // Special ops:


    return;
  }

}  // namespace virecmsserver
