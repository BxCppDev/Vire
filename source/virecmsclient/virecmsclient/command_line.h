// virecmsclient/command_line.h - Interface for Vire CMS Client command line
//
// Copyright (c) 2018 by François Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2018 by Université Caen Normandie


#ifndef VIRECMSCLIENT_COMMAND_LINE_H
#define VIRECMSCLIENT_COMMAND_LINE_H

// Standard Library:
#include <string>
#include <iostream>
#include <stdexcept>

// Third Party:
// - Boost
#include <boost/program_options.hpp>
// - Bayeux/datatools:
#include <bayeux/datatools/logger.h>

namespace virecmsclient {

  namespace bpo = boost::program_options;

  //! \brief The parameters we can receive from the command line
  struct command_line {
    datatools::logger::priority logging; //!< Logging priority threshold
    std::string configuration_path;      //!< Path to main client configuration file
  };

  //! Handle printing of version information to given ostream
  void do_version(std::ostream & out_, bool is_verbose_);

  //! Handle printing of help message to given ostream
  void do_help(std::ostream & out_, const bpo::options_description & od_);

  //! Handle command line argument dialog
  void do_cl_dialog(int argc_, char *argv_[], command_line & clargs_);

  //! Dialog help requested signal
  class dialog_help_requested
    : public std::exception
  {
  };

  //! Dialog options exception
  class dialog_options_error
    : public std::logic_error
  {
  public:
    dialog_options_error(const std::string & what_)
      : std::logic_error(what_) {}
  };

}  // namespace virecmsclient

#endif // VIRECMSCLIENT_COMMAND_LINE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
