//! \file  vire/logging/simple_logging_service.h
//! \brief Vire  simple logging service
//
// Copyright (c) 2015 by François Mauger <mauger@lpccaen.in2p3.fr>
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Vire. If not, see <http://www.gnu.org/licenses/>.

#ifndef VIRE_LOGGING_SIMPLE_LOGGING_SERVICE_H
#define VIRE_LOGGING_SIMPLE_LOGGING_SERVICE_H

// Standard library:
#include <string>
#include <memory>

// Third Party:
// - Bayeux/datatools:
#include <datatools/base_service.h>
#include <datatools/properties.h>
#include <datatools/command_utils.h>
#ifndef Q_MOC_RUN
#include <datatools/reflection_interface.h>
#endif // Q_MOC_RUN

namespace vire {

  namespace logging {

    //! \brief Logging management class
    class simple_logging_service
      : public ::datatools::base_service
    {

    public:

      static const unsigned int DEFAULT_ROTATE_SIZE = 10000;

      //! Default constructor
      simple_logging_service();

      //! Destructor
      ~simple_logging_service();

      //! Check if the service is initialized
      virtual bool is_initialized() const;

      //! Initialize the service using a list of properties with access to a dictionary of services
      virtual int initialize(const datatools::properties & config_,
                             datatools::service_dict_type & service_dict_);

      //! Reset the service
      virtual int reset();

      //! Set the flag for logging on console
      void set_use_logconsole(bool);

      //! Set the flag for logging in log file(s)
      void set_use_logfile(bool);

      //! Set the log file(s) directory
      void set_logfile_dir(const std::string &);

      //! Set the log file(s) filename pattern
      void set_logfile_pattern(const std::string &);

      //! Set the log file(s) rotate size
      void set_logfile_rotate_size(unsigned int sz_);

      //! Set the severity by label
      void set_severity_label(const std::string &);

      //! Return the severity by label
      const std::string & get_severity_label() const;

      bool is_trace() const;

      bool is_debug() const;

      bool is_info() const;

      bool is_warning() const;

      bool is_error() const;

      bool is_fatal() const;

      //! Set the enable global logging flag
      void set_enable_logging(bool);

      //! Smart print
      virtual void tree_dump(std::ostream & out_         = std::clog,
                             const std::string & title_  = "",
                             const std::string & indent_ = "",
                             bool inherit_               = false) const;

    protected:

      void _set_defaults();

      void _init();

      void _reset();

      void _update_filter();

    private:

      bool _initialized_; //!< Initialization flag

      // Configuration parameters for the global logging system:
      bool         _use_logconsole_;      //!< Flag to activate logging in a console
      bool         _use_logfile_;         //!< Flag to activate logging in a file
      std::string  _logfile_dir_;         //!< Log file directory
      std::string  _logfile_pattern_;     //!< Log file pattern
      unsigned int _logfile_rotate_size_; //!< Log file rotation size
      std::string  _severity_label_;      //!< Severity

      // Private implementation (PIMPL):
      struct work_type;
      std::unique_ptr<work_type> _work_; //!< Private logging

      //! Auto-registration of this service class in a central service database of Bayeux/datatools
      DATATOOLS_SERVICE_REGISTRATION_INTERFACE(simple_logging_service);

#ifndef Q_MOC_RUN
      //! Reflection interface
      DR_CLASS_RTTI()
#endif // Q_MOC_RUN


    };

  } // end of namespace logging

} // end of namespace vire

#ifndef Q_MOC_RUN
// Activate reflection layer:
DR_CLASS_INIT(vire::logging::simple_logging_service)
#endif // Q_MOC_RUN

#endif // VIRE_LOGGING_SIMPLE_LOGGING_SERVICE_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
