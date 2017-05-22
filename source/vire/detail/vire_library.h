//! \file    vire/detail/vire_library.h
//! \brief   Provide Vire library system singleton
//! \details
//
// Copyright (c) 2017 by Francois Mauger <mauger@lpccaen.in2p3.fr>
// Copyright (c) 2017 by Université de Caen Normandie
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
// along with Vire.  If not, see <http://www.gnu.org/licenses/>.


#ifndef VIRE_DETAIL_VIRE_LIBRARY_H
#define VIRE_DETAIL_VIRE_LIBRARY_H

// Standard Library
#include <string>
#include <stdexcept>

// Third party:
#include <boost/core/noncopyable.hpp>

// This project:
#include <datatools/logger.h>
#include <datatools/service_manager.h>

namespace vire {

  namespace detail {

    //! Exception class for directories initialization failure
    class directories_initialization_exception
      : public std::runtime_error
    {
    public:
      directories_initialization_exception(const std::string& msg);
    };

    //! Initialize the library resource paths
    //! \throw directories_initialization_exception when library cannot self locate
    void init_directories();

    //! \brief Vire system singleton
    class vire_library
     : private boost::noncopyable
    {
    public:

      /// Return the name of the Vire library's URN database service for supported setups (geometry, simulation...)
      static const std::string & setup_db_name();

      /// Return the name of the Vire library's URN to resource path resolver service
      static const std::string & resource_resolver_name();

      /// Extract the verbosity from the VIRE_LIBRARY_LOGGING environment variable (if any)
      static datatools::logger::priority process_logging_env();

      /// Default constructor
      vire_library();

      /// Destructor
      virtual ~vire_library();

      /// Return the logging priority
      datatools::logger::priority get_logging() const;

      /// Check initialization flag
      bool is_initialized() const;

      /// Initialize
      void initialize();

      /// Shutdown
      void shutdown();

      /// Return a mutable reference to the embedded service manager
      datatools::service_manager & grab_services();

      /// Return a non mutable reference to the embedded service manager
      const datatools::service_manager & get_services() const;

      /// Check if the Vire system singleton is instantiated
      static bool is_instantiated();

      /// Return a mutable reference to the Vire system singleton instance
      static vire_library & instance();

      /// Return a non-mutable reference to the Vire system singleton instance
      static const vire_library & const_instance();

      /// Instantiate the Vire system singleton
      static vire_library & instantiate();

    private:

      void _libinfo_registration_();

      void _libinfo_deregistration_();

      void _initialize_urn_services_();

      void _shutdown_urn_services_();

    private:

      // Management:
      bool _initialized_ = false;            //!< Initialization flag
      datatools::logger::priority _logging_; //!< Logging priority threshold

      // Working internal data:
      datatools::service_manager _services_; //!< Embedded services

      // Singleton:
      static vire_library * _instance_;      //!< Vire library system singleton handle

    };

  } // end of namespace detail

} // namespace vire

#endif // VIRE_DETAIL_VIRE_LIBRARY_H

// Local Variables: --
// mode: c++ --
// c-file-style: "gnu" --
// tab-width: 2 --
// End: --
