// virecmsclient/parameters.h - Interface for Vire CMS Client parameters
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2018 by Université Caen Normandie


#ifndef VIRECMSCLIENT_PARAMETERS_H
#define VIRECMSCLIENT_PARAMETERS_H

// Standard Library:
#include <string>
#include <stdexcept>

// Third Party:
// - Bayeux/datatools:
#include <datatools/logger.h>
#include <datatools/configuration/variant_service.h>

namespace virecmsclient {

  // \brief Set of parameters used to configure the CMS Vire Client
  struct parameters
  {
    //! The logging priority threshold
    datatools::logger::priority logging = datatools::logger::PRIO_FATAL;
    // std::string devel_mode;                //!< Label triggering special developper mode
    std::string configuration_path; //!< Path to main client configuration file
    datatools::configuration::variant_service::config variants; //!< Variant support
  };

  //! Parse command line arguments to configure the Vire client parameters
  void do_configure(int argc_, char * argv_[], parameters & params_);

  //----------------------------------------------------------------------
  // Exceptions for configuration

  class config_default_error
    : public std::exception
  {
  };

  class config_help_handled
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

}  // namespace virecmsclient

#endif // VIRECMSCLIENT_PARAMETERS_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
