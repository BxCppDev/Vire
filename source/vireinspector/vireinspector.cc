// vireinspector.cc
//
// Copyright (c) 2017 by François Mauger <mauger@lpccaen.in2p3.fr>
//
// This file is part of datatools.
//
// datatools is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// datatools is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with datatools. If not, see <http://www.gnu.org/licenses/>.

// Ourselves:
#include <vireinspector.h>

// Third Party:
// - Bayeux/datatools:
#include <datatools/kernel.h>

// Vire library:
#include <vire/vire_config.h>
#include <vire/vire.h>
#include <vire/version.h>

namespace vireinspector {

  ui::config_params::config_params()
  {
    logging = datatools::logger::PRIO_FATAL;
    interactive = true;
    return;
  }

  const std::string & ui::name()
  {
    static std::string _n("vireinspector");
    return _n;
  }

  void ui::print_version(std::ostream & out_)
  {
    out_ << name() << vire::version::get_version() << std::endl;
    return;
  }

  void ui::print_usage(std::ostream & out_,
                       const boost::program_options::options_description & opts_)
  {
    out_ << "Usage : " << std::endl;
    out_ << opts_ << std::endl;
    return;
  }

  void ui::print_splash(std::ostream & out_)
  {
    out_ << "                                                   \n"
         << "\tV I R E   I N S P E C T O R                      \n"
         << "\tVersion " << vire::version::get_version() << "   \n"
         << "                                                   \n"
         << "\tCopyright (C) 2015-2017                          \n"
         << "\tFrancois Mauger                                  \n"
         << "                                                   \n"
         << "\timmediate help: type \"help\"                    \n"
         << "\tquit:           type \"quit\"                    \n";
    out_ << "                                                   \n";
    return;
  }

  uint32_t ui::kernel_init_flags()
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

  void ui::build_opts(boost::program_options::options_description & opts_,
                      ui::config_params & params_)
  {
    namespace po = boost::program_options;
    opts_.add_options()
      ("help,h", "produce help message")

      ("version,v", "print version")

      ("logging-priority,L",
       po::value<std::string>()
       ->value_name("level"),
       "set the logging priority")

      ("interactive,i",
       po::value<bool>()
       ->zero_tokens(),
       "set the interactive mode")

      ("config-devices,c",
       po::value<std::string>(&params_.device_config_filename)
       ->value_name("file"),
       "set the name of the configuration file for the device manager")

      ("dll-loader-config,R",
       po::value<std::string>(&params_.dll_loader_config)
       ->value_name("path"),
       "Set the configuration file for the DLL loader.\n"
       "Example :\n"
       "  --dll-loader-config \"dll.conf\""
       )

      ("load-dll,l",
       po::value<std::vector<std::string> >(&params_.dlls)
       ->value_name("name@path"),
       "Define a DLL to be dynamically loaded.\n"
       "Example :\n"
       "  --load-dll \"boost_chrono@/opt/boost/1.66/lib\""
       )

      ;
    return;
  }

} // namespace vireinspector
