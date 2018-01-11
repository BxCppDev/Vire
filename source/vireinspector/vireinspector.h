//! \file  vire/inspector/vireinspector.h
//! \brief Vire inspector app
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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

#ifndef VIRE_INSPECTOR_VIREINSPECTOR_H
#define VIRE_INSPECTOR_VIREINSPECTOR_H

// Standard Library:
#include <string>
#include <iostream>

// Third Party:
// - Boost:
#include <boost/program_options.hpp>
// - Bayeux/datatools:
#include <datatools/kernel.h>
#include <datatools/logger.h>
#include <datatools/configuration/variant_service.h>

namespace vireinspector {

  //! \brief Vire inspector application utilities
  struct ui {

    //! \brief Application configuration parameters
    struct config_params {
      //! Default constructor
      config_params();

    public:
      // Configuration parameters:
      datatools::logger::priority logging; //!< Logging priority threshold
      bool interactive;                    //!< Interactive mode
      std::string dll_loader_config;       //!< Name of the configuration file for the library loader
      std::vector<std::string> dlls;       //!< List of DLL loading directives
      std::string device_config_filename;  //!< Name of the configuration file for the device manager
      datatools::configuration::variant_service::config variants; //!< Variant support
    };

    //! Return the application name
    static const std::string & name();

    //! Print application version
    static void print_version(std::ostream & out_);

    //! Print application usage (supported options and arguments)
    static void print_usage(std::ostream & out_,
                            const boost::program_options::options_description & desc_);

    //! Print application splash screen
    static void print_splash(std::ostream & out_ = std::clog);

    // Return kernel initialization flags for this application:
    static uint32_t kernel_init_flags();

    static void build_opts(boost::program_options::options_description & opts_,
                           config_params & params_);

  };

} // namespace vireinspector

#endif // VIRE_INSPECTOR_VIREINSPECTOR_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
