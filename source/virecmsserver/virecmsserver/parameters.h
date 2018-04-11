// virecmsserver_parameters.h - Interface for Vire CMS Server parameters
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2018 by Université Caen Normandie

#ifndef VIRECMSSERVER_PARAMETERS_H
#define VIRECMSSERVER_PARAMETERS_H

// Standard Library:
#include <string>
#include <stdexcept>

// Third Party:
// - Bayeux/datatools:
#include <bayeux/datatools/logger.h>
#include <datatools/configuration/variant_service.h>

namespace virecmsserver {

  // \brief Set of parameters used to configure the CMS Vire Server
  struct parameters
  {
    datatools::logger::priority logging; //!< The logging priority
    std::string sandbox_mode;           //!< Label triggering special developper mode
    std::string configuration_path;     //!< Path to main server configuration file
    bool run_shell = false; //!< Shell user interface
    datatools::configuration::variant_service::config variants; //!< Variant support
  };

  //! Parse command line arguments to configure the Vire Server parameters
  void do_configure(int argc_, char * argv_[], parameters & params_);

  //----------------------------------------------------------------------
  // Exceptions for configuration

  class config_default_error
    : public std::exception
  {
  };

  class config_signal_handled
    : public std::exception
  {
  };

  class config_user_error
    : public std::logic_error
  {
  public:
    config_user_error(const std::string & what_)
      : std::logic_error(what_)
    {
    }
  };

}  // namespace virecmsserver

#endif // VIRECMSSERVER_PARAMETERS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
